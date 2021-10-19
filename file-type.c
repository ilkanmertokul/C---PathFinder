#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <string.h>
#include <stdio.h>
#include "file-type.h"

/*	function get_type
*	Gets a char and returns a file type according to it.
*	Gets its enums from FILE-TYPE.h
*/
f_type get_type(char c){

	f_type type;

	switch(c){
		case 'd': // Directory
			type = FILE_D;
			break;
		case 's': // Socket
			type = FILE_S;
			break;
		case 'b': // Block Device
			type = FILE_B;
			break;
		case 'c': //Character Device
			type = FILE_C;
			break;
		case 'f': // Regular file
			type = FILE_F;
			break;
		case 'p': // Pipe
			type = FILE_P;
			break;
		case 'l': // Symbolic link
			type = FILE_L;
			break;
		default:
			type = FILE_ERR;
	}

	return type;
}

/*	function initialize
*	Returns an empty Search instance.
*/
Search initialize(){

	Search s;
	s.directory = NULL;
	s.permissions = NULL;
	s.file_name = NULL; 

	s.file_type = FILE_NOINPUT; 

	s.num_of_links = -1; 
	s.byte_size = -1; 

	return s;
}

/* 	function checkperm
*	this function checks permissions of given inputs
* 	and returns vale according to it.
*/
int checkperm(Search rules,struct stat stats){

	char permissions[9]; 

	//user
	permissions[0] = ((stats.st_mode & S_IRUSR) ? 'r' : '-');
	permissions[1] = ((stats.st_mode & S_IWUSR) ? 'w' : '-');
	permissions[2] = ((stats.st_mode & S_IXUSR) ? 'x' : '-');

	//group
	permissions[3] = ((stats.st_mode & S_IRGRP) ? 'r' : '-');
	permissions[4] = ((stats.st_mode & S_IWGRP) ? 'w' : '-');
	permissions[5] = ((stats.st_mode & S_IXGRP) ? 'x' : '-');

	//other
	permissions[6] = ((stats.st_mode & S_IROTH) ? 'r' : '-');
	permissions[7] = ((stats.st_mode & S_IWOTH) ? 'w' : '-');
	permissions[8] = ((stats.st_mode & S_IXOTH) ? 'x' : '-');

	return strcmp(rules.permissions,permissions);
}

/* 	function checktype
*	compares file types of given inputs.
*	returns value according to it.
*/
int checktype(f_type type,struct stat stats){

	switch(type){
		case FILE_D:
			return S_ISDIR(stats.st_mode);
		case FILE_S:
			return S_ISSOCK(stats.st_mode);
		case FILE_B:
			return S_ISBLK(stats.st_mode);
		case FILE_C:
			return S_ISCHR(stats.st_mode);
		case FILE_F:
			return S_ISREG(stats.st_mode);
		case FILE_P:
			return S_ISFIFO(stats.st_mode);
		case FILE_L:
			return S_ISLNK(stats.st_mode);
		default:
			return -1;
	}
}

/* 	function compare_char
*	takes two character inputs and
* 	returns their equality without any case sensivity.
*/
int compare_char(char a, char b){

	int afl = 0, bfl = 0;

	if( a >= 'a' && a <= 'z') afl = 1;
	if( b >= 'a' && b <= 'z') bfl = 1;

	if(afl == bfl){
		return (a == b);
	}else{
		if(afl == 1) return ((a-32) == b);
		else return ((b-32) == a);
	}
}

/*	function regexp_plus
*	takes regular expression and filename and compares.
* 	returns 1 if matched, 0 if not matched, -1 if error happened. 
*/
int regexp_plus(Search rules, char *filename){

	char * regexp = rules.file_name;

	int i = 0, j = 0;

	char lastplus = '\0';

	while(regexp[i] != '\0' || filename[j] != '\0'){

		//If one of them has come to an end, this is not a find!-
		if(regexp[i] == '\0') return 0;
		if(filename[j] == '\0'){
			if(regexp[i+1]=='\0' && regexp[i] == '+') return 1;
			else return 0;
		} 

		if( lastplus == '\0'){

			if(regexp[i] != '+'){
				if(!compare_char(regexp[i],filename[j])) return 0; //Not found, return 0.
				
				i++;
				j++;
			}
			else{
				if(i == 0 || regexp[i-1] == '+') return -1; //error format, return 0.
				
				lastplus = regexp[i-1]; // store last member.

				i++;
			}
		}
		else{

			if(compare_char(regexp[i],filename[j])){
				lastplus = '\0';

				i++;
				j++;
			}
			else if(compare_char(lastplus,filename[j])){

				j++;
			}
			else return 0;
		}
	}

	return 1;
}

/* 	function checkall
* 	takes Search struct and stat struct and compares,
* 	if they are equal on given conditions, returns 1, else -1.
*/
int checkall(Search rules, struct stat stats, char * filename){

	if(rules.file_name != NULL){
		//check filename
		if(regexp_plus(rules,filename)!=1)
			return -1;
	}

	//DONE
	if(rules.byte_size != -1){
		//compare bytesizes
		if(rules.byte_size != stats.st_size)
			return -1;
	}

	//DONE
	if(rules.file_type != FILE_NOINPUT){
		//compare filetypes
		if(checktype(rules.file_type,stats) == 0)
			return -1;
	}

	//DONE
	if(rules.permissions != NULL){
		//check 
		if(checkperm(rules,stats) != 0){
			return -1;
		}
	}

	//DONE
	if(rules.num_of_links != -1){
		//check num of links
		if(rules.num_of_links != stats.st_nlink)
			return -1;		
	}

	return 1; // Succesful
}

//************************************************************//
//						NOT USED FUNCTIONS				      //
//************************************************************//

//Returns -1 if directory is null, because it is mandotory.
//It is not used in main.c in this homework, just made for checking.
int debug_printsearch(Search s){

	if(s.directory == NULL){
		perror("debug error: directory must be entered");
		return -1;
	} 
		printf("directory = %s\n",s.directory);

	if(s.permissions != NULL )
		printf("permissions = %s\n",s.permissions);

	if(s.file_name != NULL)
		printf("file_name = %s\n",s.file_name);

	if(s.file_type != FILE_NOINPUT)
		printf("file_type = %d\n",s.file_type);
	
	if(s.num_of_links != -1)
		printf("num_of_links = %d\n",s.num_of_links);
	
	if(s.byte_size != -1)
		printf("byte_size = %d\n",s.byte_size);
	
	return 0;
}