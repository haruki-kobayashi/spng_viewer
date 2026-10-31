#include "core.h"
#include "ops.h"
#include "scanparams.h"
#include <QCoreApplication>
#include <iostream>

using namespace spng;
static void require(bool value,const char* message){if(!value)throw Error(message);}
static QJsonArray failures;static int checks=0;
static void compareImage(const Image& actual,const QString& expected,QString context){
    QImage q(expected);if(q.isNull())throw Error("Reference missing: "+expected);auto view=displayImage(actual,0,actual.mode=="1"?1:255,false);
    // Display comparisons are separate. Quantitative 16-bit checks use direct samples below.
    if(actual.mode=="I;16"){q=q.convertToFormat(QImage::Format_Grayscale16);require(q.size()==actual.size(),"16-bit size");for(int y=0;y<q.height();y++)for(int x=0;x<q.width();x++)require(reinterpret_cast<const quint16*>(q.constScanLine(y))[x]==actual.pixels.at<quint16>(y,x),"16-bit value");checks++;return;}
    auto a=view.convertToFormat(QImage::Format_RGBA8888),b=q.convertToFormat(QImage::Format_RGBA8888);int different=0,maxDiff=0;
    if(a.size()!=b.size()){failures.append(QJsonObject{{"case",context},{"error","size mismatch"}});checks++;return;}
    for(int y=0;y<a.height();y++)for(int x=0;x<a.width()*4;x++){int d=std::abs(int(a.constScanLine(y)[x])-int(b.constScanLine(y)[x]));if(d)different++;maxDiff=std::max(maxDiff,d);}
    if(different)failures.append(QJsonObject{{"case",context},{"different_channels",different},{"max_difference",maxDiff}});checks++;
}
static Image fromDisplay(QImage q){q=q.convertToFormat(QImage::Format_RGBA8888);Image out;out.mode="RGBA";out.pixels=cv::Mat(q.height(),q.width(),CV_8UC4);for(int y=0;y<q.height();y++)memcpy(out.pixels.ptr(y),q.constScanLine(y),q.width()*4);return out;}
int main(int argc,char** argv){
    QCoreApplication app(argc,argv);cv::setNumThreads(2);
    // Diagnostic output has a fixed build-workspace destination and is not accepted
    // from an arbitrary command-line path. This test program is not distributed.
    const QString root=QStringLiteral(SPNG_TEST_ROOT);
    QDir fixtures(root+"/tests/fixtures");QString out=root+"/tests/run_"+QUuid::createUuid().toString(QUuid::WithoutBraces);QDir().mkpath(out);
    try{
        QFile file(fixtures.filePath("reference.json"));require(file.open(QIODevice::ReadOnly),"reference.json missing");auto reference=QJsonDocument::fromJson(file.readAll()).object();
        Stack stack(fixtures.filePath(reference["normal_stack"].toString()));require(stack.frames.size()==7&&stack.frames[3].missing(),"missing layer indexing");require(stack.read(3).empty(),"missing read");
        require(parseLayers("0-4:2,3,6",7)==QVector<int>{0,2,3,4,6},"range parser");require(bottomLeftRoi(1,0,3,2,{7,5})==QRect(1,3,3,2),"bottom-left ROI");require(linePosition({2,10,20},{6,30,40},0)==QPointF(0,10),"line extrapolation");checks+=5;
        int number=0;
        for(auto v:reference["filters"].toArray()){
            auto o=v.toObject();auto config=FilterConfig::fromJson(o["config"].toObject());auto bank=process(stack,config);
            for(auto expected:o["expected"].toArray()){auto e=expected.toObject();compareImage(bank->read(e["index"].toInt(),e["stage"].toString()),fixtures.filePath(e["file"].toString()),e["file"].toString());}
            number++;std::cout<<"Filter "<<number<<" completed\n"<<std::flush;
        }
        Stack modes(fixtures.filePath(reference["modes_stack"].toString()));for(int i=0;i<modes.frames.size();i++){auto im=modes.read(i);if(im.empty())continue;compareImage(im,fixtures.filePath(QString("type_%1.png").arg(i)),QString("read mode %1").arg(i));auto saved=savePng(im,QDir(out).filePath(QString("type_%1.png").arg(i)));require(QFileInfo(saved).suffix()=="png","output suffix");if(im.size().width()>3&&im.size().height()>3)savePng(im,QDir(out).filePath(QString("crop_%1.png").arg(i)),QRect(1,2,3,4));}
        for(auto v:reference["display"].toArray()){auto e=v.toObject();auto im=modes.read(e["index"].toInt());auto rendered=displayImage(im,e["low"].toDouble(),e["high"].toDouble(),e["invert"].toBool());compareImage(fromDisplay(rendered),fixtures.filePath(e["expected"].toString()),e["expected"].toString());}
        for(auto kind:{QString("max"),QString("min")}){auto p=project([&](int i){return stack.read(i);},{6,5,4,3,2,1,0},kind);require(p->used==QVector<int>{0,1,2,4,5,6},"projection missing handling");for(auto name:{QString("kingfisher"),QString("meadow"),QString("ember"),QString("heat"),QString("rainbow")})for(bool weighted:{false,true}){auto filename=QString("projection_%1_%2_%3.png").arg(kind,name).arg(int(weighted));compareImage(projectionImage(*p,true,weighted,name),fixtures.filePath(filename),filename);}}
        auto cancellation=std::make_shared<std::atomic_bool>(true);bool stopped=false;try{process(stack,FilterConfig(),{},cancellation);}catch(const Error&){stopped=true;}require(stopped,"cancellation");checks++;
        for(QString spec:{"","4-2","0-8","0-6:0","0,,1","x"}){bool invalid=false;try{parseLayers(spec,7);}catch(const Error&){invalid=true;}require(invalid,"invalid range must fail");checks++;}
    }catch(const std::exception& e){failures.append(QJsonObject{{"exception",QString::fromUtf8(e.what())}});}
    QJsonObject result={{"checks",checks},{"failures",failures},{"passed",failures.empty()},{"exports",out}};
    QSaveFile report(root+"/evidence/core_test_result.json");if(!report.open(QIODevice::WriteOnly))return 3;report.write(QJsonDocument(result).toJson());if(!report.commit())return 3;
    std::cout<<QJsonDocument(result).toJson().constData();return failures.empty()?0:1;
}
