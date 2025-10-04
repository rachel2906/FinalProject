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

    QPushButton *startPickBtn; // Nút chọn vị trí cho Start
    QPushButton *endPickBtn;   // Nút chọn vị trí cho End
    
    // Biến trạng thái: 0: không chọn, 1: đang chọn Start, 2: đang chọn End
    int pickingState = 0; 
    bool updatingFromMap = false; // ngăn không cho suggestionList show lại khi click chuột

    QLabel *pickingNoteLabel; // Label mới để hiển thị ghi chú chọn vị trí

    // test
    QWidget *coordWidget;
    QLineEdit *xEdit;
    QLineEdit *yEdit;
    QPushButton *showBtn;
protected:
    void keyPressEvent(QKeyEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    bool eventFilter(QObject *obj, QEvent *event) override; // để phát hiện focus

    void onPickStartClicked(); // Slot cho nút chọn Start
    void onPickEndClicked();   // Slot cho nút chọn End
public:
    MainWindow();
    ~MainWindow();
private slots:
    void onFindPathClicked();
    void onTextChanged(const QString &text);
    void onSuggestionClicked(QListWidgetItem *item);

    void onMapClicked(const QPointF &scenePos); // Slot mới để nhận tọa độ click
};

#endif // MAINWINDOW_H
