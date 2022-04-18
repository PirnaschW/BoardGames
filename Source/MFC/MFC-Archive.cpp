#include "MFC-Win.h"

namespace BoardGamesMFC
{

  bool Archive::IsLoading() const { return ar_->IsLoading(); }
  void Archive::SaveByte(const std::byte b) const { *ar_ << static_cast<unsigned char>(b); }
  std::byte Archive::LoadByte() const { unsigned char c; *ar_ >> c; return static_cast<std::byte>(c); }

}
