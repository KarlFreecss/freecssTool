#include "MyDB.h"
using namespace std;

MyDB::MyDB(){
	connection = mysql_init(NULL); // 初始化数据库连接变量
	if(connection == NULL){
		cout << "Error:" << mysql_error(connection);
		exit(1);
	}
}

MyDB::~MyDB(){
	if(connection != NULL) { // 关闭数据库连接
		mysql_close(connection);
	}
}

bool MyDB::initDB(string host, string user, string pwd, string db_name)
{
	// 函数mysql_real_connect建立一个数据库连接
	// 成功返回MYSQL*连接句柄，失败返回NULL
	connection = mysql_real_connect(connection, host.c_str(),
			user.c_str(), pwd.c_str(), db_name.c_str(), 0, NULL, 0);
	mysql_set_character_set(connection, "utf-8");
	if(connection == NULL) {
		cout << "Error:" << mysql_error(connection);
		return false;
	}
	return true;
}

bool MyDB::exeSQL(string sql, vector<string> &res){
	// mysql_query()执行成功返回0，失败返回非0值。与PHP中不一样
	if(mysql_query(connection, sql.c_str())) {
		return false;
	} else {
		result = mysql_store_result(connection); // 获取结果集
		if (result == NULL) return true;
		res.clear();
		// mysql_field_count()返回connection查询的列数
		int fieldcount = mysql_num_fields(result);
		while (row = mysql_fetch_row(result)){
			// mysql_num_fields()返回结果集中的字段数
			for(int j=0; j < fieldcount; ++j) {
				if (row[j] != NULL) {
					res.push_back(row[j]);
				} else {
					res.push_back("NULL");
				}
			}
		}
		mysql_free_result(result);
	}
	return true;
}

