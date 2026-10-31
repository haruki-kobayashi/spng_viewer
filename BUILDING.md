# ビルドとライブラリの差し替え

Windows x64、Visual Studio 2022のC++デスクトップ開発、CMake、Ninjaを使用します。C++20と静的CRTでビルドします。実行時に必要なアプリのファイルはexeだけです。

## 依存ライブラリ

`vendor_sources/SHA256.json`にQt Base 6.11.2、OpenCV 5.0.0、Intel IPPの公式取得先・サイズ・SHA-256を固定しています。`build_dependencies.ps1`は不足するアーカイブを取得して検証します。取得済みならハッシュ確認だけで再利用します。アーカイブ本体はGit管理から除外しています。初回取得にはネットワーク接続が必要です。

Visual Studioの「Developer PowerShell for VS 2022」を開き、CMakeとNinjaをPATHに追加して、ソースのルートで実行してください。

```powershell
.\build_dependencies.ps1 -OutputRoot .\out\dependencies
.\build.ps1 -QtPrefix .\out\dependencies\qt-static -OpenCVConfig .\out\dependencies\opencv-static\x64\vc17\staticlib
```

生成物は`out/app/SPNGViewer.exe`です。画面・パレットのC++ソースも同梱しています。

## Qtを変更して再リンクする

Qtのソースは`out/dependencies/sources/qtbase-everywhere-src-6.11.2`に展開されます。変更後に`build_dependencies.ps1`を再実行すると、その変更を残したまま再ビルドします。別にビルドした互換Qtを使う場合は、`build.ps1`の`-QtPrefix`にそのインストール先を指定してください。

アプリソースはMITです。Qtの変更、アプリの再リンク、変更したexeの実行を妨げる認証・署名の制約は設けていません。取得したアーカイブとアプリのソースを使って再ビルドできます。ライブラリの各ライセンスは`licenses/`にあります。

## 検証

単体exeの診断は新しい出力名で実行します。

```powershell
.\out\app\SPNGViewer.exe --self-test .\out\smoke\result.json
```

追加のC++比較テストをビルドする場合はCMakeで`SPNG_BUILD_TESTS=ON`を指定します。`tests/fixtures/`は合成画像、`evidence/expected_*.png`は画面テストの期待画像です。画像取得装置のデータは含みません。画面一致テストはフォントと表示環境が同じ場合に成立します。

Qt・OpenCVの変更後は、画像処理の画素値や表示が変わる可能性があるため、同じ入力による比較を再実施してください。

## 依存ソースの事前取得と配布

`fetch_dependencies.ps1`だけを実行すれば、コンパイラを起動せずに依存ソースを取得できます。別のPCで取得して`vendor_sources/`へ移せば、そのアーカイブをオフラインで使用できます。SHA-256が異なる既存ファイルは上書きせずエラーにします。

exeを配布するときは、対応ソースを含むアーカイブも作成し、併せて提供してください。取得URLの一覧だけを対応ソースの配布に代用しないでください。

```powershell
.\fetch_dependencies.ps1
.\package_source.ps1
```

出力は`out/packages/SPNGViewer-source-and-dependencies.zip`です。既存出力は上書きしません。再作成時は`-OutputFile`で新しいZIP名を指定します。パッケージにはアプリ・依存ソース・ビルド手順・ライセンス・合成テスト素材を含め、実行ログや一時ファイル、exeは含めません。

## 合成画像と画面比較

`tests/screen_fixture/`の小さなSPNGもテストに必要なためGit管理します。性能試験の大きなSPNGは保存せず、`spng_processing_benchmark`が実行時に固定seedのxorshift32で生成します。入力は32×1024×1024、画素値50～229です。生成画像は実行後に削除し、測定時間に画像生成時間は含めません。結果は`evidence/cpp_processing_benchmark.json`に記録します。

画面比較では入力パスの表示をテスト用の相対パスへ統一します。通常実行では期待画像を上書きしません。意図した画面変更後に期待画像を更新するときだけ、`SPNG_UPDATE_EXPECTED=1`を設定して画面テストを実行し、変更した画像を確認してからコミットしてください。更新後は環境変数を解除して通常の比較が成功することを確認します。表示倍率は`QT_SCALE_FACTOR`の1、1.25、1.5、2で確認できます。

## 配布用フォルダの作成

アプリのビルド完了後、ソースのルートで実行します。

```powershell
.\package_release.ps1
```

`out/app/SPNGViewer.exe`を使い、次のフォルダを作成します。コンパイルは行わないため、ソースを変更した場合は先に再ビルドしてください。

```text
out/release/
  SPNGViewer.exe
  README.md
  docs/
  LICENSE
  THIRD_PARTY_NOTICES.txt
  licenses/
  BUILDING.md
  VERIFICATION.md
  SPNGViewer-source-and-dependencies.zip
  SHA256SUMS.txt
```

READMEは配布物の起動方法を説明します。ソースZIPには依存ライブラリも含みます。不足する依存ソースは取得してハッシュ検証し、取得済みのものは再利用します。SHA256SUMS.txtはexeとソースZIPの照合用です。

出力フォルダが存在する場合は上書きせず停止します。別の出力先やビルド済みexeを指定できます。

```powershell
.\package_release.ps1 -Executable .\out\app\SPNGViewer.exe -OutputDirectory .\out\release-v2
```

途中で失敗した場合は不完全な出力を残してエラーにします。成功の表示とSHA256SUMS.txtの作成を確認してから配布してください。再ビルドする場合はソースZIPを展開し、その中の`source/BUILDING.md`に従います。
