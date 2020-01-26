*****************************README*****************************

Author: Holly Strauch
1/21/202
CS470 Lab1

Program: cwuShell

Description: Program: Runs a custom shell for Linux systems.  
Shell accepts 4 customized commands, along with the standard 
Linux Commands.

“I pledge that I have neither given nor received help from anyone 
other than the instructor/TA for all program components included here!”

**************************COMPILE & RUN*************************

Compile code in terminal by navigating into the directory containing 
the files and enter the following command:

    g++ -std=c++0x main.cpp Shell.cpp Shell.h
    
Run the code with the following command:

    ./a.out
   
*********************INTERNAL COMMANDS***************************

To display options in shell enter 'manual'


change_prompt [new_prompt_name]: changes the current shell prompt 
    to the new_prompt_name. Entering change_prompt with no parameter 
    will reset to the default shell prompt.
    
distro [OPTION]: Displays CPU information
    -v: version
    -n: name
    -c: codename
info [OPTION]: Displays memory information
    -s: current memory page size in KB
    -n: currently available page numbers
    -p: number of available processors
quit [n]: terminates the shell.  User can enter a numerical argument 
    n as the exit value