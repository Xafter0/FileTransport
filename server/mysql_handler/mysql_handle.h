/*
*Author: 王鑫
*Description: mysql handle 底层采用连接池连接mysql
*Date: 2020-09-06
*/
#ifndef MYSQL_HANDLE
#define MYSQL_HANDLE
#include "mysql_connection_pool.h"
#include <string>
#include <vector>

namespace mysql 
{
class MysqlHandle {
public:
    MysqlHandle(int poolCapacity = 30, int poolSize = 10);

    bool Init(const std::string &host, const std::string &userName, const std::string &passWd, const std::string &database);

    bool Execute(const std::string &sql, std::vector<std::vector<std::string>> &ret);

private:
    MySqlConnectPool m_pool;
};
}

#endif