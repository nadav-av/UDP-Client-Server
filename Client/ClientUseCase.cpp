#include "ClientUseCase.h"
#include <algorithm>

using namespace std;

int sendtoWrapper(
    _In_ SOCKET s,
    _In_reads_bytes_(len) const char FAR* buf,
    _In_ int len,
    _In_ int flags,
    _In_reads_bytes_(tolen) const struct sockaddr FAR* to,
    _In_ int tolen)
{
    int bytesSent = sendto(s, buf, len, flags, to, tolen);
    if (SOCKET_ERROR == bytesSent)
    {
        cout << "Time Client: Error at sendto(): " << WSAGetLastError() << endl;
        closesocket(s);
        WSACleanup();
        return -1;
    }

    cout << "Time Client: Sent: " << len << "/" << strlen(buf) << " bytes of \"" << buf << "\" message.\n";
    return bytesSent;
}

int recvWrapper(
    _In_ SOCKET s,
    _Out_writes_bytes_to_(len, return) __out_data_source(NETWORK) char FAR* buf,
    _In_ int len,
    _In_ int flags)
{
    // Gets the server's answer using simple recieve (no need to hold the server's address).
    int bytesRecv = recv(s, buf, len, flags);
    if (SOCKET_ERROR == bytesRecv)
    {
        cout << "Time Client: Error at recv(): " << WSAGetLastError() << endl << endl;
        closesocket(s);
        WSACleanup();
        return -1;
    }

    buf[bytesRecv] = '\0'; // add the null-terminating to make it a string
    cout << "Time Client: Recieved: " << bytesRecv << " bytes of \"" << buf << "\" message.\n" << endl << endl;
    return bytesRecv;
}

double calculateAVGTimeMeasurement(long int delayTime[], int size)
{
    sort(delayTime, delayTime + size);
    long int sum = 0;
    for (int i = 1; i < size; i++)
    {
        sum += delayTime[i] - delayTime[i - 1];
    }
    double avg = (double)sum / (double)(size - 1);
    return avg;
}