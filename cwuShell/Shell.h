/**
 * Author: Holly Strauch
 * 1/20/2020
 * CS470
 * Lab 1
 * File: Shell.h
 *
 * Program: Runs a custom shell for Linux systems.  Shells accepts 4 customized commands, along with the standard Linux
 *  Commands
 *
 *
 * “I pledge that I have neither given nor received help from anyone other than the
 * instructor/TA for all program components included here!”
 */


#ifndef CWUSHELL_SHELL_H
#define CWUSHELL_SHELL_H

#include <string.h>
#include <vector>
using namespace std;

class Shell {
private:
    string D_PROPMT;
    string prompt;
    string* commands;
    int numCmds;

    void setCommands();
    vector<string> parseInput(const string& cmds);
    void executeCmds(string cmd);
    int internalCommands(vector<string> cmds);
    int change_prompt(vector<string> cmds);
    void prompt_help();
    int distro(vector<string> cmds);
    void distro_help();
    int info(vector<string> cmds);
    void info_help();
    void quit(vector<string> cmds);
    void manual();

public:
    Shell();
    ~Shell();
    int runShell();
};


#endif //CWUSHELL_SHELL_H
