#pragma once
#include <QtCore>
#include <QImage>
#include <opencv2/core.hpp>
#include <atomic>
#include <functional>
#include <memory>
#include <optional>

namespace spng {
struct Error : std::runtime_error { explicit Error(const QString& s):std::runtime_error(s.toUtf8().constData()){} };
struct Image {
    cv::Mat pixels;
    QString mode;
    QVector<QRgb> palette;
    bool hasTransparency=false;
    bool empty() const{return pixels.empty();}
    QSize size() const{return {pixels.cols,pixels.rows};}
    Image copy() const {auto out=*this;out.pixels=pixels.clone();return out;}
};
struct Frame {
    quint64 offset=0,length=0;
    int width=0,height=0,depth=0,color=0;
    bool missing() const{return length==0 || length==std::numeric_limits<quint64>::max();}
};
class Stack {
public:
    explicit Stack(QString path);
    QString path;
    QVector<Frame> frames;
    qint64 sizeBytes=0,mtimeMs=0;
    Image read(int index) const;
    int firstAvailable() const;
};
struct FileRecord {QString path,kind,view,layer;static FileRecord fromPath(const QString& path);};
using NamedPaths=QVector<QPair<QString,QString>>;
bool naturalLess(const QString& a,const QString& b);
NamedPaths discoverRoot(const QString& path);
NamedPaths discoverSensors(const QString& path);
QVector<FileRecord> listSensorFiles(const QString& path);
QRect normalizeRoi(int x,int y,int width,int height,QSize size);
QRect bottomLeftRoi(int x,int y,int width,int height,QSize size);
std::optional<QRect> dragRoi(QPointF start,QPointF end,QSize size);
QString savePng(const Image& image,QString path,std::optional<QRect> roi={});
QImage displayImage(const Image& image,double low=0,double high=255,bool invert=false);
QPair<double,double> defaultLevels(const Image& image);
QVector<int> parseLayers(const QString& text,int count);
struct Point {int z;double x,y;};
QPointF linePosition(Point first,Point second,int index,bool extend=true);
quint8 roundedByte(double value);
QString pixelText(const Image& image,int x,int topY);
QJsonObject imageSummary(const Image& image);
}
