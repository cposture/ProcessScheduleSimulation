#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtCore>
#include <QStandardItemModel>
#include <iostream>
#include <fstream>
#include "process.h"
#include "dynamicallocation.h"
#include "findfirstfit.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /* init and load data */
    jobModel = new JobList();
    processModel = new ProcessList();
    resource = new Resource;
    mem.setMemfit(new FindFirstFit());

    std::fstream f;
    f.open("test.txt");
    if(!f)
        std::cout << "open file fail" << std::endl;;
    unsigned int tapeNum,memory;

    f >> memory >> tapeNum;
    mem.mm_init(memory);
    resource->tape.init(tapeNum);
    jobModel->init(f,resource->tape);
    /* model */

    /* view */
    ui->JobView->setModel(jobModel);
    ui->processView->setModel(processModel);
    ui->resourceView->setModel(resource);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_startButton_clicked()
{
    //start
    PCB cur_process;
    while(1)
    {
        jobModel->schedule(*processModel,resource->getTime());
        if(processModel->schedule(cur_process))
        {
            cur_process.running(*processModel,resource->getTime());
            std::cout << resource->getTime() << " : "  << *cur_process.name << " :running" << std::endl;
        }
        else
        {
            char c;
            std::cout << "no running" << std::endl;
            std::cin >> c;
            break;
        }
        resource->incTime();
        QTime dieTime = QTime::currentTime().addMSecs(1000);
            while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
}

void MainWindow::on_pauseButton_clicked()
{
    stop = !stop;
}
