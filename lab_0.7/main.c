#include <dirent.h>
#include <unistd.h>
#include <getopt.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>

int input(char* archive, char* inputFile){
	FILE *arch, *file, *statInfo;
	char currentSymbol;
	int fileSize = 0;
	if((arch = fopen(archive, "a+")) !=NULL){
		if((file = fopen(inputFile, "r")) != NULL){
			statInfo = fopen("Stat.txt", "a+");
		        size_t counter = 0;
        		int ch;
        		for (ch = getc(statInfo); ch != EOF; ch = getc(statInfo)){
                		if(ch == '\n' )
                        	counter = counter + 1;
        		}
        		fseek(statInfo, 0, SEEK_SET);
			if(counter > 0){
                        	struct info{
                                	char fileName[1024];
                                	int fileSize;
                        	};
                        	struct info ar[counter];
                        	char i = 0;
                        	int find = 0;
                        	while (!feof(statInfo) && i != counter){
                                	fscanf(statInfo, "%d %s", &ar[i].fileSize, ar[i].fileName);
                                	if(strcmp(inputFile, ar[i].fileName) == 0){
                                        	++find;
                                        	break;
                                	}
                                	else
                                        	i++;
                        	}
                        	if(find > 0){
                                	printf("Файл с таким имененм уже есть\n");
					return 0;
				}
				else{
					if(file == NULL)
                                        	fileSize = 0;
                                	else{
                                        	fseek(file, 0, SEEK_END);
                                        	fileSize = ftell(file);
                                	}
                                	fseek(file, 0, SEEK_SET);
                                	while (!feof(file)){
                                        	currentSymbol = fgetc(file);
                                        	if (currentSymbol == EOF)
                                                	break;
                                        	fprintf(arch, "%c", currentSymbol);
                                	}
                                	statInfo = fopen("Stat.txt", "a");
                                	fprintf(statInfo, "%d %s\n", fileSize, inputFile);
				}
			}
			else{
				if(file == NULL)
					fileSize = 0;
				else{
					fseek(file, 0, SEEK_END);
					fileSize = ftell(file);
				}
				fseek(file, 0, SEEK_SET);
				while (!feof(file)){
        				currentSymbol = fgetc(file);
        				if (currentSymbol == EOF)
        					break;
        				fprintf(arch, "%c", currentSymbol);
				}
				statInfo = fopen("Stat.txt", "a");
				fprintf(statInfo, "%d %s\n", fileSize, inputFile);
			}
		}
		fclose(file);
        	fclose(arch);
		fclose(statInfo);
	}
	else{
		printf("Ошибка!\n");
		return -1;
	}
}

