#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
using namespace std;
#pragma comment(lib, "Ws2_32.lib")
#include <winsock2.h>
#include <string.h>
#include <time.h>
#include <chrono>
#include <ctime>
#include "ClientMenu.h"
#include "ClientUseCase.h"
#define TIME_PORT 27015

using std::cout; using std::endl;
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::chrono::system_clock;

void main()
{

    // Initialize Winsock (Windows Sockets).
    WSAData wsaData;
    if (NO_ERROR != WSAStartup(MAKEWORD(2, 2), &wsaData))
    {
        cout << "Time Client: Error at WSAStartup()\n";
        return;
    }

    // Client side:
    // Create a socket and connect to an internet address.

    SOCKET connSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (INVALID_SOCKET == connSocket)
    {
        cout << "Time Client: Error at socket(): " << WSAGetLastError() << endl;
        WSACleanup();
        return;
    }

    // For a client to communicate on a network, it must connect to a server.

    // Need to assemble the required data for connection in sockaddr structure.

    // Create a sockaddr_in object called server.
    sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(TIME_PORT);

    // Send and receive data.

    int bytesSent = 0;
    int bytesRecv = 0;
    char sendBuff[255];
    char recvBuff[255];
    int chosenReq = -1;
    long int delayTime[100];

    while (chosenReq != 0)
    {
        printMenu();
        cin >> chosenReq;
        fillBuffer(chosenReq, sendBuff);
        if (chosenReq == 4)
        {
            for (int i = 0; i < 100; i++)
            {
                bytesSent = sendtoWrapper(connSocket, sendBuff, (int)strlen(sendBuff), 0, (const sockaddr*)&server, sizeof(server));
            }
            for (int i = 0; i < 100; i++)
            {
                bytesRecv = recvWrapper(connSocket, recvBuff, 255, 0);
                delayTime[i] = atol(recvBuff);
            }
            double avgDelay = calculateAVGTimeMeasurement(delayTime, 100);
            cout << "Average delay of server: " << avgDelay << "miliseconds" << endl << endl;
        }
        if (chosenReq == 5)
        {
           double sum=0;
            chosenReq = 1;
            fillBuffer(chosenReq, sendBuff);
            for (int i = 0; i < 100; i++)
            {
                auto startMS = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
                bytesSent = sendtoWrapper(connSocket, sendBuff, (int)strlen(sendBuff), 0, (const sockaddr*)&server, sizeof(server));
                bytesRecv = recvWrapper(connSocket, recvBuff, 255, 0);
                auto endMS = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
                sum += endMS - startMS;
            }
            sum /= 100; //AVG
            cout << "RTT of server: " << sum << "miliseconds" << endl << endl;
           
        }
        else
        {
            bytesSent = sendtoWrapper(connSocket, sendBuff, (int)strlen(sendBuff), 0, (const sockaddr*)&server, sizeof(server));
            bytesRecv = recvWrapper(connSocket, recvBuff, 255, 0);
        }
    }

    // Closing connections and Winsock.
    cout << "Time Client: Closing Connection.\n";
    closesocket(connSocket);
    system("pause");
}

