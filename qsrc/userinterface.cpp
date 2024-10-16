//
// Created by CaoMengxuan on 2024/9/19.
//

#include "userinterface.hpp"
#include <QCheckBox>
#include <QComboBox>
#include <QFileDialog>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QtConcurrent/QtConcurrent>

// 异步处理图像检测
QPixmap processImageAsync(const std::string& modelPath, const std::string& imagePath, putImg& imgPutter, QTextEdit* resultTextEdit, QLabel* label) {
    imgPutter.setIsGPU(true);
    imgPutter.setModelPath(modelPath);
    imgPutter.setClassNamesPath("coco.names");
    imgPutter.setConfThreshold(0.25);
    imgPutter.setIouThreshold(0.45);

    // 处理图像
    imgPutter.setImagePath(imagePath);
    QImage show_img = imgPutter.getImage();
    QPixmap pixmap = QPixmap::fromImage(show_img);

    return pixmap;
}

UserInterface::UserInterface(QWidget *parent) : QWidget(parent) {
    setupUi(this);
    setWindowTitle("超级无敌吊炸天的C++目标检测项目");
}

UserInterface::~UserInterface() {}

void UserInterface::setupUi(QWidget *UserInterface) {
    //创建一个页面，包含一个QStringList,用于选择不同的模型,比如Yolov5m和s
    QStringList modelList;
    modelList << "Yolov5m.onnx"
              << "Yolov5s.onnx";
    QComboBox *model_comboBox = new QComboBox(UserInterface);
    model_comboBox->addItems(modelList);
    model_comboBox->setCurrentIndex(0);

    //创建一个QLabel，用于显示选择的模型
    QLabel *model_label = new QLabel(UserInterface);
    model_label->setText("Selected Model: ");
    //创建一个QPushButton，用于选择图片文件
    QPushButton *select_image_button = new QPushButton("Select Image", UserInterface);

    //创建一个QLabel，用于显示图片
    //创建一个QPushButton，用于选择视频文件
    QPushButton *select_video_button = new QPushButton("Select Video", UserInterface);

    //创建一个QLabel，用于显示视频文件或图片文件的路径
    QLabel *video_path_label = new QLabel(UserInterface);
    video_path_label->setText("Video Path: ");

    //创建一个QPushButton，用于开始检测
    QPushButton *start_detect_button = new QPushButton("Start Detect", UserInterface);
    //创建一个QPushButton，用于停止检测
    QPushButton *stop_detect_button = new QPushButton("Stop Detect", UserInterface);

    //创建一个QLabel，用于显示检测结果
    QLabel *matLabel = new QLabel(UserInterface);

    //创建一个QTextEdit，用于显示检测结果
    QTextEdit *detect_result_textEdit = new QTextEdit(UserInterface);
    detect_result_textEdit->setReadOnly(true);

    //是否使用GPU
    QCheckBox *use_gpu_checkbox = new QCheckBox("Use GPU", UserInterface);

    //创建布局，将其放在布局之中
    QGridLayout *layout = new QGridLayout(UserInterface);
    layout->addWidget(model_label, 0, 0);
    layout->addWidget(model_comboBox, 0, 1);
    layout->addWidget(select_image_button, 1, 0);
    layout->addWidget(select_video_button, 1, 1);
    layout->addWidget(video_path_label, 2, 0);
    layout->addWidget(start_detect_button, 3, 0);
    layout->addWidget(stop_detect_button, 3, 1);
    layout->addWidget(use_gpu_checkbox, 2, 1);
    layout->addWidget(matLabel, 4, 0);
    layout->addWidget(detect_result_textEdit, 4, 1);

    //默认勾选上
    connect(use_gpu_checkbox, &QCheckBox::stateChanged, [=](int state){
        imgPutter.setIsGPU(state == Qt::Checked);
        device=state;
    });

    connect(select_video_button, &QPushButton::clicked, [=]() {
        //选择视频文件，并显示路径
        QString fileName = QFileDialog::getOpenFileName(UserInterface, "Open Video", "", "Video Files (*.mp4 *.avi *.mkv)");
        video_path_label->setText(fileName);
    });

    connect(select_image_button, &QPushButton::clicked, [=]() {
        // 选择图片文件，并显示路径
        QString fileName = QFileDialog::getOpenFileName(UserInterface, "Open Image", "", "Image Files (*.png *.jpg *.bmp)");

        // 加载图片
        QImage image(fileName);

        // 创建一个QPixmap对象
        QPixmap pixmap = QPixmap::fromImage(image);

        // 根据图片大小设置QLabel的大小策略
        QSizePolicy policy;
        policy.setHorizontalPolicy(QSizePolicy::Preferred);
        policy.setVerticalPolicy(QSizePolicy::Preferred);
        policy.setHeightForWidth(true); // 设置高度随宽度变化

        matLabel->setSizePolicy(policy);
        matLabel->setPixmap(pixmap);

        // 更新QLabel的高度以匹配宽度
        matLabel->adjustSize();

        // 显示文件路径
        model_label->setText(fileName);
    });

    // 主窗口中的按钮点击事件
    connect(start_detect_button, &QPushButton::clicked, this, [=]() {
        detect_result_textEdit->clear();
        // 获取选择的模型名称
        model_name = model_comboBox->currentText();

        // 获取选择的图片路径
        image_path = model_label->text();
        // 获取选择的视频路径
        video_path = video_path_label->text();
        // 判断是否选择了图片或视频
        if (image_path.isEmpty() && video_path.isEmpty()) {
            detect_result_textEdit->setText("Please select an image or video.");
            return;
        }

        //imgPutter.setIsGPU(device);
        imgPutter.setModelPath(model_name.toStdString());
        imgPutter.setClassNamesPath("coco.names");
        imgPutter.setConfThreshold(0.25);
        imgPutter.setIouThreshold(0.45);

        // 判断是否选择了图片
        if (!image_path.isEmpty()) {
            // 创建QFutureWatcher来监听异步任务
            QFutureWatcher<QPixmap>* watcher = new QFutureWatcher<QPixmap>(this);

            // 连接信号和槽
            connect(watcher, &QFutureWatcher<QPixmap>::finished, this, [this, watcher, &matLabel, &detect_result_textEdit]() {
                // 获取异步任务的结果
                QPixmap pixmap = watcher->result();

                // 更新QLabel
                matLabel->setPixmap(pixmap);
                std::cout << "image path: " << image_path.toStdString() << std::endl;

                // 显示检测结果
                detect_result_textEdit->setText("Detect image success.");

                // 清理watcher
                delete watcher;
            });

            // 异步处理图像
            QFuture<QPixmap> future = QtConcurrent::run(
                    [&]{
                        return processImageAsync(model_name.toStdString(), image_path.toStdString(), imgPutter, detect_result_textEdit, matLabel);
                    });
            watcher->setFuture(future);

            // 显示检测结果
            detect_result_textEdit->setText("Detecting image...");
        }
        // 判断是否选择了视频
        else if (!video_path.isEmpty()) {
            // 调用detect_video函数，传入选择的模型名称和视频路径
            // detect_video(model_name, video_path);
            //todo
        }
    });
    UserInterface->setLayout(layout);

    //增加css
    model_comboBox->setStyleSheet("QComboBox { color: #000000; background-color: #FFFFFF; border: 1px solid #CCCCCC; border-radius: 4px; padding: 2px 4px; }");
    select_video_button->setStyleSheet("QPushButton { color: #FFFFFF; background-color: #4CAF50; border: 2px solid #4CAF50; border-radius: 4px; padding: 6px 12px; }");
    start_detect_button->setStyleSheet("QPushButton { color: #FFFFFF; background-color: #4CAF50; border: 2px solid #4CAF50; border-radius: 4px; padding: 6px 12px; }");
    stop_detect_button->setStyleSheet("QPushButton { color: #FFFFFF; background-color: #F44336; border: 2px solid #F44336; border-radius: 4px; padding: 6px 12px; }");
    matLabel->setStyleSheet("QTextEdit { color: #000000; background-color: #FFFFFF; border: 1px solid #CCCCCC; border-radius: 4px; padding: 2px 4px; }");
    video_path_label->setStyleSheet("QLabel { color: #000000; background-color: #FFFFFF; border: 1px solid #CCCCCC; border-radius: 4px; padding: 2px 4px; }");
    model_label->setStyleSheet("QLabel { color: #000000; background-color: #FFFFFF; border: 1px solid #CCCCCC; border-radius: 4px; padding: 2px 4px; }");
    select_image_button->setStyleSheet("QPushButton { color: #FFFFFF; background-color: #4CAF50; border: 2px solid #4CAF50; border-radius: 4px; padding: 6px 12px; }");
    detect_result_textEdit->setStyleSheet("QTextEdit { color: #000000; background-color: #FFFFFF; border: 1px solid #CCCCCC; border-radius: 4px; padding: 2px 4px; }");
}
