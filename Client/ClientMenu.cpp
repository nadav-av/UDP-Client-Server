#define _CRT_SECURE_NO_WARNINGS
#include "ClientMenu.h"
#include <iostream>
using namespace std;

void printMenu()
{
    cout << "=============== Requests Options ===============" << endl;
    cout << "1. Get time" << endl;
    cout << "2. Get time without date" << endl;
    cout << "3. Get seconds since epoch" << endl;
    cout << "4. Get client-server delay" << endl;
    cout << "5. Measure RTT" << endl;
    cout << "6. Get time without date and seconds" << endl;
    cout << "7. Get year" << endl;
    cout << "8. Get month and day" << endl;
    cout << "9. Get seconds since start of month" << endl;
    cout << "10. Get week number of year (1-52)" << endl;
    cout << "11. Get time type (standart or daylight saving)" << endl;
    cout << "12. Get time in city" << endl;
    cout << "13. Start / Stop timer" << endl;
    cout << "0. Exit" << endl;
    cout << "=================================================" << endl << endl;
}

void fillBuffer(int choseRequest, char* buffer)
{
    char city[100];
    switch (choseRequest)
    {
    case 1:
        strcpy(buffer, "1");
        break;
    case 2:
        strcpy(buffer, "2");
        break;
    case 3:
        strcpy(buffer, "3");
        break;
    case 4:
        strcpy(buffer, "4");
        break;
    case 5:
        strcpy(buffer, "5");
        break;
    case 6:
        strcpy(buffer, "6");
        break;
    case 7:
        strcpy(buffer, "7");
        break;
    case 8:
        strcpy(buffer, "8");
        break;
    case 9:
        strcpy(buffer, "9");
        break;
    case 10:
        strcpy(buffer, "10");
        break;
    case 11:
        strcpy(buffer, "11");
        break;
    case 12:
        strcpy(buffer, "12#");
        cout << "Enter city: ";
        cin >> city;
        strcat(buffer, city);
        break;
    case 13:
        strcpy(buffer, "13");
    default:
        break;
    }
}