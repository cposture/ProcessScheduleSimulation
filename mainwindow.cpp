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
    /* 加载 stylesheet */
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

    /* 初始化和加载数据 */

    /* 模型 */
    jobModel = new JobModel();
    processModel = new ProcessInMemModel();
    processFinModel = new ProcessFinModel();
    resourceModel = new ResourceModel;

    processFinModel->setProcess(&processModel->fplist);

    // 读取外部的Job文件
    std::fstream f;
    f.open("test.txt");
    if(!f)
        std::cout << "open file fail" << std::endl;;

    unsigned int tapeNum,memory;

    f >> memory >> tapeNum;

    // 根据用户在外部Job文件指定的memory大小申请内存
    Memory::getInstance()->mm_init(memory);

    // mem 策略模式，设置查找空闲内存的方法
    Memory::getInstance()->setMemfit(new FindFirstFit());

    resourceModel->tape.init(tapeNum);
    jobModel->init(f,resourceModel->tape);

    /* 模型，对应界面的4个表格 */
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

void MainWindow::on_addJobButton_clicked()
{
    jobModel->insertJob(PCB(&resourceModel->tape));
    jobModel->updateView();
}

void MainWindow::on_delJobButton_clicked()
{
    QModelIndex index = ui->JobView->currentIndex();
    int row = index.row();
    jobModel->deleteJob(row);
    jobModel->updateView();
}
