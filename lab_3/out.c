#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <time.h>
#include <sys/shm.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>

typedef struct data_struct{
	time_t time;
	pid_t pid;
} data_struct;

#define FILE "shared_memory"

int shmid;

void funcExit(int sig){
	printf("\n");
    	struct shmid_ds *buf = 0;
    	shmctl(shmid, IPC_RMID, buf);
    	exit(0);
}

int main(){
    	signal(SIGINT, funcExit);
	struct shmid_ds shmidStat;
    	void* addr;
    	key_t key = ftok(FILE, 'a');
	time_t sendTime = time(0);
	pid_t sendPid = getpid();
	data_struct sendStruct = {sendTime, sendPid};
    	shmid = (shmget(key, sizeof(data_struct), IPC_CREAT | 0666));
    	if(shmid == -1){
        	printf("Can't create shared memory\n");
        	exit(0);
    	}
    	if((addr = shmat( shmid, NULL, 0 )) == (void*)-1){
        	printf("Shmat error\n");
		exit(0);
	}
	shmctl(shmid, IPC_STAT,&shmidStat);
	if(shmidStat.shm_nattch > 1){
                printf("there is already a sending process\n");
                exit(0);
        }
    	time_t buft = sendTime;
    	while(1){
		sleep(3);
		sendTime = time(0);
		sendPid = getpid();
		sendStruct.time = sendTime;
		sendStruct.pid = sendPid;
        	if (sendTime != buft){
            		buft = sendTime;
			*((data_struct*)addr) = sendStruct;
        		printf("Placed val :\n Time:%s Pid:%d\n",ctime(&sendStruct.time), sendStruct.pid);
        	}
    	}
	shmdt(addr);
	shmctl(shmid, IPC_RMID, NULL);
    	return 0;
}
