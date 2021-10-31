#include "userinfodao.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

//增
bool UserInfoDao::addUserInfo(const UserInfoEntity &e)
{
    QSqlQuery query;
    query.prepare("insert into user_info(name,passwd) values(?,?)");
    query.bindValue(0, e.name);
    query.bindValue(1, e.passwd);
    bool ok = query.exec();
    if(ok)
    {
        qDebug()<<"插入user_info表成功!!";
        return true;
    }
    qDebug()<<"插入user_info表失败!!";
    return false;
}

//删
bool UserInfoDao::deleteUserInfo(const UserInfoEntity &e)
{
    //删除，通过主键id进行删除
    QSqlQuery query;
    query.prepare("delete from user_info where id = ?");
    query.bindValue(0, e.id);
    bool ok = query.exec();
    if(ok)
    {
        qDebug()<<"删除user_info表中的数据成功!!";
        return true;
    }
    qDebug()<<"删除user_info表中的数据失败!!";
    return false;
}

//改
bool UserInfoDao::updateUserInfo(const UserInfoEntity &e)
{
    //根据用户的名字，修改用户的密码
    QSqlQuery query;
    query.prepare("update user_info set passwd = ? where name = ?");

    query.bindValue(0, e.passwd);
    query.bindValue(1, e.name);
    bool ok = query.exec();
    if(ok)
    {
        qDebug()<<"修改user_info表中的数据成功!!";
        return true;
    }
    qDebug()<<"修改user_info表中的数据失败!!";
    return false;
}

//查
bool UserInfoDao::selectUserInfoByName(const UserInfoEntity &e)
{
    //如果查找到了 返回true，没有查找到返回false
    QSqlQuery query;
    query.prepare("select * from user_info where name = ?");
    query.bindValue(0, e.name);
    query.exec();
    while(query.next())//只要能进入while循环，就说明找到了，代表用户已经被注册
    {
        return true;
    }
    return false;//代表用户没有被注册
}

//通过名字和密码进行查找
bool UserInfoDao::selectUserInfoByNamePasswd(const UserInfoEntity &e)
{
    //true 代表登录成功 false代表登录失败
    QSqlQuery query;
    query.prepare("select * from user_info where name = ? and passwd = ?");
    query.bindValue(0,e.name);
    query.bindValue(1,e.passwd);
    query.exec();
    while(query.next())//只要能进入while循环，就说明找到了，说明用户名和密码正确
    {
        return true;//代表用户名和密码正确
    }
    return false;//代表用户名或密码不正确
}

UserInfoDao::UserInfoDao()
{

}

UserInfoDao::~UserInfoDao()
{

}

