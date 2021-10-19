# C---PathFinder

YOU SHOULD READ REPORT TO SEE HOW IT IS WRITTEN. THIS README EXPLAINS HOW TO USE PROGRAM.

# Objective
You are expected to write an “advanced” file search program for POSIX compatible operating
systems. Your program must be able to search for files satisfying the given criteria, and print out the
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
The symbol + means one or more of the character preceding it. You are to implement it on your
own. Don’t use a library for regexp parsing. + is the only regexp operator to support, at its
simplest form; no braces or brackets (i.e. { or } or [ or ]) just one or more of the preceding
character. Check the output example for more details. Ask questions at the forum if it’s still unclear.
Let’s simplify this, because I grew tired of your emails. You can have multiple occurrences of + as
regexp; e.g. los+t+file that matches lossssstttttfile for instance. We’ll assume that the user will never
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
