#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QTime>
#include <QTimer>
#include <QFile>
#include <QIODevice>
#include <QTextCodec>
#include <QPainter>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->on_pushButton_reset_clicked();            //启动初始化

    this->roolTimer = new QTimer(this);
    connect(roolTimer,&QTimer::timeout,this,&MainWindow::roolTimerTimeOut);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_reset_clicked()      //复位函数
{
    qDebug() << "复位键按下" ;
    ui->label_message->setText("幸运儿会是你吗？");
    ui->label_rollNames->setText("滚动标签");
    this->initNameList();
}

void MainWindow::on_pushButton_start_clicked()      //开始函数
{
    qDebug() << "开始键按下" ;
    if(roolTimer->isActive() == false )
    {
        roolTimer->start(50);
    }
}

void MainWindow::on_pushButton_stop_clicked()       //停止函数
{
    qDebug() << "停止键按下" ;
    if(roolTimer->isActive() == true )
    {
        roolTimer->stop();
    }
    QString currentName = ui->label_rollNames->text();
    QString showMessage = "恭喜" + currentName + "!!";
    ui->label_message->setText(showMessage);
}

void MainWindow::roolTimerTimeOut()
{
    QString name = this->getRandomName();
    ui->label_rollNames->setText(name);
}

void MainWindow::initNameList()             //参与姓名初始化
{
    this->allNameList.clear();

    this->allNameList.append(QString("zhangsan"));          //直接定义添加姓名
    this->allNameList.append(QString("lisi"));
    this->allNameList.append(QString("wangwu"));
    this->allNameList.append(QString("zhao"));
    this->allNameList.append(QString("qian"));
    this->allNameList.append(QString("sun"));
    this->allNameList.append(QString("li"));

    QFile file("D:/Qt/Demo/CallName_System/memberName.txt");     //从文件中读取姓名，每行字符串为一个姓名
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&file);
    in.setCodec("UTF-8");
    QString line ;
    while(!in.atEnd())
    {
        line = in.readLine();
        this->allNameList.append(line);
        qDebug() << line;
    }
    qDebug() << "allNameList" << allNameList;
    qDebug() << "姓名列表长度为：" << allNameList.length();
}

QString MainWindow::getRandomName()
{
    int namelenth = allNameList.length();
    QTime time= QTime::currentTime();
    qDebug() << time;
    qsrand(time.msec()+time.second()*1000);
    int rand = qrand() % namelenth;
    QString randomName = this ->allNameList[rand];
    return randomName;

}
