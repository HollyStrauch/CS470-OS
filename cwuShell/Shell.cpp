/**
 * Author: Holly Strauch
 * 1/20/2020
 * CS470
 * Lab 1
 * File: Shell.cpp
 *
 * Program: Runs a custom shell for Linux systems.  Shells accepts 4 customized commands, along with the standard Linux
 *  Commands
 *
 *
 * “I pledge that I have neither given nor received help from anyone other than the
 * instructor/TA for all program components included here!”
 */


#include <iostream>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>
#include <regex>
#include <unistd.h>
#include <sys/utsname.h>
#include "Shell.h"

using namespace std;

/**
 *  CONSTRUCTOR
 */
Shell::Shell(){
    D_PROPMT = "cwushell";
    prompt = D_PROPMT;
    numCmds = 5;

    setCommands();
}

/**
 * DESTRUCTOR
 */
Shell::~Shell(){
    delete[] commands;
}

/**
 * \brief Sets string array of internal commands for the shell.  Called in constructor.
 */
void Shell::setCommands(){
    commands = new string[numCmds];

    commands[0] = "quit";
    commands[1] = "change_prompt";
    commands[2] = "distro";
    commands[3] = "info";
    commands[4] = "manual";
}

/**
 * \brief Initiates the shell functionality.  Reads in user input and reates fork for each new command.
 * @return 0 if proccess run correctly, 1 if it could not fork
 */
int Shell::runShell() {
    cout << "Launching cwushell.  Enter 'manual' for help" << endl;
    string cmds = "";

    while (true) {
        cout << prompt << ">";
        getline(cin, cmds);
        pid_t pid = fork();

        if (pid == -1){
            cout << "Could not fork ";
            return 1;
        } else if (pid == 0) {
            executeCmds(cmds);
        } else {
            int status = 0;
            wait(&status);
            status = status>>8;
            return status;
        }
    }
}


/**
 * \brief Parses user input and sends it to be run as an internal command.  If input does not match an internal command,
 *  input will be sent to system() to be run as a Linux command.
 * @param cmd string input from the console
 */
void Shell::executeCmds(string cmd){
    vector<string> parsedCmds = parseInput(cmd);

    if(internalCommands(parsedCmds) == -1) {
        system(cmd.c_str());
    }

}

/**
 * \brief Takes string input from the console and tokenizes the values on spaces to get individual commands and switches
 * @param cmds string input from console
 * @return string vector with parsed values
 */
vector<string> Shell::parseInput(const string& cmds){

    regex delim(" ");
    sregex_token_iterator first{cmds.begin(), cmds.end(), delim, -1}, last;
    return {first, last};
}

/**
 * \brief Runs the internal command that will change what is displayed as the shell prompt
 * @param cmds - a string vector of user input
 * @return 0
 */
int Shell::change_prompt(vector<string> cmds){

    if (cmds.size() == 1){
        this->prompt = D_PROPMT;
    }
    else if (cmds[1] == "-h" || cmds[1] == "--help") {
        prompt_help();
        return 0;
    }
    else {
        this->prompt = cmds[1];
    }

    cmds.clear();
    return 0;
}

/**
 * \brief Prints out a help message for using the change_prompt command
 */
void Shell::prompt_help() {
    cout << "Usage: change_prompt [NEW_PROMPT]" << endl;
    cout << "Changes the shell prompt.  Leave NEW_PROMPT empty to set to the default prompt." << endl;
}

/**
 * \brief Runs the internal command that will display information about the cpu
 * @param cmds string vector of user input
 * @return 0
 */
int Shell::distro(vector<string> cmds) {

    if (cmds.size() == 1 || cmds[1] == "-h" || cmds[1] == "--help") {
        distro_help();
        return 0;
    }

    pid_t c_pid, pid;
    int status;

    c_pid = fork();

    if (c_pid == 0) {
        /* CHILD */

        if (cmds[1] == "-v") {
            execlp("lsb_release", "lsb_release", "-r", NULL);
        } else if (cmds[1] == "-n") {
            execlp("lsb_release", "lsb_release", "-i", NULL);
        } else if (cmds[1] == "-c") {
            execlp("lsb_release", "lsb_release", "-c", NULL);
        } else {
            cout << "Unknown switch..." << endl;
            exit(0);
        }

    } else if (c_pid > 0) {
        /* PARENT */

        if ((pid = wait(&status)) < 0) {
            perror("wait");
            _exit(1);
        }

    } else {
        perror("fork failed");
        _exit(1);
    }

    return 0; //return success

}

