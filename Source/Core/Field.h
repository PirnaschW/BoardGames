
namespace BoardGamesCore
{

  class Field final // combines a Location and a Piece on it
  {
  public:
    constexpr Field(const Location& l, const Piece& p) noexcept : l_{ l }, p_{ p } {}
    constexpr bool operator==(const Field& f) const noexcept { return l_ == f.l_ && p_ == f.p_; }
    constexpr bool operator!=(const Field& f) const noexcept { return !(f == *this); }

    constexpr const Location& GetLocation(void) const noexcept { return l_; }
    constexpr const Piece& GetPiece(void) const noexcept { return p_; }

  private:
    Location l_;      // can't be const, or assignments between Fields wouldn't work.
    const Piece& p_;
  };
  using Fields = std::vector<Field>;

  static_assert(!std::is_abstract<Field>::value, "must not be abstract");
  static_assert(!std::is_trivially_constructible<class Field>::value, "must not be trivially constructible");
  static_assert(std::is_constructible<class Field, Location, Piece&>::value, "is not constructible");
  static_assert(std::is_nothrow_constructible<class Field, Location, Piece&>::value, "is not nothrow constructible");

}