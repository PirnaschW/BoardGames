#include "Core.h"

#ifdef LOG

#define VC_EXTRALEAN        // Exclude rarely-used stuff from Windows headers
#include "targetver.h"
#include <windows.h>
#include <winuser.h>
#include <debugapi.h>

namespace BoardGamesCore
{
  void Move::Log(void)
  {
    const auto& aa = GetActions();
    assert(aa.size() > 0);
    const auto s = aa.size();
    wchar_t buffer[256];
    assert(s < sizeof(buffer)/3);
    size_t pBuf = 0U;
    bool jump = false;

    for (int i = 0; i < aa.size(); ++i)
    {
      if (aa[i]->IsJump())
      {
        jump = true;
        i += 2;
      }
      else
      {
        const auto x = aa[i]->GetLocation().x_;
        const auto y = aa[i]->GetLocation().y_;
        pBuf += wsprintfW(buffer + pBuf, L"%c%c%s-", x + 'a', y + '1', jump ? "x" : "");
        jump = false;
      }
    }
    --pBuf;
    wsprintfW(buffer + pBuf, L"\n");
    OutputDebugString(buffer);

  }

}

#endif // LOG
