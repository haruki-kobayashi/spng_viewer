#include "viewer.h"
#include "smoke.h"
#include <QFontDatabase>
#include <QCommandLineParser>
int main(int argc,char** argv){
    cv::setNumThreads(2);
    QApplication app(argc,argv);app.setApplicationName("SPNG Viewer");app.setFont(QFontDatabase::systemFont(QFontDatabase::GeneralFont));app.setStyle("Fusion");
    // Normal viewing opens acquisition files read-only. Diagnostic output requires
    // an explicit --self-test option and a new .json path, validated in runSmoke.
    QCommandLineParser parser;parser.setApplicationDescription("SPNG Viewer — file/folder browsing and original-pixel PNG export");parser.addHelpOption();parser.addPositionalArgument("path","spngファイル、DATA/IMAGE/センサーフォルダ","[path]");
    QCommandLineOption selfTest("self-test","合成画像による動作確認。新しいJSON出力先を指定します。","REPORT_JSON");
    QCommandLineOption testInput("self-test-input","診断時に読み取りだけ行う任意のSPNG。","SPNG");
    parser.addOption(selfTest);parser.addOption(testInput);
    parser.addOption(QCommandLineOption("licenses","同梱ライブラリのライセンスを表示します。"));
    if(!parser.parse(app.arguments())||parser.positionalArguments().size()>1){QMessageBox::warning(nullptr,"起動引数",parser.errorText().isEmpty()?"ファイルまたはフォルダを1つ指定してください。":parser.errorText());return 2;}
    if(parser.isSet("help")){QMessageBox::information(nullptr,"SPNG Viewer",parser.helpText());return 0;}
    if(parser.isSet("licenses")){QFile file(":/licenses/THIRD_PARTY_NOTICES.txt");if(!file.open(QIODevice::ReadOnly))return 2;QDialog dialog;dialog.setWindowTitle("SPNG Viewer — Licenses");dialog.resize(850,650);auto layout=new QVBoxLayout(&dialog);auto text=new QTextBrowser();text->setPlainText(QString::fromUtf8(file.readAll()));layout->addWidget(text);auto buttons=new QDialogButtonBox(QDialogButtonBox::Close);QObject::connect(buttons,&QDialogButtonBox::rejected,&dialog,&QDialog::reject);layout->addWidget(buttons);dialog.exec();return 0;}
    if(parser.isSet(selfTest))return spng::runSmoke(parser.value(selfTest),parser.value(testInput));
    if(parser.isSet(testInput))return 2;
    try{spng::Viewer win;win.show();auto paths=parser.positionalArguments();if(!paths.empty())QTimer::singleShot(0,&win,[&win,path=paths.first()]{win.openPath(path);});return app.exec();}
    catch(const std::exception& e){QMessageBox::critical(nullptr,"SPNG Viewer",QString::fromUtf8(e.what()));return 1;}
}
