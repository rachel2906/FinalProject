#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <MapView.h>
#include <QMainWindow>
#include <QKeyEvent>
#include <QTransform>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QGraphicsPathItem>
#include <QPolygonF>
#include <vector>


class MainWindow : public QMainWindow {
    Q_OBJECT
private:
    MapView *view;
    QWidget *zoomWidget;
    QTransform initialTransform; //reset về ban đầu

    QWidget *startEndWidget;    //panel tìm kiếm
    QLineEdit *startEdit;      // ô nhập liệu Start
    QLineEdit *endEdit;        // ô nhập liệu End
    QPushButton *findBtn;
    QWidget *introWidget;     //panel giới thiệu
    QWidget *resultWidget;
    QLabel *resultLabel;

    QListWidget *suggestionList;  // danh sách gợi ý
    QStringList locations;        // tất cả địa điểm có trong map
    QLineEdit *activeEdit;        // đang gõ vào ô nào (start/end)

    QGraphicsPathItem* currentPathItem = nullptr;  // Đường đi hiện tại
    QGraphicsEllipseItem *startMarker = nullptr;
    QGraphicsEllipseItem *endMarker = nullptr;

    // test
    QWidget *coordWidget;
    QLineEdit *xEdit;
    QLineEdit *yEdit;
    QPushButton *showBtn;
protected:
    void keyPressEvent(QKeyEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    bool eventFilter(QObject *obj, QEvent *event) override; // để phát hiện focus
public:
    MainWindow();
    ~MainWindow();
private slots:
    void onFindPathClicked();
    void onTextChanged(const QString &text);
    void onSuggestionClicked(QListWidgetItem *item);
};

#endif // MAINWINDOW_H
