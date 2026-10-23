# スキャン設定ファイル

## 自動検出

SPNGを開くと、画像ファイル名からLayer番号を取得し、撮像フォルダの設定ファイルを検索します。対象は乳剤面を表すLayer番号で、断層画像の層番号ではありません。

```text
scan/
  PARAMS/
    CombinedParam.json
  IMAGE/
    00_00/
      Raw_0_00000000_0_044.spng
  DATA/
    00_00/
      TrackHit2_0_00000000_1_000.spng
```

IMAGEまたはDATAがある撮像フォルダ内で、次の順に最初に見つかった設定ファイルを使用します。データフォルダ全体の再帰検索は行いません。

1. `PARAMS/CombinedParam.json`
2. `CombinedParam.json`
3. `ScanControllParam.json`

画像の保存先の上位にIMAGEまたはDATAがない場合は、SPNGがあるフォルダとその上位2階層を順に調べます。

「設定JSONを選択」からファイルを指定することもできます。指定したJSONは同じ撮像フォルダ内のLayer切り替えに使用します。別の撮像フォルダを開くと自動検出へ戻ります。「自動検出をやり直す」は指定を解除して検索します。ファイル名からLayerを判定できないSPNGでは、「設定Layer」を指定してください。

## Layerの対応

`ImageFilterParam.CommonParamArray[0]`をLayer0、`[1]`をLayer1として読み込みます。各項目に`LayerID`または`LayerId`がある場合は、その番号を使用します。指定Layerがない場合や同じ番号が重複する場合は、エラーを表示して処理を停止します。

## 読み込む項目

| JSONの項目 | ビューアの設定 |
| --- | --- |
| `ImageFilterParam.Algorithm` | GPU形式またはCPU cubic、画素補間の有無 |
| `ResizeFactor` | 補間倍率（補間を行うAlgorithmの場合） |
| `NormCutOffFreq` | 2D filterのカットオフ周波数 |
| `WeightZFilter` | Z filter係数 |
| `ThrBinary` + `ThrBinaryPlus` | 二値化閾値。ThrBinaryPlusが省略された場合は0 |
| `Expansion` | 膨張サイズ。0または1は膨張なし |

対応Algorithmは`ImageFilterWithInterpolation_GPU`、`ImageFilterWithInterpolation_CPU`、`ImageFilter_GPU`、`ImageFilter_CPU`です。必要な項目がない場合、未対応Algorithmや設定範囲外の値の場合は、理由を画面に表示します。

FIRのtap数は15、Z参照間隔は前後1層として設定します。画像処理タブで変更できます。画像処理は設定の読み込みと別操作で、「設定を現在のspngの全層へ適用」を押して実行します。

読み込んだ数値は表示桁数を超える精度も保持します。その項目の入力値が表示値と同じ場合は読み込み精度で処理し、入力値を変更した場合は指定値を使用します。

## 手動設定と記録

自動読み込みをOFFにすると、ファイルを切り替えても入力値を保持します。ONの場合はファイルを開くたびにそのLayerの設定を読み込みます。設定ファイルが見つからない場合はビューアの初期値を使用します。読み込みに失敗した場合は画像を閲覧できますが、処理は設定を修正するか手動設定に切り替えるまで実行できません。

処理後のPNGに付く処理記録には、実際に適用した設定に加え、読み込み元のファイルパス、Layer番号、ThrBinary、ThrBinaryPlus、合計閾値を保存します。

## 適用範囲

自動読み込みは画像処理の初期値を設定する機能です。撮像層の選別、dead pixel補正、affine、走査マスク、tracking条件は適用しません。処理済みの画像種別に対して全工程を適用すると重複処理になるため、画像種別と有効な処理段階を確認してください。[画像処理の仕様](usage_ja.md#全層画像処理)も参照してください。
