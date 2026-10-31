# SPNG Viewer

SPNG形式の断層画像を表示・切り出し・重ね合わせ・画像処理できるWindows x64用ビューアです。Qtによる画面とシステム標準フォントを使用します。

`out/app/SPNGViewer.exe`を直接起動してください。このフォルダは[ビルド手順](BUILDING.md)の出力先です。配布されたexeは任意のフォルダから起動できます。追加ライブラリのインストールは不要です。実行に必要なアプリのファイルはexeだけです。

## 主な機能

- 単一SPNG、IMAGE / DATAからのセンサー・Layer・画像種別・View切り替え
- 左下原点のpixel座標とカーソル画素値、移動・ズーム・矩形選択
- 表示層・選択範囲・指定した複数層のPNG保存
- 層送りの自動再生・ループと、異なる層の2点を結ぶ位置への表示追従
- 最大／最小値合成、極値を与えた層番号による色分け、5種類のパレット
- 全層への補間・2D filter・Z filter・二値化・膨張と中間画像の表示・保存
- 前後1層／前後2層のZ filter、Layer別スキャン設定の自動読み込み

[利用ガイド（全機能・操作手順）](docs/usage_ja.md)、[スキャン設定](docs/scan_parameters_ja.md)、[SPNG形式](docs/spng_format.md)、[パレット](docs/palettes_ja.md)

## ファイルの扱い

入力SPNGは読み取り専用で開きます。PNG保存には枠や表示濃淡を含めず、選んだ処理段階の画素値を保存します。

アプリのローカルコピー、起動時のライブラリ展開、永続キャッシュは作成しません。全層処理や先読みは、メモリ使用量を抑えるためOSの一時領域を使います。この領域はWindowsのdelete-on-closeで管理され、正常終了とプロセス強制終了の両方で削除されます。

## 起動引数

```powershell
.\out\app\SPNGViewer.exe "C:\scan\0000\IMAGE"
.\out\app\SPNGViewer.exe "C:\scan\0000\IMAGE\00_00\Raw_0_00000000_0_032.spng"
.\out\app\SPNGViewer.exe --licenses
```

合成画像による診断も実行できます。出力先には**存在しない新しいJSON**を指定します。既存ファイルは上書きしません。

```powershell
.\out\app\SPNGViewer.exe --self-test "C:\work\viewer-test\result.json"
```

## 動作環境と処理範囲

Windows 10 / 11 x64向けです。検証はWindows 11 x64で実施しています。Windows標準DLLは使用します。GPUは不要です。

画像処理は可視化・調査用です。affine、dead pixel補正、走査マスクには対応しません。GPU形式補間もCPUによる模擬処理です。16bitカラー画像は画素値の損失を避けるためエラーにします。

ネットワーク上から直接実行できます。起動時間は回線、ファイルサーバー、Windowsのキャッシュやセキュリティ検査によって変化します。[検証記録](VERIFICATION.md)

## ソース・ライセンス

アプリとパレットは[MIT](LICENSE)です。**Qt 6.11.2をLGPL v3で使用しています。** OpenCV等を含む[第三者ライセンス](THIRD_PARTY_NOTICES.txt)も適用されます。

配布用フォルダは`package_release.ps1`で作成できます（[手順](BUILDING.md#配布用フォルダの作成)）。配布には`SPNGViewer-source-and-dependencies.zip`を添付してください。Qtの対応ソース、アプリの全ソース、依存ソース、ライセンス、[再ビルド・再リンク手順](BUILDING.md)を含みます。
