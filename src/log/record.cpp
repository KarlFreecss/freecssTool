#include "record.h"

std::string getDate(){
	time_t t = time(0); 
	char tmp[64]; 
	strftime(tmp, sizeof(tmp), "%Y%m%d",localtime(&t) ); 
	return tmp;
}

std::string getTime(){
	time_t t = time(0); 
	char tmp[64]; 
	strftime(tmp, sizeof(tmp), "%Y/%m/%d-%X",localtime(&t) ); 
	return tmp;
}

int RECORD::write(const std::string & text){
	os << getTime() << ' ' << text << std::endl;

	FILE * fp = fopen((cfgPath).c_str(), "w");
	fprintf(fp, "%d", count++);
	fclose(fp);
	return 0;
}

std::string RECORD::getPath(){
	return path;
}

int RECORD::getCount(){
	return count;
}
#include<iostream>
RECORD::RECORD(const std::string &describe){
	int flag = 0;
	path = getDate() + "/";
	cfgPath = path + "recorder.cfg";
	std::string listPath = getDate() + "/list.txt";
	
	if (1){
	//if (_access(listPath.c_str(), 0) != 0) {
		system((std::string("mkdir ") + getDate()).c_str());
		flag = 1;
		count = 0;
		FILE * fp = fopen((cfgPath).c_str(), "w");
		fprintf(fp, "%d", count);
		fclose(fp);
	}
	FILE * fp = fopen((cfgPath).c_str(), "r");
	if (fp != NULL) {
		fscanf(fp, "%d", &count);
		fclose(fp);
	} else count = 0;
	os.open(listPath, std::ios::app);
	if (flag) os << describe << std::endl;
}