/**
 * \brief Prints out a help message for the distro command
 */
void Shell::distro_help(){
    cout << "Usage: distro [OPTION]" << endl;
    cout << "Prints CPU related information\n" << endl;
    cout << "-v \t Prints the distro's version" << endl;
    cout << "-n \t Prints the distro's name" << endl;
    cout << "-c \t Prints the distro's codename" << endl;
}


/**
 * \brief Runs the internal command that displays memory related info
 * @param cmds string vector of user input
 * @return 0
 */
int Shell::info(vector<string> cmds){

    if ((cmds.size() == 1 || cmds[1] == "-h" || cmds[1] == "--help")) {
        info_help();
    }
    else if (cmds[1].compare("-s") == 0){
        long pagesize = sysconf(_SC_PAGE_SIZE);
        cout << "Pagesize: " << pagesize / 1000 << "KB" << endl;
    }
    else if (cmds[1] == "-n"){
        long pages = sysconf(_SC_AVPHYS_PAGES);
        cout << pages << endl;
    }
    else if (cmds[1] == "-p"){
        int numCPU = sysconf(_SC_NPROCESSORS_ONLN);
        cout << "Processors: " << numCPU << endl;
    }
    else{
        cout << "Unknown switch..." << endl;
    }

    return 0;
}

/**
 * \brief Prints out a help message for the info command
 */
void Shell::info_help() {
    cout << "Usage: info [OPTION]" << endl;
    cout << "Prints memory related information\n" << endl;
    cout << "-s \t Prints the current memory page size in KB" << endl;
    cout << "-n \t Prints the currently available page numbers" << endl;
    cout << "-p \t Prints the number of available processors" << endl;
}

/**
 * \brief Exits the shell.  Can take in an integer for an exit code
 * @param cmds string vector of user input
 */
void Shell::quit(vector<string> cmds){
    int ec;

    if (cmds.size() == 1) {
        exit(0);
    }

    try {
        ec = stoi(cmds[1]);
        exit(ec);
    }catch(invalid_argument const&){
        exit(0);
    }

}

/**
 * \brief Takes in user input and attempts to run an internal command.
 * @param cmds - string vector of user input
 * @return 0 if and internal command was entered, -1 if the command was not recognized
 */
int Shell::internalCommands(vector<string> cmds){
    int i;

    int select = -1;
    for (i = 0; i < numCmds; i++){

        if (cmds[0].compare(commands[i]) == 0) {
            select = i;
            break;
        }
    }

    if (select >= 0 && cmds.size() > 2){
        cout << "Unknown parameter... Enter -h or --help after a command for information." << endl;
        return 0;
    }


    switch(select){
        case -1:
            cmds.clear();
            return -1;
        case 0:
            quit(cmds);
        case 1: change_prompt(cmds);
            break;
        case 2: distro(cmds);
            break;
        case 3: info(cmds);
            break;
        case 4: manual();
    }

    cmds.clear();
    return 0;
}

/**
 *
 */
void Shell::manual(){
    cout << "Welcome to the cwushell Manual!\n" << endl;
    cout << "This shell will read in commands, switches, and parameters the way a normal terminal will. " << endl;
    cout << "The shell accepts the standard Linux commands as well as several built in commands: \n" << endl;
    cout << "change_prompt [new_prompt_name]: changes the current shell prompt to the new_prompt_name. " << endl;
    cout << "\tEntering change_prompt with no parameter will reset to the default shell prompt." << endl;
    cout << "distro [OPTION]: Displays CPU information \n -v: version\n -n: name\n -c: codename " << endl;
    cout << "info [OPTION]: Displays memory information \n -s: current memory page size in KB" << endl;
    cout << " -n: currently available page numbers\n -p: number of available processors" << endl;
    cout << "quit [n]: terminates the shell.  User can enter a numerical argument n as the exit value\n" << endl;
}