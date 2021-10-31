#ifndef DAOFACTORY_H
#define DAOFACTORY_H
#include <QSqlDatabase>
#include "userinfodao.h"
#include "persondao.h"

class DaoFactory
{
public:
    //返回单例对象的指针
    static DaoFactory *getInstance();
    //释放空间
    static void releaseInstance();
    //获取操作数据库UserInfoDao对象的指针
    UserInfoDao *getUserInfoDao();
    PersonDao* getPersonDao();
private:
    UserInfoDao* userinfodao;//指向UserInfoDao对象
    PersonDao* persondao;//指向PersonDao对象
    QSqlDatabase db;
    static DaoFactory* instance;
    DaoFactory();
    ~DaoFactory();
};

#endif // DAOFACTORY_H
