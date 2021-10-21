#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void handler(){
	printf("Потомок %d и родитель %d завершины.\n", getpid(), getppid());
}

int main(){
	pid_t pid;
	int exit_stat = 0;
	atexit(handler);
	switch(pid=fork()) {
	case -1:
		exit(1);
	case 0:
		printf(" CHILD: Это процесс-потомк!\n");
		printf(" CHILD: Мой  PID -- %d\n", getpid());
		printf(" CHILD: PID моего родителя -- %d\n", getppid());
		printf(" CHILD: Выход!\n");
		exit(exit_stat);
	default:
		printf("PARENT: Это процесс-родитель!\n");
		printf("PARENT: Мой PID -- %d\n", getpid());
		printf("PARENT: PID моего родителя %d\n",getppid());
		printf("PARENT: Я жду, пока потомк не вызовет exit()...\n"); wait(&exit_stat);
		printf("PARENT: Код возврата потомка:%d\n", WEXITSTATUS(exit_stat));
		printf("PARENT: Выход!\n");
	}
	return 0;
}