void extract(char* archive, char* outputFile){
	FILE *arch, *file, *statInfo;
	statInfo = fopen("Stat.txt", "a+");
	size_t counter = 0;
        int ch;
	fseek(statInfo, 0, SEEK_SET);
        for (ch = getc(statInfo); ch != EOF; ch = getc(statInfo)){
                if(ch == '\n' )
                        counter = counter + 1;
        }
        if(counter > 0){
		if((file = fopen(outputFile, "r")) != NULL){
			struct info{
        			char fileName[1024];
                		int fileSize;
        		};
        		struct info helpArch[counter];
        		char findPos = 0;
			int find = 0;
			char* beforeFind = 0;
			int beforeSize = 0;
			fseek(statInfo, 0, SEEK_SET);
        		while (!feof(statInfo) && findPos != counter){
        			fscanf(statInfo, "%d %s", &helpArch[findPos].fileSize, helpArch[findPos].fileName);
                		if(strcmp(outputFile, helpArch[findPos].fileName) == 0){
					++find;
					break;
				}
				else{
					beforeSize = beforeSize + helpArch[findPos].fileSize;
                			findPos++;
				}
        		}
			if(find > 0){
				int archSize = 0;
				char i;
				struct info ar[counter];
				int outputSize = helpArch[findPos].fileSize;
				fseek(statInfo, 0, SEEK_SET);
				while (!feof(statInfo) && i != counter){
					fscanf(statInfo, "%d %s", &ar[i].fileSize, ar[i].fileName);
					archSize = archSize + ar[i].fileSize;
					i++; 
				}
				char* afterFind = 0;
                                int afterSize = archSize - beforeSize - outputSize;
				if((arch = fopen(archive, "r")) != NULL){
					if(beforeSize != 0){
						beforeFind = malloc(beforeSize);
						fread(beforeFind, 1, beforeSize, arch);
					}
					for(int i = 0; i < outputSize; i++){
						fgetc(arch);
					}
					if(afterSize != 0){
						afterFind = malloc(afterSize);
						fread(afterFind, 1, afterSize, arch);
					}
					fclose(arch);
				}
				if((arch = fopen(archive, "w")) != NULL){
					if(beforeFind != 0)
						fwrite(beforeFind, beforeSize, 1, arch);
					if(afterFind !=0)
						fwrite(afterFind, afterSize, 1, arch);
					fclose(arch);
				}
				fclose(statInfo);
				if((statInfo = fopen("Stat.txt", "w")) != NULL){
					fseek(statInfo, 0, SEEK_SET);
					for(int i = 0; i < counter; i++){
						if(strcmp(ar[i].fileName, helpArch[findPos].fileName) != 0){
							fprintf(statInfo, "%d %s\n", ar[i].fileSize, ar[i].fileName);
						}
					}
					fclose(statInfo);	
				}
			}		
			else
				printf("Нет такого файла\n");
			fclose(file);
		}
	}
	else
		printf("Архив пустой\n");
}

void archStat(){
	FILE *statInfo;
	if(statInfo = fopen("Stat.txt", "r")){
		size_t counter = 0;
		int ch;
		for (ch = getc(statInfo); ch != EOF; ch = getc(statInfo)){
        		if(ch == '\n' ) 
        			counter = counter + 1; 
    		}       
		printf("Количество файлов в архиваторе: %d\n", counter);
		fseek(statInfo, 0, SEEK_SET);
		if(counter > 0){
			size_t archSize = 0;
			struct info{
				char fileName[1024];
				int fileSize;
			};
			struct info arch[counter];
			char i = 0;
			printf("Файлы в архиве:\n");
			while (!feof(statInfo) && i != counter){
				fscanf(statInfo, "%d %s", &arch[i].fileSize, arch[i].fileName);
				printf("%s\n", arch[i].fileName);
				archSize = archSize + arch[i].fileSize; 
        			i++;
    			}
			printf("Размер архива: %d\n", archSize);
		}
		else{
			printf("Файлов нет\n");
			printf("Размер архива: 0\n");
		}
		fclose(statInfo);                                            
	}
}

void help(){
	printf("./archiver arch_name -i(--input) file1 : добавить файл в архив\n");
	printf("./archiver arch_name -e(--extract) file1 : удалить файл из архива\n");
	printf("./archiver arch_name -s(--stat) : вывести текущее состояние архива\n");
	printf("./archiver arch_name -h(--help) file1 : вывести справку по работе с архиватором\n");
}

int main(int argc, char* argv[]){
	int rez = 0;
	char *filename, *archName;
	if((argc > 1) && (argv[1][0] != '-'))
		archName = argv[1];
	else 
		archName = NULL;
	while ((rez = getopt(argc, argv, "i:e:sh")) != -1) {
		switch (rez) {
			case 'i': 
				if(archName != 0) 
					input(archName, optarg); 
				else 
					printf("Invalid archname\n"); 
				break;
			case 'e': 
				if(archName != 0) 
					extract(archName, optarg); 
				else 
					printf("Invalid archname\n"); 
				break;
			case 's':  
				archStat(); 
				break;
			case 'h': 
				help(); 
				break;
			default:  
				break;
		}
	}
}
