#include "ops.h"
#include <QBuffer>
#include <QTemporaryDir>
#include <iostream>

int main(int argc,char**argv){
    QCoreApplication app(argc,argv);cv::setNumThreads(2);
    const QString root=QStringLiteral(SPNG_TEST_ROOT);
    try {
        QDir().mkpath(root+"/tests");QDir().mkpath(root+"/evidence");
        QTemporaryDir temporary(root+"/tests/run_benchmark_XXXXXX");
        if(!temporary.isValid())throw spng::Error("Cannot create benchmark workspace");
        auto path=temporary.filePath("synthetic.spng");QFile output(path);
        if(!output.open(QIODevice::WriteOnly|QIODevice::NewOnly))throw spng::Error(output.errorString());
        quint32 state=23031;QCryptographicHash sourceHash(QCryptographicHash::Sha256);
        for(int z=0;z<32;z++){
            QImage image(1024,1024,QImage::Format_Grayscale8);
            for(int y=0;y<image.height();y++){
                auto line=image.scanLine(y);
                for(int x=0;x<image.width();x++){state^=state<<13;state^=state>>17;state^=state<<5;line[x]=quint8(50+state%180);}
                sourceHash.addData(QByteArrayView(reinterpret_cast<const char*>(line),image.width()));
            }
            QByteArray png;QBuffer buffer(&png);buffer.open(QIODevice::WriteOnly);
            if(!image.save(&buffer,"PNG"))throw spng::Error("Synthetic PNG encoding failed");
            char length[8];qToLittleEndian<quint64>(quint64(png.size()),length);
            if(output.write(length,8)!=8||output.write(png)!=png.size())throw spng::Error(output.errorString());
        }
        output.close();spng::Stack input(path);spng::FilterConfig config;config.scale=1.4142;
        QElapsedTimer timer;timer.start();auto bank=spng::process(input,config);double seconds=timer.elapsed()/1000.;
        QJsonObject hashes;
        for(auto stage:bank->stages){QCryptographicHash hash(QCryptographicHash::Sha256);for(int i=0;i<bank->valid.size();i++){auto im=bank->read(i,stage);if(im.empty())continue;hash.addData(QByteArrayView(reinterpret_cast<const char*>(im.pixels.data),im.pixels.total()*im.pixels.elemSize()));}hashes[stage]=QString::fromLatin1(hash.result().toHex());}
        QJsonObject result={{"seconds",seconds},{"hashes",hashes},{"config",config.json()},{"generator","xorshift32-v1"},{"seed",23031},{"input_shape",QJsonArray{32,1024,1024}},{"input_pixel_sha256",QString::fromLatin1(sourceHash.result().toHex())}};
        QSaveFile report(root+"/evidence/cpp_processing_benchmark.json");if(!report.open(QIODevice::WriteOnly))return 2;auto data=QJsonDocument(result).toJson();if(report.write(data)!=data.size()||!report.commit())return 2;
        std::cout<<QJsonDocument(result).toJson(QJsonDocument::Compact).constData()<<std::endl;
    } catch(const std::exception& e){std::cerr<<e.what()<<std::endl;return 1;}
    return 0;
}
