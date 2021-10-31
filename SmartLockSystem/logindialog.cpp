#include "logindialog.h"
#include "ui_logindialog.h"
#include "daofactory.h"
#include "userinfoentity.h"
#include <QDebug>
#include <QMessageBox>

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

//点击注册按钮
void LoginDialog::on_pushButton_register_clicked()
{
    //1.将输入的用户名和密码获取到
    QString name = ui->lineEdit_name->text();
    QString passwd = ui->lineEdit_passwd->text();
    if(name.isEmpty() || passwd.isEmpty())//当用户名或密码为空的时候
    {
        QMessageBox::warning(this,"提示","忘记输入用户名或密码!!");
        return;
    }


    //2.去数据库中比对，用户名是否已经被注册
    UserInfoDao* dao = DaoFactory::getInstance()->getUserInfoDao();
    UserInfoEntity e(0,name,passwd);
    bool ok = dao->selectUserInfoByName(e);//查找用户名是否被注册过
    if(ok)//找到了返回值是true，代表已经被注册了
    {
        QMessageBox::warning(this,"失败","用户名已经被注册了!!");
    }
    else//返回值是false没有找到，代表未注册
    {
        QMessageBox::warning(this,"成功","恭喜，注册成功!!");
        dao->addUserInfo(e);//将用户名和密码保存到数据库中
    }
    ui->lineEdit_name->clear();
    ui->lineEdit_passwd->clear();
}

//点击登陆按钮
void LoginDialog::on_pushButton_login_clicked()
{
    //1.获取输入的用户名和密码
    QString name = ui->lineEdit_name->text();
    QString passwd = ui->lineEdit_passwd->text();
    //2.得到UserInfoDao指针，准备操作数据库
    UserInfoDao* dao = DaoFactory::getInstance()->getUserInfoDao();
    UserInfoEntity e(0,name,passwd);
    //3.对登录判断
    if(dao->selectUserInfoByName(e))//如果返回值是true说明，输入的用户名正确
    {
        if(dao->selectUserInfoByNamePasswd(e))//如果返回值是true，说明用户民和密码正确，登录成功
        {
            QMessageBox::warning(this,"成功","恭喜你，登录成功!!");
            //给MainWindow发送一个登录成功的信号
            emit sendLoginSuccess();
        }
        else
        {
            QMessageBox::warning(this,"提示","登录的用户密码不正确!!");
        }
    }
    else//输入的用户名不存在
    {
        QMessageBox::warning(this,"提示","您输入的用户名不存在!!");
    }

}
