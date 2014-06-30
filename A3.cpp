//================================================================
// Name        : A3.cpp
// Author      : Katie Brigham MASC1420, Shawn Vega MASC1440
// Description : Assignment 3, CS570 Summer 2014
//================================================================
#include "A3.h"
#include "Clock.h"

#define purple "\x1b[35m"
#define reset "\e[m"

using namespace std;
/*
This program shall
*/

int main(int argc, char *argv[]){
    int numOfSec = 10;
    int procNum = 0;
	if(argc>1){
		numOfSec = atoi(argv[1]);
	}
	createProc(numOfSec);

    return 0;
}


void createProc(int numOfSec){
//forks everything
	Clock Clk(numOfSec);
	cout << endl << endl << endl <<endl ;

	pid_t pid = fork();
    
    int counter = 0;
    
    if (pid == 0) // child process 1 (prints time)
    {
		int fildes[2];
		if (pipe(fildes) < 0) { // if creating pipe fails
			perror("createProc: pipe error");
			exit(EXIT_FAILURE);
		}
		pid_t pid2 = fork(); //forking process
		if (pid2 > 0) { // process 1
			cout << "createProc: calling proc 1\n";
			Clk.process1(fildes);
			wait(NULL); //every parent waits for it's child.
		}
		else if (pid2 == 0) // child process proc 2 (runs uptime)
		{
			pid_t pid3 = fork(); //forking
			if (pid3 > 0) { // parent process
				Clk.process2(fildes);
				wait(NULL);
			}
			else if (pid3 == 0) // child process proc 3 (countDown)
			{
				cout << "createProc: calling proc countdown\n";
				Clk.process3(fildes);
				
			}
		}
		else if(pid2 < 0){
			cout << "error creating process: " << pid2 <<endl;
		}
	}
    else if (pid > 0){ // process 0 (parent of all) Just waits.
        int returnStatus;
        waitpid(pid, &returnStatus, 0);
        cout << "createProc: \x1b[32m closing parent \e[m" <<endl;
    }
    else{
        printf("createProc: fork() failed!\n");
    }
}