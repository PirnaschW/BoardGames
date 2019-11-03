#include "pch.h"

#include<wininet.h>
#pragma comment(lib, "Wininet")

namespace URL
{

  const std::string GetHTMLFromURL(const std::wstring& url)
  {
    constexpr static const int len{ 2 << 15 };

    HINTERNET connect = InternetOpen(L"Browser", INTERNET_OPEN_TYPE_PRECONFIG, nullptr, nullptr, 0);
    assert(connect);

    HINTERNET OpenAddress = InternetOpenUrl(connect, url.c_str(), nullptr, 0, INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_KEEP_CONNECTION, 0);
    assert(OpenAddress);

    std::string DataReceived{};
    DataReceived.resize(len);
    DWORD NumberOfBytesRead = 0;
    InternetReadFile(OpenAddress, DataReceived.data(), len, &NumberOfBytesRead);
    assert(NumberOfBytesRead > 0 && NumberOfBytesRead < len);

    InternetCloseHandle(OpenAddress);
    InternetCloseHandle(connect);
    return DataReceived;
  }

}
