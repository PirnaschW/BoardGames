#include "Util.h"

#define VC_EXTRALEAN        // Exclude rarely-used stuff from Windows headers
#include "targetver.h"
#include <afxwin.h>         // MFC core and standard components
#include <wininet.h>
#pragma comment(lib, "Wininet")

namespace URL
{
  const std::string GetHTMLFromURL(const std::string& url) { return GetHTMLFromURL(std::wstring(url.begin(), url.end())); }

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

namespace Example
{
  class XX {
  public:
    // ...
    virtual ~XX() = default;             // virtual destructor as class is a base class
    XX(const XX&) = default;             // copy constructor
    XX& operator=(const XX&) = default;  // copy assignment
    XX(XX&&) = default;                  // move constructor
    XX& operator=(XX&&) = default;       // move assignment
  };
}
