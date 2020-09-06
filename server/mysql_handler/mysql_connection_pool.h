/*
*Author: 王鑫
*Description: mariadb封装mysql连接池
*Date: 2020-09-06
*/
#ifndef MYSQL_CONNECT_POOL
#define MYSQL_CONNECT_POOL
#define MYSQL_CONNECT_POOL
#include <mariadb/mysql.h>
#include <memory>
#include <deque>
#include <condition_variable>


namespace mysql
{

class MySqlConnectPool 
{
public:
    MySqlConnectPool(int capacity, int size);

    ~MySqlConnectPool();

    bool Init(const std::string &host, const std::string &userName, const std::string &passWd, const std::string &database);

    std::shared_ptr<MYSQL> GetConn();

    bool ReturnConn(std::shared_ptr<MYSQL> conn);

private:
    std::shared_ptr<MYSQL> CreateNewConn();
    std::deque<std::shared_ptr<MYSQL>> m_sqlConn;
    std::shared_ptr<std::mutex> m_mutex;
    std::shared_ptr<std::condition_variable> m_cond;
    int m_capacity;
    int m_size;
    std::string m_host;
    std::string m_userName;
    std::string m_passWd;
    std::string m_database;
};

}
#endif