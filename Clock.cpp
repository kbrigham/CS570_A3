
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
    cout << "process1: \x1b[33m about to read \e[m" << endl;
    ///fcntl(mypipe[0], F_SETFL, O_NONBLOCK)
    struct stat stats;
    int r;
    off_t bytesAvail = -1;
    //for(int i =0; i<2; i++){
	while(true){
		time_t now = time(0);
		tm *ltm = localtime(&now);
		cout << "process1: The time is now: " << ltm->tm_hour << "h:" << ltm->tm_min << "m:" << ltm->tm_sec << "s\n";
        r = fstat(fildes[0], &stats);
        if (r < 0) perror("process1: fstat failed");
        else {
            ///if (S_ISCHR(stats.st_mode)) cout << "S_ISCHR\n";
            ///else if (S_ISFIFO(stats.st_mode)) cout << "S_ISFIFO\n";
            ///else if (S_ISREG(stats.st_mode)) cout << "S_ISREG\n";
            ///else cout << "unknown stat mode\n";
            bytesAvail = stats.st_size;
            //cout << "process1:\x1b[36m bytes available= \e[m" << (int)bytesAvail<<endl;
            if(bytesAvail > 0){
                nbytes = read(fildes[0], buf, 1);
				//nbytes = read(fildes[0], buf, 1);
                cout << "process1:\x1b[35m reading data, \e[m" << buf[0] <<endl;
                break;
            }
            sleep(1);
        }
    }
	cout << "process1:\x1b[35m done reading \e[m" << endl;
    write(STDOUT_FILENO, "\n", 1);
    close(fildes[0]);
    cout << "process1:\x1b[32m child pipe closed \e[m" << endl;
    _exit(EXIT_SUCCESS);
}


void Clock::process2(int *fildes){
	cout << "process2: called proc1\n";
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
				cout << "process2: proc1 sleeping \n";
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
					///if (S_ISCHR(stats.st_mode)) cout << "S_ISCHR\n";
					///else if (S_ISFIFO(stats.st_mode)) cout << "S_ISFIFO\n";
					///else if (S_ISREG(stats.st_mode)) cout << "S_ISREG\n";
					///else cout << "unknown stat mode\n";
					bytesAvail = stats.st_size;
					//cout << "process2:\x1b[36m bytes available= \e[m" << (int)bytesAvail<<endl;
					if(bytesAvail > 0){
						nbytes = read(fildes[0], buf, 1);
						//nbytes = read(fildes[0], buf, 1);
						cout << "process2:\x1b[35m reading data, \e[m" << buf[0] <<endl;
						break;
					}
				}
			}
			else
				cout << "process2: errorrr";
	}
}


void Clock::process3(int *fildes){
		int minutes;
		//seconds = 30; //for testing
		minutes = seconds / 60;
		seconds = seconds - (60 * minutes);
		while(true){
		seconds--;
			cout << "process3: "<< minutes << ":" << seconds << endl;
			if(seconds == 0 && minutes > 0){
				seconds=59;
				minutes--;
			}
			if(seconds == 0 && minutes == 0)
				break;
			sleep(1);
		}
		cout << endl;
        /* process3 writes argv[1] to pipe */
        close(fildes[0]);          /* Close unused read end */ 
        //char *test = "blah";
        cout << "process3: \x1b[32m sending \e[m"<<endl;
        write(fildes[1], "Hello world\n\0", 13);
        cout << "process3: \x1b[32m done sending \e[m" << endl;
        close(fildes[1]);          /* Reader will see EOF */
        cout << "process3: \x1b[33m write pipe closed \e[m" << endl;
}


Clock::~Clock() {
	// TODO Auto-generated destructor stub
}

