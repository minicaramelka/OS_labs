#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>

typedef struct data_struct{
	time_t parentTime;
	pid_t pid;
} data_struct;

int main(int argc, char** argv){
	int pipeArr[2];
	pid_t pid;
	pipe(pipeArr);
	switch(pid = fork()){
		case -1:
			printf("ERROR fork()\n");
			return -1;
		case 0:
			printf("[CHILD]\n");
			sleep(5);
			data_struct data1;
			close(pipeArr[1]);
			read(pipeArr[0], &data1, sizeof(data_struct));
			close(pipeArr[0]);
			time_t curTime = time(NULL);
			printf("[CHILD] time: %s\n", ctime(&curTime));
			printf("[PARENT] pid: %d\n", data1.pid);
			printf("[PARENT] time: %s\n", ctime(&data1.parentTime));
			return 0;
		default:
			printf("[PARENT]\n");
			time_t parentTime = time(NULL);
            		data_struct data2 = {parentTime, getpid()};
			close(pipeArr[0]);
			write(pipeArr[1], (void*)(&data2), sizeof(data_struct));
			close(pipeArr[1]);
			printf("[PARENT] Parent %d send data\n", data2.pid);
			wait(0);
			return 0;
	}	
}
