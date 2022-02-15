#include "Util.h"

#undef UNICODE

#define VC_EXTRALEAN        // Exclude rarely-used stuff from Windows headers
#include "targetver.h"
#include <windows.h>
#include <wininet.h>
#pragma comment(lib, "Wininet")

namespace URL
{
  const std::string GetHTMLFromURL(const std::string& url)
  {
    Cookies c{};
    return GetHTMLFromURL(url, c);
  }

  const std::string GetHTMLFromURL(const std::string& url, const Cookies& cookies)
  {
    HINTERNET connect = InternetOpen("Browser", INTERNET_OPEN_TYPE_PRECONFIG, nullptr, nullptr, 0);
    assert(connect);
    InternetSetOption(connect, INTERNET_OPTION_END_BROWSER_SESSION, NULL, 0);

    for (const auto& cookie : cookies)
    {
      const std::string s = cookie.name + "=" + cookie.value;
      BOOL cookieOk = InternetSetCookie(cookie.url.c_str(), NULL, s.c_str());
      assert(cookieOk);
    }

    HINTERNET OpenAddress = InternetOpenUrl(connect, url.c_str(), nullptr, 0, INTERNET_FLAG_PRAGMA_NOCACHE, 0);
    assert(OpenAddress);

    std::string DataReceived{};

    constexpr static const DWORD len{ 2UL << 12 };
    char buffer[len];
    bool success{ false };
    DWORD NumberOfBytesRead = 0UL;
    do
    {
      success = InternetReadFile(OpenAddress, buffer, len, &NumberOfBytesRead);
      if (success && NumberOfBytesRead > 0UL)
      {
        assert(NumberOfBytesRead > 0UL && NumberOfBytesRead <= len);
        DataReceived.append(buffer, NumberOfBytesRead);
      }
      else
      {
        DWORD ErrorCode = GetLastError();
        switch (ErrorCode)
        {
          case ERROR_INSUFFICIENT_BUFFER:
            throw;
          default:
            break;
        }
      }
    } while (success && NumberOfBytesRead > 0UL);

    InternetCloseHandle(OpenAddress);
    InternetSetOption(connect, INTERNET_OPTION_END_BROWSER_SESSION, NULL, 0);
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
