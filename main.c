#include <unistd.h>

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <dirent.h>

#include <getopt.h>

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

//FILE-S WRITTEN BY ME
#include "file-type.h"

#define fileflags O_RDONLY

/* 	function search_file
*	RECURSIVELY goes directories.
*	prints out directory and found files according to given rule set.
*/
int search_file(Search rules, DIR * dir, char* current_pathname,int dir_level,int foundflag){

	char temp_path[100];
	DIR * newdir;
	int fd,i;
	struct stat stat_current;
	struct dirent * files;

	while ((files = readdir(dir)) != NULL) {

		//checks if the file is not . or ..
      	if(strcmp(files->d_name,"..") != 0 && strcmp(files->d_name,".") != 0){
      	
      		if(files->d_type == DT_DIR){
				for(i = 0 ; i<dir_level*2 ; i++ ) printf("-");
      				printf("%s ", files->d_name);
      		}

      		//Prepare pathname

      		memset(temp_path,'\0',100);
     		i=0;
      		do{
      			temp_path[i] = current_pathname[i];
      			i++;
      		}while(current_pathname[i] != '\0');
      		strcat(temp_path,"/");
      		strcat(temp_path,files->d_name);

      		//Open pathname
      		
      		if((fd = open(temp_path,fileflags)) == -1){
      			perror("open");
      			return -1;
      		}

      		if(fstat(fd,&stat_current) == -1){
      			perror("open");
      			return -1;
      		}

      		if(close(fd) == -1){
      			perror("close");
      			return -1;
      		}
      		
      		//CHECKS IF GIVEN FILE IS WHAT WE SEARCHED FOR.

      		if(checkall(rules,stat_current,files->d_name) == 1){
      			foundflag = 1;

      			if(files->d_type != DT_DIR){
					for(i = 0 ; i<dir_level*2 ; i++ ) printf("-");
      					printf("%s ", files->d_name);
      			}
      			printf("FOUND!");
      		};

      		printf("\n");

      		//IF found file is directory, it calls it too.
      		if(files->d_type == DT_DIR){
      			if((newdir = opendir(temp_path)) == NULL){
					perror("opendir");
					return 1;
				}
      			foundflag = search_file(rules, newdir, temp_path, dir_level+1, foundflag);
      			closedir(newdir);
      		}
    	}
    }

    free(files);
	return foundflag;
}

int main (int argc, char **argv) {

	DIR * dir;
	int i , chosen; 
	char char_of_ftype;
	Search rules = initialize();

	//WHILE LOOP THAT PARSES ARGUMENT VALUES.
	while( (chosen = getopt(argc, argv, "w:f:b:t:p:l:")) != -1){
		switch(chosen){

			case 'w': //MANDOTORY PATH INPUT 
			 		  //input is always valid if there is a optarg.
				rules.directory = optarg;
				break;

			case 'f': //FILENAME INPUT
				rules.file_name = optarg;
				break;

			case 'b': //BYTE SIZE & ERROR CHECKS
				for(i=0; i<strlen(optarg) ;i++ ){
					if( !(optarg[i] >= '0' && optarg[i] <= '9')){
						perror("Error: invalid bytesize.");
						return 1;
					}
				}
				rules.byte_size = atoi(optarg);
				break;

			case 't': //FILETYPE INPUT
				if(strlen(optarg)!=1){
					perror("Error: invalid lenght of filetype");
					return 1;
				}
				char_of_ftype = optarg[0];
				if(get_type(char_of_ftype) == -1){
					perror("Error: invalid fileTYPE to search");
					return 1;
					}
				rules.file_type = get_type(char_of_ftype);
				break;

			case 'p': //PERMISSIONS
				if(strlen(rules.permissions = optarg)!=9){
					perror("Error: invalid permission to search");
					return 1;
				}
				break;

			case 'l': //NUMBER OF LINKS
				for(i=0; i<strlen(optarg) ;i++ ){
					if( !(optarg[i] >='0' && optarg[i] <= '9')){
						perror("Error: invalid number of links to search.");
						return 1;
					}
				}
				rules.num_of_links = atoi(optarg);
				break;

			default:
				printf("incorrect!\n");
				return 1;
		}
	}

	//CHECKS IF DIRECTORY IS NOT ENTERED
	if(rules.directory == NULL){
		perror("Error: directory must be entered.");
		return 1;
	}

	//CHECKS IF DIRECTORY CAN BE OPENED.
	if((dir = opendir(rules.directory)) == NULL){
		perror("opendir");
		return 1;
	}
	else{
		if(search_file(rules,dir,rules.directory,1,0) != 1 )
			printf("NOT a single file found for given rule set.\n");
	} 

	if(closedir(dir) == -1){
		perror("closedir");
	}

	printf("exiting\n");
	return 0;
}