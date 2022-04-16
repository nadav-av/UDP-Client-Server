#define _CRT_SECURE_NO_WARNINGS
#include "serverUtilities.h"
#include <time.h>
#include <string.h>
#include <iostream>
#include <windows.h>
#include <chrono>
#include <cstdlib>

using namespace std;

extern CityTZ sf = { "San Francisco", -7 };
extern CityTZ porto = { "Porto", 0 };
extern CityTZ Tokyo = { "Tokyo", 9 };
extern CityTZ Melbourne = { "Melbourne", 11 };

extern array<CityTZ, 4> cities = { sf, porto, Tokyo, Melbourne };

long int activeTimer = 0;
bool timerStartFlag = true;


void MapRequest(char* request, char* response, DWORD time)
{
	bool flag = true;
	if (strcmp(request, "1") == 0)
		GetTime(response);
	else if (strcmp(request, "2") == 0)
		GetTimeWithoutDate(response);
	else if (strcmp(request, "3") == 0)
		GetTimeSinceEpoch(response);
	else if (strcmp(request, "4") == 0)
		GetClientToServerDelayEstimation(response, time);
	else if (strcmp(request, "6") == 0)
		GetTimeWithoutDateOrSeconds(response);
	else if (strcmp(request, "7") == 0)
		GetYear(response);
	else if (strcmp(request, "8") == 0)
		GeyMonthAndDay(response);
	else if (strcmp(request, "9") == 0)
		GetSecondsSinceBeginingOfMonth(response);
	else if (strcmp(request, "10") == 0)
		GetWeekOfYear(response);
	else if (strcmp(request, "11") == 0)
		GetDaylightSavings(response);
	else if (getTimeInCityReqValidation(request))
	{
		CityTZ* city = extractCity(request);
		GetTimeWhthoutDateInCity(response, city);
	}
	else if (strcmp(request, "13") == 0)
		MeasureTimeLap(response);

	else
	{
		sprintf(response, "Invalid request");
	}

}

void GetTime(char* response) // year month day hour minute second
{
	time_t timer;
	time(&timer);
	tm* local_time = localtime(&timer);
	sprintf(response, "%02d/%02d/%02d %02d:%02d:%02d", local_time->tm_mday, local_time->tm_mon + 1, local_time->tm_year + 1900, local_time->tm_hour, local_time->tm_min, local_time->tm_sec);
}

void GetTimeWithoutDate(char* response)
{
	time_t timer;
	time(&timer);
	tm* local_time = localtime(&timer);
	sprintf(response, "%02d:%02d:%02d", local_time->tm_hour, local_time->tm_min, local_time->tm_sec);
}

void GetTimeSinceEpoch(char* response)
{
	// Answer client's request by the current time.
	// Get the current time.
	time_t timer;
	time(&timer);
	// Parse the current time to printable string.
	sprintf(response, "%ld", timer);
}

void GetClientToServerDelayEstimation(char* response, DWORD time)
{
	sprintf(response, "%ld", time);
}

void GetTimeWithoutDateOrSeconds(char* response)
{
	time_t timer;
	time(&timer);
	tm* local_time = localtime(&timer);
	sprintf(response, "%02d:%02d", local_time->tm_hour, local_time->tm_min);
}

void GetYear(char* response)
{
	// Answer client's request by the current time.
	// Get the current time.
	time_t timer;
	time(&timer);
	tm* local_time = localtime(&timer);
	sprintf(response, "%d", local_time->tm_year + 1900);
}

void GeyMonthAndDay(char* response)
{
	time_t timer;
	time(&timer);
	tm* local_time = localtime(&timer);
	sprintf(response, "%d/%d", local_time->tm_mday, local_time->tm_mon + 1);
}

void GetSecondsSinceBeginingOfMonth(char* response)
{
	time_t timer;
	time(&timer);
	tm* local_time = localtime(&timer);
	// calculates the seconds since the beginning of the month by the day multiplied by the number of seconds in a day, the hour multiplied by the number of seconds in an hour, the minute multiplied by the number of seconds in a minute and the second.
	sprintf(response, "%d", local_time->tm_mday * 24 * 60 * 60 + local_time->tm_hour * 60 * 60 + local_time->tm_min * 60 + local_time->tm_sec);
}

void GetWeekOfYear(char* response)
{
	time_t timer;
	time(&timer);
	tm* local_time = localtime(&timer);
	// yday = days since Jan 1st, div by 7 to get week number plus 1 (eliminate zero)
	sprintf(response, "%d", local_time->tm_yday / 7 + 1);
}

void GetDaylightSavings(char* response)
{
	time_t timer;
	time(&timer);
	tm* local_time = localtime(&timer);
	// tm_isdst = 1 if daylight savings time is in effect, 0 if not.
	sprintf(response, "%d", local_time->tm_isdst);
}

void GetTimeWhthoutDateInCity(char* response, CityTZ* reqCity)
{
	time_t timer;
	time(&timer);
	tm* gmt = localtime(&timer);
	gmt = gmtime(&timer);
	if (!reqCity)
		sprintf(response, "The city you entered is unavailable, %02d:%02d:%02d - GMT", gmt->tm_hour, gmt->tm_min, gmt->tm_sec);
	else
	{
		int UTCoffset = reqCity->UTC;
		sprintf(response, "%s Time: %02d:%02d:%02d - %s", reqCity->city, (gmt->tm_hour + UTCoffset)%24, gmt->tm_min, gmt->tm_sec, reqCity->city);
	}

}

void MeasureTimeLap(char* response)
{

	if (activeTimer == 0)
	{
		time_t timer;
		time(&timer);
		activeTimer = timer;
		sprintf(response, "Timer Started");
	}
	else
	{
		time_t timer;
		time(&timer);
		long int end = timer;
		long int start = activeTimer;
		long int res = end - start;
		int minutes = res / 60;
		int seconds = res % 60;
		sprintf(response, "Timer ended: %02d:%02d", minutes, seconds);
		activeTimer = 0;
	}
	
	
}


CityTZ* extractCityfromCitiesArray(char* city)
{
	for (int i = 0; i < cities.size(); i++)
	{
		if (strcmp(city, cities[i].city) == 0)
			return &cities[i];
	}

	return NULL;
}

CityTZ* extractCity(char* request)
{
	char city[100];
	int i = 0;
	int k = 0;
	bool passed = false;
	for (i = 0; i < strlen(request); i++)
	{
		if (request[i] == '#')
			passed = true;
		else if (passed)
		{
			city[k] = request[i];
			k++;
		}
	}
	city[k] = '\0';
	return extractCityfromCitiesArray(city);
}

bool getTimeInCityReqValidation(char* request)
{
	return request[0] == '1' && request[1] == '2' && request[2] == '#';
}

void restartActiveTimerIfNeeded()
{
	time_t timer;
	time(&timer);
	long int end = timer;
	long int start = activeTimer;
	long int res = end - start;
	if (res > 180)
	{
		activeTimer = 0;
	}
	return;
}