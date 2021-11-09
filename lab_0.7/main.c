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
	FILE *arch, *file;
	char currentSymbol;
	int fileSize = 0;
	int archSize = 0;
	int bufFileSize = 0;
	char* bufFileName;
	bufFileName = malloc(strlen(inputFile));
	if((arch = fopen(archive, "a+")) != NULL){
		if((file = fopen(inputFile, "r")) != NULL){
			fseek(arch, 0, SEEK_END);
                        archSize = ftell(arch);
			fseek(arch, 0, SEEK_SET);
			if(archSize != 0){
				int find = 0;
				while(!feof(arch)){
					fscanf(arch, "%d %s", &bufFileSize, bufFileName);
					if(strcmp(inputFile, bufFileName) == 0){
                                        	++find;
                                		break;
                                	}
					else{
						for(int i = 0; i < bufFileSize; i++){
							fgetc(arch);
						}
					}
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
                                	fprintf(arch, "%d %s\n", fileSize, inputFile);
                                	while (!feof(file)){
                                        	currentSymbol = fgetc(file);
                                        	if (currentSymbol == EOF)
                                                	break;
                                        	fprintf(arch, "%c", currentSymbol);
                                	}
					fclose(file);
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
				fprintf(arch, "%d %s\n", fileSize, inputFile);
				while (!feof(file)){
        				currentSymbol = fgetc(file);
        				if (currentSymbol == EOF)
        					break;
        				fprintf(arch, "%c", currentSymbol);
				}
				fclose(file);
			}
		}
		else
			printf("Нет такого файла\n");
        	fclose(arch);
	}
	else{
		free(bufFileName);
		printf("Ошибка!\n");
		return -1;
	}
	free(bufFileName);
}

void extract(char* archive, char* outputFile){
	FILE *arch, *file;
	int archSize = 0;
	int bufFileSize = 0;
        char* bufFileName;
	if((arch = fopen(archive, "r")) != NULL){
        	bufFileName = malloc(strlen(outputFile));
		fseek(arch, 0, SEEK_END);
        	archSize = ftell(arch);
        	fseek(arch, 0, SEEK_SET);
		char* beforeFind = 0;
                int beforeSize = 0;
		if(archSize > 0){
			int find = 0;
			int counter = 1;
                	while(!feof(arch)){
                		fscanf(arch, "%d %s", &bufFileSize, bufFileName);
				int bufSize = bufFileSize;
                        	if(strcmp(outputFile, bufFileName) == 0){
                                        while(bufSize / 10 > 0){
                                                bufSize = bufSize /10;
                                                counter++;
                                        }
                        		++find;
                                	break;
                        	}
                        	else{
        	                        while(bufSize / 10 > 0){
						bufSize = bufSize /10;
	                                        counter++;
					}
                	                beforeSize = beforeSize + bufFileSize + strlen(bufFileName) + counter + 2;
                        		for(int i = 0; i < bufFileSize; i++){
                                		fgetc(arch);
                                	}
					counter = 1;
				}
                	}
                	if(find > 0){
				file = fopen(outputFile, "w");
				char* delAr[bufFileSize];
				char* afterFind = 0;
				int afterSize = archSize - beforeSize - bufFileSize - strlen(bufFileName) - counter - 2;
				fseek(arch, 0, SEEK_SET);
				if(beforeSize != 0){
                                        beforeFind = malloc(beforeSize);
                                        fread(beforeFind, 1, beforeSize, arch);
                                }
				int bufStrSize = strlen(bufFileName) + counter + 2;
				for(int i = 0; i < bufStrSize; i++){
                                                fgetc(arch);
                                }
				fread(delAr, bufFileSize, 1, arch);
                                fwrite(delAr, bufFileSize, 1, file);
                                if(afterSize != 0){
                                        afterFind = malloc(afterSize);
                                        fread(afterFind, 1, afterSize, arch);
                                }
				fclose(arch);
				if((arch = fopen(archive, "w")) != NULL){
                                	if(beforeFind != 0)
                                        	fwrite(beforeFind, beforeSize, 1, arch);
                                	if(afterFind !=0)
                                        	fwrite(afterFind, afterSize, 1, arch);
                                	fclose(arch);
                        	}
				fclose(file);
			}
			else
                        	printf("Нет такого файла\n");
		}
		else
			printf("Архив пустой\n");
	}
	else
		printf("Нет такого архива\n");
	
}


void archStat(char* archive){
	FILE *arch;
	int bufFileSize = 0;
        char bufFileName[1024];
	int archSize = 0;
	if((arch = fopen(archive, "r")) != NULL){
		fseek(arch, 0, SEEK_END);
                archSize = ftell(arch);
                fseek(arch, 0, SEEK_SET);
		if(archSize != 0){
			int Size = 0;
			printf("Файлы в архиве:\n");
			while(archSize > 0){
                        	fscanf(arch, "%d %s", &bufFileSize, bufFileName);
				printf("%s\n", bufFileName);
				int counter = 1;
				int bufSize = bufFileSize; 
				while(bufSize / 10 > 0){
					bufSize = bufSize /10;
					counter++;
				}
				archSize = archSize - bufFileSize - strlen(bufFileName) - counter - 2;
				for(int i = 0; i < bufFileSize; i++){
                                	fgetc(arch);
                                }
				Size = Size + bufFileSize;
			}
			printf("Размер архива: %d\n", Size);
		}
		else{
			printf("Файлов нет\n");
                        printf("Размер архива: 0\n");
		}
	}
	else
		printf("Нет такого архива\n");
}

void help(){
	printf("./lab arch_name -i file1 : добавить файл в архив\n");
	printf("./lab arch_name -e file1 : удалить файл из архива\n");
	printf("./lab arch_name -s : вывести текущее состояние архива\n");
	printf("./lab -h : вывести справку по работе с архиватором\n");
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
				if(archName != 0){
					input(archName, optarg);
				} 
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
				if(archName != 0)
					archStat(archName);
				else
                                        printf("Invalid archname\n");
                                break; 
			case 'h': 
				help(); 
				break;
			default:  
				break;
		}
	}
}
