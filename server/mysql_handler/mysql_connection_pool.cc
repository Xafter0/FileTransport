/*
*Author: 王鑫
*Description: mariadb封装mysql连接池
*Date: 2020-09-06
*/
#include "mysql_connection_pool.h"
#include <chrono>
#include <iostream>

const int MYSQL_TIME_OUT = 7;

namespace mysql
{

MySqlConnectPool::MySqlConnectPool(int capacity, int size)
    : m_capacity(capacity),
      m_size(size)
{
}

MySqlConnectPool::~MySqlConnectPool()
{
}

inline void CloseConne(MYSQL* mysql)
{
    mysql_close(mysql);
}

bool MySqlConnectPool::Init(const std::string &host, const std::string &userName, const std::string &passWd, const std::string &database)
{
    m_host = host;
    m_userName = userName;
    m_passWd = passWd;
    m_database = database;
    m_mutex = std::make_shared<std::mutex>();
    m_cond = std::make_shared<std::condition_variable>();
    for (int i = 0; i < m_size; ++i) {
        auto conn = CreateNewConn();
        if (conn == nullptr) {
            continue;
        }
        m_sqlConn.emplace_back(conn);
    }
    if (m_sqlConn.empty()) {
        std::cout << "failed to init mysql connection pool" << std::endl;
        return false;
    }
    std::cout << "success to init mysql connection pool with size: " << m_sqlConn.size() << std::endl;
    return true;
}

std::shared_ptr<MYSQL> MySqlConnectPool::CreateNewConn()
{
    MYSQL* mysql = mysql_init(NULL);
    if (!mysql)
    {
        std::cout << "mysql connet init faliled\n" << std::endl;
        return nullptr;
    }

    auto ret = mysql_options(mysql, MYSQL_OPT_CONNECT_TIMEOUT, (const char*)& MYSQL_TIME_OUT);
    if (ret) {
        std::cout << "mysql set option failed\n"  << std::endl;
        return nullptr;
    }

    mysql = mysql_real_connect(mysql, m_host.c_str(), m_userName.c_str(), m_passWd.c_str(), m_database.c_str(), 0, NULL, 0);
    if (!mysql) {
        std::cout << "mysql connet faliled" << std::endl;
        return nullptr;
    }
    std::shared_ptr<MYSQL> mysqlShared(mysql, mysql_close); // 指定mysql_close为删除器来关闭连接
    return mysqlShared;
}

std::shared_ptr<MYSQL> MySqlConnectPool::GetConn()
{
    std::unique_lock<std::mutex> lck(*m_mutex);
    if (!m_sqlConn.empty()) {
        auto ret = m_sqlConn.front();
        m_sqlConn.pop_front();
        return ret;
    }
    m_cond->wait_for(lck, std::chrono::milliseconds(300), [this] () {return !m_sqlConn.empty();});
    if (m_sqlConn.empty()) {
        return nullptr;
    }

    auto ret = m_sqlConn.front();
    m_sqlConn.pop_front();

    //连接断开需要重新连接
    if (mysql_ping(ret.get()) != 0) {
        auto newConn = mysql_real_connect(ret.get(), m_host.c_str(), m_userName.c_str(), m_passWd.c_str(), m_database.c_str(), 0, NULL, 0);
        ret.reset(newConn, mysql_close);
    }
    return ret;
}

bool MySqlConnectPool::ReturnConn(std::shared_ptr<MYSQL> conn)
{
    std::unique_lock<std::mutex> lck(*m_mutex);
    if (m_sqlConn.size() < m_capacity) {
        m_sqlConn.emplace_back(conn);
        return true;
    }

    std::cout << "connect no need to return" << std::endl;
    return true;
}


MysqlPoolHandle::MysqlPoolHandle(MySqlConnectPool &pool)
    : m_pool(pool),
      m_conn(pool.GetConn())
{
}

MYSQL* MysqlPoolHandle::Get() //返回的MYSQL句柄不可被传递到局部区域之外使用
{
    return m_conn.get();
}

MysqlPoolHandle::~MysqlPoolHandle()
{
    if (m_conn == nullptr) {
        return;
    }
    m_pool.ReturnConn(m_conn);
}

}
