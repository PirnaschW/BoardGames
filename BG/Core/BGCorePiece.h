
namespace BoardGamesCore
{

  // cannot be constexpr as it loads bitmaps at runtime
  class Piece
  {
  protected:
    inline Piece(const Kind* k, const Color* c, UINT nID_l, UINT nID_d, UINT nID_s) noexcept
      : kind{ k }, color{ c }, ID_l{ nID_l }, ID_d{ nID_d }, ID_s{ nID_s } {}
  public:
    Piece(void) noexcept = delete;
    Piece(const Piece&) noexcept = delete;            // delete copy constructor
    Piece& operator=(const Piece&) = delete;          // delete assignment operator
    virtual inline ~Piece(void) noexcept {}

    constexpr inline bool operator ==(const Piece& p) const noexcept { return p.IsKind(*kind) && p.IsColor(color); }
    constexpr inline bool IsBlank(void) const noexcept { return color == &Color::NoColor && kind == &noKind::NoKind; }
    inline size_t GetHash(void) const noexcept { return kind->GetHash() + color->GetHash(); }
    inline void CollectMoves(const MainPosition& p, const Location l, Moves& m) const { kind->CollectMoves(p, l, m); }
    constexpr inline bool IsKind(const Kind& k) const noexcept { return k == *kind; }
    constexpr inline bool IsColor(const Color* c) const noexcept { return c == color; }
    constexpr inline const Color* GetColor(void) const noexcept { return color; }

    virtual inline void Serialize(CArchive* ar) const { color->Serialize(ar); kind->Serialize(ar); }
    virtual inline unsigned int GetValue(const MainPosition& p, const Location l) const noexcept { return kind->GetValue(p, l); }
    virtual inline bool IsPromotable(void) const noexcept { return false; }                     // by default: no promotions
    virtual inline const Piece* Promote(bool /*up*/) const noexcept { return this; };           // by default: no promotions
    virtual void Draw(CDC* pDC, const CRect& r, const TileColor* f) const;

    static const std::unordered_map<std::string, const Piece*>& GetHTMLPieceMap(void) noexcept;
    static std::vector<const Piece*> ListFromHTML(std::string s, const std::unordered_map<std::string, const Piece*>&);

  public:
    static const Piece NoTile;    // nothing exists there, don't draw the tile at all
    static const Piece NoPiece;   // no piece on the tile, but still draw it

  protected:
    const Kind* kind;      // points to a static shared object ('Singleton') - we don't own it
    const Color* color;    // points to a static shared object ('Singleton') - we don't own it

  private:
    UINT ID_l;             // bitmap ID for light tile
    UINT ID_d;             // bitmap ID for dark tile
    UINT ID_s;             // bitmap ID for small tile
    mutable CBitmap cb_l{};  // mutable to allow 'lazy' fill - also, Windows doesn't allow filling before main()
    mutable CBitmap cb_d{};
    mutable CBitmap cb_s{};
  };

  static_assert(!std::is_abstract<Piece>::value, "is abstract");
  static_assert(!std::is_trivially_constructible<Piece>::value, "must not be trivially constructible");
  static_assert(!std::is_constructible<Piece, Kind*, Color*, UINT, UINT, UINT>::value, "is not constructible");
  static_assert(!std::is_assignable<Piece, Piece>::value, "is assignable");

}
