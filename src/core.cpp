#include "core.h"
#include <cmath>
#include <algorithm>
#include <QSaveFile>

namespace spng {
static const QByteArray signature=QByteArray::fromHex("89504e470d0a1a0a");
static const QRegularExpression fileRe("^(.+)_(\\d{8})_(\\d+)_(\\d+)\\.spng$",QRegularExpression::CaseInsensitiveOption);
static quint32 be32(const char* p){return qFromBigEndian<quint32>(p);}
static void append32(QByteArray& a,quint32 v){char b[4];qToBigEndian(v,b);a.append(b,4);}
static quint32 crc(const QByteArray& a){quint32 c=~quint32(0);for(unsigned char b:a){c^=b;for(int k=0;k<8;k++)c=(c>>1)^(0xedb88320u & (0u-(c&1u)));}return ~c;}
static void chunk(QByteArray& out,const char* name,const QByteArray& data){append32(out,quint32(data.size()));QByteArray body(name,4);body+=data;out+=body;append32(out,crc(body));}
quint8 roundedByte(double v){return quint8(std::nearbyint(std::clamp(v,0.0,255.0)));}
Stack::Stack(QString p):path(QFileInfo(p).absoluteFilePath()){
    QFile f(path);if(!f.open(QIODevice::ReadOnly))throw Error(f.errorString());
    sizeBytes=f.size();mtimeMs=QFileInfo(path).lastModified().toMSecsSinceEpoch();
    while(f.pos()<sizeBytes){auto at=f.pos();auto header=f.read(8);if(header.size()!=8)throw Error(QString("層ヘッダが途中で切れています: byte %1").arg(at));
        Frame r;r.length=qFromLittleEndian<quint64>(header.constData());r.offset=f.pos();
        if(!r.missing()){
            if(r.length<33 || r.length>quint64(sizeBytes)-r.offset)throw Error(QString("PNGデータの長さが不正です: 層 %1, byte %2").arg(frames.size()).arg(at));
            auto ihdr=f.read(33);
            if(ihdr.size()!=33 || ihdr.first(8)!=signature || ihdr.mid(12,4)!="IHDR" || be32(ihdr.constData()+8)!=13)throw Error(QString("PNGのヘッダが不正です: 層 %1").arg(frames.size()));
            auto w=be32(ihdr.constData()+16),h=be32(ihdr.constData()+20);
            if(!w || !h || w>INT_MAX || h>INT_MAX)throw Error("画像サイズが不正です。");
            r.width=int(w);r.height=int(h);r.depth=quint8(ihdr[24]);r.color=quint8(ihdr[25]);
            if(!f.seek(qint64(r.offset+r.length)))throw Error(f.errorString());
        }
        frames.append(r);
    }
    if(frames.empty())throw Error("画像が1枚もありません。");
}
int Stack::firstAvailable()const{for(int i=0;i<frames.size();i++)if(!frames[i].missing())return i;return 0;}
Image Stack::read(int index)const{
    if(index<0 || index>=frames.size())throw Error("層番号が範囲外です。");
    auto r=frames[index];if(r.missing())return {};
    if(r.depth==16 && r.color!=0)throw Error("16bitカラー／alpha付き16bit PNGは未対応です。画素値を保つため変換せず停止しました。16bitグレースケールは開けます。");
    QFile f(path);if(!f.open(QIODevice::ReadOnly)||!f.seek(qint64(r.offset)))throw Error(f.errorString());
    QByteArray bytes=f.read(qint64(r.length));if(quint64(bytes.size())!=r.length)throw Error("画像の読み込み途中でファイル長が変化しました。開き直してください。");
    QImage q=QImage::fromData(bytes,"PNG");if(q.isNull())throw Error("PNGを展開できません。");
    Image out;
    if(r.color==0 && r.depth==16){out.mode="I;16";q=q.convertToFormat(QImage::Format_Grayscale16);out.pixels=cv::Mat(r.height,r.width,CV_16UC1);for(int y=0;y<r.height;y++)memcpy(out.pixels.ptr(y),q.constScanLine(y),r.width*2);}
    else if(r.color==0 && r.depth==1){out.mode="1";out.pixels=cv::Mat(r.height,r.width,CV_8UC1);for(int y=0;y<r.height;y++)for(int x=0;x<r.width;x++)out.pixels.at<quint8>(y,x)=qGray(q.pixel(x,y))?1:0;}
    else if(r.color==0){out.mode="L";q=q.convertToFormat(QImage::Format_Grayscale8);out.pixels=cv::Mat(r.height,r.width,CV_8UC1);for(int y=0;y<r.height;y++)memcpy(out.pixels.ptr(y),q.constScanLine(y),r.width);}
    else if(r.color==3){out.mode="P";out.palette=q.colorTable();out.hasTransparency=std::any_of(out.palette.begin(),out.palette.end(),[](auto c){return qAlpha(c)!=255;});
        if(q.format()!=QImage::Format_Indexed8 && q.format()!=QImage::Format_Mono && q.format()!=QImage::Format_MonoLSB)throw Error("パレット画像のインデックスを保持できません。");
        out.pixels=cv::Mat(r.height,r.width,CV_8UC1);for(int y=0;y<r.height;y++)for(int x=0;x<r.width;x++)out.pixels.at<quint8>(y,x)=quint8(q.pixelIndex(x,y));}
    else{int c=r.color==4?2:r.color==6?4:3;out.mode=c==2?"LA":c==4?"RGBA":"RGB";out.pixels=cv::Mat(r.height,r.width,CV_MAKETYPE(CV_8U,c));q=q.convertToFormat(QImage::Format_RGBA8888);
        for(int y=0;y<r.height;y++)for(int x=0;x<r.width;x++){auto s=q.constScanLine(y)+4*x;auto d=out.pixels.ptr(y)+c*x;if(c==2){d[0]=s[0];d[1]=s[3];}else memcpy(d,s,c);}}
    return out;
}
FileRecord FileRecord::fromPath(const QString& p){auto m=fileRe.match(QFileInfo(p).fileName());if(m.hasMatch())return {p,m.captured(1),QString::number(m.captured(2).toLongLong()),"Layer"+m.captured(3)};return {p,"その他のファイル",QFileInfo(p).completeBaseName(),"指定なし"};}
bool naturalLess(const QString&a,const QString&b){QCollator c;c.setNumericMode(true);c.setCaseSensitivity(Qt::CaseInsensitive);return c.compare(a,b)<0;}
NamedPaths discoverRoot(const QString& p){QDir d(p);if(!d.exists())throw Error("フォルダが見つかりません。");QMap<QString,QString> paths;for(auto e:d.entryInfoList(QDir::Dirs|QDir::NoDotAndDotDot)){auto name=e.fileName().toUpper();if(name=="IMAGE"||name=="DATA")paths[name]=e.absoluteFilePath();}NamedPaths out;for(auto k:{QString("IMAGE"),QString("DATA")})if(paths.contains(k))out.append({k,paths[k]});if(out.empty())out.append({d.dirName().isEmpty()?p:d.dirName(),d.absolutePath()});return out;}
NamedPaths discoverSensors(const QString& p){QDir d(p);if(!d.exists())throw Error("フォルダが見つかりません。");QRegularExpression re("^\\d{2}_\\d{2}$");NamedPaths out;bool direct=false;for(auto e:d.entryInfoList(QDir::AllEntries|QDir::NoDotAndDotDot)){if(e.isDir()&&re.match(e.fileName()).hasMatch())out.append({e.fileName(),e.absoluteFilePath()});if(e.isFile()&&e.suffix().compare("spng",Qt::CaseInsensitive)==0)direct=true;}std::sort(out.begin(),out.end(),[](auto a,auto b){return naturalLess(a.first,b.first);});if(direct||out.empty())out.prepend({re.match(d.dirName()).hasMatch()?d.dirName():"このフォルダ",d.absolutePath()});return out;}
QVector<FileRecord> listSensorFiles(const QString& p){QDir d(p);if(!d.exists())throw Error("フォルダが見つかりません。");QVector<FileRecord> out;for(auto e:d.entryInfoList(QDir::Files))if(e.suffix().compare("spng",Qt::CaseInsensitive)==0)out.append(FileRecord::fromPath(e.absoluteFilePath()));std::sort(out.begin(),out.end(),[](auto a,auto b){return naturalLess(QFileInfo(a.path).fileName(),QFileInfo(b.path).fileName());});return out;}
QRect normalizeRoi(int x,int y,int w,int h,QSize size){if(w<=0||h<=0)throw Error("幅と高さは1 pixel以上にしてください。");if(x<0||y<0||qint64(x)+w>size.width()||qint64(y)+h>size.height())throw Error(QString("範囲が画像の外です。画像サイズは %1 × %2 pixelです。").arg(size.width()).arg(size.height()));return {x,y,w,h};}
QRect bottomLeftRoi(int x,int y,int w,int h,QSize size){normalizeRoi(x,y,w,h,size);return {x,size.height()-y-h,w,h};}
std::optional<QRect> dragRoi(QPointF a,QPointF b,QSize size){int x0=std::clamp(int(std::floor(std::min(a.x(),b.x()))),0,size.width()),y0=std::clamp(int(std::floor(std::min(a.y(),b.y()))),0,size.height());int x1=std::clamp(int(std::ceil(std::max(a.x(),b.x()))),0,size.width()),y1=std::clamp(int(std::ceil(std::max(a.y(),b.y()))),0,size.height());if(x1<=x0||y1<=y0)return {};return QRect(x0,y0,x1-x0,y1-y0);}
QString savePng(const Image& image,QString path,std::optional<QRect> roi){
    if(image.empty())throw Error("保存する画像がありません。");
    QFileInfo info(path);if(info.suffix().compare("png",Qt::CaseInsensitive)!=0)path=info.dir().filePath(info.completeBaseName()+".png");
    cv::Mat a=image.pixels;if(roi){auto r=normalizeRoi(roi->x(),roi->y(),roi->width(),roi->height(),image.size());a=a(cv::Rect(r.x(),r.y(),r.width(),r.height()));}
    int depth=image.mode=="1"?1:(a.depth()==CV_16U?16:8);int color=image.mode=="P"?3:image.mode=="LA"?4:image.mode=="RGBA"?6:image.mode=="RGB"?2:0;
    QByteArray out=signature,ihdr;append32(ihdr,a.cols);append32(ihdr,a.rows);ihdr.append(char(depth));ihdr.append(char(color));ihdr.append(QByteArray(3,0));chunk(out,"IHDR",ihdr);
    if(color==3){QByteArray plte,alpha;for(auto c:image.palette){plte+=char(qRed(c));plte+=char(qGreen(c));plte+=char(qBlue(c));alpha+=char(qAlpha(c));}chunk(out,"PLTE",plte);if(image.hasTransparency)chunk(out,"tRNS",alpha);}
    QByteArray raw;qsizetype row=depth==1?(a.cols+7)/8:qsizetype(a.cols)*a.elemSize();if(row>INT_MAX||qint64(a.rows)*(row+1)>INT_MAX)throw Error("PNG出力の画像サイズが大きすぎます。");raw.reserve((row+1)*a.rows);
    for(int y=0;y<a.rows;y++){raw+=char(0);if(depth==1){QByteArray packed(row,0);for(int x=0;x<a.cols;x++)if(a.at<quint8>(y,x))packed[x/8]=char(quint8(packed[x/8])|(0x80>>(x%8)));raw+=packed;}
        else if(depth==16){auto s=a.ptr<quint16>(y);for(int x=0;x<a.cols;x++){raw+=char(s[x]>>8);raw+=char(s[x]&255);}}
        else raw.append(reinterpret_cast<const char*>(a.ptr(y)),row);}
    chunk(out,"IDAT",qCompress(raw,6).mid(4));chunk(out,"IEND",{});
    QSaveFile f(path);f.setDirectWriteFallback(false);if(!f.open(QIODevice::WriteOnly)||f.write(out)!=out.size()||!f.commit())throw Error(f.errorString());return path;
}
QImage displayImage(const Image& im,double low,double high,bool invert){
    if(im.empty())return {};auto a=im.pixels;int channels=im.mode=="P"?(im.hasTransparency?4:3):im.mode=="LA"?4:a.channels();
    QImage out(a.cols,a.rows,channels==1?QImage::Format_Grayscale8:channels==4?QImage::Format_RGBA8888:QImage::Format_RGB888);high=std::max(high,low+1e-9);
    for(int y=0;y<a.rows;y++)for(int x=0;x<a.cols;x++)for(int c=0;c<channels;c++){
        double v;
        if(im.mode=="P"){auto p=im.palette.value(a.at<quint8>(y,x));v=c==0?qRed(p):c==1?qGreen(p):c==2?qBlue(p):qAlpha(p);}
        else if(im.mode=="LA")v=a.ptr<quint8>(y)[2*x+(c==3?1:0)];
        else v=a.depth()==CV_16U?a.ptr<quint16>(y)[x*channels+c]:a.ptr<quint8>(y)[x*channels+c];
        auto b=(c==3&&channels==4)?quint8(v):roundedByte((v-low)*(255.0/(high-low)));if(invert&&!(channels==4&&c==3))b=255-b;out.scanLine(y)[x*channels+c]=b;
    }return out;
}
QPair<double,double> defaultLevels(const Image& im){if(im.mode=="P")return {0,255};if(im.mode=="1")return {0,1};if(im.pixels.channels()==1&&!im.empty()){double max;cv::minMaxLoc(im.pixels,nullptr,&max);if(max<=1)return {0,1};}return {0,im.pixels.depth()==CV_16U?65535.0:255.0};}
QVector<int> parseLayers(const QString& text,int count){if(text.trimmed().isEmpty())throw Error("層番号を指定してください。例: 0-15,20,25-31:2");QSet<int> chosen;QRegularExpression re("^\\s*(\\d+)\\s*(?:[-〜～]\\s*(\\d+)\\s*(?::\\s*(\\d+)\\s*)?)?$");for(auto p:QString(text).replace(QChar(0xff0c),',').split(',')){auto m=re.match(p);if(!m.hasMatch())throw Error("層の指定が不正です。例: 0-15,20,25-31:2（両端を含む）");bool ok;auto a=m.captured(1).toLongLong(&ok);if(!ok)throw Error("層番号が範囲外です。");auto b=m.captured(2).isEmpty()?a:m.captured(2).toLongLong();auto step=m.captured(3).isEmpty()?1:m.captured(3).toLongLong();if(a>b||step<1||b>=count)throw Error(QString("層番号は0-%1、開始≦終了、間隔は1以上にしてください。").arg(count-1));for(qint64 i=a;i<=b;i+=step)chosen.insert(int(i));}QVector<int> out(chosen.begin(),chosen.end());std::sort(out.begin(),out.end());return out;}
QPointF linePosition(Point a,Point b,int index,bool extend){if(a.z==b.z)throw Error("異なる層に2点を指定してください。");double t=double(index-a.z)/(b.z-a.z);if(!extend)t=std::clamp(t,0.0,1.0);return {a.x+t*(b.x-a.x),a.y+t*(b.y-a.y)};}
QString pixelText(const Image& im,int x,int y){QStringList values;for(int c=0;c<im.pixels.channels();c++)values.append(QString::number(im.pixels.depth()==CV_16U?im.pixels.ptr<quint16>(y)[x*im.pixels.channels()+c]:im.pixels.ptr<quint8>(y)[x*im.pixels.channels()+c]));return values.size()==1?values[0]:"("+values.join(", ")+")";}
QJsonObject imageSummary(const Image& im){return {{"width",im.pixels.cols},{"height",im.pixels.rows},{"mode",im.mode},{"channels",im.pixels.channels()},{"bytes",double(im.pixels.total()*im.pixels.elemSize())}};}
}
