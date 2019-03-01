#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>

void sys_err(const char * str){
	perror(str);
	exit(1);
}

#define PIPE_ERROE -1
#define FORK_ERROR -2

int readFromC(const char * cmd, char * const argv[]){
	pid_t pid;
	int fd[2];
	if (pipe(fd) == -1)
		return PIPE_ERROE;

	pid = fork();
	if (pid < 0) {
		return FORK_ERROR;
	} else if (pid != 0) {
		wait(NULL);
		close(fd[1]);
	} else {
		close(fd[0]);
		dup2(fd[1], 1);
		execv(cmd, argv);
		close(fd[1]);
	}
	return fd[0];
}

int main(void){
	char buf[1024];
  	char * argv[] = {"ls", "-al", NULL};
	int pipe_fd = readFromC("/bin/ls", argv);

	int len = read(pipe_fd, buf, sizeof(buf));
	FILE * fp;
	fp = fopen("test.txt", "w");
	fwrite(buf, 1, len, fp);

	close(pipe_fd);
	return 0;
}
