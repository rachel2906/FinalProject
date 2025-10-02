#include "MainWindow.h"
#include "UTEpath.h"
#include <QGraphicsPixmapItem>
#include <QLabel>
#include <QStatusBar>
#include <QPixmap>
#include <QDebug>
#include <QPushButton>
#include <QVBoxLayout>
#include <QGraphicsDropShadowEffect>
#include <QTimer>
#include <QFile>
#include <QTextStream>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QPainterPath>
#include <vector>
using std::vector;


MainWindow::MainWindow() {
    QGraphicsScene *scene = new QGraphicsScene(this);
    setWindowTitle("Find the path in HCMUTE");

    // Load ảnh bản đồ
    QPixmap map("../Resources/map3.jpg");
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

        // ===== Panel giới thiệu góc trái =====
        introWidget = new QWidget(view);
        introWidget->setStyleSheet(
            "background-color: rgba(240, 240, 240, 220);"
            "border: 1px solid #999;"
            "border-radius: 8px;"
        );
        introWidget->setFixedSize(300, 150);

        QVBoxLayout *introLayout = new QVBoxLayout(introWidget);
        introLayout->setContentsMargins(10, 8, 10, 8);
        introLayout->setSpacing(6);

        // Thanh tiêu đề
        QLabel *titleLabel = new QLabel("Find the path in HCMUTE");
        titleLabel->setStyleSheet("font-weight: bold; font-size: 14px; color: #333;");
        introLayout->addWidget(titleLabel);

        // Tên thành viên
        QLabel *membersLabel = new QLabel(
            "Thành viên:\n"
            "- Huỳnh Vũ Minh Hiền\n"
            "- Nguyễn Quốc Bảo Khang\n"
            "- Lê Sĩ Nhật Khuê\n"
            "- Ngô Quang Trường"
        );
        introLayout->addWidget(membersLabel);

        introWidget->move(20, 20); // góc trái
        introWidget->show();


        // ===== Panel Start-End =====
        startEndWidget = new QWidget(view);
        startEndWidget->setStyleSheet(
            "background-color: rgba(240, 240, 240, 230);"
            "border: 1px solid #999;"
            "border-radius: 8px;"
        );
        startEndWidget->setFixedSize(300, 150);

        QVBoxLayout *panelLayout = new QVBoxLayout(startEndWidget);
        panelLayout->setContentsMargins(10, 8, 10, 8);
        panelLayout->setSpacing(6);

        // Start Edit
        startEdit = new QLineEdit();
        startEdit->setStyleSheet(
            "background-color: white;"
            "border: 1px solid #666;"
            "border-radius: 4px;"
            "padding: 4px;"
        );
        startEdit->setPlaceholderText("Start: Nhập địa điểm");
        startEdit->installEventFilter(this);
        panelLayout->addWidget(startEdit);

        // End Edit
        endEdit = new QLineEdit();
        endEdit->setStyleSheet(
            "background-color: white;"
            "border: 1px solid #666;"
            "border-radius: 4px;"
            "padding: 4px;"
        );
        endEdit->setPlaceholderText("End: Nhập địa điểm");
        endEdit->installEventFilter(this);
        panelLayout->addWidget(endEdit);

        // Find button
        findBtn = new QPushButton("Tìm đường");
        findBtn->setAutoDefault(false);
        findBtn->setDefault(false);

        findBtn->setStyleSheet(
            "QPushButton {"
            "  background-color: #4CAF50;"
            "  color: white;"
            "  border: none;"
            "  border-radius: 6px;"
            "  padding: 6px;"
            "  font-weight: bold;"
            "}"
            "QPushButton:hover {"
            "  background-color: #45a049;"
            "}"
            "QPushButton:pressed {"
            "  background-color: #3e8e41;"
            "}"
        );
        panelLayout->addWidget(findBtn);

        startEndWidget->move(view->width() - startEndWidget->width() - 20, 20);
        startEndWidget->show();

        // ===== Danh sách gợi ý =====
        suggestionList = new QListWidget(view);
        suggestionList->setStyleSheet(
            "QListWidget {"
        "   background-color: #ffffff;"
        "   border: 1px solid #aaa;"
        "   selection-background-color: #3399ff;"
        "   selection-color: black;"
        "}"
        "QScrollBar:vertical {"
        "   background: #f0f0f0;"    /* nền */
        "   width: 10px;"
        "}"
        "QScrollBar::handle:vertical {"
        "   background: #b0b0b0;"    /* nút kéo */
        "   border-radius: 4px;"
        "}"
        );
        suggestionList->setFixedWidth(startEndWidget->width());
        suggestionList->setMaximumHeight(200);
        suggestionList->hide();

        //Font
        QFont suggestionFont("Times New Roman", 11);
        suggestionList->setFont(suggestionFont);

        // Danh sách địa điểm
        QFile file("../Resources/locations.txt");
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            in.setCodec("UTF-8");   //tránh bị lỗi font khi đọc file 
            while (!in.atEnd()) {
                QString line = in.readLine().trimmed();
                if (!line.isEmpty())
                    locations << line;
            }
            file.close();
        } else {
            qDebug() << "Cannot open locations.txt!";
        }

        // Kết nối tín hiệu
        connect(findBtn, &QPushButton::clicked, this, &MainWindow::onFindPathClicked);
        connect(startEdit, &QLineEdit::textChanged, this, &MainWindow::onTextChanged);
        connect(endEdit, &QLineEdit::textChanged, this, &MainWindow::onTextChanged);
        connect(suggestionList, &QListWidget::itemClicked, this, &MainWindow::onSuggestionClicked);

        // Enter chọn khi chỉ còn 1 item
        connect(startEdit, &QLineEdit::returnPressed, this, [this]() {
            if (suggestionList->count() == 1 && activeEdit)
                activeEdit->setText(suggestionList->item(0)->text());
            suggestionList->hide();
        });
        connect(endEdit, &QLineEdit::returnPressed, this, [this]() {
            if (suggestionList->count() == 1 && activeEdit)
                activeEdit->setText(suggestionList->item(0)->text());
            suggestionList->hide();
        });

        // ===== Panel kết quả (ẩn mặc định) =====
        resultWidget = new QWidget(view);
        resultWidget->setStyleSheet(
            "background-color: rgba(255,255,255,230);"
            "border: 1px solid #666;"
            "border-radius: 6px;"
        );
        resultWidget->setFixedSize(300, 120);

        QVBoxLayout *resultLayout = new QVBoxLayout(resultWidget);
        resultLayout->setContentsMargins(8,8,8,8);

        resultLabel = new QLabel("Kết quả đường đi và thời gian sẽ hiển thị ở đây");
        resultLabel->setWordWrap(true);
        resultLayout->addWidget(resultLabel);

        resultWidget->move(view->width() - resultWidget->width() - 20, 
                        startEndWidget->y() + startEndWidget->height() + 10);
        resultWidget->hide();   // ẩn mặc định
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

    // ===== Panel nhập tọa độ =====
    coordWidget = new QWidget(view);
    coordWidget->setStyleSheet(
        "background-color: rgba(255, 255, 255, 200);"
        "border: 1px solid #666;"
        "border-radius: 6px;"
    );
    coordWidget->setFixedSize(150, 90);

    QVBoxLayout *coordLayout = new QVBoxLayout(coordWidget);
    coordLayout->setContentsMargins(5,5,5,5);
    coordLayout->setSpacing(5);

    // ô X
    xEdit = new QLineEdit();
    xEdit->setPlaceholderText("X");
    coordLayout->addWidget(xEdit);

    // ô Y
    yEdit = new QLineEdit();
    yEdit->setPlaceholderText("Y");
    coordLayout->addWidget(yEdit);

    // nút hiển thị
    showBtn = new QPushButton("Hiển thị");
    coordLayout->addWidget(showBtn);

    coordWidget->move(20, 500); // góc trái
    coordWidget->show();

    connect(showBtn, &QPushButton::clicked, [this]() {
    bool okX, okY;
    double x = xEdit->text().toDouble(&okX);
    double y = yEdit->text().toDouble(&okY);
    if (!okX || !okY) return;

    // Tạo một QGraphicsEllipseItem nhỏ làm marker
    QGraphicsEllipseItem *marker = new QGraphicsEllipseItem(0, 0, 10, 10);
    marker->setBrush(Qt::red);   // màu đỏ
    marker->setPen(Qt::NoPen);
    marker->setPos(x, y);        // tọa độ trên scene
    view->scene()->addItem(marker);
});

}

