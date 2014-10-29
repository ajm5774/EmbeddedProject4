#include "TellerInterface.h"

// The time of the day in MSec
static int timeOfDay = 0;

int getTimeMS() {
	return timeOfDay;
}

int getTime() {
	return timeOfDay/SIMULATION_MINUTE_MSEC;
}

void setTimeMS(int msec) {
	timeOfDay = msec;
}

void setTime(int sec) {
	timeOfDay = sec * SIMULATION_MINUTE_MSEC;
}

int getHour(int time) {
	int hour = (START_HOUR + timeOfDay/SIMULATION_MINUTE_MSEC/60) % 13;
	return (hour < 9) ? hour+1 : hour;
}

int getMinute(int time) {
	return time/SIMULATION_MINUTE_MSEC % 60;
}

