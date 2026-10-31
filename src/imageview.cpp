#include "imageview.h"
#include <QScrollBar>
#include <cmath>
namespace spng {
ImageView::ImageView(QWidget* parent):QGraphicsView(parent){
    setScene(new QGraphicsScene(this));pixmapItem=new QGraphicsPixmapItem;scene()->addItem(pixmapItem);roiItem=new QGraphicsRectItem;
    QPen pen(QColor("#ffb448"),2);pen.setCosmetic(true);roiItem->setPen(pen);roiItem->setBrush(Qt::NoBrush);roiItem->setZValue(10);scene()->addItem(roiItem);roiItem->hide();
    setBackgroundBrush(QColor("#202834"));setRenderHint(QPainter::SmoothPixmapTransform,false);setTransformationAnchor(AnchorUnderMouse);setResizeAnchor(AnchorViewCenter);setMouseTracking(true);viewport()->setMouseTracking(true);setFocusPolicy(Qt::StrongFocus);
}
void ImageView::setImage(const QImage& image,bool reset){pixmapItem->setPixmap(QPixmap::fromImage(image));imageSize=image.size();if(!trackingActive)scene()->setSceneRect(QRectF(QPointF(0,0),imageSize));if(reset){setRoi({},false);fitMode=true;}if(fitMode)fitAll();}
void ImageView::clearImage(){pixmapItem->setPixmap(QPixmap());imageSize={};setRoi({},false);scene()->setSceneRect({});emit cursorChanged({},false);}
void ImageView::fitAll(){trackingActive=false;if(!imageSize.isEmpty())scene()->setSceneRect(QRectF(QPointF(0,0),imageSize));fitMode=true;if(!imageSize.isEmpty()){fitInView(pixmapItem->boundingRect(),Qt::KeepAspectRatio);emit zoomChanged(transform().m11()*100);}}
void ImageView::actualSize(){fitMode=false;resetTransform();emit zoomChanged(100);}
void ImageView::setZoom(double percent){fitMode=false;double current=transform().m11(),target=std::clamp(percent/100,.02,32.);scale(target/current,target/current);emit zoomChanged(target*100);}
void ImageView::fitRoi(){if(roi){fitMode=false;fitInView(QRectF(*roi).adjusted(-2,-2,2,2),Qt::KeepAspectRatio);emit zoomChanged(transform().m11()*100);}}
void ImageView::setRoi(std::optional<QRect> rect,bool notify){if(!rect&&notify){pickMode.reset();unsetCursor();}roi=rect;if(roi){roiItem->setRect(QRectF(*roi));roiItem->show();}else roiItem->hide();if(notify)emit roiChanged();}
void ImageView::centerOnPixel(double x,double y){if(imageSize.isEmpty())return;fitMode=false;trackingActive=true;double z=std::max(.001,transform().m11());double mx=viewport()->width()/z+std::abs(x)+2,my=viewport()->height()/z+std::abs(y)+2;scene()->setSceneRect(-mx,-my,imageSize.width()+2*mx,imageSize.height()+2*my);centerOn(x+.5,imageSize.height()-.5-y);}
void ImageView::armPoint(int tag){pickMode=tag;setCursor(Qt::CrossCursor);}
void ImageView::resizeEvent(QResizeEvent* event){QGraphicsView::resizeEvent(event);emit viewportResized();if(fitMode)fitAll();}
void ImageView::wheelEvent(QWheelEvent* event){int delta=event->angleDelta().y();if(!delta)return;if(event->modifiers()&Qt::ControlModifier)setZoom(transform().m11()*100*(delta>0?1.2:1/1.2));else emit layerStep(delta>0?-1:1);event->accept();}
void ImageView::mousePressEvent(QMouseEvent* event){
    if(!imageSize.isEmpty()&&pickMode&&event->button()==Qt::LeftButton&&!(event->modifiers()&Qt::ControlModifier)){auto p=mapToScene(event->position().toPoint());if(p.x()>=0&&p.x()<imageSize.width()&&p.y()>=0&&p.y()<imageSize.height()){int tag=*pickMode;pickMode.reset();unsetCursor();emit pointPicked(tag,int(std::floor(p.x())),imageSize.height()-1-int(std::floor(p.y())));event->accept();return;}}
    if(!imageSize.isEmpty()&&event->button()==Qt::LeftButton&&(event->modifiers()&Qt::ControlModifier)){auto p=mapToScene(event->position().toPoint());if(p.x()>=0&&p.x()<imageSize.width()&&p.y()>=0&&p.y()<imageSize.height()){dragStart=p;setCursor(Qt::CrossCursor);event->accept();return;}}
    else if(!imageSize.isEmpty()&&(event->button()==Qt::LeftButton||event->button()==Qt::RightButton||event->button()==Qt::MiddleButton)){panStart=event->position();setCursor(Qt::ClosedHandCursor);event->accept();return;}
    QGraphicsView::mousePressEvent(event);
}
void ImageView::mouseMoveEvent(QMouseEvent* event){auto p=mapToScene(event->position().toPoint());emit cursorChanged(p,true);if(panStart){auto d=event->position()-*panStart;horizontalScrollBar()->setValue(horizontalScrollBar()->value()-int(std::nearbyint(d.x())));verticalScrollBar()->setValue(verticalScrollBar()->value()-int(std::nearbyint(d.y())));panStart=event->position();emit cursorChanged(mapToScene(event->position().toPoint()),true);event->accept();return;}if(dragStart){setRoi(dragRoi(*dragStart,p,imageSize));event->accept();return;}QGraphicsView::mouseMoveEvent(event);}
void ImageView::mouseReleaseEvent(QMouseEvent* event){if(panStart){panStart.reset();unsetCursor();event->accept();return;}if(dragStart){setRoi(dragRoi(*dragStart,mapToScene(event->position().toPoint()),imageSize));dragStart.reset();unsetCursor();event->accept();return;}QGraphicsView::mouseReleaseEvent(event);}
void ImageView::leaveEvent(QEvent* event){emit cursorChanged({},false);QGraphicsView::leaveEvent(event);}
}
