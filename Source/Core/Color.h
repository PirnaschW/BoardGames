
namespace BoardGamesCore
{
  using BoardGamesMFC::Bitmap;
  using BoardGamesMFC::Archive;
  using BoardGamesMFC::DC;
  using BoardGamesMFC::Rect;

  class PieceColor final
  {
  private:
    constexpr PieceColor(const char& c) noexcept : color_{ c } {}
  public:
    constexpr bool operator ==(const PieceColor& c) const noexcept { return c.color_ == color_; }
    constexpr bool operator !=(const PieceColor& c) const noexcept { return !(*this == c); }
    constexpr bool operator ==(const char c) const noexcept { return c == color_; }
    size_t GetHash(void) const noexcept { return std::hash<char>()(color_); }
    void Serialize(Archive& ar) const;

  private:
    const char color_;

  public:  // the only instances ever to exists; handed around by pointer
    static const PieceColor Void;
    static const PieceColor NoColor;
    static const PieceColor White;
    static const PieceColor Black;
  };

  static_assert(!std::is_abstract<PieceColor>::value, "must not be abstract");
  static_assert(!std::is_trivially_constructible<class PieceColor>::value, "must not be trivially constructible");
  static_assert(!std::is_constructible<class PieceColor, char&>::value, "must not be 'char&' constructible");


  class TileColor
  {
  protected:
    TileColor(const char& f, unsigned int ID) noexcept : tilecolor_{ f }, cb_{ ID } {}
    virtual ~TileColor(void) noexcept {};
  public:
    constexpr bool operator ==(const TileColor& t) const noexcept { return t.tilecolor_ == tilecolor_; }
    constexpr bool operator !=(const TileColor& t) const noexcept { return !(*this == t); }
    virtual void Draw(DC* pDC, const Rect& r) const;
    void Serialize(Archive& ar) const;

  private:
    const char tilecolor_;

  private:
    const Bitmap cb_;

  public:  // the only instances ever to exists; handed around by pointer
    static const TileColor Light;
    static const TileColor Dark;
    static const TileColor Small;
  };

  static_assert(!std::is_abstract<TileColor>::value, "must not be abstract");
  static_assert(!std::is_trivially_constructible<class TileColor>::value, "must not be trivially constructible");
  static_assert(!std::is_constructible<class TileColor, char&>::value, "must not be 'char&' constructible");

}