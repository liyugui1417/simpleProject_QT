#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_reset_clicked();     //复位

    void on_pushButton_start_clicked();     //开始

    void on_pushButton_stop_clicked();      //停止

    void roolTimerTimeOut();                //定时

private:
    void initNameList();                    //名字初始化

    QString getRandomName();                //得到随机名

    QTimer *roolTimer;
private:
    Ui::MainWindow *ui;

    QVector<QString> allNameList;
};
#endif // MAINWINDOW_H