// ===== Event để hiện toàn bộ danh sách khi focus vào ô nhập =====
bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
    if(event->type() == QEvent::FocusIn) {
        if(obj==startEdit || obj==endEdit) {
            activeEdit = qobject_cast<QLineEdit*>(obj);
            if (resultWidget) resultWidget->hide(); //ẩn panel kết quả
            suggestionList->clear();
            for(const QString &loc : locations) suggestionList->addItem(loc);
            suggestionList->move(startEndWidget->x(), startEndWidget->y() + startEndWidget->height() + 5);
            suggestionList->show();
        }
    }
    return QMainWindow::eventFilter(obj,event);
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
    if (startEndWidget && view) { 
        startEndWidget->move(view->width() - startEndWidget->width() - 20, 20); // góc phải trên
    }
    if (resultWidget && view) {
        resultWidget->move(view->width() - resultWidget->width() - 20,
                           startEndWidget->y() + startEndWidget->height() + 20);
    }
    QMainWindow::resizeEvent(event);
}


// ===== Tìm đường =====
void MainWindow::onFindPathClicked() {
    QString start = startEdit->text();
    QString end = endEdit->text();
    if(start.isEmpty() || end.isEmpty()){
        QMessageBox::warning(this,"Thông báo","Vui lòng nhập cả điểm bắt đầu và kết thúc!");
        return;
    }

    vector<int> path = UTEPath::findPath(start.toStdString(), end.toStdString());
    if(path.empty()){
        QMessageBox::warning(this,"Thông báo","Không tìm thấy đường đi!");
        return;
    }

    // Xóa đường cũ
    if(currentPathItem){
        view->scene()->removeItem(currentPathItem);
        delete currentPathItem;
        currentPathItem = nullptr;
    }

    // Tạo đường mới
    QPainterPath painterPath;
    bool first = true;
    QPointF firstPoint, lastPoint; //node đầu, node cuối
    for(int id: path){
        const Node &node = UTEPath::graph.getNode(id);
        QPointF pt(node.x, node.y);
        if(first){ painterPath.moveTo(pt); firstPoint = pt; first = false; }
        else painterPath.lineTo(pt);
        lastPoint = pt; //sau vòng lặp sẽ là node cuối cùng
    }

    currentPathItem = new QGraphicsPathItem(painterPath);
    currentPathItem->setPen(QPen(Qt::red,10));
    view->scene()->addItem(currentPathItem);

    // Vẽ marker tròn cho start
    startMarker = new QGraphicsEllipseItem(-6, -6, 12, 12);
    startMarker->setBrush(Qt::black);   // xanh cho điểm bắt đầu
    startMarker->setPen(Qt::NoPen);
    startMarker->setPos(firstPoint);
    view->scene()->addItem(startMarker);

    // Vẽ marker tròn cho end
    endMarker = new QGraphicsEllipseItem(-6, -6, 12, 12);
    endMarker->setBrush(Qt::blue);      // xanh dương cho điểm kết thúc
    endMarker->setPen(Qt::NoPen);
    endMarker->setPos(lastPoint);
    view->scene()->addItem(endMarker);

    // Tính tổng quãng đường
    double totalDist = 0.0;
    for (size_t i = 1; i < path.size(); ++i) {
        const Node &a = UTEPath::graph.getNode(path[i-1]);
        const Node &b = UTEPath::graph.getNode(path[i]);
        double dx = a.x - b.x;
        double dy = a.y - b.y;
        totalDist += std::sqrt(dx*dx + dy*dy);
    }

    // Giả sử tốc độ trung bình: 80 đơn vị / phút
    double speed = 80.0;
    double travelTime = totalDist / speed;

    // Cập nhật kết quả
    QString resultText = QString("Quãng đường: %1 m\nThời gian: %2 phút")
            .arg(totalDist, 0, 'f', 1)
            .arg(travelTime, 0, 'f', 1);

    resultLabel->setText(resultText);
    resultWidget->show();
}

void MainWindow::onTextChanged(const QString &text) {
    QLineEdit *edit = qobject_cast<QLineEdit*>(sender());
    if(!edit) return;
    activeEdit = edit;
    suggestionList->clear();
    if(text.isEmpty()){ suggestionList->hide(); return; }
    for(const QString &loc: locations){
        if(loc.contains(text,Qt::CaseInsensitive)) suggestionList->addItem(loc);
    }
    if(suggestionList->count()>0){
        suggestionList->move(startEndWidget->x(), startEndWidget->y()+startEndWidget->height()+5);
        suggestionList->show();
    }else suggestionList->hide();
}

void MainWindow::onSuggestionClicked(QListWidgetItem *item) {
    if (activeEdit) {
        activeEdit->setText(item->text());
    }
    suggestionList->hide();
}


MainWindow::~MainWindow() {}
