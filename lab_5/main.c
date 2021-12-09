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

void *read_func(void *arg){
	for(int i = 0; i < 10; i++){
                pthread_mutex_lock(&mutex);
		i++;
                printf("Я %d-й поток, мой tid =  %lu, счетчик  = %d\n", i, pthread_self(), count);
                pthread_mutex_unlock(&mutex);
		i--;
                sleep(2);
        }
        pthread_exit(NULL);
} 

void *write_func(void *arg){
	for(int i = 0; i < 10; i++){
                pthread_mutex_lock(&mutex);
                count = count + 1;
                pthread_mutex_unlock(&mutex);
                sleep(2);
        }

}

int main(int argc, char** argv){
	pthread_t write,read[10];
	pthread_create(&write, NULL, write_func, NULL);
	for(int i = 0; i < 10; i++){
		pthread_create(&read[i], NULL, read_func, NULL);
	}
	pthread_join(write,NULL);	
	return 0;
}
