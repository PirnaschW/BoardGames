#include "Core.h"

#ifdef LOG

#define VC_EXTRALEAN        // Exclude rarely-used stuff from Windows headers
#include "targetver.h"
#include <windows.h>
#include <winuser.h>
#include <debugapi.h>

namespace BoardGamesCore
{
  void PositionValue::Log(void)
  {
    wchar_t buffer[256];
    switch (type_)
    {
      case BoardGamesCore::PositionValue::PValueType::Undefined:
        wsprintfW(buffer, L"[undefined]\n");
        break;
      case BoardGamesCore::PositionValue::PValueType::Normal:
        wsprintfW(buffer, L"[%d]\n",value_);
        break;
      case BoardGamesCore::PositionValue::PValueType::Won:
        wsprintfW(buffer, L"[Won]\n");
        break;
      case BoardGamesCore::PositionValue::PValueType::Lost:
        wsprintfW(buffer, L"[Lost]\n");
        break;
      case BoardGamesCore::PositionValue::PValueType::Tie:
        wsprintfW(buffer, L"[Tie]\n");
        break;
      default:
        wsprintfW(buffer, L"[invalid!]\n");
        break;
    }
    OutputDebugString(buffer);
  }

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
    buffer[--pBuf] = '\0';
    OutputDebugString(buffer);
    value_.Log();
  }

}

#endif // LOG
