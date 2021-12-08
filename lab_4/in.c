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
#include <sys/sem.h>

typedef struct data_struct{
	time_t time;
	pid_t pid;
} data_struct;

#define FILE "shared_memory"

void funcExit(int sig){
    printf("\n");
    exit(0);
}

struct sembuf sem_lock = {0,-1,0}, sem_open = {0,1,0};

int main(){
	signal(SIGINT, funcExit);
	void* addr;
	int mem;
	int semid;
	key_t key = ftok(FILE, '1');
	if((mem = (shmget(key, sizeof(data_struct), IPC_CREAT | 0666))) == -1 ){
                printf("SM open error\n");
                exit(0);
        }
	if((semid = (semget(key, 1, IPC_CREAT | 0666))) == -1 ){
        	printf("Sem open error\n");
        	exit(0);
    	}
    	if((addr = shmat(mem, NULL, 0)) == (void*)-1){
		printf("SM link error\n");
        	exit(0);
    	}
	semop(semid, &sem_lock, 1);
	time_t timer = time(0);
	data_struct getStruct = *((data_struct*)addr);
        printf("\ntime_Consumer = %s | pid_Consumer = %d\n", ctime(&timer), getpid());
	printf("sent time = %s | sent pid = %d\n", ctime(&getStruct.time), getStruct.pid);
	semop(semid, &sem_open, 1);
	return 0;
}
