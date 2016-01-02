#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QStandardItemModel>
#include "dynamicallocation.h"
#include "processinmemmodel.h"
#include "processfinmodel.h"
#include "jobmodel.h"
#include "resourcemodel.h"

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

private:
    Ui::MainWindow *ui;
    JobModel *jobModel;
    ProcessInMemModel *processModel;
    ProcessFinModel *processFinModel;
    ResourceModel *resourceModel;
};

#endif // MAINWINDOW_H
