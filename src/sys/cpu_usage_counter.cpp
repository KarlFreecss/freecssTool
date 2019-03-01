#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/resource.h>
#include <errno.h>

#include <sys/stat.h>

#include <assert.h>
#include <cstdio>
#include <cstring>

#include <iostream>
#include <string>
#include <vector>
#include <sstream>

#include <cstdio>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <dirent.h>
#include <cstring>

#define ERROR_EXIT(file, line, name) printf("%s,%d : %s\n", file, line, name)

using namespace std;
const int MAX_PATH = 1024;
const int MAX_BUFFER_SIZE = 1024;

vector<string> split(string str, const char c){
    vector<string> ret;
    int begin = 0;
    str += c;
    for (int i = 0; i < str.length(); ++i){
        if (str[i] == c){
            ret.push_back(str.substr(begin, i - begin));
            begin = i + 1;
        }
    }
    return ret;
}

int is_all_num(const string & str){
    if (str.length() == 0){
        return false;
    }
    for (int i = 0; i < str.length(); ++i){
        if (str[i] < '0' || str[i] > '9'){
            return false;
        }
    }
    return true;
}

void dir_order(const char pathname[]){
    DIR *dfd;
    char name[MAX_PATH];
    struct dirent *dp;
    if ((dfd = opendir(pathname)) == NULL){
        printf("dir_order: can't open %s\n %s", pathname,strerror(errno));
        return;
    }
    while ((dp = readdir(dfd)) != NULL){
        if (strncmp(dp->d_name, ".", 1) == 0)
            continue; /* 跳过当前文件夹和上一层文件夹以及隐藏文件*/
        if (strlen(pathname) + strlen(dp->d_name) + 2 > sizeof(name)){
            printf("dir_order: name %s %s too long\n", pathname, dp->d_name);
        }else{
            if (is_all_num(dp->d_name)){
                memset(name, 0, sizeof(name));
                sprintf(name, "%s/%s/status", pathname, dp->d_name);
                //FILE * fp = fopen(name, "wb");
                int fd = open(name, O_RDONLY, S_IWRITE | S_IREAD);
                if (fd < 0){
                    ERROR_EXIT(__FILE__, __LINE__, "read_file_error");
                    return ;
                }
                char buff[MAX_BUFFER_SIZE];
                memset(buff, 0, sizeof(buff));
                int len = read(fd, buff, MAX_BUFFER_SIZE);
                close(fd);
                vector<string> res = split(buff, '\n');
                for (int i = 0; i < res.size(); ++i) {
                    if (strncmp(res[i].c_str(), "Uid", 3) == 0){
                        string uid_str = res[i].substr(4);
                        int begin = -1, end = -1;
                        for (int i = 0; i < uid_str.length(); ++i){
                            if (uid_str[i] == '\t' & begin != -1){
                                end = i;
                                break;
                            }
                            if (uid_str[i] != '\t' & begin == -1){
                                begin = i;
                            }
                        }
                        if (end - begin> 3){
                            printf("%s : %s\n", dp->d_name, res[i].c_str());
                        }
                    }
                }
            }
        }
    }
    closedir(dfd);
}

map<string, string> get_uid_name(){
    const string file_name = "/etc/passwd";
    ifstream fin(file_name);
    string line_data;
    map<string, string> ret;
    while (fin >> line_data){
        vector<string> data = split(line_data, ':');
        if (data.size() > 2){
            ret[data[2]] = data[0];
        }
    }
    fin.close();
    return ret;
}

int main(){
    map<string, string> dict = get_uid_name();
    for (auto x : dict){
        cout << x.first << ' ' << x.second << endl;
    }
    dir_order("/proc");   
    return 0;
}

