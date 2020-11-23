namespace BoardGamesCore
{

  class PieceColor final
  {
  private:
    constexpr inline PieceColor(const char& c) noexcept : color_{ c } {}
  public:
    constexpr inline bool operator ==(const PieceColor& c) const noexcept { return c.color_ == color_; }
    constexpr inline bool operator !=(const PieceColor& c) const noexcept { return !(*this == c); }
    inline size_t GetHash(void) const noexcept { return std::hash<char>()(color_); }
    void Serialize(CArchive* ar) const;

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
    inline TileColor(const char& f, UINT ID) noexcept : tilecolor_{ f }, ID_{ ID } {}
  public:
    constexpr inline bool operator ==(const TileColor& t) const noexcept { return t.tilecolor_ == tilecolor_; }
    constexpr inline bool operator !=(const TileColor& t) const noexcept { return !(*this == t); }
    virtual void Draw(CDC* pDC, const CRect& r) const;
    void Serialize(CArchive* ar) const;

  private:
    const char tilecolor_;

  private:
    UINT ID_;               // bitmap ID for tile
    mutable CBitmap cb_{};  // mutable to allow 'lazy' fill - also, Windows doesn't allow filling before main()

  public:  // the only instances ever to exists; handed around by pointer
    static const TileColor Light;
    static const TileColor Dark;
    static const TileColor Small;
  };

  static_assert(!std::is_abstract<TileColor>::value, "must not be abstract");
  static_assert(!std::is_trivially_constructible<class TileColor>::value, "must not be trivially constructible");
  static_assert(!std::is_constructible<class TileColor, char&>::value, "must not be 'char&' constructible");

}