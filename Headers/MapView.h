#ifndef MAPVIEW_H
#define MAPVIEW_H

#include <QGraphicsView>
#include <QWheelEvent>
#include <QKeyEvent>

#include <QGraphicsLineItem>
#include <vector>
#include <string>
#include <QPen>

class MapView : public QGraphicsView {
    Q_OBJECT
protected:
    void wheelEvent(QWheelEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    //void mousePressEvent(QMouseEvent *event) override;
public:
    MapView(QWidget *parent = nullptr);
    
    double minScale = 0.5; // map3 bị kích thước nên chỉnh lại

};

#endif
