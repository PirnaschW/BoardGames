namespace BoardGamesCore
{

  class Piece;  // forward declaration needed in class Field
  class Field final // combines a Location and a Piece on it
  {
  public:
    constexpr inline Field(const Location l, const Piece* p) noexcept : _l{ l }, _p{ p } {}
    constexpr inline bool operator==(const Field& f) const noexcept { return _l == f._l && _p == f._p; }
    constexpr inline bool operator!=(const Field& f) const noexcept { return !(f == *this); }

    constexpr inline const Location GetLocation(void) const noexcept { return _l; }
    constexpr inline const Piece* GetPiece(void) const noexcept { return _p; }

  private:
    Location _l;      // can't be const, or assignments between Fields wouldn't work.
    const Piece* _p;
  };
  using Fields = std::vector<Field>;

  static_assert(!std::is_abstract<Field>::value, "must not be abstract");
  static_assert(!std::is_trivially_constructible<class Field>::value, "must not be trivially constructible");
  static_assert(std::is_constructible<class Field, Location, Piece*>::value, "is not constructible");
  static_assert(std::is_nothrow_constructible<class Field, Location, Piece*>::value, "is not nothrow constructible");

}