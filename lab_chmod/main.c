#include <dirent.h>
#include <unistd.h>
#include <getopt.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>

void help() {
	printf("./chmod -h // вывести справку по использованию\n");
	printf("./chmod -uaw file1 // добавить право пользователю на запись\n");
	printf("./chmod -grr file1 // убрать право группы на чтение\n");
	printf("./chmod -oarwx file1 // добавить права на запись, чтение и исполнение файла для всех остальных\n");
};

int main(int argc, char* argv[]){
	int rez = 0;
	char *filename;
	if((argc > 1) && (argv[argc - 1][0] != '-'))
		filename = argv[argc - 1];
	else{
		filename = NULL;
		printf("Нет названия файла!\n");
                return 1;
	}
	struct stat stFile;
	if(filename != NULL){
		if(access(filename,0) == 0){
			stat(filename,&stFile);
		}
		else{
			printf("Файла не существует\n");
			return 1;
		}
	}
	mode_t curMode = stFile.st_mode;
	while((rez = getopt(argc, argv, "ugoarwxh")) != -1){
		switch (rez){
			case 'h':
				if(argc != 2  && !argv[1][2]){
					printf("Неправильный запрос\n");
					return 0;
				}
				help();
				return 0;
				break;
			case 'u':
				if(argv[1][1] == 'u' && argv[1][2] == 'a' && argv[1][3] == 'r' && argv[1][4] == 'w' && argv[1][5] == 'x' && !argv[1][6]){
					chmod(filename, (S_IRUSR | S_IWUSR | S_IXUSR) | curMode);
                                }
                                else if(argv[1][1] == 'u' && argv[1][2] == 'r' && argv[1][3] == 'r' && argv[1][4] == 'w' && argv[1][5] == 'x' && !argv[1][6]){
                                        chmod(filename, (~(S_IRUSR | S_IWUSR | S_IXUSR)) & curMode);
                                }
                                else if(argv[1][1] == 'u' && argv[1][2] == 'a' && argv[1][3] == 'r' && argv[1][4] == 'w' && !argv[1][5]){
                                        chmod(filename, (S_IRUSR | S_IWUSR) | curMode);
                                }
                                else if(argv[1][1] == 'u' && argv[1][2] == 'r' && argv[1][3] == 'r' && argv[1][4] == 'w' && !argv[1][5]){
                                        chmod(filename, (~(S_IRUSR | S_IWUSR)) & curMode);
                                }
                                else if(argv[1][1] == 'u' && argv[1][2] == 'a' && argv[1][3] == 'r' && argv[1][4] == 'x' && !argv[1][5]){
                                        chmod(filename, (S_IRUSR | S_IXUSR) | curMode);
                                }
                                else if(argv[1][1] == 'u' && argv[1][2] == 'r' && argv[1][3] == 'r' && argv[1][4] == 'x' && !argv[1][5]){
                                        chmod(filename, (~(S_IRUSR | S_IXUSR)) & curMode);
                                }
                                else if(argv[1][1] == 'u' && argv[1][2] == 'a' && argv[1][3] == 'w' && argv[1][4] == 'x' && !argv[1][5]){
                                        chmod(filename, (S_IWUSR | S_IXUSR) | curMode);
                                }
                                else if(argv[1][1] == 'u' && argv[1][2] == 'r' && argv[1][3] == 'w' && argv[1][4] == 'x' && !argv[1][5]){
                                        chmod(filename, (~(S_IWUSR | S_IXUSR)) & curMode);
                                }
                                else if(argv[1][1] == 'u' && argv[1][2] == 'a' && argv[1][3] == 'w' && !argv[1][4])
                                        chmod(filename, S_IWUSR | curMode);
                                else if(argv[1][1] == 'u' && argv[1][2] == 'r' && argv[1][3] == 'w' && !argv[1][4])
                                        chmod(filename, (~S_IWUSR) & curMode);
                                else if(argv[1][1] == 'u' && argv[1][2] == 'a' && argv[1][3] == 'r' && !argv[1][4])
                                        chmod(filename, S_IRUSR | curMode);
                                else if(argv[1][1] == 'u' && argv[1][2] == 'r' && argv[1][3] == 'r' && !argv[1][4])
                                        chmod(filename, (~S_IRUSR) & curMode);
                                else if(argv[1][1] == 'u' && argv[1][2] == 'a' && argv[1][3] == 'x' && !argv[1][4])
                                        chmod(filename, S_IXUSR | curMode);
                                else if(argv[1][1] == 'u' && argv[1][2] == 'r' && argv[1][3] == 'x' && !argv[1][4])
                                        chmod(filename, (~S_IXUSR) & curMode);
                                else
                                        printf("Неправильный запрос\n");

				break;
			case 'g':
				if(argv[1][1] == 'g' && argv[1][2] == 'a' && argv[1][3] == 'r' && argv[1][4] == 'w' && argv[1][5] == 'x' && !argv[1][6]){
					chmod(filename, (S_IRGRP | S_IWGRP | S_IXGRP) | curMode);
                                }
				else if(argv[1][1] == 'g' && argv[1][2] == 'r' && argv[1][3] == 'r' && argv[1][4] == 'w' && argv[1][5] == 'x' && !argv[1][6]){
                                        chmod(filename, (~(S_IRGRP | S_IWGRP | S_IXGRP)) & curMode);
                                }
				else if(argv[1][1] == 'g' && argv[1][2] == 'a' && argv[1][3] == 'r' && argv[1][4] == 'w' && !argv[1][5]){
                                        chmod(filename, (S_IRGRP | S_IWGRP) | curMode);
                                }
				else if(argv[1][1] == 'g' && argv[1][2] == 'r' && argv[1][3] == 'r' && argv[1][4] == 'w' && !argv[1][5]){
                                        chmod(filename, (~(S_IRGRP | S_IWGRP)) & curMode);
                                }
                                else if(argv[1][1] == 'g' && argv[1][2] == 'a' && argv[1][3] == 'r' && argv[1][4] == 'x' && !argv[1][5]){
                                        chmod(filename, (S_IRGRP | S_IXGRP) | curMode);
                                }
				else if(argv[1][1] == 'g' && argv[1][2] == 'r' && argv[1][3] == 'r' && argv[1][4] == 'x' && !argv[1][5]){
                                        chmod(filename, (~(S_IRGRP | S_IXGRP)) & curMode);
                                }
                                else if(argv[1][1] == 'g' && argv[1][2] == 'a' && argv[1][3] == 'w' && argv[1][4] == 'x' && !argv[1][5]){
                                        chmod(filename, (S_IWGRP | S_IXGRP) | curMode);
                                }
				else if(argv[1][1] == 'g' && argv[1][2] == 'r' && argv[1][3] == 'w' && argv[1][4] == 'x' && !argv[1][5]){
                                        chmod(filename, (~(S_IWGRP | S_IXGRP)) & curMode);
                                }
				else if(argv[1][1] == 'g' && argv[1][2] == 'a' && argv[1][3] == 'w' && !argv[1][4])
					chmod(filename, S_IWGRP | curMode);
				else if(argv[1][1] == 'g' && argv[1][2] == 'r' && argv[1][3] == 'w' && !argv[1][4])
                                        chmod(filename, (~S_IWGRP) & curMode);
				else if(argv[1][1] == 'g' && argv[1][2] == 'a' && argv[1][3] == 'r' && !argv[1][4])
                                        chmod(filename, S_IRGRP | curMode);
				else if(argv[1][1] == 'g' && argv[1][2] == 'r' && argv[1][3] == 'r' && !argv[1][4])
                                        chmod(filename, (~S_IRGRP) & curMode);
				else if(argv[1][1] == 'g' && argv[1][2] == 'a' && argv[1][3] == 'x' && !argv[1][4])
                                        chmod(filename, S_IXGRP | curMode);
				else if(argv[1][1] == 'g' && argv[1][2] == 'r' && argv[1][3] == 'x' && !argv[1][4])
                                        chmod(filename, (~S_IXGRP) & curMode);
				else
					printf("Неправильный запрос\n");
				break;
			case 'o':
				if(argv[1][1] == 'o' && argv[1][2] == 'a' && argv[1][3] == 'r' && argv[1][4] == 'w' && argv[1][5] == 'x' && !argv[1][6]){
                                        chmod(filename, (S_IROTH | S_IWOTH | S_IXOTH) | curMode);
                                }
                                else if(argv[1][1] == 'o' && argv[1][2] == 'r' && argv[1][3] == 'r' && argv[1][4] == 'w' && argv[1][5] == 'x' && !argv[1][6]){
                                        chmod(filename, (~(S_IROTH | S_IWOTH | S_IXOTH)) & curMode);
                                }
                                else if(argv[1][1] == 'o' && argv[1][2] == 'a' && argv[1][3] == 'r' && argv[1][4] == 'w' && !argv[1][5]){
                                        chmod(filename, (S_IROTH | S_IWOTH) | curMode);
                                }
                                else if(argv[1][1] == 'o' && argv[1][2] == 'r' && argv[1][3] == 'r' && argv[1][4] == 'w' && !argv[1][5]){
                                        chmod(filename, (~(S_IROTH | S_IWOTH)) & curMode);
                                }
                                else if(argv[1][1] == 'o' && argv[1][2] == 'a' && argv[1][3] == 'r' && argv[1][4] == 'x' && !argv[1][5]){
                                        chmod(filename, (S_IROTH | S_IXOTH) | curMode);
                                }
                                else if(argv[1][1] == 'o' && argv[1][2] == 'r' && argv[1][3] == 'r' && argv[1][4] == 'x' && !argv[1][5]){
                                        chmod(filename, (~(S_IROTH | S_IXOTH)) & curMode);
                                }
                                else if(argv[1][1] == 'o' && argv[1][2] == 'a' && argv[1][3] == 'w' && argv[1][4] == 'x' && !argv[1][5]){
                                        chmod(filename, (S_IWOTH | S_IXOTH) | curMode);
                                }
                                else if(argv[1][1] == 'o' && argv[1][2] == 'r' && argv[1][3] == 'w' && argv[1][4] == 'x' && !argv[1][5]){
                                        chmod(filename, (~(S_IWOTH | S_IXOTH)) & curMode);
                                }
                                else if(argv[1][1] == 'o' && argv[1][2] == 'a' && argv[1][3] == 'w' && !argv[1][4])
                                        chmod(filename, S_IWOTH | curMode);
                                else if(argv[1][1] == 'o' && argv[1][2] == 'r' && argv[1][3] == 'w' && !argv[1][4])
                                        chmod(filename, (~S_IWOTH) & curMode);
                                else if(argv[1][1] == 'o' && argv[1][2] == 'a' && argv[1][3] == 'r' && !argv[1][4])
                                        chmod(filename, S_IROTH | curMode);
                                else if(argv[1][1] == 'o' && argv[1][2] == 'r' && argv[1][3] == 'r' && !argv[1][4])
                                        chmod(filename, (~S_IROTH) & curMode);
                                else if(argv[1][1] == 'o' && argv[1][2] == 'a' && argv[1][3] == 'x' && !argv[1][4])
                                        chmod(filename, S_IXOTH | curMode);
                                else if(argv[1][1] == 'o' && argv[1][2] == 'r' && argv[1][3] == 'x' && !argv[1][4])
                                        chmod(filename, (~S_IXOTH) & curMode);
                                else
                                        printf("Неправильный запрос\n");

				break;
			/*default:
				printf("Неправильный запрос\n");
                                break;*/

		}
	}
} 
