#ifndef USERINFODAO_H
#define USERINFODAO_H

#include "userinfoentity.h"

//UserInfoDao类里面只有对用户信息表操作的相关函数 增 删 改 查
class UserInfoDao
{
public:
    bool addUserInfo(const UserInfoEntity& e);//增加
    bool deleteUserInfo(const UserInfoEntity& e);//删除
    bool updateUserInfo(const UserInfoEntity& e);//修改
    bool selectUserInfoByName(const UserInfoEntity& e);//通过名字查找
    bool selectUserInfoByNamePasswd(const UserInfoEntity& e);//通过名字和密码查找


public:
    UserInfoDao();
    ~UserInfoDao();
};

#endif // USERINFODAO_H
