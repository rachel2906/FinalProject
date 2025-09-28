#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QWheelEvent>

// ================= MapView =================
class MapView : public QGraphicsView {
    Q_OBJECT
protected:
    void wheelEvent(QWheelEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
public:
    MapView(QWidget *parent = nullptr);
    
    double minScale = 0.5; // map3 bị kích thước nên chỉnh lại
};

// ================= MainWindow =================
class MainWindow : public QMainWindow {
    Q_OBJECT
private:
    MapView *view;
    QWidget *zoomWidget;
    QTransform initialTransform; //reset về ban đầu
protected:
    void keyPressEvent(QKeyEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
public:
    MainWindow();
    ~MainWindow();
};

#endif // MAINWINDOW_H
