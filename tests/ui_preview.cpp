#include "ui_generated.h"
#include <QApplication>
#include <QFontDatabase>
#include <QTabWidget>
#include <QDir>
#include <QTest>
#include <QtPlugin>
#include <iostream>
Q_IMPORT_PLUGIN(QOffscreenIntegrationPlugin)
int main(int argc,char** argv){
    std::cerr<<"preview: starting QApplication\n"<<std::flush;
    qInstallMessageHandler([](QtMsgType,const QMessageLogContext&,const QString& message){std::cerr<<message.toUtf8().constData()<<"\n"<<std::flush;});
    qputenv("QT_QPA_PLATFORM","windows");QApplication app(argc,argv);
    std::cerr<<"preview: building widgets\n"<<std::flush;
    app.setFont(QFontDatabase::systemFont(QFontDatabase::GeneralFont));app.setStyle("Fusion");
    QMainWindow win;QHash<QString,QObject*> objects;spng::buildReferenceUi(&win,objects);
    std::cerr<<"preview: rendering\n"<<std::flush;
    win.setAttribute(Qt::WA_DontShowOnScreen,true);win.show();app.processEvents();auto tabs=qobject_cast<QTabWidget*>(objects.value("feature_tabs"));
    if(!tabs)return 2;
    QDir out(QStringLiteral(SPNG_TEST_ROOT)+"/evidence");
    if(!out.mkpath("."))return 3;
    for(int i=0;i<tabs->count();i++){tabs->setCurrentIndex(i);app.processEvents();if(!win.grab().save(out.filePath(QString("cpp_tab_%1.png").arg(i))))return 3;}
    return 0;
}
