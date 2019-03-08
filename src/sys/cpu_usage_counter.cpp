#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

#include <sys/stat.h>

#include <assert.h>
#include <cstdio>

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
#include <sched.h>

#include <sys/time.h>
#include <sys/resource.h>

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

string get_uid(const string & file_name){
    string uid = "";
    int fd = open(file_name.c_str(), O_RDONLY, S_IWRITE | S_IREAD);
    if (fd < 0){
        ERROR_EXIT(__FILE__, __LINE__, "read_file_error");
        return uid;
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
            if (end > 0){
                uid = uid_str.substr(begin, end - begin);
            }
        }
    }
    return uid;
}

unsigned int get_cpu_count(const string & file_name){
    unsigned int cpu_count = 0;
    int fd = open(file_name.c_str(), O_RDONLY, S_IWRITE | S_IREAD);
    if (fd < 0){
        ERROR_EXIT(__FILE__, __LINE__, "read_file_error");
        return cpu_count;
    }
    char buff[MAX_BUFFER_SIZE];
    memset(buff, 0, sizeof(buff));
    int len = read(fd, buff, MAX_BUFFER_SIZE);
    close(fd);

    vector<string> res = split(buff, ' ');
    unsigned long utime = atoi(res[13].c_str());
    unsigned long stime = atoi(res[14].c_str());
    //printf("%lu %lu\n", utime, stime);
    cpu_count = utime + stime;
    return cpu_count;
}

map<string, pair<string, unsigned long>> walk_proc(const char pathname[]){
    DIR *dfd;
    char name[MAX_PATH];
    struct dirent *dp;
    map<string, pair<string, unsigned long>> pid_uid_cpu;
    if ((dfd = opendir(pathname)) == NULL){
        printf("dir_order: can't open %s\n %s", pathname,strerror(errno));
        return pid_uid_cpu;
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
                string uid = get_uid(name);

                sprintf(name, "%s/%s/stat", pathname, dp->d_name);
                unsigned int cpu_count = get_cpu_count(name);
                pid_uid_cpu[dp->d_name] = make_pair(uid, cpu_count);
            }
        }
    }
    closedir(dfd);
    return pid_uid_cpu;
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

//PRIO_USER does not work now, I do not know why.
void set_pri(vector<int> pid_list, const int pri){
    for (auto pid : pid_list){
        int res = setpriority(PRIO_PROCESS, pid, pri);
        if (res == -1 & errno != 0){
            cout << "setpriority result : " << res << ' ' << strerror(errno) << endl;
        }
    }
}

void set_running_cpu(vector<int> pid_list, vector<int> cpu_ids){
    cpu_set_t mask;
    CPU_ZERO(&mask);
    for (auto cpu : cpu_ids){
        CPU_SET(cpu, &mask);
    }
    for (auto pid : pid_list){
        cpu_set_t set_cpus;
        int res = sched_getaffinity(pid, sizeof(cpu_set_t), &set_cpus);
        for (auto cpu : cpu_ids){
            printf("%d %d %d\n", pid, cpu, CPU_ISSET(cpu, &set_cpus));
        }

        res = sched_setaffinity(pid, sizeof(cpu_set_t), &mask);
        if (res < 0){
            printf("Error: setaffinity()\n");
        }
    }
}

int main(){
    map<string, pair<string, unsigned long>> pid_uid_cpu_last = walk_proc("/proc");
    while (true) {
        sleep(1);
        map<string, string> uid_name = get_uid_name();
        map<string, pair<string, unsigned long>> pid_uid_cpu = walk_proc("/proc");   
        map<string, unsigned long> uid_cpu_usage;
        map<string, vector<int>> uid_pid_list;
        for (auto x : pid_uid_cpu){
            string pid = x.first, uid = x.second.first;
            unsigned long cpu_count = x.second.second;
            if (uid_cpu_usage.find(uid) == uid_cpu_usage.end()){
                uid_cpu_usage[uid] = 0;
                uid_pid_list[uid] = vector<int>();
            }
            uid_pid_list[uid].push_back(atoi(pid.c_str()));
            if (pid_uid_cpu_last.find(pid) == pid_uid_cpu_last.end() or (pid_uid_cpu_last[pid].first != uid)){
                uid_cpu_usage[uid] += cpu_count;
            } else {
                uid_cpu_usage[uid] += cpu_count - pid_uid_cpu_last[pid].second;
            }
        }
        for (auto info : uid_cpu_usage){
            cout << "user : " << uid_name[info.first] << ' ' << info.second << endl;
            vector<int> cpu_ids = {1, 2};
            set_running_cpu(uid_pid_list[info.first], cpu_ids);
            //if (info.second > 800 / 1){
            //    set_pri(uid_pid_list[info.first], 1);
            //} else {
            //    set_pri(uid_pid_list[info.first], -1);
            //}
        }
        pid_uid_cpu_last = pid_uid_cpu;
    }
    return 0;
}

