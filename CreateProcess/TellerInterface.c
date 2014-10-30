#include <time.h>
#include "TellerInterface.h"

struct timespec start, current;
int DayStart = 9; //day starts at 9 am

long getSimTimeMins()
{
	return getSimTimeMilliMins() /1000;
}

long getSimTimeMilliMins()
{
	long secDiff;
	long nanoSecDiff;
	long ret;
    if( clock_gettime( CLOCK_REALTIME, &current) == -1 ) {
      perror( "clock gettime" );
      return 0;
    }
    secDiff = current.tv_sec - start.tv_sec;
    nanosecDiff = current.tv_nsec - start.tv_nsec;
    ret = secDiff*1000 + nanosecDiff/100000;
    printf("Sim Milli-Minutes: %d, with secDiff %d\n", ret, secDiff);
	return ret;
}

long setStartTime()
{
	if( clock_gettime( CLOCK_REALTIME, &start) == -1 ) {
	  perror( "clock gettime" );
	  return (long)0;
	}
	return start.tv_nsec;
}

int getHour(int timeMs) {
	return ((timeMs /60 / 1000) + DayStart) % 12;
}

int getMinute(int timeMs) {
	return timeMs/1000 % 60;
}

