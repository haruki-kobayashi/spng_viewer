#pragma once
#include "core.h"
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QMouseEvent>
#include <QWheelEvent>
namespace spng {
class ImageView:public QGraphicsView {
    Q_OBJECT
public:
    explicit ImageView(QWidget* parent=nullptr);
    QGraphicsPixmapItem* pixmapItem;
    QGraphicsRectItem* roiItem;
    QSize imageSize;
    std::optional<QRect> roi;
    std::optional<QPointF> dragStart,panStart;
    std::optional<int> pickMode;
    bool fitMode=true,trackingActive=false;
    void setImage(const QImage& image,bool reset=false);
    void clearImage();
    void fitAll();
    void actualSize();
    void setZoom(double percent);
    void fitRoi();
    void setRoi(std::optional<QRect> rect,bool notify=true);
    void centerOnPixel(double x,double y);
    void armPoint(int tag);
signals:
    void roiChanged();
    void cursorChanged(QPointF point,bool valid);
    void layerStep(int delta);
    void zoomChanged(double percent);
    void pointPicked(int tag,int x,int y);
    void viewportResized();
protected:
    void resizeEvent(QResizeEvent* event)override;
    void wheelEvent(QWheelEvent* event)override;
    void mousePressEvent(QMouseEvent* event)override;
    void mouseMoveEvent(QMouseEvent* event)override;
    void mouseReleaseEvent(QMouseEvent* event)override;
    void leaveEvent(QEvent* event)override;
};
}
