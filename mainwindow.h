#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QStandardItemModel>
#include "process.h"
#include "dynamicallocation.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_startButton_clicked();

    void on_pauseButton_clicked();

private:
    Ui::MainWindow *ui;
    JobList *jobModel;
    ProcessList *processModel;
    Resource *resource;
    bool stop = false;
};

#endif // MAINWINDOW_H
