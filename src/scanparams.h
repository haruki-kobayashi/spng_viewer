#pragma once
#include "ops.h"
namespace spng {
int imageLayer(const QString& path);
QString acquisitionDirectory(const QString& image);
QString findParameterFile(const QString& image);
struct ScanSettings {QString source;int layer;FilterConfig config;double thresholdBase=0,thresholdPlus=0;QString algorithm;QJsonObject metadata()const;};
ScanSettings readSettings(const QString& path,int layer);
}
