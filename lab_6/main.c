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
pthread_mutex_t mutex;
pthread_cond_t cond;

void *read_func(void *arg){
	for(int i = 0; i < 10; i++){
                pthread_mutex_lock(&mutex);
		pthread_cond_wait(&cond,&mutex);
                printf("мой tid =  %x, счетчик  = %d\n", pthread_self(), count);
                pthread_mutex_unlock(&mutex);
        }
        pthread_exit(NULL);
} 

void *write_func(void *arg){
	for(int i = 0; i < 10; i++){
                pthread_mutex_lock(&mutex);
                count = count + 1;
		pthread_cond_broadcast(&cond);
                pthread_mutex_unlock(&mutex);
                sleep(2);
        }

}

int main(int argc, char** argv){
	pthread_t write,read[10];
	for(int i = 0; i < 10; i++){
		pthread_create(&read[i], NULL, read_func, NULL);
	}
	pthread_create(&write, NULL, write_func, NULL);
	pthread_join(write,NULL);
	for(int i = 0;i < 10;i++){
		pthread_join(read[i],NULL);	
	}
	pthread_mutex_destroy(&mutex);
        pthread_cond_destroy(&cond);	
	return 0;
}
