# my-own-shell
C implementation of  shell program. This software is a terminal (shell/bash) simulator with linux command 
                          Software installation and user guide 
     This doc contains information regarding how to build and run myshell program. 
 
 ***********what this program do************ 
This software is a terminal (shell/bash) simulator with linux command ,
This shell print the user and the host in this format: user@current dir> then waiting for command, and try to run the command that the user write.  
 ***********Included Files: *********** 
My assignment solution consists of the following files:  - myshell.c  - makefile - batch ( a batch file)  

********** How to Compile this program************* 
in order to compile in linux needed to use gcc command worthwhile compile by using the file "makefile" to compile all files. using the command : "make"  

 ***********Running my solution: *********** 
 You can run my assignment as follows:  > make This will create an executable file called myshell, and other files. 
 
 ***********Compiler: ***********   
gcc version 4.6.3  
                  
 
 ***********How to run this program*********** 
command line: "./<the name of the file that the makefile created>" (in this case "./shell"). 


 **********Description of program files************* 
-shell.c : hold the main and the functions  -makefile - this file compiling the file type .c and creating an "out" file that contain the program.   


***********Instructions for cleaning executable: *********** 
Run "make clean" to delete the myshell executable file. This will also delete the files created when it was first complied   



**********Description What input we needed************* 
the input need to be real command.  (you can write not real command - the program print: "command not found").  for exit write the word quit or eof and press enter.   
***********An explanation of the output we get************ 
if the command not exist - the output will: "command not found".  else the output will the out that we given from the orginal sheel  for example - for the command: ls -l :   the orginal sheel display list of the files that there are in the folder  and this program display the same.  
                          Software installation and user guide 
   

