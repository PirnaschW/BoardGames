
class CArchive;

namespace BoardGamesMFC
{

  class Archive
  {
  public:
    constexpr Archive(CArchive* ar) noexcept : ar_(ar) {}

    Archive& operator << (const char c);
    Archive& operator >> (char& c);

  private:
    CArchive* ar_;  // just store it so we can use it, we do NOT own it
  };

}
