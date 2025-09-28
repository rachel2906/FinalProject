#include "MainWindow.h"
#include <QLabel>
#include <QStatusBar>
#include <QPixmap>
#include <QDebug>
#include <QPushButton>
#include <QVBoxLayout>
#include <QGraphicsDropShadowEffect>
#include <QTimer>

// ================= MapView =================
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

// ================= MainWindow =================
MainWindow::MainWindow() {
    QGraphicsScene *scene = new QGraphicsScene(this);
    setWindowTitle("Find the path in HCMUTE");

    // Load ảnh bản đồ
    QPixmap map("D:/Discrete Mathematics and Graph Theory/Project/Resources/map3.jpg");
    if (map.isNull()) {
        qDebug() << "Failed to load map!";
    } else {
        // Thêm pixmap vào scene
        QGraphicsPixmapItem *mapItem = scene->addPixmap(map);
        scene->setSceneRect(map.rect());

        // Tạo view và set scene (nhưng CHƯA gọi fitInView ở đây)
        view = new MapView(this);
        view->setScene(scene);
        setCentralWidget(view);

        // Chờ tới khi view/window đã được show -> rồi fitInView (đảm bảo scale đúng)
        QTimer::singleShot(0, this, [this, scene, mapItem]() {
            // Fit toàn bộ map vào view với tỉ lệ giữ nguyên
            view->fitInView(mapItem, Qt::KeepAspectRatio);
            view->scale(1.4, 1.4); //phóng to thêm

            // Lưu lại minScale bằng scale hiện tại (để dùng làm giới hạn thu nhỏ)
            view->minScale = view->transform().m11();
            initialTransform = view->transform(); //lưu transform gốc để reset
            qDebug() << "minScale set to:" << view->minScale;
        });
    }

    // ===== Nút Zoom nổi =====
    zoomWidget = new QWidget(view);
    zoomWidget->setStyleSheet(
        "background-color: rgba(0, 0, 0, 180);"
        "border-radius: 12px;"
    );

    QPushButton *zoomInBtn = new QPushButton("+", zoomWidget);
    QPushButton *zoomOutBtn = new QPushButton("-", zoomWidget);

    QString btnStyle = R"(
        QPushButton {
            color: white;
            font-size: 22px;
            font-weight: bold;
            border: none;
            background-color: transparent;
        }
        QPushButton:hover {
            background-color: rgba(255, 255, 255, 30);
        }
        QPushButton:pressed {
            background-color: rgba(255, 255, 255, 50);
        }
    )";

    zoomInBtn->setStyleSheet(btnStyle);
    zoomOutBtn->setStyleSheet(btnStyle);

    zoomInBtn->setFixedSize(50, 50);
    zoomOutBtn->setFixedSize(50, 50);

    QVBoxLayout *btnLayout = new QVBoxLayout(zoomWidget);
    btnLayout->addWidget(zoomInBtn);
    btnLayout->addWidget(zoomOutBtn);
    btnLayout->setSpacing(0);
    btnLayout->setContentsMargins(0, 0, 0, 0);

    zoomWidget->move(view->width() - 70, 20);
    zoomWidget->show();

    // Hiệu ứng bóng nổi
    auto *shadow = new QGraphicsDropShadowEffect(zoomWidget);
    shadow->setBlurRadius(15);
    shadow->setOffset(3, 3);
    zoomWidget->setGraphicsEffect(shadow);

    // ===== Status bar hiển thị tên thành viên =====
    QLabel *statusLabel = new QLabel("by Huỳnh Vũ Minh Hiền, Nguyễn Quốc Bảo Khang, Lê Sĩ Nhật Khuê, Ngô Quang Trường");
    statusLabel->setAlignment(Qt::AlignCenter);
    statusBar()->addWidget(statusLabel, 1);

    // Kết nối nút
    connect(zoomInBtn, &QPushButton::clicked, [this]() {
        double currentScale = view->transform().m11();
        if (currentScale < 5.0) {
            view->scale(1.15, 1.15);
            // Giới hạn lại nếu vượt quá 5.0
            if (view->transform().m11() > 5.0) {
                QTransform t;
                t.scale(5.0, 5.0);
                view->setTransform(t);
            }
        }
    });
    connect(zoomOutBtn, &QPushButton::clicked, [this]() {
        view->setTransform(initialTransform);
    });
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape) {
        close();
    } else {
        QMainWindow::keyPressEvent(event);
    }
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    if (zoomWidget && view) {
        int x = view->width() - zoomWidget->width() - 20;   // 20 px từ cạnh phải
        int y = view->height() - zoomWidget->height() - 20; // 20 px từ cạnh dưới
        zoomWidget->move(x, y);
    }
    QMainWindow::resizeEvent(event);
}


MainWindow::~MainWindow() {}
