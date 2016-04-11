#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    // 管理程序的所有资源
    QApplication a(argc, argv);
    // 创建一个主窗口
    MainWindow w;
    w.show();
    // 事件循环，并且响应用户的动作
    return a.exec();
}
