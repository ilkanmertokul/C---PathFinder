# C---PathFinder

# Objective
Write an “advanced” file search program for POSIX compatible operating
systems. Program must be able to search for files satisfying the given criteria, and print out the
results in the form of a nicely formatted tree.
# How
The search criteria can be any combination of the following (at least one of them must be
employed):
• -f : filename (case insensitive), supporting the following regular expression: +<br />
• -b : file size (in bytes)<br />
• -t : file type (d: directory, s: socket, b: block device, c: character device f: regular file, p:<br />
pipe, l: symbolic link)<br />
• -p : permissions, as 9 characters (e.g. ‘rwxr-xr--’)<br />
• -l: number of links<br />
The symbol + means one or more of the character preceding it. + is the only regexp operator to support, at its
simplest form; no braces or brackets (i.e. { or } or [ or ]) just one or more of the preceding
character.
e.g. los+t+file that matches lossssstttttfile for instance. We’ll assume that the user will never
search for strings containing the actual ASCII ‘+’ character. So there is no need for escape
characters either. Just implement the + regexp, that’s all!<br />
The program will additionally receive as mandatory parameter:<br />
• -w: the path in which to search recursively (i.e. across all of its subtrees)<br />
You can use the getopt() library method for parsing command-line arguments.
# Example
./myFind -w targetDirectoryPath -f ‘lost+file‘ -b 100 -t b<br />
means it will search in the targetDirectoryPath path, for block device files named lost+file of
size exactly 100 bytes
# Output
In the form of nicely formatted tree<br />
targetDirectoryPath<br />
|--subDirectory<br />
|----subDirectory2<br />
|------subDirectory3<br />
|--------LOStttttttFile<br />
|--subDirectory6<br />
|----lOstFile<br />
If no file satisfying the search criteria has been found, then simple output “No file found”. All error
messages are to be printed to stderr. All system calls are to be checked for failure and the user is to
be notified accordingly.

# REPORT

## Design Choice on Parsing Command Line:
### This part explained is in main.c , Lines 108 -to-164
This part has become pretty straightfoward with the help of getopt(). This function basically
parses the commant line with given input, in a while loop with switch cases that checks if
command line has come to an end.<br />
It checks 6 different options : -w, -f, -b, -t, -p, -l. <br />
Each of those options have different rules, for example, -b indicates bits and should only
have digits in its argument, so there will be an error given if input is like “15QWE8” instead
of pure digit input.<br />
An other example, if -p (permissions) is not 9-lenght string, there must be an error and
program should end.<br />
And we have an mandotory input, -w, which is directory of starting file-search. This
condition is checked in the end of while loop.<br />
## Other main(..) checks:
### This part explained is in main.c , Lines 172-to-188, end of file.
This part is what starts the main function, search_file() function. Before it does, it opens dir
for given directory inputs and checks if there is an error. Before explaining search_file()
function, i should explain file-type.h and its data structures.<br />
After search_file() function ends, main() closes the dir and exits the program.<br />
## ”file-type.h” and “file-type.c” files:
### This part explained is in “file-type.h” , Lines 7-to-21.
Struct Search is just a compact struct that holds the information that user given as
commandline argument.<br />
Enum f_type is just a compact enumeration that holds file type, that is converted from
chars, to hold in Search.<br />
## Functions of file-type.c:
### This part explained is in “file-type.c” , Lines 13-265.
The functions written in this file is already explained well in that file, but lets give some more
information.<br />
### f_type get_type(char c);
This function converts the char input to enum f_type, returns FILE_ERR if not recognized.
It is pure switch-case function.
### Search initialize();
This function just made for struct
initialization, because i needed to know if
user give spesific parameter or not. For
example bytes set -1 here.
### int checkperm(Seach rules, struct stat stats);
This function generates permission of file, from stat struct that can be accessed from
<sys/stat.h>, which i gathered from fstat(). When i talk about permission generating , i mean
that it will generate 9 lenght-string from “stat” and compare it to user-given permission.
### int checktype(f_type type,struct stat stats);
This function gets file type from
struct stat, and it compares to
f_type input from user, and
returns value according to it. For
example if f_type is DIR, this
function will return the value of
S_ISDIR(), which checks if given file stat is dir or not.
### int regexp_plus(Search rules, char * filename);
This is the regular-expression parser for plus operator. Returns 1 if equal, 0 if not equal, -1 if
the given format is not correct, Error.. Used a char lastplus to keep track of given +
operators, it helps astoundly well.
### int compare_char(char a,char b);
This function helps regexp_plus(..) to be an case – insensitivite. It just takes 2 chars and
returns if they are equal, without sensitivity. Wrote by thinking that this will also work with
non char characters like ‘8’.
### int checkall(Search rules, struct stat stats, char * filename);
The last function of file-type.c, this uses all functions above to check if all those given rules is
matched, returns 1 if succesful. It checks rules’ parameters one by one.
## search_file(..) Function :
### This part explained is in main.c , Lines 24-to-98.
Main function that recursively looks for directories and finds matched rule files. <br />
Parameters:<br />
• Search rules : Rule struct given by user, defined in file-type.h.<br />
• DIR * dir: dir information to hold, because files in it can be accessed here, from
dirent.h.<br />
• char *current_pathname: This needs to be hold, because this function is recursive
and to go foward and keep tracking , we need that.<br />
• int dir_level: This helps program to print out directory tree better. It levels up by 1
every directory it enters.<br />
• int foundflag: This only holds the information if ANY files that satisfies the rules could
be found.<br />
Function is recursive through directories, but runs a small while loop in itself to check every
single file in directory.


