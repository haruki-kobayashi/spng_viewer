# SPNG Viewer

SPNG形式の断層画像を表示・切り出し・重ね合わせ・画像処理できるWindows 10 / 11 x64用ビューアです。

`SPNGViewer.exe`を直接起動してください。追加ライブラリのインストールやソースZIPの展開は不要です。

- [利用ガイド（全機能・操作手順）](docs/usage_ja.md)
- [スキャン設定](docs/scan_parameters_ja.md)
- [パレット](docs/palettes_ja.md)
- [アプリのライセンス](LICENSE)・[第三者ライセンス](THIRD_PARTY_NOTICES.txt)

## ソースと再ビルド

`SPNGViewer-source-and-dependencies.zip`には、アプリと依存ライブラリのソース、ビルド手順、ライセンスを収録しています。再ビルドする場合は展開し、`source/BUILDING.md`を参照してください。再配布時も、このZIPとライセンス文書を併せて提供してください。

## ファイルの照合

`SHA256SUMS.txt`にはexeとソースZIPのSHA-256を記録しています。PowerShellの次のコマンドで計算した値と比較できます。

```powershell
Get-FileHash .\SPNGViewer.exe -Algorithm SHA256
Get-FileHash .\SPNGViewer-source-and-dependencies.zip -Algorithm SHA256
```
