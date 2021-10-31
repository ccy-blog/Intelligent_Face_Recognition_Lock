#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "opencvtools.h"
#include <QTimerEvent>
#include "logindialog.h"
#include <QUdpSocket>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public://成员函数
    explicit MainWindow(QWidget *parent = 0);//构造函数
    ~MainWindow();//析构函数

public://成员函数
    void timerEvent(QTimerEvent* event);//定时事件
    void showImage();//将frame显示在label中
    void hideSome();//隐藏iP 录入人脸输入等控件
    void showSome();//显示iP 录入人脸输入等控件
    void initDoorIp();//读取出ipconfig文件中的内容，显示在lineEdit_ip中

private slots:
    void on_pushButton_manage_clicked();
    void onLoginSuccess();// 对登录成功绑定的槽函数
    void onStudyFinish();//对学习完成的响应
    void onCheckResult(int label);//对sendCheckResult信号的响应

    void on_pushButton_study_clicked();

    void on_pushButton_open_clicked();

    void on_pushButton_save_clicked();

    void on_pushButton_set_clicked();

private://成员变量
    Ui::MainWindow *ui;
    OpencvTools opencvtools;//创建一个opencv工具对象
    Mat frame;//用来保存获取到的一帧
    LoginDialog* login = new LoginDialog;
    bool isStudy = false;//是否开始学习
    bool isCheck = false;//是否开始识别人脸
    QString doorIp;//用来保存门锁的IP地址
    QUdpSocket socket;//UDP 套接字
};

#endif // MAINWINDOW_H
