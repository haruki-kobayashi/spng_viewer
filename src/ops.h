#pragma once
#include "core.h"
namespace spng {
struct FilterConfig {
    double scale=1.0;QString interpolation="GPU形式 A=-0.5";
    bool highpass=true;int taps=15;double cutoff=.1;
    bool zFilter=true;double zWeight=-.2;int zStep=1;
    bool binary=true;double threshold=9;int dilation=2;
    void validate() const;
    QJsonObject json() const;
    static FilterConfig fromJson(const QJsonObject& j);
};
using Cancel=std::shared_ptr<std::atomic_bool>;
using Progress=std::function<void(int,int,QString)>;
using Reader=std::function<Image(int)>;
void checkCancel(const Cancel& cancel);
class Storage;
class Bank {
public:
    QStringList stages;QVector<bool> valid;QJsonObject metadata;
    QHash<QString,std::shared_ptr<Storage>> maps;
    Image read(int index,QString stage={}) const;
    void createStage(const QString& name,int count,QSize size,int type,QString mode);
    cv::Mat frame(const QString& name,int index);
};
using BankPtr=std::shared_ptr<Bank>;
BankPtr preload(const Stack& stack,Progress progress={},Cancel cancel={});
BankPtr process(const Stack& stack,const FilterConfig& config,Progress progress={},Cancel cancel={});
cv::Mat firKernel(int taps,double cutoff);
cv::Mat gpuResize(const cv::Mat& input,QSize size);
struct Projection {Image values;cv::Mat layers;QVector<int> requested,used;QString kind,source;};
using ProjectionPtr=std::shared_ptr<Projection>;
ProjectionPtr project(Reader read,QVector<int> indices,QString kind="max",QString source="元画像",Progress progress={},Cancel cancel={});
cv::Vec3f paletteValue(const QString& name,int index);
QImage paletteBar(const QString& name,int height=22);
Image projectionImage(const Projection& projection,bool colored=false,bool weighted=false,QString palette="kingfisher");
QJsonObject exportLayers(Reader read,QVector<int> indices,QString folder,QString stem,QString source="元画像",std::optional<QRect> roi={},QJsonObject metadata={},Progress progress={},Cancel cancel={});
}
