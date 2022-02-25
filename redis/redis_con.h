//
// Created by BinG on 2022/2/14.
//

#ifndef TINY_WEB_CMAKE_REDIS_CON_H
#define TINY_WEB_CMAKE_REDIS_CON_H

#include <stdio.h>
#include <list>
#include "hiredis/hiredis.h"
#include <error.h>
#include <string.h>
#include <iostream>
#include <string>
#include "../lock/locker.h"
#include "../log/log.h"

using namespace std;

class redis_con {
public:
    redisContext *GetConnection();                 //获取数据库连接
    bool ReleaseConnection(redisContext *conn); //释放连接
    int GetFreeConn();                     //获取连接
    void DestroyPool();                     //销毁所有连接

    //单例模式
    static redis_con *GetInstance();

    void init(string url, int Port, int Max_conn, int close_log);

private:
    redis_con();

    ~redis_con();

    int m_MaxConn;  //最大连接数
    int m_CurConn;  //当前已使用的连接数
    int m_FreeConn; //当前空闲的连接数
    locker lock;
    list<redisContext *> connList; //MySQL连接池
    sem reserve;

public:
    string m_url;             //主机地址
    string m_Port;         //数据库端口号
    //string m_User;         //登陆数据库用户名
    //string m_PassWord;     //登陆数据库密码
    //string m_DatabaseName; //使用数据库名
    int m_close_log;    //日志开关
};

class redisconnectionRAII {

public:
    redisconnectionRAII(redisContext **con, redis_con *connPool);

    ~redisconnectionRAII();

private:
    redisContext *conRAII;
    redis_con *poolRAII;
};


#endif //TINY_WEB_CMAKE_REDIS_CON_H
