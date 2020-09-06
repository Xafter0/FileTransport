/*
*Author: 王鑫
*Description: mysql handle 底层采用连接池连接mysql
*Date: 2020-09-06
*/
#include "mysql_handle.h"
#include <vector>
#include <iostream>

namespace mysql
{

MysqlHandle::MysqlHandle(int poolCapacity, int poolSize)
    : m_pool(poolCapacity, poolSize)
{
}

bool MysqlHandle::Init(const std::string &host, const std::string &userName, const std::string &passWd, const std::string &database)
{
    if (!m_pool.Init(host, userName, passWd, database)) {
        std::cout << "mysql connection pool init failed" << std::endl;
        return false;
    }
    return true;
}

bool MysqlHandle::Execute(const std::string &sql, std::vector<std::vector<std::string>> &ret)
{
    MysqlPoolHandle poolHandle(m_pool);
    auto conn = poolHandle.Get();
    if (conn == nullptr) {
        std::cout << "get connection from mysql connection pool failed" << std::endl;
        return false;
    }

    auto quryRet = mysql_query(conn, sql.c_str());
    if (quryRet) {
        std::cout << "execute sql cmd failed of: " << sql << ", error msg is " << mysql_error(conn) << std::endl;
        return false;
    }

    auto result = mysql_store_result(conn);
    if (!result) {
        return true;
    }
    std::shared_ptr<MYSQL_RES> resultShared(result, mysql_free_result);
    result = nullptr; // result 已置空后续不可再继续使用

    int numRow = mysql_num_rows(resultShared.get());
    int columnNum = mysql_field_count(conn);
    ret = std::vector<std::vector<std::string>>(numRow, std::vector<std::string>(columnNum));

    for (int i = 0; i < numRow; ++i) {
        auto row = mysql_fetch_row(resultShared.get());
        for (int j = 0; j < columnNum; ++j) {
            ret[i][j] = row[j];
        }
    }
    return true;
}

}