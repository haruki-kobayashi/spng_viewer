#include "smoke.h"
#include "viewer.h"
#include <cstdio>
namespace spng {
int runSmoke(const QString& report,const QString& input){
    // Validate before creating any folder or output. Never overwrite an existing
    // file, even if it happens to have a diagnostic-looking name or extension.
    QFileInfo destination(report);
    if(destination.suffix().compare("json",Qt::CaseInsensitive)!=0||destination.exists()){
        std::fputs("Self-test requires a new .json report path; existing files are never overwritten.\n",stderr);return 2;
    }
    QJsonObject checks;QString error;QString folder;
    try{
        if(!QDir().mkpath(destination.absolutePath()))throw Error("Cannot create diagnostic directory");
        folder=destination.dir().filePath("smoke_"+QUuid::createUuid().toString(QUuid::WithoutBraces));
        if(!QDir().mkdir(folder))throw Error("Cannot create fresh diagnostic workspace");
        QVector<Image> images;for(int i=0;i<5;i++)images.append({cv::Mat(24,32,CV_8U,cv::Scalar(50+20*i)),"L"});
        const QString source=folder+"/Raw_0_00000000_0_005.spng";QFile file(source);
        if(!file.open(QIODevice::WriteOnly|QIODevice::NewOnly))throw Error(file.errorString());
        for(const auto& im:images){auto png=savePng(im,folder+"/fixture.png");QFile p(png);if(!p.open(QIODevice::ReadOnly))throw Error(p.errorString());auto bytes=p.readAll();char length[8];qToLittleEndian<quint64>(bytes.size(),length);file.write(length,8);file.write(bytes);}file.close();
        auto ensure=[&](bool condition,const QString& name){checks[name]=condition;if(!condition)throw Error(name);};
        auto wait=[&](auto condition){QElapsedTimer timer;timer.start();while(!condition()&&timer.elapsed()<30000){QCoreApplication::processEvents();QThread::msleep(2);}ensure(condition(),"asynchronous operation completed");};
        Viewer viewer;viewer.setAttribute(Qt::WA_DontShowOnScreen,true);viewer.show();viewer.openPath(source);
        wait([&]{return !viewer.image.empty()&&!viewer.loadingImage&&!viewer.scanPending;});
        ensure(viewer.image.size()==QSize(32,24),"spng decoding");
        viewer.spin("layer_number")->setValue(4);wait([&]{return viewer.loadedIndex==4&&!viewer.loadingImage;});ensure(viewer.image.pixels.at<quint8>(0,0)==130,"layer navigation");
        viewer.view->setRoi(QRect(1,2,5,6));auto output=viewer.saveDisplayedTo(folder+"/crop.png",true);ensure(QImage(output).size()==QSize(5,6),"crop PNG export");
        viewer.check("resize_check")->setChecked(false);viewer.check("highpass_check")->setChecked(false);viewer.check("z_check")->setChecked(false);viewer.real("threshold_box")->setValue(100);viewer.check("dilate_check")->setChecked(false);viewer.startProcessing();
        wait([&]{return viewer.processed&&!viewer.bulkRunning&&!viewer.loadingImage;});ensure(viewer.image.pixels.at<quint8>(0,0)==1,"processed binary pixels");
        viewer.get<QLineEdit>("layer_spec")->setText("0-4");viewer.startProjection("max");wait([&]{return bool(viewer.projection)&&!viewer.bulkRunning;});ensure(viewer.projection->layers.at<int>(0,0)==3,"projection and first tie ownership");
        viewer.check("depth_check")->setChecked(true);ensure(viewer.image.mode=="RGB","depth palette rendering");
        auto exported=exportLayers(viewer.reader(),{0,2,4},folder+"/layers","synthetic",viewer.stage());ensure(exported["saved"].toInt()==3,"multiple PNG export");
        viewer.close();
        if(!input.isEmpty()){Stack extra(input);auto image=extra.read(extra.firstAvailable());ensure(!image.empty(),"optional input opened read-only");}
    }catch(const std::exception& e){error=QString::fromUtf8(e.what());}
    QJsonObject result={{"success",error.isEmpty()},{"native_cpp",true},{"checks",checks},{"workspace",folder},{"error",error}};
    QFile reportFile(report);if(!reportFile.open(QIODevice::WriteOnly|QIODevice::NewOnly))return 2;
    auto bytes=QJsonDocument(result).toJson();if(reportFile.write(bytes)!=bytes.size())return 2;return error.isEmpty()?0:1;
}
}
