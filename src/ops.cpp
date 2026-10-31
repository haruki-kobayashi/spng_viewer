#include "ops.h"
#include "palettes_generated.h"
#include <opencv2/imgproc.hpp>
#include <cmath>
#include <windows.h>

namespace spng {
class Storage {
public:
    int count,type;QSize size;QString mode;quint64 frameBytes;
    HANDLE file=INVALID_HANDLE_VALUE,mapping=nullptr;void* data=nullptr;
    Storage(int n,QSize s,int t,QString m):count(n),type(t),size(s),mode(m){
        frameBytes=quint64(size.width())*size.height()*CV_ELEM_SIZE(type);quint64 bytes=frameBytes*n;
        if(!bytes || bytes>quint64(std::numeric_limits<qint64>::max()))throw Error("画像処理のサイズが大きすぎます。");
        // Temporary processing storage only. Windows removes it when the last handle
        // closes, including process termination. Never used for application startup.
        auto name=QDir::temp().filePath("SPNGViewer-work-"+QUuid::createUuid().toString(QUuid::WithoutBraces)+".tmp");
        file=CreateFileW(reinterpret_cast<LPCWSTR>(name.utf16()),GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE,nullptr,CREATE_NEW,FILE_ATTRIBUTE_TEMPORARY|FILE_FLAG_DELETE_ON_CLOSE,nullptr);
        if(file==INVALID_HANDLE_VALUE)throw Error("画像処理の一時領域を作成できません。");
        mapping=CreateFileMappingW(file,nullptr,PAGE_READWRITE,DWORD(bytes>>32),DWORD(bytes),nullptr);
        if(mapping)data=MapViewOfFile(mapping,FILE_MAP_ALL_ACCESS,0,0,SIZE_T(bytes));
        if(!data){if(mapping)CloseHandle(mapping);CloseHandle(file);throw Error("画像処理の一時領域を確保できません。空き容量を確認してください。");}
    }
    ~Storage(){if(data)UnmapViewOfFile(data);if(mapping)CloseHandle(mapping);if(file!=INVALID_HANDLE_VALUE)CloseHandle(file);}
    cv::Mat frame(int i){if(i<0||i>=count)throw Error("層番号が範囲外です。");return cv::Mat(size.height(),size.width(),type,static_cast<quint8*>(data)+frameBytes*i);}
};
void Bank::createStage(const QString& name,int count,QSize size,int type,QString mode){maps[name]=std::make_shared<Storage>(count,size,type,mode);stages.append(name);}
cv::Mat Bank::frame(const QString& name,int i){return maps.value(name)->frame(i);}
Image Bank::read(int i,QString stage)const{if(i<0||i>=valid.size())throw Error("層番号が範囲外です。");if(!valid[i])return {};if(stage.isEmpty())stage=stages.last();auto map=maps.value(stage);if(!map)throw Error("処理段階がありません。");return {map->frame(i).clone(),map->mode};}
void checkCancel(const Cancel& c){if(c&&c->load())throw Error("処理を中止しました。");}
void FilterConfig::validate()const{
    if(!std::isfinite(scale)||scale<.25||scale>3)throw Error("補間倍率は0.25-3にしてください。");
    if(taps<3||taps>101||taps%2==0)throw Error("FIR tap数は3-101の奇数にしてください。");
    if(!std::isfinite(cutoff)||cutoff<=0||cutoff>=.5)throw Error("NormCutOffFreqは0より大きく0.5未満にしてください。");
    if(zStep!=1&&zStep!=2)throw Error("Z参照間隔は1または2層です。");
    if(dilation<1||dilation>31)throw Error("膨張サイズは1-31にしてください。");
    if(dilation>1&&!binary)throw Error("膨張する場合は二値化も有効にしてください。");
    if(interpolation!="CPU cubic"&&interpolation!="GPU形式 A=-0.5")throw Error("補間方式が不正です。");
    if(!std::isfinite(zWeight)||!std::isfinite(threshold))throw Error("係数は有限の数値にしてください。");
}
QJsonObject FilterConfig::json()const{return {{"scale",scale},{"interpolation",interpolation},{"highpass",highpass},{"taps",taps},{"cutoff",cutoff},{"z_filter",zFilter},{"z_weight",zWeight},{"z_step",zStep},{"binary",binary},{"threshold",threshold},{"dilation",dilation}};}
FilterConfig FilterConfig::fromJson(const QJsonObject& j){FilterConfig c;c.scale=j.value("scale").toDouble(c.scale);c.interpolation=j.value("interpolation").toString(c.interpolation);c.highpass=j.value("highpass").toBool(c.highpass);c.taps=j.value("taps").toInt(c.taps);c.cutoff=j.value("cutoff").toDouble(c.cutoff);c.zFilter=j.value("z_filter").toBool(c.zFilter);c.zWeight=j.value("z_weight").toDouble(c.zWeight);c.zStep=j.value("z_step").toInt(c.zStep);c.binary=j.value("binary").toBool(c.binary);c.threshold=j.value("threshold").toDouble(c.threshold);c.dilation=j.value("dilation").toInt(c.dilation);c.validate();return c;}
BankPtr preload(const Stack& stack,Progress progress,Cancel cancel){auto bank=std::make_shared<Bank>();bank->valid.fill(false,stack.frames.size());QElapsedTimer timer;timer.start();for(int i=0;i<stack.frames.size();i++){checkCancel(cancel);auto im=stack.read(i);if(!im.empty()){if(im.mode=="P")throw Error("パレット画像の全層先読みには未対応です。通常表示は使用できます。");if(bank->stages.empty())bank->createStage("元画像",int(stack.frames.size()),im.size(),im.pixels.type(),im.mode);auto dest=bank->frame("元画像",i);if(dest.size()!=im.pixels.size()||dest.type()!=im.pixels.type())throw Error("層ごとに画像サイズ／型が異なるため、全層キャッシュを作れません。");im.pixels.copyTo(dest);bank->valid[i]=true;}if(progress)progress(i+1,int(stack.frames.size()),"全層を先読み");}if(bank->stages.empty())throw Error("保存された画像がありません。");bank->metadata={{"operation","preload"},{"elapsed_seconds",timer.elapsed()/1000.0}};return bank;}
cv::Mat firKernel(int taps,double cutoff){cv::Mat k(1,taps,CV_32F);QVector<double> d;double total=0;const double pi=std::acos(-1.);for(int i=0;i<taps;i++){double n=i-(taps-1)/2.0,x=2*cutoff*n;double sinc=x==0?1:std::sin(pi*x)/(pi*x);double v=2*cutoff*sinc*(.54-.46*std::cos(2*pi*i/(taps-1)));d.append(v);total+=v;}for(int i=0;i<taps;i++)k.at<float>(0,i)=float(d[i]/total);return k;}
static cv::Mat toByte(const cv::Mat& a){cv::Mat b(a.rows,a.cols,CV_8U);for(int y=0;y<a.rows;y++)for(int x=0;x<a.cols;x++)b.at<quint8>(y,x)=roundedByte(a.at<float>(y,x));return b;}
struct Weights {int i[4];float w[4];};
static QVector<Weights> weights(int n,int m){QVector<Weights> out(m);float ratio=float(double(n)/m);for(int x=0;x<m;x++){float p=float(x)*ratio;int ix=int(std::floor(p));float sum=0;for(int j=0;j<4;j++){int idx=ix+j-1;float d=std::abs(p-float(idx));float v=d<=1?((1.5f*d-2.5f)*d*d+1):d<2?((-.5f*d+2.5f)*d-4)*d+2:0;out[x].i[j]=std::clamp(idx,0,n-1);out[x].w[j]=v;sum+=v;}for(float& w:out[x].w)w/=sum;}return out;}
cv::Mat gpuResize(const cv::Mat& a,QSize size){auto wx=weights(a.cols,size.width()),wy=weights(a.rows,size.height());cv::Mat tmp(a.rows,size.width(),CV_32F),result(size.height(),size.width(),CV_8U);for(int y=0;y<a.rows;y++)for(int x=0;x<size.width();x++){float v=0;for(int j=0;j<4;j++)v+=float(a.at<quint8>(y,wx[x].i[j]))*wx[x].w[j];tmp.at<float>(y,x)=v;}for(int y=0;y<size.height();y++)for(int x=0;x<size.width();x++){float v=0;for(int j=0;j<4;j++)v+=tmp.at<float>(wy[y].i[j],x)*wy[y].w[j];result.at<quint8>(y,x)=roundedByte(v);}return result;}
static void afterZ(const cv::Mat& a,int i,const FilterConfig& c,const BankPtr& bank){if(!c.binary)return;auto b=bank->frame("二値化後",i);for(int y=0;y<a.rows;y++)for(int x=0;x<a.cols;x++)b.at<quint8>(y,x)=a.at<quint8>(y,x)>c.threshold?1:0;if(c.dilation>1){auto d=bank->frame("膨張後",i);cv::dilate(b,d,cv::Mat::ones(c.dilation,c.dilation,CV_8U));}}
BankPtr process(const Stack& stack,const FilterConfig& c,Progress progress,Cancel cancel){
    c.validate();QElapsedTimer timer;timer.start();auto bank=std::make_shared<Bank>();int n=int(stack.frames.size());bank->valid.fill(false,n);QStringList stages={"補間後"};if(c.highpass)stages<<"2D filter後";if(c.zFilter)stages<<"Z filter後";if(c.binary)stages<<"二値化後";if(c.binary&&c.dilation>1)stages<<"膨張後";
    int total=n*(c.zFilter?2:1),done=0;QSize shape;auto kernel=firKernel(c.taps,c.cutoff);
    for(int i=0;i<n;i++){checkCancel(cancel);auto im=stack.read(i);if(!im.empty()){
        if(im.mode!="L"||im.pixels.type()!=CV_8UC1)throw Error("画像処理は8bitグレースケール画像に対応します。16bitやカラーは通常表示・PNG保存をご利用ください。");
        QSize size(std::max(1,int(std::nearbyint(im.pixels.cols*c.scale))),std::max(1,int(std::nearbyint(im.pixels.rows*c.scale))));
        if(shape.isEmpty()){shape=size;for(auto name:stages)bank->createStage(name,n,shape,CV_8UC1,"L");}if(size!=shape)throw Error("全層で同じ画像サイズが必要です。");
        cv::Mat a=im.pixels;if(size!=im.size()){if(c.interpolation.startsWith("GPU"))a=gpuResize(a,size);else cv::resize(im.pixels,a,cv::Size(size.width(),size.height()),0,0,cv::INTER_CUBIC);}
        a.copyTo(bank->frame("補間後",i));bank->valid[i]=true;
        if(c.highpass){cv::Mat bg;cv::sepFilter2D(a,bg,CV_32F,kernel,kernel,cv::Point(-1,-1),0,cv::BORDER_REFLECT_101);cv::subtract(toByte(bg),a,a);a.copyTo(bank->frame("2D filter後",i));}
        if(!c.zFilter)afterZ(a,i,c,bank);
    }if(progress)progress(++done,total,"補間・2D filter・画像処理");}
    if(bank->stages.empty())throw Error("保存された画像がありません。");
    if(c.zFilter){QString before=c.highpass?"2D filter後":"補間後";for(int i=0;i<n;i++){checkCancel(cancel);if(bank->valid[i]){auto a=bank->frame(before,i);auto out=bank->frame("Z filter後",i);QVector<int> neighbors;for(int j:{i-c.zStep,i+c.zStep})if(j>=0&&j<n)neighbors.append(j);
        cv::Mat b;if(neighbors.size()==2){auto p=bank->valid[neighbors[0]]?bank->frame(before,neighbors[0]):a;auto q=bank->valid[neighbors[1]]?bank->frame(before,neighbors[1]):a;cv::add(p,q,b);}else if(neighbors.size()==1)b=bank->valid[neighbors[0]]?bank->frame(before,neighbors[0]):a;
        if(b.empty())a.copyTo(out);else {float w=float(c.zWeight/(neighbors.size()==2?2:1));for(int y=0;y<a.rows;y++)for(int x=0;x<a.cols;x++){float product=w*float(b.at<quint8>(y,x));float value=float(a.at<quint8>(y,x))+product;out.at<quint8>(y,x)=roundedByte(value);}}
        afterZ(out,i,c,bank);
    }if(progress)progress(++done,total,"Z filter・二値化・膨張");}}
    bank->metadata={{"operation","filter"},{"config",c.json()},{"elapsed_seconds",timer.elapsed()/1000.0},{"stage_order",QJsonArray::fromStringList(stages)},{"source_path",stack.path},{"binary_values",QJsonArray{0,1}},{"border_rule","XY reflect101; Z endpoints single neighbor at full weight; missing neighbor uses current; intermediate uint8 round-to-even and saturation"}};return bank;
}
ProjectionPtr project(Reader read,QVector<int> indices,QString kind,QString source,Progress progress,Cancel cancel){std::sort(indices.begin(),indices.end());indices.erase(std::unique(indices.begin(),indices.end()),indices.end());auto p=std::make_shared<Projection>();p->requested=indices;p->kind=kind;p->source=source;int done=0;for(int i:indices){checkCancel(cancel);auto im=read(i);if(!im.empty()){if(im.pixels.channels()!=1)throw Error("最大・最小の合成はグレースケール画像に対応します。");if(p->values.empty()){p->values=im.copy();if(p->values.mode=="P")p->values.mode="L";p->layers=cv::Mat(im.pixels.rows,im.pixels.cols,CV_32S,cv::Scalar(i));}else{if(im.pixels.size()!=p->values.pixels.size()||im.pixels.type()!=p->values.pixels.type())throw Error("重ねる画像のサイズ・型が一致していません。");cv::Mat mask;cv::compare(im.pixels,p->values.pixels,mask,kind=="max"?cv::CMP_GT:cv::CMP_LT);im.pixels.copyTo(p->values.pixels,mask);p->layers.setTo(i,mask);}p->used.append(i);}if(progress)progress(++done,int(indices.size()),kind=="max"?"最大値を合成":"最小値を合成");}if(p->values.empty())throw Error("指定範囲に保存済みの画像がありません。");return p;}
cv::Vec3f paletteValue(const QString& name,int index){static QStringList names={"kingfisher","meadow","ember","heat","rainbow"};int p=names.indexOf(name);if(p<0)throw Error("カラーパレットが不正です。");return {palette_tables[p][index][0],palette_tables[p][index][1],palette_tables[p][index][2]};}
QImage paletteBar(const QString& name,int height){QImage out(256,height,QImage::Format_RGB888);for(int y=0;y<height;y++)for(int x=0;x<256;x++){auto v=paletteValue(name,x);for(int c=0;c<3;c++)out.scanLine(y)[x*3+c]=roundedByte(float(v[c]*255.f));}return out;}
Image projectionImage(const Projection& p,bool colored,bool weighted,QString name){if(!colored)return p.values.copy();Image out;out.mode="RGB";out.pixels=cv::Mat(p.values.pixels.rows,p.values.pixels.cols,CV_8UC3);int lo=p.requested.first(),hi=p.requested.last();double max;cv::minMaxLoc(p.values.pixels,nullptr,&max);float top=max<=1?1:p.values.pixels.depth()==CV_16U?65535.f:255.f;for(int y=0;y<out.pixels.rows;y++)for(int x=0;x<out.pixels.cols;x++){int index=int(std::nearbyint(double(p.layers.at<int>(y,x)-lo)/std::max(1,hi-lo)*255));auto rgb=paletteValue(name,index);float strength=1;if(weighted){auto& a=p.values.pixels;strength=float(a.depth()==CV_16U?a.at<quint16>(y,x):a.at<quint8>(y,x))/top;strength=std::clamp(strength,0.f,1.f);if(p.kind=="min")strength=1-strength;}for(int c=0;c<3;c++)out.pixels.at<cv::Vec3b>(y,x)[c]=roundedByte(float(float(rgb[c]*strength)*255.f));}return out;}
static QJsonArray ints(const QVector<int>& values){QJsonArray a;for(int v:values)a.append(v);return a;}
QJsonObject exportLayers(Reader read,QVector<int> indices,QString folder,QString stem,QString source,std::optional<QRect> roi,QJsonObject metadata,Progress progress,Cancel cancel){
    if(QFileInfo::exists(folder)||!QDir().mkpath(folder))throw Error("出力専用フォルダを作成できません。別の名前を指定してください。");
    if(QFileInfo(stem).fileName()!=stem || stem=="." || stem=="..")throw Error("出力ファイル名が不正です。");
    QJsonArray saved,missing;QJsonObject report={{"source",source},{"requested",ints(indices)},{"metadata",metadata},{"status","running"}};report["ROI_top_left_box"]=roi?QJsonValue(QJsonArray{roi->x(),roi->y(),roi->x()+roi->width(),roi->y()+roi->height()}):QJsonValue();
    auto writeReport=[&](){report["saved"]=saved;report["missing"]=missing;QSaveFile f(QDir(folder).filePath("export_manifest.json"));if(!f.open(QIODevice::WriteOnly))throw Error(f.errorString());auto data=QJsonDocument(report).toJson();if(f.write(data)!=data.size()||!f.commit())throw Error(f.errorString());};
    try{int done=0;for(int i:indices){checkCancel(cancel);auto im=read(i);if(im.empty())missing.append(i);else{QString name=stem+QString("_z%1.png").arg(i,3,10,QChar('0'));savePng(im,QDir(folder).filePath(name),roi);auto size=roi?roi->size():im.size();saved.append(QJsonObject{{"layer",i},{"file",name},{"size",QJsonArray{size.width(),size.height()}},{"mode",im.mode}});}if(progress)progress(++done,int(indices.size()),"PNGを書き出し");}report["status"]="complete";writeReport();}
    catch(...){report["status"]=(cancel&&cancel->load())?"cancelled":"error";writeReport();throw;}
    return {{"folder",folder},{"saved",saved.size()},{"missing",missing}};
}
}
