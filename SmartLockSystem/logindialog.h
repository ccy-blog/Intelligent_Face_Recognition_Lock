#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = 0);
    ~LoginDialog();

//信号
signals:
    void sendLoginSuccess();//登录成功的信号

private slots:
    void on_pushButton_register_clicked();

    void on_pushButton_login_clicked();

private:
    Ui::LoginDialog *ui;
};

#endif // LOGINDIALOG_H
