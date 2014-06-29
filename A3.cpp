//================================================================
// Name        : A3.cpp
// Author      : Katie Brigham MASC1420, Shawn Vega MASC1440
// Description : Assignment 3, CS570 Summer 2014
//================================================================
#include "A3.h"
#include "A3.h"

#define purple "\x1b[35m"
#define reset "\e[m"

using namespace std;
/*
This program shall simulate
*/

int createProc(int *fildes,int numOfSec){
pid_t pid = fork();
    const int BUFSIZE =100;
    char buf[BUFSIZE];
    ssize_t nbytes;
    int counter = 0;
    
    if (pid == 0) // child process
    {
        
        int i = 0;
        for (; i < 1; ++i)
        {
            printf(purple "child process: counter=%d" reset "\n", ++counter);
            
            sleep(1);
        }
        

        close(fildes[1]);          /* Close unused write end */
        cout << "\x1b[33m about to read \e[m" << endl;
        
        //cout << fstat(fildes[0])<<endl;
        //dup2(fildes[0],1);
        //while ( == 0){
        //fcntl(mypipe[0], F_SETFL, O_NONBLOCK)
        struct stat stats;
        int r;
        off_t bytesAvail = -1;
        for(int i =0; i<2; i++){
            r = fstat(fildes[0], &stats);
            if (r < 0) perror("fstat failed");
            else {
                //if (S_ISCHR(stats.st_mode)) cout << "S_ISCHR\n";
                //else if (S_ISFIFO(stats.st_mode)) cout << "S_ISFIFO\n";
                //else if (S_ISREG(stats.st_mode)) cout << "S_ISREG\n";
                //else cout << "unknown stat mode\n";
                bytesAvail = stats.st_size;
                cout << "\x1b[36m bytes available= \e[m" << (int)bytesAvail<<endl;
                sleep(1);
            }
        }
            nbytes = read(fildes[0], buf, BUFSIZE);
            //write(STDOUT_FILENO, buf, sizeof(buf));
            cout << "\x1b[35m reading data, \e[m" << buf <<endl;
            //nbytes = read(fildes[0], buf, BUFSIZE);// == 0);//{
            //write(STDOUT_FILENO, buf, BUFSIZE);
            //cout << "reading data," << (int)nbytes<<endl;
            //}
        cout << "\x1b[35m done reading \e[m" << endl;
        write(STDOUT_FILENO, "\n", 1);
        close(fildes[0]);
        cout << "\x1b[32m child pipe closed \e[m" << endl;
        char *argv = "/opt/local/bin/uptime";
        char *newargv[] = {NULL};
        char *newenviron[] = { NULL };
        execve(argv, newargv, newenviron);
        _exit(EXIT_SUCCESS);
    }
    else if (pid > 0)
    {
        // parent process
        int j = 0;
        for (; j < 1; ++j)
        {
            printf(purple "parent process: counter=%d" reset "\n", ++counter);
            sleep(1);
        }
        /* Parent writes argv[1] to pipe */
        close(fildes[0]);          /* Close unused read end */
        //char *test = "blah";
        cout << "\x1b[32m sending \e[m"<<endl;
        write(fildes[1], "Hello world\n\0", 13);
        cout << "\x1b[32m done sending \e[m" << endl;
        close(fildes[1]);          /* Reader will see EOF */
        cout << "\x1b[33m parent pipe closed \e[m" << endl;
        wait(NULL);                /* Wait for child */
        //int returnStatus;
        //waitpid(pid, &returnStatus, 0);
        cout << "\x1b[32m closing parent \e[m" <<endl;
        exit(EXIT_SUCCESS);
    }
    else
    {
        // fork failed
        printf("fork() failed!\n");
        return 1;
    }
}

