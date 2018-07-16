#include "stdafx.h"

#include<wininet.h>

namespace Util
{

  const std::string URL::GetHTMLFromURL(const std::wstring& url)
  {
    constexpr const static int len{ 2 << 15 };

    HINTERNET connect = InternetOpen(L"Browser", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
    assert(connect);

    HINTERNET OpenAddress = InternetOpenUrl(connect, url.c_str(), NULL, 0, INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_KEEP_CONNECTION, 0);
    assert(OpenAddress);

    char DataReceived[len];
    DWORD NumberOfBytesRead = 0;
    InternetReadFile(OpenAddress, DataReceived, len, &NumberOfBytesRead);
    assert(NumberOfBytesRead > 0 && NumberOfBytesRead < len);

    InternetCloseHandle(OpenAddress);
    InternetCloseHandle(connect);
    return DataReceived;
  }

}
