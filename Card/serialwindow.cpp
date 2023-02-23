#include "serialwindow.h"
#include "ui_serialwindow.h"

QSerialPort *serialport;       //实例化串口对象
serialwindow::serialwindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::serialwindow)
{
    ui->setupUi(this);

    setWindowTitle("设置");
    //serialport = new QSerialPort;       //实例化串口对象

}

serialwindow::~serialwindow()
{
    delete ui;
}


void serialwindow::on_pushButton_refresh_clicked()      //扫描串口，显示串口名和设备信息
{
    ui->comboBox_number->clear();
    portStringLine.clear();
    foreach(const QSerialPortInfo &info , QSerialPortInfo::availablePorts())
    {
        QString serialportinfo = info.portName() + ":" + info.description();
        portStringLine << serialportinfo;
    }
    ui->comboBox_number->addItems(portStringLine);
}

void serialwindow::on_pushButton_serialport_open_clicked()
{
    QString portname = ui->comboBox_number->currentText();              //设置串口名
    portname = portname.section(':',0,0);
    serialport->setPortName(portname);
    serialport->setBaudRate(ui->comboBox_baud->currentText().toUInt()); //设置波特率
    switch(ui->comboBox_data->currentText().toUInt())                   //设置数据位
    {
    case 5:serialport->setDataBits(QSerialPort::Data5);break;
    case 6:serialport->setDataBits(QSerialPort::Data6);break;
    case 7:serialport->setDataBits(QSerialPort::Data7);break;
    case 8:serialport->setDataBits(QSerialPort::Data8);break;
    default:serialport->setDataBits(QSerialPort::UnknownDataBits);break;
    }
    switch(ui->comboBox_stop->currentIndex())                           //设置停止位
    {
    case 0:serialport->setStopBits(QSerialPort::OneStop);break;
    case 1:serialport->setStopBits(QSerialPort::OneAndHalfStop);break;
    case 2:serialport->setStopBits(QSerialPort::TwoStop);break;
    default:serialport->setStopBits(QSerialPort::UnknownStopBits);break;
    }
    switch (ui->comboBox_check->currentIndex())                         //设置校验位
    {
    case 0:serialport->setParity(QSerialPort::NoParity);break;
    case 1:serialport->setParity(QSerialPort::OddParity);break;
    case 2:serialport->setParity(QSerialPort::EvenParity);break;
    default:serialport->setParity(QSerialPort::UnknownParity);break;
    }
    serialport->setFlowControl(QSerialPort::NoFlowControl);             //设置流控制
    if(ui->pushButton_serialport_open->text() == "打开串口")             //设置串口启动
    {
        if(serialport->open(QIODevice::ReadWrite) == true)
        {
            ui->pushButton_serialport_open->setText("关闭串口");
            ui->comboBox_number->setEnabled(false);
            ui->comboBox_baud->setEnabled(false);
            ui->comboBox_data->setEnabled(false);
            ui->comboBox_stop->setEnabled(false);
            ui->comboBox_check->setEnabled(false);
        }
        else
        {
            QMessageBox::critical(this,"失败提示","串口打开失败！\r\n检查串口是否被占用",QMessageBox::Cancel);
        }
    }
    else
    {
        serialport->close();
        ui->pushButton_serialport_open->setText("打开串口");
        ui->comboBox_number->setEnabled(true);
        ui->comboBox_baud->setEnabled(true);
        ui->comboBox_data->setEnabled(true);
        ui->comboBox_stop->setEnabled(true);
        ui->comboBox_check->setEnabled(true);
    }
}

