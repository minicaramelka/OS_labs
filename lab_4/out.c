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
#include <sys/sem.h>
#include <signal.h>

typedef struct data_struct{
	time_t time;
	pid_t pid;
} data_struct;

#define FILE "shared_memory"

int shmid;

struct sembuf sem_lock = {0,-1,0}, sem_open = {0,1,0};

void funcExit(int sig){
	printf("\n");
    	struct shmid_ds *buf = 0;
    	shmctl(shmid, IPC_RMID, buf);
    	exit(0);
}

int main(int argc, char* argv[]){
	signal(SIGINT, funcExit);
	struct shmid_ds shmidStat;
	void * addr;
	key_t key = ftok(FILE, '1');
	time_t sendTime = time(0);
	pid_t sendPid = getpid();
	data_struct sendStruct = {sendTime, sendPid};
    	shmid = (shmget(key, sizeof(data_struct), IPC_CREAT | 0666));
	int semid = (semget(key, 1, IPC_CREAT | 0666));
	if(semid == -1){
        	printf("Can't create sem\n");
        	exit(0);
    	}
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
	semop(semid, &sem_open, 1);
	while(1){
		semop(semid, &sem_lock, 1);
		sendTime = time(0);
		sendPid = getpid();
		sendStruct.time = sendTime;
		sendStruct.pid = sendPid;
		buft = sendTime;
               	*((data_struct*)addr) = sendStruct;
		sleep(5);
		semop(semid, &sem_open, 1);
		printf("send time = %s | send pid = %d\n",ctime(&sendStruct.time), sendStruct.pid);
    	}
	shmdt(addr);
	shmctl(shmid, IPC_RMID, NULL);
    	return 0;
}
