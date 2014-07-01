#include "Clock.h"

Clock::Clock() {
	seconds=10;
}

Clock::Clock(int sec){
	seconds=sec;
}

void Clock::process1(int *fildes){
	const int BUFSIZE =100;
    char buf[BUFSIZE];
    ssize_t nbytes;
    close(fildes[1]);          /* Close unused write end */
    struct stat stats;
    int r;
    off_t bytesAvail = -1;
	while(true){
		time_t now = time(0);
		tm *ltm = localtime(&now);
		cout << "\x1b[35m The time is now: \e[m" << ltm->tm_hour << "h:" << ltm->tm_min << "m:" << ltm->tm_sec << "s\n";
        sleep(1);
        r = fstat(fildes[0], &stats);
        if (r < 0) perror("process1: fstat failed");
        else {
            bytesAvail = stats.st_size;
            if(bytesAvail > 0){
                nbytes = read(fildes[0], buf, 1);
                break;
            }
        }
    }
    write(STDOUT_FILENO, "\n", 1);
    close(fildes[0]);
    _exit(EXIT_SUCCESS);
}


void Clock::process2(int *fildes){
	cout << "process2: called proc1 \n";
	int sleepT = 1;
	close(fildes[1]);          /* Close unused write end */
	pid_t pid;
	for(int x = 0; true; x++){
		if(x % 5 ==0){
		pid = fork();
		}
		if (pid == 0) { // child process
			char *argv = "/opt/local/bin/uptime";
			char *newargv[] = {NULL};
			char *newenviron[] = { NULL };
			execve(argv, newargv, newenviron);
			//_exit(EXIT_SUCCESS);
		}
		else 
			if(pid>0){
				//cout << "process2: proc1 sleeping \n";
				sleep(sleepT);
				const int BUFSIZE =100;
				char buf[BUFSIZE];
				ssize_t nbytes;
				int r;
				struct stat stats;
				off_t bytesAvail = -1;
				r = fstat(fildes[0], &stats);
				if (r < 0) perror("process2: fstat failed");
				else {
					bytesAvail = stats.st_size;
					if(bytesAvail > 0){
						nbytes = read(fildes[0], buf, 1);
						break;
					}
				}
			}
			else
				cout << "process2: error";
	}
}


void Clock::process3(int *fildes){
		int minutes;
		minutes = seconds / 60;
		seconds = seconds - (60 * minutes);
		while(true){
			cout << "\x1b[35m Time Remaining: \e[m" <<  minutes << ":" << seconds <<endl;
			if(seconds == 0 && minutes > 0){
				seconds=59;
				minutes--;
			}
			if(seconds == 0 && minutes == 0)
				break;
			sleep(1);
			seconds--;
		}
		cout << endl;
        close(fildes[0]);          /* Close unused read end */ 
        write(fildes[1], "Hello world\n\0", 13);
        close(fildes[1]);          /* Reader will see EOF */
}


Clock::~Clock() {
}

