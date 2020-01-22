/**
 * Author: Holly Strauch
 * 1/20/2020
 * CS470
 * Lab 1
 * File: main.cpp
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
#include "Shell.h"
using namespace std;



int main() {
    int status;

    Shell* s = new Shell();
    status = s->runShell();

    return status;
}