#include "../Headers/MapView.h"
#include <QWheelEvent>
#include <QKeyEvent>
#include <QDebug> 

#include <QGraphicsScene>


MapView::MapView(QWidget *parent) : QGraphicsView(parent) {
    setDragMode(QGraphicsView::ScrollHandDrag);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
}

void MapView::wheelEvent(QWheelEvent *event) {
    const double scaleFactor = 1.15;
    double currentScale = transform().m11();

    if (event->angleDelta().y() > 0 && currentScale < 5.0) {
        scale(scaleFactor, scaleFactor);
    } else if (event->angleDelta().y() < 0 && currentScale > minScale) {
        scale(1.0 / scaleFactor, 1.0 / scaleFactor);
        if (transform().m11() < minScale) { // giữ không nhỏ hơn minScale
            QTransform t;
            t.scale(minScale, minScale);
            setTransform(t);
        }
    }
}

void MapView::keyPressEvent(QKeyEvent *event) {
    const double scaleFactor = 1.15;
    double currentScale = transform().m11();

    if ((event->key() == Qt::Key_Plus || event->key() == Qt::Key_Equal) && currentScale < 5.0) {
        scale(scaleFactor, scaleFactor);
    } else if (event->key() == Qt::Key_Minus) {
        resetTransform();
    } else {
        QGraphicsView::keyPressEvent(event);
    }
}

/*
void MapView::mousePressEvent(QMouseEvent *event) {
    QPointF scenePos = mapToScene(event->pos());
    qDebug() << "Clicked at:" << scenePos;   // in ra tọa độ khi click
    QGraphicsView::mousePressEvent(event);
}
*/
void MapView::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) { // Chỉ xử lý click chuột trái
        QPointF scenePos = mapToScene(event->pos());
        // Phát tín hiệu kèm tọa độ scene
        emit mapClicked(scenePos); 
        // Không gọi QGraphicsView::mousePressEvent(event) nếu đang chờ chọn 
        // để tránh xung đột với chức năng kéo bản đồ (ScrollHandDrag)
    }
    QGraphicsView::mousePressEvent(event); // Vẫn gọi để xử lý kéo/thả
}
