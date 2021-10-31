#ifndef USERINFOENTITY_H
#define USERINFOENTITY_H
#include <QString>

//UserInfoEntity类中将user_info表中的所有字段当做成员变量
struct UserInfoEntity
{//三个成员变量对应表中的三个字段
    int id;
    QString name;
    QString passwd;
public:
    UserInfoEntity(int i, QString n, QString p);
    UserInfoEntity();
    ~UserInfoEntity();
};

#endif // USERINFOENTITY_H
