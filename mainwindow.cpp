#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtCore>
#include <QStandardItemModel>
#include <iostream>
#include <fstream>
#include <QFile>
#include "findfirstfit.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    /* load the stylesheet */
    QFile qssf(":qdarkstyle/style.qss");
    if (!qssf.exists())
    {
        std::cout << "Unable to set stylesheet, file not found" << std::endl;
    }
    else
    {
        qssf.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&qssf);
        this->setStyleSheet(ts.readAll());
    }

    /* init and load data */
    jobModel = new JobModel();
    processModel = new ProcessInMemModel();
    processFinModel = new ProcessFinModel();
    resourceModel = new ResourceModel;
    mem.setMemfit(new FindFirstFit());
    processFinModel->setProcess(&processModel->fplist);
    std::fstream f;
    f.open("test.txt");
    if(!f)
        std::cout << "open file fail" << std::endl;;
    unsigned int tapeNum,memory;

    f >> memory >> tapeNum;
    mem.mm_init(memory);
    resourceModel->tape.init(tapeNum);
    jobModel->init(f,resourceModel->tape);
    /* model */

    /* view */
    ui->JobView->setModel(jobModel);
    ui->processView->setModel(processModel);
    ui->finView->setModel(processFinModel);
    ui->resourceView->setModel(resourceModel);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_startButton_clicked()
{
    //start
    PCB cur_process;
    stop = !stop;
    if(stop)
        ui->startButton->setText("开始");
    else
        ui->startButton->setText("暂停");
    while(1)
    {
        while(stop)
        {
            QTime dieTime = QTime::currentTime().addMSecs(250);
            while( QTime::currentTime() < dieTime )
                QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        }
        jobModel->schedule(*processModel,resourceModel->getTime());
        if(processModel->schedule(cur_process))
        {
            cur_process.running(*processModel,resourceModel->getTime());
            std::cout << resourceModel->getTime() << " : "  << *cur_process.name << " :running" << std::endl;
        }
        else
        {
            std::cout << "no running" << std::endl;
        }
        resourceModel->incTime();
        processModel->updateView();
        processFinModel->updateView();
        jobModel->updateView();
        resourceModel->updateView();
        QTime dieTime = QTime::currentTime().addMSecs(250);
        while( QTime::currentTime() < dieTime )
            QCoreApplication::processEvents(QEventLoop::AllEvents);
    }
}
