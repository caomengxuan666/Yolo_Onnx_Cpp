//
// Created by CaoMengxuan on 2024/9/19.
//
#include <QApplication>
#include <userinterface.hpp>

int main(int argc,char**argv){
    QApplication app(argc,argv);
    UserInterface ui;
    ui.show();
    return app.exec();
}