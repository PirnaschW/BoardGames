#include "stdafx.h"

#include<wininet.h>

namespace Util
{
  const std::string URL::GetHTMLFromURL(const std::wstring& url)
  {
    HINTERNET connect = InternetOpen(L"Browser", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
    assert(connect);

    HINTERNET OpenAddress = InternetOpenUrl(connect, url.c_str(), NULL, 0, INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_KEEP_CONNECTION, 0);
    assert(OpenAddress);

    char DataReceived[2 << 16];
    DWORD NumberOfBytesRead = 0;
    InternetReadFile(OpenAddress, DataReceived, 40960, &NumberOfBytesRead);
    assert(NumberOfBytesRead > 0 && NumberOfBytesRead < 2 << 16);

    InternetCloseHandle(OpenAddress);
    InternetCloseHandle(connect);
    return DataReceived;
  }
}


using namespace std;
void func(void) {
  HINTERNET connect = InternetOpen(L"Chrome", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);

  if (!connect) {
//    cout << "Connection Failed or Syntax error";
    return;
  }

  HINTERNET OpenAddress = InternetOpenUrl(connect, LR"(http://brainking.com/en/ShowGame?g=7934562)", NULL, 0, INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_KEEP_CONNECTION, 0);

  if (!OpenAddress)
  {
    DWORD ErrorNum = GetLastError();
//    cout << "Failed to open URL \nError No: " << ErrorNum;
    InternetCloseHandle(connect);
    return;
  }

  char DataReceived[40960];
  DWORD NumberOfBytesRead = 0;
  while (InternetReadFile(OpenAddress, DataReceived, 40960, &NumberOfBytesRead) && NumberOfBytesRead)
  {
//    cout << DataReceived;
  }

  InternetCloseHandle(OpenAddress);
  InternetCloseHandle(connect);

//  cin.get();
  return;
}


//
//
//
//
//#include <winsock2.h>
//#include <windows.h>
//#include <iostream>
//
////#define _WINSOCK_DEPRECATED_NO_WARNINGS
//
//
//
//
//
//#pragma comment(lib,"ws2_32.lib")
//using namespace std;
//void func() {
//  WSADATA wsaData;
//  if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
//    cout << "WSAStartup failed.\n";
//    return;
//  }
//  SOCKET Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
//  struct hostent *host;
//  getaddrinfo();
//  host = gethostbyname("www.google.com");
//  SOCKADDR_IN SockAddr;
//  SockAddr.sin_port = htons(80);
//  SockAddr.sin_family = AF_INET;
//  SockAddr.sin_addr.s_addr = *((unsigned long*)host->h_addr);
//  cout << "Connecting...\n";
//  if (connect(Socket, (SOCKADDR*)(&SockAddr), sizeof(SockAddr)) != 0) {
//    cout << "Could not connect";
//    return;
//  }
//  cout << "Connected.\n";
//  send(Socket, "GET / HTTP/1.1\r\nHost: www.google.com\r\nConnection: close\r\n\r\n", strlen("GET / HTTP/1.1\r\nHost: www.google.com\r\nConnection: close\r\n\r\n"), 0);
//  char buffer[10000];
//  int nDataLength;
//  while ((nDataLength = recv(Socket, buffer, 10000, 0)) > 0) {
//    int i = 0;
//    while (buffer[i] >= 32 || buffer[i] == '\n' || buffer[i] == '\r') {
//      cout << buffer[i];
//      i += 1;
//    }
//  }
//  closesocket(Socket);
//  WSACleanup();
//}