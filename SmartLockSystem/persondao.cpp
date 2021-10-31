#include "persondao.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

bool PersonDao::addPersonInfo(const PersonEntity &e)
{
    QSqlQuery query;
    query.prepare("insert into person_info(name) values(?)");
    query.bindValue(0, e.name);
    bool ok = query.exec();
    if(ok)
    {
        qDebug()<<"插入person_info表成功!!";
        return true;
    }
    qDebug()<<"插入person_info表失败!!";
    return false;
}

//查找最大的主键ID
bool PersonDao::getMaxID(const PersonEntity &e, int &maxid)
{
    QSqlQuery query;
    query.exec("select max(id) as id from person_info");
    while(query.next())
    {
        maxid = query.value(0).toInt();
        return true;
    }
    return false;
}

bool PersonDao::selectPersonInfoById(const PersonEntity &e, QString &name)
{
    QSqlQuery query;
    query.prepare("select name from person_info where id = ?");
    query.bindValue(0, e.id);
    query.exec();
    while(query.next())
    {
        name = query.value(0).toString();
        return true;
    }
    return false;
}

PersonDao::PersonDao()
{

}

PersonDao::~PersonDao()
{

}

