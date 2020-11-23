#include "MFC-Win.h"

namespace BoardGamesMFC
{

  Archive& Archive::operator << (const char c) { *ar_ << c; return *this; }
  Archive& Archive::operator >> (char& c)      { *ar_ >> c; return *this; }

}
