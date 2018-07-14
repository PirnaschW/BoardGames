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
