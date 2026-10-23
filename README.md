# SPNG Viewer

SPNG形式の断層画像を表示・切り出し・重ね合わせ・画像処理できるPython / PySide6製ビューアです。Windows x64を対象とし、システム標準フォントを使用します。

## 主な機能

- SPNG単体、IMAGE / DATAフォルダからのセンサー・Layer・View切り替え
- 左下原点のpixel座標とカーソル位置・画素値の表示
- 左ドラッグで移動、Ctrl＋左ドラッグで矩形選択、ホイールで層送り、Ctrl＋ホイールで拡大縮小
- 表示層・選択範囲・指定した複数層のPNG保存（保存対象の画素値を保持）
- 自動層送り、ループ再生、異なる層の2点を結ぶ経路への表示追従
- 最大値／最小値の重ね合わせと、極値を与えた層番号による色分け
- カラーバー付き選択：kingfisher / meadow / ember / heat / rainbow
- 全層への画素補間・2D filter・Z filter・二値化・膨張と、中間段階の表示・保存
- Z filterの前後1層／前後2層（1層飛ばし）参照
- スキャン設定JSONの自動検出とLayer別の画像処理初期値の読み込み

画像・研究データはこのリポジトリに含みません。テストはすべて人工的に生成する小さな画像を使います。

## Windows exeから起動

配布物の`SPNGViewer.exe`を直接起動します。Pythonのインストールや起動用スクリプトは不要です。フォルダ配布形式では、exeと`_internal`フォルダを同じ場所に置いてください。

## ソースから起動

Windows / Python 3.13で動作確認しています。

~~~powershell
py -3.13 -m venv .venv
.\.venv\Scripts\python.exe -m pip install -r requirements-tested.txt
.\.venv\Scripts\python.exe spng_viewer.py
~~~

ファイルやフォルダを引数にすることもできます。

~~~powershell
.\.venv\Scripts\python.exe spng_viewer.py "C:\scan\0000\IMAGE"
~~~

互換バージョンの依存関係を使う場合はrequirements.txtを使用してください。検証した版を再現する場合はrequirements-tested.txtを使います。

## 操作

[詳しい操作説明](docs/usage_ja.md)をご覧ください。

座標は左下原点、0始まりのpixel単位です。層番号も0始まりです。PNG保存には、表示の反転・濃淡変更や選択枠は含まれません。二値画像は0/1のまま保存します。

画像処理は全層を一度計算して一時キャッシュへ保存します。設定変更後は「全層へ適用」を押してください。キャッシュは通常終了・解放時に削除します。

## テスト

~~~powershell
.\.venv\Scripts\python.exe -B -m unittest discover -s tests
.\.venv\Scripts\python.exe -B spng_viewer.py --self-test build/smoke/result.json
~~~

テスト画像・結果はtests/_generated/または指定した診断用フォルダに作成します。実際のスキャンデータを使う必要はありません。

GitHub ActionsにはWindows上の自動テストと、手動起動するWindows exeビルドを用意しています。

## Windows exeを作る

~~~powershell
.\.venv\Scripts\python.exe -m pip install -r requirements-build-tested.txt
.\.venv\Scripts\python.exe -B build_exe.py --onefile
.\.venv\Scripts\python.exe -B scripts/check_packaged_exe.py
~~~

成功するとdist/SPNGViewer.exeが生成されます。exeにはPython・Qt等の実行に必要なライブラリが含まれるため、配布先にPythonをインストールする必要はありません。

生成物はGitへ登録せず、配布時はGitHub Releases等の添付ファイルとして扱ってください。[ビルド・公開手順](docs/development_ja.md)

## 画像処理の適用範囲

この処理は可視化・調査用です。撮像装置が行う全処理を再現するものではありません。affine、dead pixel補正、走査マスク等は実装していません。GPU形式の補間もCPU上での模擬処理です。

IMAGE / DATAと同じ階層の`PARAMS/CombinedParam.json`を自動検出し、画像ファイルのLayerに対応する初期値を読み込めます。二値化閾値には`ThrBinary + ThrBinaryPlus`を使用します。[設定ファイルの読み込み](docs/scan_parameters_ja.md)を参照してください。

[SPNGの読み取り仕様](docs/spng_format.md)

## ライセンス

アプリの実装とパレットは[MITライセンス](LICENSE)で提供します。[パレットの配色と設計](docs/palettes_ja.md)も参照してください。

依存ライブラリにはそれぞれのライセンスが適用されます。詳細は[第三者ライセンスと出典](THIRD_PARTY_NOTICES.md)を参照してください。
