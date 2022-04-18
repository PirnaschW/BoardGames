#include "Core.h"

#ifdef LOG

#define VC_EXTRALEAN        // Exclude rarely-used stuff from Windows headers
#include "targetver.h"
#include <windows.h>
#include <winuser.h>
#include <debugapi.h>

namespace BoardGamesCore
{
  void PositionValue::Log() const
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

  void Move::Log() const
  {
    GetActions().Log();
    value_.Log();
  }
  
  void Actions::Log() const
  {
    const auto s = size();
    wchar_t buffer[256]{};
    assert(s < sizeof(buffer) / 3);
    size_t pBuf = 0U;
    bool jump = false;

    for (size_t i = 0; i < size(); ++i)
    {
      if ((*this)[i]->IsJump())
      {
        jump = true;
        i += 2;
      }
      else
      {
        const auto x = (*this)[i]->GetLocation().x_;
        const auto y = (*this)[i]->GetLocation().y_;
        pBuf += wsprintfW(buffer + pBuf, L"%c%c%s-", x + 'a', y + '1', jump ? "x" : "");
        jump = false;
      }
    }
    if (pBuf) buffer[--pBuf] = '\0';
    OutputDebugString(buffer);
  }

}

#endif // LOG
