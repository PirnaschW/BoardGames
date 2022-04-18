// not dependent on anything

namespace BoardGamesCore
{

  class TileColor final
  {
  private:
    constexpr TileColor(const char& c, BoardGamesMFC::BitmapID id) noexcept : tilecolor_{ c }, bitmap_{ id } {}
    constexpr ~TileColor() noexcept = default;
  public:
    constexpr bool operator ==(const TileColor& t) const noexcept { return t.tilecolor_ == tilecolor_; }
    constexpr bool operator !=(const TileColor& t) const noexcept { return !(*this == t); }
    void Draw(DC* dc, const Rect& r) const { bitmap_.Draw(dc, r); }
    void Serialize(std::stringstream& s) const { s << tilecolor_; };

  private:
    const char tilecolor_;
    const BoardGamesMFC::Bitmap bitmap_;

  public:  // the only instances ever to exists; handed around by pointer
    static const TileColor Light;
    static const TileColor Dark;
    static const TileColor Small;
  };

  // can't be constexpr because of Bitmap usage
  constexpr inline const TileColor TileColor::Light{ 'L', IDB_XXL };  // normal size light tile
  constexpr inline const TileColor TileColor::Dark { 'D', IDB_XXD };  // normal size dark  tile
  constexpr inline const TileColor TileColor::Small{ 'l', IDB_XXS };  // small  size light tile


  static_assert(!std::is_abstract<TileColor>::value, "must not be abstract");
  static_assert(!std::is_trivially_constructible<class TileColor>::value, "must not be trivially constructible");
  static_assert(!std::is_constructible<class TileColor, char&>::value, "must not be 'char&' constructible");
  static_assert(TileColor::Light != TileColor::Small, "TileColor singletons are not constexpr");

}
