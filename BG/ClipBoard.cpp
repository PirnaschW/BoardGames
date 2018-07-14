#include "stdafx.h"

#include <exception>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <string>
#include <windows.h>
using namespace std;

namespace ClipBoard
{

  class RaiiClipboard
  {
  public:
    RaiiClipboard()
    {
      if (!OpenClipboard(nullptr))
        throw runtime_error("Can't open clipboard.");
      // ... or define some custom exception class for clipboard errors.
    }

    ~RaiiClipboard()
    {
      CloseClipboard();
    }

    // Ban copy   
  private:
    RaiiClipboard(const RaiiClipboard&);
    RaiiClipboard& operator=(const RaiiClipboard&);
  };

  class RaiiTextGlobalLock
  {
  public:
    explicit RaiiTextGlobalLock(HANDLE hData)
      : m_hData(hData)
    {
      m_psz = static_cast<const char*>(GlobalLock(m_hData));
      if (!m_psz)
        throw runtime_error("Can't acquire lock on clipboard text.");
    }

    ~RaiiTextGlobalLock()
    {
      GlobalUnlock(m_hData);
    }

    const char* Get() const
    {
      return m_psz;
    }

  private:
    HANDLE m_hData;
    const char* m_psz;

    // Ban copy
    RaiiTextGlobalLock(const RaiiTextGlobalLock&);
    RaiiTextGlobalLock& operator=(const RaiiTextGlobalLock&);
  };

  string GetClipboardText()
  {
    RaiiClipboard clipboard;

    HANDLE hData = GetClipboardData(CF_TEXT);
    if (hData == nullptr)
      throw runtime_error("Can't get clipboard text.");

    RaiiTextGlobalLock textGlobalLock(hData);
    string text(textGlobalLock.Get());

    return text;
  }

}