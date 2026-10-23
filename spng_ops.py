"""Stack operations: bounded-RAM disk caches, filtering, projections and export."""
from __future__ import annotations
from dataclasses import dataclass,asdict
from pathlib import Path
import colorsys,datetime,json,math,re,sys,tempfile,time,weakref
import numpy as np
from PIL import Image
from spng_core import save_png,normalize_roi,box_to_bottom_left_roi
import cv2
cv2.setNumThreads(2)

class Cancelled(RuntimeError):
    def __init__(self):super().__init__('処理を中止しました。')

def check_cancel(cancel):
    if cancel is not None and cancel.is_set():raise Cancelled()

def parse_layers(text,count):
    chosen=set()
    if not text.strip():raise ValueError('層番号を指定してください。例: 0-15,20,25-31:2')
    for part in text.replace('，',',').split(','):
        m=re.fullmatch(r'\s*(\d+)\s*(?:[-〜～]\s*(\d+)\s*(?::\s*(\d+)\s*)?)?',part)
        if not m:raise ValueError('層の指定が不正です。例: 0-15,20,25-31:2（両端を含む）')
        a=int(m[1]);b=int(m[2] or a);step=int(m[3] or 1)
        if a>b or step<1 or a<0 or b>=count:raise ValueError(f'層番号は0-{count-1}、開始≦終了、間隔は1以上にしてください。')
        chosen.update(range(a,b+1,step))
    return sorted(chosen)

def line_position(p1,p2,index,extend=True):
    z1,x1,y1=p1;z2,x2,y2=p2
    if z1==z2:raise ValueError('異なる層に2点を指定してください。')
    t=(index-z1)/(z2-z1)
    if not extend:t=max(0,min(1,t))
    return x1+t*(x2-x1),y1+t*(y2-y1)

class ImageBank:
    """Finalized cache. Reads return copies so worker lifetime cannot invalidate GUI pixels."""
    def __init__(self,owner,maps,valid,metadata):
        self.owner=owner;self.maps=maps;self.valid=np.array(valid,dtype=bool);self.metadata=metadata
        self.stages=list(maps)
        # Runs before TemporaryDirectory's own exit cleanup, including interpreter shutdown.
        self._finalizer=weakref.finalize(self,_close_bank,maps,owner)
    def read_frame(self,index,stage=None):
        if not self.valid[index]:return None
        return Image.fromarray(np.array(self.maps[stage or self.stages[-1]][index],copy=True))
    def __len__(self):return len(self.valid)
    def close(self):
        self._finalizer()


def _close_bank(maps,owner):
    for a in list(maps.values()):
        if getattr(a,'_mmap',None):a._mmap.close()
    maps.clear();owner.cleanup()

def new_workspace(cache_dir):
    root=Path(cache_dir);root.mkdir(parents=True,exist_ok=True)
    return tempfile.TemporaryDirectory(prefix='stack_',dir=root)

def preload_stack(stack,cache_dir,progress=None,cancel=None):
    start=time.perf_counter();owner=new_workspace(cache_dir);data=None;valid=np.zeros(len(stack),bool)
    try:
        for i in range(len(stack)):
            check_cancel(cancel);im=stack.read_frame(i)
            if im is not None:
                if im.mode=='P':raise ValueError('パレット画像の全層先読みには未対応です。通常表示は使用できます。')
                a=np.asarray(im)
                if data is None:data=np.lib.format.open_memmap(Path(owner.name)/'raw.npy',mode='w+',dtype=a.dtype,shape=(len(stack),)+a.shape)
                if a.shape!=data.shape[1:] or a.dtype!=data.dtype:raise ValueError('層ごとに画像サイズ／型が異なるため、全層キャッシュを作れません。')
                data[i]=a;valid[i]=True
            if progress:progress(i+1,len(stack),'全層を先読み')
        if data is None:raise ValueError('保存された画像がありません。')
        data.flush()
        return ImageBank(owner,{'元画像':data},valid,{'operation':'preload','elapsed_seconds':time.perf_counter()-start})
    except Exception:
        if data is not None:data._mmap.close()
        owner.cleanup();raise

@dataclass(frozen=True)
class FilterConfig:
    scale:float=1.0
    interpolation:str='GPU形式 A=-0.5'
    highpass:bool=True
    taps:int=15
    cutoff:float=0.1
    z_filter:bool=True
    z_weight:float=-0.2
    z_step:int=1
    binary:bool=True
    threshold:float=9.0
    dilation:int=2
    def validate(self):
        if not .25<=self.scale<=3:raise ValueError('補間倍率は0.25-3にしてください。')
        if self.taps<3 or self.taps>101 or self.taps%2==0:raise ValueError('FIR tap数は3-101の奇数にしてください。')
        if not 0<self.cutoff<.5:raise ValueError('NormCutOffFreqは0より大きく0.5未満にしてください。')
        if self.z_step not in (1,2):raise ValueError("Z参照間隔は1または2層です。")
        if self.dilation<1 or self.dilation>31:raise ValueError('膨張サイズは1-31にしてください。')
        if self.dilation>1 and not self.binary:raise ValueError('膨張する場合は二値化も有効にしてください。')
        if self.interpolation not in ['CPU cubic','GPU形式 A=-0.5']:raise ValueError('補間方式が不正です。')


