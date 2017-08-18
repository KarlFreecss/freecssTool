#ifndef __MYBD_H__
#define __MYDB_H__
#include <iostream>  
#include <string>  
#include <mysql/mysql.h> 
#include <vector>
#include <iostream>
#include <bitset>
#include <zconf.h>
#include <cstring>
#include <sys/stat.h>
#include <sys/param.h>
#include <netinet/in.h>
#include <fstream>
#include <fcntl.h>
#include <poll.h>
#include <errno.h>
#include <sstream>
#include <cstdlib>
#include <string>
#include <mysql/mysql.h>
#include <string>
#include <vector>

class MyDB {  
public:  
    MyDB();  
    ~MyDB();  
    bool initDB(std::string host, 
			std::string user, 
			std::string pwd, 
			std::string db_name);  
    bool exeSQL(std::string sql, 
			std::vector<std::string> &SQL_ans);

private:  
    MYSQL *connection;  
    MYSQL_RES *result;  
    MYSQL_ROW row;  
};

#endif
