// not dependent on anything

namespace BoardGamesCore
{

  class PieceColor final
  {
  private:
    constexpr PieceColor(const char& c) noexcept : color_{ c } {}
    constexpr ~PieceColor() noexcept = default;
  public:
    constexpr bool operator ==(const PieceColor& c) const noexcept { return c.color_ == color_; }
    constexpr bool operator !=(const PieceColor& c) const noexcept { return !(*this == c); }
    constexpr bool operator ==(const char c) const noexcept { return c == color_; }
    constexpr PieceColor operator ~() const noexcept { return *this == White ? Black : (*this == Black ? White : Void); }
    constexpr bool IsPlayable() const noexcept { return ~(*this) != ~~(*this); }
    size_t GetHash() const noexcept { return hash_ == 0 ? hash_ = std::hash<char>()(color_) : hash_; }
    void Serialize(std::stringstream& s) const { s << color_; };

  private:
    const char color_;
    mutable size_t hash_{ 0 };

  public:  // the only instances ever to exists; handed around by pointer
    static const PieceColor Void;
    static const PieceColor NoColor;
    static const PieceColor White;
    static const PieceColor Black;
  };
  constexpr inline const PieceColor PieceColor::Void{ 'X' };
  constexpr inline const PieceColor PieceColor::NoColor{ ' ' };
  constexpr inline const PieceColor PieceColor::White{ 'W' };
  constexpr inline const PieceColor PieceColor::Black{ 'B' };

  static_assert(!std::is_abstract<PieceColor>::value, "must not be abstract");
  static_assert(!std::is_trivially_constructible<class PieceColor>::value, "must not be trivially constructible");
  static_assert(!std::is_constructible<class PieceColor, char&>::value, "must not be 'char&' constructible");
  static_assert(PieceColor::Void == PieceColor::Void, "PieceColor singletons are not constexpr");
  static_assert(PieceColor::Void != PieceColor::NoColor, "PieceColor singletons are not constexpr");

}
