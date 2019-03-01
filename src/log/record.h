#ifndef _RECORD_H_
#define _RECORD_H_

#include <cstdlib>
#include <fstream>
#include <ctime>
#include <string>

//#include <io.h>

class RECORD{
private:
	std::ofstream os;
	std::string path;
	std::string cfgPath;
	int count;

public:
	RECORD(const std::string &describe);
	int write(const std::string & text);
	std::string getPath();
	int getCount();
};

#endif
