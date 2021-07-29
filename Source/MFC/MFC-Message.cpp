#include "MFC-Win.h"

namespace BoardGamesMFC
{

  void AfxMessageBox(std::wstring_view s)
  {
    ::AfxMessageBox((LPCTSTR) s.data());
  }

};
