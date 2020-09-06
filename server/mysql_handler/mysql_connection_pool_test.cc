/*
*Author: 王鑫
*Description: mysql连接池测试程序
*Date: 2020-09-06
*/
#include "mysql_connection_pool.h"
#include <iostream>

int main()
{
    mysql::MySqlConnectPool pool(10, 10);
    pool.Init("127.0.0.1", "root", "123456", "loginUser");
    auto conn = pool.GetConn();
    if (conn == nullptr) {
        std::cout << "get connection failed" << std::endl;
        return -1;
    }
    std::string sql = "select * from user";
    auto ret = mysql_query(conn.get(), sql.c_str());
    if (ret) {
        std::cout << "execute sql cmd failed of: " << sql << std::endl;
        return -1;
    }
    auto result = mysql_store_result(conn.get());
    std::shared_ptr<MYSQL_RES> resultShared(result, mysql_free_result);
    result = nullptr;
    int num = mysql_num_rows(resultShared.get());
    int columnNum = mysql_field_count(conn.get());
    for (int i = 0; i < num; ++i) {
        auto row = mysql_fetch_row(resultShared.get());
        for (int j = 0; j < columnNum; ++j) {
            std::cout << row[j] << " ";
        }
        std::cout << std::endl;
    }
    pool.ReturnConn(conn);
    return 0;
}