int main(int argc, char **argv){
    //const int PAGESIZE = getInput("Please enter the number of pages: ");
    int numOfSec = 10;
    int procNum = 0;
    int fildes[2];


    
    if (pipe(fildes) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    int status = createProc(fildes,numOfSec);
    if(status != 0){
        cout << "Error" << endl;
        return 1;
        }

    return 0;
}




vector<int> readFile(char *filename){
    ifstream infile(filename);
    string line;
    getline(infile, line); //iterates each line in file
    istringstream strstr(line);
    vector<int> intVec;
    string word = "";
    while (getline(strstr,word, ' ')){ // this while loop helps skip blank spaces.
        int result = atoi(word.c_str());
        intVec.push_back(result);
    }

    return intVec;
}

void opt(const vector<int> &vec, const int PAGESIZE){
    int faults = 0;
    int lastItem = 0;
    int pageMem[PAGESIZE][2];
    int arrow = 0;
    const int VECSIZE = vec.size();
    //const int VECSIZE = 20;

    
    clkZeros(pageMem, PAGESIZE);
    for(int x = 0; x < VECSIZE; x++){
        int loc;
        decrement(pageMem, PAGESIZE);
        int nextInst= -1;
        //cout << "nextInst(-1) =" << nextInst << endl;

        loc = contains(vec[x],pageMem, PAGESIZE);
        
        //////////////////////////////////////////////////
        //debugger(vec[x],pageMem, PAGESIZE);
        //cout << "looking at " << vec[x] << endl;
        //////////////////////////////////////////////////
        
        if ( loc == -1){
            faults++;
            loc = findBest(pageMem, PAGESIZE);
            pageMem[loc][0] = vec[x];
        }
        for(int s = x; s < VECSIZE - 1;){
            s++;
            if((nextInst == -1) && (vec[s] == pageMem[loc][0])){
                nextInst = s-x;
                }
        }
        pageMem[loc][1] = nextInst;
    }
    cout << "opt faults = " << faults << endl;
}

void lru(const vector<int> & vec, const int PAGESIZE){
    deque<int> pageMem (PAGESIZE,0);
    int faults = 0; 
    const int VECSIZE = vec.size();
    
    int loc;
    for(int x = 0; x < VECSIZE; x++){
        if(dequeContains(vec[x], pageMem)){
            loc = dequeFind(vec[x], pageMem);
            dequeSwap(pageMem,0,loc);
        }
        else{
            faults++;
            pageMem.pop_back();
            pageMem.push_front(vec[x]);
        }
    }
    cout << "lru faults = " << faults << endl;
}

void clk(const vector<int> &vec, const int PAGESIZE){
    int faults = 0;
    int lastItem = 0;
    int pageMem[PAGESIZE][2];
    int arrow = 0;
    const int VECSIZE = vec.size();
    
    clkZeros(pageMem, PAGESIZE);
    for(int x = 0; x < VECSIZE; x++){
        int loc;
        if ( (loc = contains(vec[x],pageMem, PAGESIZE)) != -1){
            pageMem[loc][1] = 1;
        }
        else{
            while(true){ 
                if(arrow == PAGESIZE)
                        arrow = 0;
                if(pageMem[arrow][0] == vec[x]){
                    pageMem[arrow][1] = 1;
                    arrow++;
                    break;
                }
                else{
                    if(pageMem[arrow][1] == 0){
                        faults++;
                        pageMem[arrow][0] = vec[x];
                        pageMem[arrow][1] = 1;
                        arrow++;
                        break;
                    }
                    pageMem[arrow][1] = 0;
                }
                arrow++;
            }
        }
    }
    cout << "clk faults = " << faults << endl;
}

void fifo(const vector<int> &vec, const int PAGESIZE){
    int faults = 0;
    int lastItem = 0;
    int pageMem[PAGESIZE];
    const int VECSIZE = vec.size();
    
    zeros(pageMem, PAGESIZE);
    for(int x = 0; x < VECSIZE; x++){
        if(contains(vec[x], pageMem, PAGESIZE))
            continue;
        else{
            faults++;
            pageMem[lastItem++] = vec[x];
            if(lastItem == PAGESIZE)
                lastItem = 0;
        }
    }
    cout << "fifo faults = " << faults << endl;
}

int getInput(string prompt){
     string input = "";
    int number = 0;

    while (true) {
        cout << prompt;
        getline(cin, input);
        stringstream myStream(input);
        if (myStream >> number && number < 100 && number > 0)
            break;
        cout << "Error: Invalid number" << endl;
    }
    return number;
}

/////////////////////////////////////////////////////////////////////
// HELPER METHODS BELOW
/////////////////////////////////////////////////////////////////////

int dequeFind(const int VALUE,const deque<int> & deq){
    for(int x = 0; x < deq.size(); x++)
        if(deq[x] == VALUE)
            return x;
    return -1;
}

void dequeSwap(deque<int> & deq,const int loci,const int locj){
    int temp = deq[loci];
    deq[loci] = deq[locj];
    deq[locj] = temp;
}
void debug(const int checking,const deque<int> & deq){
    cout << "Checking for" << checking << "; ";
    for(int x = 0; x < deq.size(); x++){
        cout <<deq[x] << " ";
    }
    cout << endl;
}

bool dequeContains(const int VALUE,const deque<int> deq){
    for(int x = 0; x < deq.size(); x++)
        if(deq[x] == VALUE)
            return true;
    return false;
}

bool contains(const int VALUE,const int arr[], const int ARRSIZE){
    for(int x = 0; x < ARRSIZE; x++)
        if(arr[x] == VALUE)
            return true;
    return false;
}

void zeros(int *arr, const int SIZE){
for (int x = 0; x < SIZE; x++)
    arr[x] = 0;
}

void clkZeros(int pageMem[][2], int SIZE){
    for(int x = 0; x < SIZE; x++){
    pageMem[x][0] =0;
    pageMem[x][1] =0;
    }
}

int contains(const int VALUE,const int arr[][2], const int ARRSIZE){
    for(int x = 0; x < ARRSIZE; x++)
        if(arr[x][0] == VALUE)
            return x;
    return -1;
}

void decrement(int pageMem[][2], int SIZE){
    for(int x = 0; x < SIZE; x++){
    pageMem[x][1]--;
    }
}

int findBest(int pageMem[][2], int SIZE){
    int high = 0;
    for(int x = 0; x < SIZE; x++){
    if(pageMem[x][1] < 0)
        return x;
    } 
    for(int x = 0; x < SIZE; x++){
        if(pageMem[x][1] > pageMem[high][1])
            high =x;
    }
    return high;
}