def fir_kernel(taps,cutoff):
    if taps%2==0:raise ValueError('FIR tap数は奇数です。')
    n=np.arange(taps,dtype=np.float64)-(taps-1)/2
    k=2*cutoff*np.sinc(2*cutoff*n)*np.hamming(taps)
    return (k/k.sum()).astype(np.float32)

def uint8(a):return np.clip(np.rint(a),0,255).astype(np.uint8)

def gpu_style_resize(a,size):
    """CPU implementation of integer-origin cubic interpolation with A=-0.5."""
    w,h=size
    def weights(n,m):
        x=np.arange(m,dtype=np.float32)*np.float32(n/m);ix=np.floor(x).astype(np.int32)
        inds=ix[:,None]+np.arange(-1,3,dtype=np.int32);d=np.abs(x[:,None]-inds).astype(np.float32);aa=-.5
        k=np.where(d<=1,((aa+2)*d-(aa+3))*d*d+1,np.where(d<2,((aa*d-5*aa)*d+8*aa)*d-4*aa,0)).astype(np.float32)
        k/=k.sum(axis=1)[:,None]
        return np.clip(inds,0,n-1),k
    ix,wx=weights(a.shape[1],w);iy,wy=weights(a.shape[0],h)
    tmp=sum(a[:,ix[:,k]].astype(np.float32)*wx[:,k] for k in range(4))
    return uint8(sum(tmp[iy[:,k],:]*wy[:,k,None] for k in range(4)))

def process_stack(stack,config,cache_dir,progress=None,cancel=None):
    """Compute every enabled stage for all layers once. All image stages are uint8."""
    config.validate();start=time.perf_counter();owner=new_workspace(cache_dir);maps={};valid=np.zeros(len(stack),bool)
    stages=['補間後']
    if config.highpass:stages+=['2D filter後']
    if config.z_filter:stages+=['Z filter後']
    if config.binary:stages+=['二値化後']
    if config.binary and config.dilation>1:stages+=['膨張後']
    total=len(stack)*(2 if config.z_filter else 1);done=0;shape=None
    try:
        for i in range(len(stack)):
            check_cancel(cancel);im=stack.read_frame(i)
            if im is not None:
                a=np.asarray(im)
                if a.ndim!=2 or a.dtype!=np.uint8:raise ValueError('画像処理は8bitグレースケール画像に対応します。16bitやカラーは通常表示・PNG保存をご利用ください。')
                size=(max(1,round(a.shape[1]*config.scale)),max(1,round(a.shape[0]*config.scale)))
                if shape is None:
                    shape=(len(stack),size[1],size[0])
                    for j,key in enumerate(stages):maps[key]=np.lib.format.open_memmap(Path(owner.name)/f'stage_{j}.npy',mode='w+',dtype=np.uint8,shape=shape)
                if (size[1],size[0])!=shape[1:]:raise ValueError('全層で同じ画像サイズが必要です。')
                if size!=(a.shape[1],a.shape[0]):a=gpu_style_resize(a,size) if config.interpolation.startswith('GPU') else cv2.resize(a,size,interpolation=cv2.INTER_CUBIC)
                maps['補間後'][i]=a;valid[i]=True
                if config.highpass:
                    k=fir_kernel(config.taps,config.cutoff)
                    bg=uint8(cv2.sepFilter2D(a,cv2.CV_32F,k,k,borderType=cv2.BORDER_REFLECT_101))
                    a=cv2.subtract(bg,a);maps['2D filter後'][i]=a
                if not config.z_filter:
                    _after_z(a,i,config,maps)
            done+=1
            if progress:progress(done,total,'補間・2D filter・画像処理')
        if not valid.any():raise ValueError('保存された画像がありません。')
        before=maps['2D filter後' if config.highpass else '補間後']
        if config.z_filter:
            for i in range(len(stack)):
                check_cancel(cancel)
                if valid[i]:
                    a=np.array(before[i],copy=True)
                    neighbors=[j for j in (i-config.z_step,i+config.z_step) if 0<=j<len(stack)]
                    if not neighbors:out=a
                    elif len(neighbors)==1:
                        j=neighbors[0];b=before[j] if valid[j] else a
                        out=uint8(a.astype(np.float32)+config.z_weight*b.astype(np.float32))
                    else:
                        prev,nxt=[before[j] if valid[j] else a for j in neighbors]
                        summed=cv2.add(prev,nxt)
                        out=uint8(a.astype(np.float32)+(config.z_weight/2)*summed.astype(np.float32))
                    maps['Z filter後'][i]=out;_after_z(out,i,config,maps)
                done+=1
                if progress:progress(done,total,'Z filter・二値化・膨張')
        for a in maps.values():a.flush()
        return ImageBank(owner,maps,valid,{'operation':'filter','config':asdict(config),'elapsed_seconds':time.perf_counter()-start,'stage_order':stages,'source_path':str(getattr(stack,'path','memory')),'border_rule':'XY reflect101; Z endpoints single neighbor at full weight; missing neighbor uses current; intermediate uint8 round-to-even and saturation','binary_values':[0,1],'interpolation_note':'CPU cubic differs from HTS GPU; optional GPU-style A=-0.5 integer-origin emulator, without affine/dead pixel/stage masks; not a complete HTS reproduction'})
    except Exception:
        for a in maps.values():a._mmap.close()
        maps.clear();owner.cleanup();raise

