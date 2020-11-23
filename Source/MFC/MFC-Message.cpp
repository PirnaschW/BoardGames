#include "MFC-Win.h"

namespace BoardGamesMFC
{

  void AfxMessageBox(std::string_view s)
  {
    ::AfxMessageBox((LPCTSTR) s.data());
  }

};
