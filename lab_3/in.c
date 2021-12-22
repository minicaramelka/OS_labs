#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <time.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <signal.h>

typedef struct data_struct{
	time_t time;
	pid_t pid;
} data_struct;

#define FILE "shared_memory"

void funcExit(int sig)
{
    printf("\n");
    exit(0);
}

int main(){
	signal(SIGINT, funcExit);
	void* addr;
	int mem;
	key_t key = ftok(FILE, 'a');
	if((mem = (shmget(key, sizeof(data_struct), IPC_EXCL))) == -1 ){
        	printf("SM open error\n");
        	exit(0);
    	}
    	if((addr = shmat(mem, NULL, 0)) == (void*)-1){
		printf("SM link error\n");
        	exit(0);
    	}
	time_t timer = time(0);
	data_struct getStruct = *((data_struct*)addr);
        printf("\ntime_Consumer = %s | pid_Consumer = %d\n", ctime(&timer), getpid());
	printf("sent time = %s | sent pid = %d\n", ctime(&getStruct.time), getStruct.pid);
	shmdt(addr);
    	return 0;
}
