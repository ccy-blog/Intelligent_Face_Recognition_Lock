#include "daofactory.h"
#include <cstdio>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

DaoFactory* DaoFactory::instance = NULL;

//返回单例对象的指针
DaoFactory *DaoFactory::getInstance()
{
    if(instance == NULL)
        instance = new DaoFactory;
    return instance;
}

void DaoFactory::releaseInstance()
{
    if(instance != NULL)
    {
        delete instance;
        instance = NULL;
    }
}

UserInfoDao *DaoFactory::getUserInfoDao()
{
    return userinfodao;
}

PersonDao *DaoFactory::getPersonDao()
{
    return persondao;
}

//单例模式构造函数，将创建数据库 创建表等操作写在构造函数中
DaoFactory::DaoFactory():userinfodao(new UserInfoDao),persondao(new PersonDao)
{
    //1.加载驱动
    db = QSqlDatabase::addDatabase("QSQLITE");
    //2.创建数据库文件
    db.setDatabaseName("user.db");
    //3.打开数据库
    db.open();
    //4.创建用户信息表
    QSqlQuery query;
    bool ok = query.exec("create table if not exists user_info("
               "id integer primary key autoincrement,"
               "name varchar(20),"
               "passwd varchar(20))");
    if(ok)
    {
        qDebug()<<"创建user_info表成功!!";
    }
    else
    {
        qDebug()<<"创建user_info表失败!!";
    }
    //5.创建person_info信息表
    ok = query.exec("create table if not exists person_info("
               "id integer primary key autoincrement,"
               "name varchar(20))");
    if(ok)
    {
        qDebug()<<"创建person_info表成功!!";
    }
    else
    {
        qDebug()<<"创建person_info表失败!!";
    }
}

DaoFactory::~DaoFactory()
{
    db.close();
}

