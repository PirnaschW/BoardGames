#include "MFC-Win.h"

namespace BoardGamesMFC
{
  static_assert(Mouse_LButton_Down   == WM_LBUTTONDOWN,   "Constant Mouse_LButton_Down   does not match Windows constant WM_LBUTTONDOWN  ");
  static_assert(Mouse_LButton_Up     == WM_LBUTTONUP,     "Constant Mouse_LButton_Up     does not match Windows constant WM_LBUTTONUP    ");
  static_assert(Mouse_LButton_DblClk == WM_LBUTTONDBLCLK, "Constant Mouse_LButton_DblClk does not match Windows constant WM_LBUTTONDBLCLK");
  static_assert(Mouse_RButton_Down   == WM_RBUTTONDOWN,   "Constant Mouse_RButton_Down   does not match Windows constant WM_RBUTTONDOWN  ");
  static_assert(Mouse_RButton_Up     == WM_RBUTTONUP,     "Constant Mouse_RButton_Up     does not match Windows constant WM_RBUTTONUP    ");
  static_assert(Mouse_RButton_DblClk == WM_RBUTTONDBLCLK, "Constant Mouse_RButton_DblClk does not match Windows constant WM_RBUTTONDBLCLK");
  static_assert(Mouse_Move           == WM_MOUSEMOVE,     "Constant Mouse_Move           does not match Windows constant WM_MOUSEMOVE    ");


  static_assert(No_Brush             == NULL_BRUSH,       "Constant No_Brush             does not match Windows constant NULL_BRUSH      ");
  static_assert(PenStyle_Solid       == PS_SOLID,         "Constant PenStyle_Solid       does not match Windows constant PS_SOLID        ");

}
