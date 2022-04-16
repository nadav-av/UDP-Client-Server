#ifndef UNIQUE
#define UNIQUE
    #include <Windows.h>
    #include <array>
    using namespace std;

    typedef struct _CityTZ
    {
        char city[100];
        int UTC;
    } CityTZ;

    extern CityTZ sf;
    extern CityTZ porto;
    extern CityTZ Tokyo;
    extern CityTZ Melbourne;

    extern array<CityTZ, 4> cities;
     

    void MapRequest(char* request, char* response, DWORD time);
    void GetTime(char* response);
    void GetTimeWithoutDate(char* response);
    void GetTimeSinceEpoch(char* response);
    void GetClientToServerDelayEstimation(char* response, DWORD time);
    void GetTimeWithoutDateOrSeconds(char* response);
    void GetYear(char* response);
    void GeyMonthAndDay(char* response);
    void GetSecondsSinceBeginingOfMonth(char* response);
    void GetWeekOfYear(char* response);
    void GetDaylightSavings(char* response);
    void GetTimeWhthoutDateInCity(char* response, CityTZ*);
    void MeasureTimeLap(char* response);
    bool getTimeInCityReqValidation(char* request);
    CityTZ* extractCityfromCitiesArray(char* city);
    CityTZ* extractCity(char* request);
    void restartActiveTimerIfNeeded();


#endif UNIQUE