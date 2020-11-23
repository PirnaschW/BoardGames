
namespace BoardGamesMFC
{
  template <typename T> Archive& Archive::operator << (T& t) { *ar_ << t; return *this; }
  template <typename T> Archive& Archive::operator >> (T& t) { *ar_ >> t; return *this; }
}
