namespace BoardGamesCore
{

  class Color final
  {
  private:
    constexpr inline Color(const char& c) noexcept : color_{ c } {}
  public:
    constexpr inline bool operator ==(const Color& c) const noexcept { return c.color_ == color_; }
    inline size_t GetHash(void) const noexcept { return std::hash<char>()(color_); }
    constexpr inline const Color* operator !(void) const noexcept { return color_ == 'W' ? &Black : &White; }
    void Serialize(CArchive* ar) const;

  private:
    const char color_;

  public:  // the only instances ever to exists; handed around by pointer
    static const Color Void;
    static const Color NoColor;
    static const Color White;
    static const Color Black;
  };

  static_assert(!std::is_abstract<Color>::value, "must not be abstract");
  static_assert(!std::is_trivially_constructible<class Color>::value, "must not be trivially constructible");
  static_assert(!std::is_constructible<class Color, char&>::value, "must not be 'char&' constructible");


  class TileColor final
  {
  private:
    constexpr inline TileColor(const char& f) noexcept : tilecolor_{ f } {}
  public:
    void Serialize(CArchive* ar) const;

  private:
    const char tilecolor_;

  public:  // the only instances ever to exists; handed around by pointer
    static const TileColor Light;
    static const TileColor Dark;
    static const TileColor Small;
  };

  static_assert(!std::is_abstract<TileColor>::value, "must not be abstract");
  static_assert(!std::is_trivially_constructible<class TileColor>::value, "must not be trivially constructible");
  static_assert(!std::is_constructible<class TileColor, char&>::value, "must not be 'char&' constructible");

}