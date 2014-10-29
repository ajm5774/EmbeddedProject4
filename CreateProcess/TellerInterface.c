#include <time.h>
#include "TellerInterface.h"

struct timespec start, current;
int DayStart = 9; //day starts at 9 am

int getSimTime()
{
	return getSimTimeMs() / 1000;
}

int getSimTimeMs()
{
    if( clock_gettime( CLOCK_REALTIME, &current) == -1 ) {
      perror( "clock gettime" );
      return -1;
    }
    printf("start: %d, current: %d ", start.tv_nsec, current.tv_nsec);

    return (current.tv_nsec - start.tv_nsec)/1000;
}

int setStartTime()
{
	if( clock_gettime( CLOCK_REALTIME, &start) == -1 ) {
	  perror( "clock gettime" );
	  return -1;
	}
	return start.tv_nsec;
}

int getHour(int timeMs) {
	return ((timeMs /60 / 1000) + DayStart) % 12;
}

int getMinute(int timeMs) {
	return timeMs/1000 % 60;
}

