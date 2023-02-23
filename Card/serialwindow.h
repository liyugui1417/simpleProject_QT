#ifndef SERIALWINDOW_H
#define SERIALWINDOW_H

#include <QWidget>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QCloseEvent>

extern QSerialPort *serialport;        //定义串口指针
namespace Ui {
class serialwindow;
}

class serialwindow : public QWidget
{
    Q_OBJECT

public:
    explicit serialwindow(QWidget *parent = nullptr);
    ~serialwindow();

    //QSerialPort *serialport;        //定义串口指针
    QStringList portStringLine;     //串口端口链表

private slots:
    void on_pushButton_refresh_clicked();           //串口扫描

    void on_pushButton_serialport_open_clicked();   //串口初始化，打开串口


public:
    Ui::serialwindow *ui;

};

#endif // SERIALWINDOW_H
