#ifndef PERSONDAO_H
#define PERSONDAO_H
#include "personentity.h"


class PersonDao
{
public:
    bool addPersonInfo(const PersonEntity& e);//插入数据
    bool getMaxID(const PersonEntity& e, int& maxid);//获取最大的主键ID
    bool selectPersonInfoById(const PersonEntity& e, QString& name);//根据id查找
    PersonDao();
    ~PersonDao();
};

#endif // PERSONDAO_H
