#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "serialwindow.h"
#include "ui_serialwindow.h"
#include <QDebug>

#include <iostream>
using namespace std;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("Card(M1)");

    sendNum = 0;    // 发送、接收计数清零
    recvNum = 0;

    QStatusBar *sBar = statusBar();                 // 状态栏

    lblSendNum = new QLabel(this);                  // 状态栏的收、发计数标签
    lblRecvNum = new QLabel(this);

    lblSendNum->setMinimumSize(100, 20);            // 设置标签最小大小
    lblRecvNum->setMinimumSize(100, 20);

    setNumOnLabel(lblSendNum, "S: ", sendNum);
    setNumOnLabel(lblRecvNum, "R: ", recvNum);

    sBar->addPermanentWidget(lblSendNum);           // 从右往左依次添加
    sBar->addPermanentWidget(lblRecvNum);

    win = new serialwindow;                         //串口设置界面
    ui->widget_2->setVisible(false);
    ui->widget->setVisible(false);

    serialport = new QSerialPort(this);             // 新建一串口对象
    connect(serialport, SIGNAL(readyRead()), this, SLOT(serialPortRead_Slot()));    // 串口接收，信号槽关联

    //将button全部关联在一个槽函数
    QSignalMapper *pushButtonMapper;
    pushButtonMapper = new QSignalMapper;
    QPushButton *button[11]={ui->pushButton_UID,ui->pushButton_autoFind,ui->pushButton_autoFind_close,
                             ui->pushButton_read1,ui->pushButton_read2,ui->pushButton_read3,ui->pushButton_read4,
                             ui->pushButton_write1,ui->pushButton_write2,ui->pushButton_write3,ui->pushButton_write4};
    for (int i =0 ;i<11;i++)
    {
        connect(button[i],SIGNAL(clicked(bool)),pushButtonMapper,SLOT(map()));
        pushButtonMapper->setMapping(button[i],i);
    }
    connect(pushButtonMapper,SIGNAL(mapped(int)),this,SLOT(sendCommand(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

//串口设置窗口打开
void MainWindow::on_pushButton_serialport_set_clicked()
{
     win->show();
}

//读命令抽屉按键
void MainWindow::on_pushButton_read_clicked()
{
    if(iswidget2==true)
    {
        ui->widget_2->setVisible(false);
        iswidget2 = false;
        return;
    }
    ui->widget_2->setVisible(true);
    iswidget2 = true;
}

//写命令抽屉按键
void MainWindow::on_pushButton_write_clicked()
{
    if(iswidget==true)
    {
        ui->widget->setVisible(false);
        iswidget = false;
        return;
    }
    ui->widget->setVisible(true);
    iswidget = true;
}

//串口接收
void MainWindow::serialPortRead_Slot()
{
    QByteArray recBuf;
    recBuf = serialport->readAll();
    recvNum += recBuf.size();                           // 接收字节计数
    setNumOnLabel(lblRecvNum, "R: ", recvNum);          // 状态栏显示计数值
    QString str1 = recBuf.toHex().toUpper();
    QString str2;
    for(int i=0;i<str1.length();i+=2)
    {
        str2 += str1.mid (i,2);
        str2 += " ";
    }
    ui->textEdit->insertPlainText(str2+ '\n');
    ui->textEdit->moveCursor(QTextCursor::End);                 // 移动光标到文本结尾
}

//串口发送
void MainWindow::on_pushButton_send_clicked()
{
    QByteArray sendData = QByteArray::fromHex(ui->lineEdit->text().toUtf8());
    serialport->write(sendData);
    sendNum += sendData.size();
    setNumOnLabel(lblSendNum,"S:",sendNum);
}

//按键指令
void MainWindow::sendCommand(int index)
{

    QVector<QString> commandS = {"AA0101","AA0495FF1402","AA0495001402",                //操作命令
                                 "AA020401","AA020402","AA020403","AA020404",
                                "AA120501","AA120502","AA120503","AA120504"} ;
    QByteArray command = QByteArray::fromHex(commandS[index].toUtf8());
    QByteArray sendData = QByteArray::fromHex(ui->lineEdit->text().toUtf8());           //输入框数据
    if(index < 7)
    {
        serialport->write(command);
        sendNum += command.size();
        setNumOnLabel(lblSendNum,"S:",sendNum);
    }
    else if(index < 11 )
    {
       serialport->write(command+sendData);                         //命令和数据一起发送
       sendNum += (command+sendData).size();
       setNumOnLabel(lblSendNum,"S:",sendNum);
    }
}


//输入框自动空格,并排除非十六进制数
void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{
    QString HexBuff = "0123456789ABCDEFabcdef";
    int flag = 0;
    for(int j=0;j<=HexBuff.length();j++)
    {
        if(arg1.length() != 0)                                          //在删除最后一个字符串时字符串长度为0，不判断会导致奔溃
        {
            if(arg1[arg1.length()-1] == HexBuff[j])
                flag = 1;
        }
    }
    if(flag)
    {
        flag = 0;
        if(arg1.length() < 2 )
        {
            QString on_lineEdit_textChanged_to2 = arg1.toUpper();
            ui->lineEdit->setText(on_lineEdit_textChanged_to2);
            return;
        }
        QString on_lineEdit_textChanged_Temp = arg1.toUpper();          //将读到的字符转换为大写
                                                                        //去掉字符中所有的空格
        string on_lineEdit_textChanged_Temp_String = on_lineEdit_textChanged_Temp.toStdString();
        string::iterator it;
        for (it = on_lineEdit_textChanged_Temp_String.begin(); it < on_lineEdit_textChanged_Temp_String.end(); it++)
        {
            if (*it == ' ')
            {
                on_lineEdit_textChanged_Temp_String.erase(it);
                it--;
            }
        }
        on_lineEdit_textChanged_Temp = QString::fromStdString(on_lineEdit_textChanged_Temp_String);
                                                                        //每两个字符插入空格，只需在输入奇数字符时插入空格
        int mo = (on_lineEdit_textChanged_Temp.length()/2);
        int yu = (on_lineEdit_textChanged_Temp.length()%2);
        if(yu)
        {
            for(int i = 1; i <= mo; i++)
            {
                on_lineEdit_textChanged_Temp.insert(((2*i)+(i-1))," ");
            }
        }
        else
        {
            QString Buff_Temp = arg1.toUpper();
            ui->lineEdit->setText(Buff_Temp);
            return;
        }
            ui->lineEdit->setText(on_lineEdit_textChanged_Temp);
    }
    else
    {
        QString Delete_End_QString = arg1.left(arg1.length()-1);        //删除最后一个字符,不再我的QString内的字符
        ui->lineEdit->setText(Delete_End_QString);
    }
}


//接收清除
void MainWindow::on_pushButton_dataClear_clicked()
{
    ui->textEdit->clear();
}

//发送清除
void MainWindow::on_pushButton_sendClear_clicked()
{
    ui->lineEdit->clear();
}

// 状态栏标签显示计数值
void MainWindow::setNumOnLabel(QLabel *lbl, QString strS, long num)
{
    QString strN;
    strN.sprintf("%ld", num);
    QString str = strS + strN;
    lbl->setText(str);
}
