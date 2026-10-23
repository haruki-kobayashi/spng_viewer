"""Read Layer-specific image-filter settings without modifying acquisition files."""
from dataclasses import dataclass
from pathlib import Path
import json, math
from spng_core import FILE_RE
from spng_ops import FilterConfig

class ScanParameterError(ValueError):
    pass

def image_layer(path):
    match=FILE_RE.fullmatch(Path(path).name)
    return int(match['layer']) if match else None

def acquisition_directory(image):
    directory=Path(image).parent
    for parent in [directory,*list(directory.parents)[:4]]:
        if parent.name.upper() in ('IMAGE','DATA'):return parent.parent
    return directory

def find_parameter_file(image):
    """Inspect only nearby acquisition directories; never recursively scan data."""
    directory=Path(image).parent
    # IMAGE/DATA may contain one sensor directory; stop at their acquisition root.
    found_container=False
    for parent in [directory,*list(directory.parents)[:4]]:
        if parent.name.upper() in ('IMAGE','DATA'):
            directory=parent.parent
            found_container=True
            break
    for parent in ([directory] if found_container else [directory,*list(directory.parents)[:2]]):
        for relative in ('PARAMS/CombinedParam.json','CombinedParam.json','ScanControllParam.json'):
            path=parent/relative
            if path.is_file():return path
    return None

@dataclass(frozen=True)
class ScanSettings:
    source:Path
    layer:int
    config:FilterConfig
    threshold_base:float
    threshold_plus:float
    algorithm:str
    def metadata(self):
        return {'source':str(self.source),'layer':self.layer,'algorithm':self.algorithm,
                'ThrBinary':self.threshold_base,'ThrBinaryPlus':self.threshold_plus,
                'effective_threshold':self.config.threshold}

def read_settings(path,layer):
    if layer is None or isinstance(layer,bool) or not isinstance(layer,int) or layer<0:
        raise ScanParameterError('ファイル名からLayerを判定できません。設定対象Layerを指定してください。')
    path=Path(path)
    if path.stat().st_size>16*1024*1024:raise ScanParameterError('設定ファイルが大きすぎます（上限16 MiB）。')
    try:root=json.loads(path.read_text(encoding='utf-8-sig'))
    except (ValueError,UnicodeError) as exc:raise ScanParameterError('設定JSONを読み込めません。') from exc
    if not isinstance(root,dict):raise ScanParameterError('設定JSONの先頭はオブジェクトである必要があります。')
    root=root.get('ScanControllParam',root)
    section=root.get('ImageFilterParam') if isinstance(root,dict) else None
    if not isinstance(section,dict):raise ScanParameterError('ImageFilterParamがありません。')
    array=section.get('CommonParamArray')
    if not isinstance(array,list):raise ScanParameterError('ImageFilterParam.CommonParamArrayがありません。')
    if any(isinstance(item,dict) and ('LayerID' in item or 'LayerId' in item) for item in array):
        matches=[item for item in array if isinstance(item,dict) and item.get('LayerID',item.get('LayerId'))==layer]
        if len(matches)!=1:raise ScanParameterError(f'Layer{layer}に一意に対応する設定がありません。')
        values=matches[0]
    else:
        if layer>=len(array):raise ScanParameterError(f'Layer{layer}の設定がありません（設定数{len(array)}）。')
        values=array[layer]
    if not isinstance(values,dict):raise ScanParameterError(f'Layer{layer}の設定が不正です。')
    def number(key,default=None):
        value=values.get(key,default)
        if isinstance(value,bool) or not isinstance(value,(int,float)) or not math.isfinite(value):
            raise ScanParameterError(f'Layer{layer}: {key}の数値がありません、または不正です。')
        return float(value)
    algorithm=section.get('Algorithm','')
    if algorithm not in ('ImageFilterWithInterpolation_GPU','ImageFilterWithInterpolation_CPU','ImageFilter_GPU','ImageFilter_CPU'):
        raise ScanParameterError(f'未対応のImageFilter Algorithmです: {algorithm}')
    scale=number('ResizeFactor') if 'WithInterpolation' in algorithm else 1.0
    weight=number('WeightZFilter');cutoff=number('NormCutOffFreq')
    base=number('ThrBinary');plus=number('ThrBinaryPlus',0.0)
    expansion=number('Expansion')
    if expansion!=int(expansion) or not 0<=expansion<=31:raise ScanParameterError('Expansionは0〜31の整数で指定してください。')
    if not -5<=weight<=5 or not .001<=cutoff<=.499 or not -1<=base+plus<=255:
        raise ScanParameterError('Z係数・NormCutOffFreq・合計二値化閾値がビューアの設定範囲外です。')
    config=FilterConfig(scale=scale,interpolation='GPU形式 A=-0.5' if algorithm.endswith('_GPU') else 'CPU cubic',
                        cutoff=cutoff,z_weight=weight,threshold=base+plus,dilation=max(1,int(expansion)))
    try:config.validate()
    except ValueError as exc:raise ScanParameterError(str(exc)) from exc
    return ScanSettings(path,layer,config,base,plus,algorithm)
