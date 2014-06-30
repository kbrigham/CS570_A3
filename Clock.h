#ifndef CLOCK_H_
#define CLOCK_H_
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <ctime>
#include <iostream>
#include <time.h>

using namespace std;

class Clock {
private:
	int seconds;
public:
	Clock();
	Clock(int);

	void process1(int *fildes);
	void process2(int *fildes);
	void process3(int *fildes);

	virtual ~Clock();
};

#endif /* CLOCK_H_ */
