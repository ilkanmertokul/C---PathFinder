#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


//enumerated typedef to get rid of magic numbers and letters in program.
typedef enum {FILE_ERR = -1,FILE_NOINPUT =0,FILE_D = 1, FILE_S, FILE_B, FILE_C, FILE_F, FILE_P, FILE_L} f_type;

typedef struct search_info{

	char *directory; //MANDOTORY , START POINT

	char *permissions; //permissions as 9 chars
	char *file_name; //case-insensitive

	f_type file_type; 

	int num_of_links; 
	int byte_size; 

} Search;

/*	function get_type
*	Gets a char and returns a file type according to it.
*	Gets its enums from FILE-TYPE.h
*/
f_type get_type(char c);


/*	function initialize
*	Returns an empty Search instance.
*/
Search initialize();


/* 	function compare_char
*	takes two character inputs and
* 	returns their equality without any case sensivity.
*/
int compare_char(char a, char b);


/* 	function checkall
* 	takes Search struct and stat struct and compares,
* 	if they are equal on given conditions, returns 1, else -1.
*/
int checkall(Search rules, struct stat stats, char * filename);


/* 	function checktype
*	compares file types of given inputs.
*	returns value according to it.
*/
int checktype(f_type type,struct stat stats);


/* 	function checkperm
*	this function checks permissions of given inputs
* 	and returns vale according to it.
*/
int checkperm(Search rules,struct stat stats);


/*	function regexp_plus
*	takes regular expression and filename and compares.
* 	returns 1 if matched, 0 if not matched, -1 if error happened. 
*/
int regexp_plus(Search rules, char * filename);


//---

//Returns -1 if directory is null, because it is mandotory.
//It is not used in main.c in this homework, just made for checking.
int debug_printsearch(Search s);