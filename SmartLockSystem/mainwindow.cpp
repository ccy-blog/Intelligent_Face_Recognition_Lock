#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDebug>
#include "daofactory.h"
#include "persondao.h"
#include <QFile>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    hideSome();//将管理界面的一些控件隐藏
    initDoorIp();//读取上次设置的IP地址

    //信号与槽绑定
    connect(login,SIGNAL(sendLoginSuccess()),this,SLOT(onLoginSuccess()));
    connect(&opencvtools,SIGNAL(sendStudyFinish()),this,SLOT(onStudyFinish()));
    connect(&opencvtools,SIGNAL(sendCheckResult(int)),this,SLOT(onCheckResult(int)));


    startTimer(30);//启动定时器，每隔50ms，进入一次timerEvent事件，也就是执行一次timerEvent函数

}

MainWindow::~MainWindow()
{
    delete ui;
}

//定时事件，启动定时器后，每隔一段时间，就会进入一次事件
//就会执行一次timerEvent函数
void MainWindow::timerEvent(QTimerEvent *event)
{
    //1.获取一帧图像
    frame = opencvtools.getOneFrame();
    if(isStudy)
    {
        opencvtools.study(ui->lineEdit_name->text());//开始学习
    }
    else if(isCheck)
    {
        //调用识别人脸的方法
        opencvtools.check();
    }
    //2.将frame显示在label中
    showImage();

}

void MainWindow::showImage()
{
    //1.bgr-->rgb
    Mat rgb;
    cvtColor(frame,rgb,CV_BGR2RGB);
    //2.沿y轴翻转图像
    flip(rgb,rgb,1);
    //3.将opencv中的Mat--->QT中的QImage
    QImage img(rgb.data,rgb.cols,rgb.rows,rgb.cols*rgb.channels(),QImage::Format_RGB888);
    //4.将QImage显示在label中
    ui->label_show->setPixmap(QPixmap::fromImage(img));
    //5.重置label的大小与QImage大小一样
    ui->label_show->resize(img.size());
}

//隐藏一些控件
void MainWindow::hideSome()
{
    ui->lineEdit_ip->hide();
    ui->lineEdit_name->hide();
    ui->pushButton_set->hide();
    ui->pushButton_study->hide();
    ui->pushButton_save->hide();
}
//显示一些控件
void MainWindow::showSome()
{
    ui->lineEdit_ip->show();
    ui->lineEdit_name->show();
    ui->pushButton_set->show();
    ui->pushButton_study->show();
    ui->pushButton_save->show();
}

//读取ipconfig中保存的IP地址
void MainWindow::initDoorIp()
{
    QFile file("ipconfig");
    if(!file.exists())//如果文件不存在
    {
        qDebug()<<"ipconfig文件不存在";
        return;
    }
    //如果能执行到这，说明文件存在
    file.open(QIODevice::ReadOnly);//只读方式打开
    QByteArray data = file.readAll();//将所有内容读取出来
    doorIp = QString(data);
    ui->lineEdit_ip->setText(doorIp);//将上次设置的IP地址，显示在lineEdit中
    file.close();//关闭文件

}

//点击管理按钮
void MainWindow::on_pushButton_manage_clicked()
{
    this->hide();//MainWindow隐藏
    login->show();//显示登录界面
}

//槽函数，与LoginDialog发送的成功信号绑定
void MainWindow::onLoginSuccess()
{
    login->close();//关闭登录界面
    this->show();//显示MainWindow
    showSome();//将管理界面的一些控件显示出来
}

//学习完成的槽函数
void MainWindow::onStudyFinish()
{
    isStudy = false;//学习完成，停止学习
    ui->lineEdit_name->clear();//清空输入的人脸名字
    QMessageBox::warning(this,"成功","录入人脸完毕!!");
}

//槽函数 对预测结果进行判断
void MainWindow::onCheckResult(int label)
{
    if(label == -1)//未识别到
    {
        qDebug()<<"未识别到!!";
        ui->label_showFaceName->clear();
    }
    else//识别到对应的人脸
    {
        PersonDao* dao = DaoFactory::getInstance()->getPersonDao();
        PersonEntity e(label,"");
        QString name = "";
        dao->selectPersonInfoById(e,name);//通过ID查找到人脸的名字
        qDebug()<<"识别到:"<<name;
        ui->label_showFaceName->setText(name+"开门");
        socket.writeDatagram("open",QHostAddress(doorIp),20000);//发送open字符串给门锁
        QMessageBox::warning(this,"提示",name+"开门成功!!");
        isCheck = false;
        ui->label_showFaceName->clear();
    }
}
//点击录入按钮
void MainWindow::on_pushButton_study_clicked()
{
    QString name = ui->lineEdit_name->text();
    if(name.isEmpty())//如果是空字符串
    {
        QMessageBox::warning(this,"提示","忘记输入录入的名字!!");
        return;
    }
    isStudy = true;//开始学习
}

//点击开门按钮
void MainWindow::on_pushButton_open_clicked()
{
    isCheck = true;
}

//点击保存按钮
void MainWindow::on_pushButton_save_clicked()
{
    hideSome();//将管理界面隐藏起来
}

//点击设置按钮
void MainWindow::on_pushButton_set_clicked()
{
    doorIp = ui->lineEdit_ip->text();//获取IP地址
    if(doorIp.isEmpty())
    {
        QMessageBox::warning(this,"提示","忘记输入设置门锁IP地址");
        return;
    }
    QFile file("ipconfig");
    file.open(QIODevice::WriteOnly|QIODevice::Truncate);//只写，文件存在清空，不存在创建
    file.write(doorIp.toLocal8Bit());//写入文件
    file.close();//关闭文件
    QMessageBox::warning(this,"提示","设置门锁IP地址成功!!");
}
