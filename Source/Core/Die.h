
namespace BoardGamesCore
{

  using Side = unsigned char;
  class Die final // a single Die - for display mostly
  {
  private:
    constexpr Die(Side s, BoardGamesMFC::BitmapID IDL, BoardGamesMFC::BitmapID IDS) noexcept : side_{ s }, bmpL_{ IDL }, bmpS_{ IDS } { }
    constexpr Die() noexcept = delete;
    constexpr Die(const Die&) noexcept = delete;            // delete copy constructor
    constexpr Die(Die&&) noexcept = delete;                 // delete move constructor
    constexpr Die& operator=(const Die&) = delete;          // delete copy assignment operator
    constexpr Die& operator=(Die&&) = delete;               // delete move assignment operator
  public:
    constexpr ~Die() noexcept = default;

  public:
    constexpr bool operator ==(const Die& d) const noexcept { return d.side_ == side_; }
    constexpr bool operator !=(const Die& d) const noexcept { return !(*this == d); }
    void Serialize(std::stringstream& s) const { s << side_; }
    void Draw(DC* dc, const Rect& r) const { bmpL_.Draw(dc, r); }

  public:
    static const std::array<Die, 6> Sides;

  private:
    const Side side_;
    const Bitmap bmpL_;
    const Bitmap bmpS_;
  };

  constexpr inline const std::array<Die, 6> Die::Sides{
    Die{ 1, IDB_S1L, IDB_S1S },
    Die{ 2, IDB_S2L, IDB_S2S },
    Die{ 3, IDB_S3L, IDB_S3S },
    Die{ 4, IDB_S4L, IDB_S4S },
    Die{ 5, IDB_S5L, IDB_S5S },
    Die{ 6, IDB_S6L, IDB_S6S },
  };

  static_assert(!std::is_abstract<Die>::value, "is abstract");
  static_assert(!std::is_trivially_constructible<Die>::value, "must not be trivially constructible");
  static_assert(!std::is_assignable<Die, Die>::value, "is assignable");
  static_assert(Die::Sides[1] != Die::Sides[2], "Die singletons are not constexpr");

}
