/*
*Author: 王鑫
*Description: mysql handle 测试程序
*Date: 2020-09-06
*/
#include "mysql_handle.h"
#include <vector>
#include <iostream>

int main()
{

    mysql::MysqlHandle handle;
    handle.Init("127.0.0.1", "root", "123456", "loginUser");
    std::vector<std::vector<std::string>> ret;
    handle.Execute("select * from user", ret);
    for (unsigned int i = 0; i < ret.size(); ++i) {
        for(unsigned int j = 0; j < ret[i].size(); ++j) {
            std::cout << ret[i][j] << " ";
        }
        std::cout << std::endl;
    }
    handle.Execute("insert into user (name, passwd) values ('oumeie', '123456')", ret);
    handle.Execute("select * from user", ret);
    for (unsigned int i = 0; i < ret.size(); ++i) {
        for(unsigned int j = 0; j < ret[i].size(); ++j) {
            std::cout << ret[i][j] << " ";
        }
        std::cout << std::endl;
    }

    handle.Execute("delete from user where id >= 3", ret);

    handle.Execute("select * from user", ret);
    for (unsigned int i = 0; i < ret.size(); ++i) {
        for(unsigned int j = 0; j < ret[i].size(); ++j) {
            std::cout << ret[i][j] << " ";
        }
        std::cout << std::endl;
    }
    handle.Execute("alter table user AUTO_INCREMENT=2", ret);
    return 0;
}