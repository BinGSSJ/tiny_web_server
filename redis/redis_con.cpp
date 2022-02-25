//
// Created by BinG on 2022/2/14.
//

#include "redis_con.h"
#include <stdlib.h>
#include <stddef.h>


using namespace std;

redis_con::redis_con() {
    m_CurConn = 0;
    m_FreeConn = 0;
}

redis_con *redis_con::GetInstance() {
    static redis_con redis_connPool;
    return &redis_connPool;
}

//构造初始化
void
redis_con::init(string url, int Port, int MaxConn, int close_log) {
    m_url = url;
    m_Port = Port;
    //m_User = User;
    //m_PassWord = PassWord;
    //m_DatabaseName = DBName;
    m_close_log = close_log;

    for (int i = 0; i < MaxConn; i++) {
        redisContext *con = redisConnect("docker.for.mac.host.internal", 6379);
        if (con->err) {
            redisFree(con);
            LOG_ERROR("Connect to redisServer failed");
            exit(1);
        }

        connList.push_back(con);
        ++m_FreeConn;
    }

    reserve = sem(m_FreeConn);// 设置信号量

    m_MaxConn = m_FreeConn;
}


//当有请求时，从数据库连接池中返回一个可用连接，更新使用和空闲连接数
redisContext *redis_con::GetConnection() {
    redisContext *con = NULL;

    if (0 == connList.size())
        return NULL;

    reserve.wait();

    lock.lock();

    con = connList.front();
    connList.pop_front();

    --m_FreeConn;
    ++m_CurConn;

    lock.unlock();
    return con;
}

//释放当前使用的连接
bool redis_con::ReleaseConnection(redisContext *con) {
    if (NULL == con)
        return false;

    lock.lock();

    connList.push_back(con);
    ++m_FreeConn;
    --m_CurConn;

    lock.unlock();

    reserve.post();
    return true;
}

//销毁数据库连接池
void redis_con::DestroyPool() {

    lock.lock();
    if (connList.size() > 0) {
        list<redisContext *>::iterator it;
        for (it = connList.begin(); it != connList.end(); ++it) {
            redisContext *con = *it;
            redisFree(con);
        }
        m_CurConn = 0;
        m_FreeConn = 0;
        connList.clear();
    }

    lock.unlock();
}

//当前空闲的连接数
int redis_con::GetFreeConn() {
    return this->m_FreeConn;
}

redis_con::~redis_con() {
    DestroyPool();
}

redisconnectionRAII::redisconnectionRAII(redisContext **conne, redis_con *connPool) {
    *conne = connPool->GetConnection();

    conRAII = *conne;
    poolRAII = connPool;
}

redisconnectionRAII::~redisconnectionRAII() {
    poolRAII->ReleaseConnection(conRAII);
}

