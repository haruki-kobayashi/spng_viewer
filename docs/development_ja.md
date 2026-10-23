# 開発・ビルド・公開

## 開発環境

Windows x64 / Python 3.13を使用します。仮想環境を作り、requirements-build-tested.txtをインストールします。固定版の依存関係は動作確認した構成です。更新する場合は合成画像によるテストも実行してください。

~~~powershell
py -3.13 -m venv .venv
.\.venv\Scripts\python.exe -m pip install -r requirements-build-tested.txt
.\.venv\Scripts\python.exe -B -m unittest discover -s tests
~~~

## ビルド

~~~powershell
.\.venv\Scripts\python.exe -B build_exe.py --onefile
~~~

単体exeはdist/SPNGViewer.exe、ビルド記録はdist/build_manifest.json、詳細ログはbuild/build_onefile.logに生成します。フォルダ配布形式の場合は--onefileを外してください。`dist/SPNGViewer/SPNGViewer.exe`と`_internal`フォルダが生成されます。

ビルドが失敗した場合は非ゼロの終了コードを返します。

~~~powershell
.\.venv\Scripts\python.exe -B scripts/check_packaged_exe.py
~~~

上のコマンドは単体exeを検証します。診断結果はbuild/packaged_smoke/result.jsonへ保存します。フォルダ配布形式を検証する場合は、次のようにexeを直接指定します。

~~~powershell
.\dist\SPNGViewer\SPNGViewer.exe --self-test build/packaged_smoke/result.json
~~~

いずれの自己検証も合成SPNGを使用します。

## ライセンスと配布物

本体と独自パレットの実装はMITです。Qt / PySide6などは各配布物のライセンスに従います。独自パレットの設計はdocs/palettes_ja.md、依存ライブラリのライセンスはTHIRD_PARTY_NOTICES.mdとlicenses/に記載しています。

distにはexeに加えてライセンス、説明書、再ビルド用ソースを用意します。配布用の圧縮はdist一式を対象にします。単体exeだけでも起動できますが、ライセンス・再ビルド情報は一緒に提供してください。

## GitHubでの公開

このリポジトリにはソース、合成画像を作るテスト、説明書だけを登録します。実際のSPNG、顕微鏡画像、解析結果、ネットワークの保存先、ログ、キャッシュ、実行ファイル、配布ZIPは登録しません。

GitHubリポジトリを作成し、remoteを設定してソースをpushします。ソース管理に含めるファイルは.gitignoreで定義しています。exeや配布ZIPはGitHub Releasesの添付ファイルとして配布します。

GitHub Actionsのtestsはpush / pull requestで実行します。Build Windows executableはworkflow_dispatchで手動実行し、distをartifactとして保存します。Releaseの公開やリポジトリへの書き戻しは行いません。

## 開発時の入力データ

テストにはtests/の合成画像を使用できます。実データで検証する場合は入力と出力の保存先を分け、入力データは読み取り専用で扱ってください。