def _after_z(a,i,c,maps):
    if c.binary:
        a=(a>c.threshold).astype(np.uint8);maps['二値化後'][i]=a
        if c.dilation>1:maps['膨張後'][i]=cv2.dilate(a,np.ones((c.dilation,c.dilation),np.uint8),iterations=1)

@dataclass
class Projection:
    values:np.ndarray
    layers:np.ndarray
    requested:list
    used:list
    kind:str
    source:str

def project_stack(read_frame,indices,kind='max',source='元画像',progress=None,cancel=None):
    indices=sorted(set(indices))
    values=None;owners=None;used=[]
    for j,i in enumerate(indices):
        check_cancel(cancel);im=read_frame(i)
        if im is not None:
            a=np.asarray(im)
            if a.ndim!=2:raise ValueError('最大・最小の合成はグレースケール画像に対応します。')
            if values is None:values=np.array(a,copy=True);owners=np.full(a.shape,i,np.int32)
            else:
                if a.shape!=values.shape or a.dtype!=values.dtype:raise ValueError('重ねる画像のサイズ・型が一致していません。')
                changed=a>values if kind=='max' else a<values
                values[changed]=a[changed];owners[changed]=i
            used.append(i)
        if progress:progress(j+1,len(indices),'最大値を合成' if kind=='max' else '最小値を合成')
    if values is None:raise ValueError('指定範囲に保存済みの画像がありません。')
    return Projection(values,owners,list(indices),used,kind,source)

def depth_lut(n=256,palette="kingfisher"):
    from spng_palettes import palette_lut
    return palette_lut({"虹色":"rainbow"}.get(palette,palette), n)


def projection_image(projection,colored=False,weighted=False,palette="kingfisher"):
    if not colored:return Image.fromarray(projection.values)
    lo,hi=min(projection.requested),max(projection.requested)
    inds=np.rint((projection.layers-lo)/max(1,hi-lo)*255).astype(np.uint8)
    rgb=depth_lut(palette=palette)[inds]
    if weighted:
        top=1 if projection.values.max()<=1 else (65535 if projection.values.dtype.itemsize>1 else 255)
        strength=np.clip(projection.values.astype(np.float32)/top,0,1)
        if projection.kind=='min':strength=1-strength
        rgb=rgb*strength[...,None]
    return Image.fromarray(np.rint(rgb*255).astype(np.uint8))

def export_layers(read_frame,indices,folder,stem,source='元画像',roi=None,metadata=None,progress=None,cancel=None):
    folder=Path(folder);folder.mkdir(parents=True,exist_ok=False)
    manifest={'source':source,'requested':list(indices),'saved':[],'missing':[],'metadata':metadata or {},'ROI_top_left_box':roi,'status':'running'}
    report=folder/'export_manifest.json'
    try:
        for j,i in enumerate(indices):
            check_cancel(cancel);im=read_frame(i)
            if im is None:manifest['missing'].append(i)
            else:
                if roi:normalize_roi(roi[0],roi[1],roi[2]-roi[0],roi[3]-roi[1],im.size)
                name=f'{stem}_z{i:03d}.png';save_png(im,folder/name,roi)
                manifest['saved'].append({'layer':i,'file':name,'size':list(im.crop(roi).size if roi else im.size),'mode':im.mode})
            if progress:progress(j+1,len(indices),'PNGを書き出し')
        manifest['status']='complete'
    except Exception:
        manifest['status']='cancelled' if cancel is not None and cancel.is_set() else 'error'
        raise
    finally:report.write_text(json.dumps(manifest,ensure_ascii=False,indent=2),'utf8')
    return {'folder':str(folder),'saved':len(manifest['saved']),'missing':manifest['missing']}
