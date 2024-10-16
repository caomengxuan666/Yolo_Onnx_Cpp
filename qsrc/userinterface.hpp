//
// Created by CaoMengxuan on 2024/9/19.
//

#ifndef YOLO_ORT_USERINTERFACE_HPP
#define YOLO_ORT_USERINTERFACE_HPP

#include <QWidget>
#include "putImg.hpp"


QT_BEGIN_NAMESPACE
namespace Ui {
    class UserInterface;
}
QT_END_NAMESPACE

class UserInterface : public QWidget {
    Q_OBJECT

public:
    explicit UserInterface(QWidget *parent = nullptr);
    ~UserInterface() override;

private:
    void setupUi(QWidget *UserInterface);
    Ui::UserInterface *ui;
    int device;

    QString model_name;
    QString image_path;
    QString video_path;
    putImg imgPutter;
};


#endif//YOLO_ORT_USERINTERFACE_HPP
