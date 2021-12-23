#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

int count = 0;
pthread_rwlock_t lock = PTHREAD_RWLOCK_INITIALIZER;

void *read_func(void *arg){
	for(int i = 0; i < 10; i++){
                pthread_rwlock_rdlock(&lock);
                printf("мой tid =  %x, счетчик  = %d\n", pthread_self(), count);
		pthread_rwlock_unlock(&lock);
                sleep(2);
        }
        pthread_exit(NULL);
} 

void *write_func(void *arg){
	for(int i = 0; i < 10; i++){
                pthread_rwlock_wrlock(&lock);
                count = count + 1;
                pthread_rwlock_unlock(&lock);
                sleep(2);
        }

}

int main(int argc, char** argv){
	pthread_t write,read[10];
	pthread_rwlock_init (&lock, NULL);
	pthread_create(&write, NULL, write_func, NULL);
	for(int i = 0; i < 10; i++){
		pthread_create(&read[i], NULL, read_func, NULL);
	}
	pthread_join(write,NULL);
	for (int i = 0; i < 10; i++) {
    		pthread_join(read[i], NULL);
	}
	pthread_rwlock_destroy(&lock);
	return 0;
}
