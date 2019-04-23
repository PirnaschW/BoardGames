
namespace BoardGamesCore
{

  class Offset final
  {
    friend class Location;
  public:
    constexpr Offset(int dx, int dy) noexcept : _dx{ dx }, _dy{ dy } {}
    constexpr inline bool operator==(const Offset& o) const noexcept { return o._dx == _dx && o._dy == _dy; }
    constexpr inline const Offset operator*(int i) const noexcept { return Offset(_dx * i, _dy * i); }

  private:
    const int _dx;
    const int _dy;

  public:
    static const Offset Rdirection[4];   // standard 4 'Rook'   directions
    static const Offset Bdirection[4];   // standard 4 'Bishop' directions
    static const Offset Qdirection[8];   // standard 8 'Queen'  directions
  };
  static_assert(!std::is_abstract<Offset>::value, "must not be abstract");
  static_assert(!std::is_trivially_constructible<class Offset>::value, "must not be trivially constructible");
  static_assert(std::is_constructible<class Offset, int, int>::value, "is not constructible");
  static_assert(std::is_nothrow_constructible<class Offset, int, int>::value, "is not nothrow constructible");

}
