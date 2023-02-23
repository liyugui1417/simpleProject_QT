#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "serialwindow.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QString>
#include <QTimer>
#include <QPainter>
#include <QComboBox>
#include <QMouseEvent>

#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QStringList>
#include <QSignalMapper>
#include <QVector>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    serialwindow *win;                          //串口设置窗口

    bool isNewWidget = false;
    bool iswidget = false;
    bool iswidget2 = false;

    long sendNum, recvNum;          // 发送、接收字节计数
    QLabel *lblSendNum;
    QLabel *lblRecvNum;
    void setNumOnLabel(QLabel *lbl, QString strS, long num);

private slots:
    void on_pushButton_serialport_set_clicked();    //串口设置打开

    void on_pushButton_read_clicked();              //读命令抽屉

    void on_pushButton_write_clicked();             //写命令抽屉

    void serialPortRead_Slot();                     //串口接收

    void on_pushButton_send_clicked();              //串口发送

    void on_lineEdit_textChanged(const QString &arg1);      //发送框自动空格，删除最后一个字符会导致程序崩溃

    void sendCommand(int index);                    //发送命令

    void on_pushButton_dataClear_clicked();

    void on_pushButton_sendClear_clicked();


};
#endif // MAINWINDOW_H
