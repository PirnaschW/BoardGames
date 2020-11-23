#include "Util.h"
namespace Message
{

  void AfxMessageBox(std::string_view)
  {
    ::AfxMessageBox(L"Computer resigns - Player wins!");
  }

};
