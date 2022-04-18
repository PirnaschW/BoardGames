// dependent on PieceColor 
namespace BoardGamesCore
{

  class Piece // an object that can be on a Location - could be checkers, a wall, or nothing
  {
    friend class PieceMap;
  protected:
    constexpr Piece(const Kind& k, const PieceColor& c, unsigned int ID) noexcept : kind_{ k }, color_{ c }, bmp_{ ID } { };
  public:
    constexpr Piece() noexcept = delete;                    // delete constructor
    constexpr Piece(const Piece&) noexcept = delete;            // delete copy constructor
    constexpr Piece& operator=(const Piece&) = delete;          // delete copy assignment
    constexpr Piece(Piece&&) noexcept = delete;                 // delete move constructor
    constexpr Piece& operator=(Piece&&) = delete;               // delete move assignment
    constexpr virtual ~Piece() noexcept = default;          // trivial destructor

    virtual void Serialize(std::stringstream& s) const { kind_.Serialize(s); color_.Serialize(s); }
    virtual void Draw(DC* dc, const Rect& r) const { bmp_.Draw(dc, r); }

    constexpr bool operator ==(const Piece& p) const noexcept { return p.IsKind(kind_) && p.IsColor(color_); }
    constexpr bool operator !=(const Piece& p) const noexcept { return !(*this == p); }
    constexpr bool IsKind(const Kind& k) const noexcept { return k == kind_; }
    constexpr bool IsColor(const PieceColor& c) const noexcept { return c == color_; }
    constexpr bool IsColor(const Piece& p) const noexcept { return p.color_ == color_; }
    constexpr bool IsBlank() const noexcept { return IsColor(PieceColor::NoColor) && IsKind(noKind::NoKind); }

    inline size_t GetHash() const noexcept { return kind_.GetHash() + color_.GetHash(); }
    inline void CollectMoves(const Board& p, const Location& l, Moves& m) const { kind_.CollectMoves(p, l, m); }

    constexpr virtual PositionValue GetValue(const Board& p, const Location& l) const noexcept { return kind_.GetValue(p, l); }
    constexpr virtual bool CanTake(const Piece& p) const noexcept { return kind_.CanTake(p.kind_); }
    constexpr virtual bool IsPromotable() const noexcept { return false; }                        // by default: no promotions
    constexpr virtual const Piece& Promote(bool /*up*/) const noexcept { return *this; };             // by default: no promotions

  private:
    constexpr bool IsPiece(const char kind, const char color) const noexcept { return kind_ == kind && color_ == color; }

  protected:
    const Kind& kind_;            // static shared object ('Singleton')
    const PieceColor& color_;     // static shared object ('Singleton')

  private:
    const Bitmap bmp_;

  public:
    static const Piece NoTile;    // used to indicate 'not part of the board'
    static const Piece NoPiece;   // used to indicate 'empty field'
  };
  constexpr inline const Piece Piece::NoTile { noKind::NoKind, PieceColor::Void,    0 };   // nothing exists there, not even a tile
  constexpr inline const Piece Piece::NoPiece{ noKind::NoKind, PieceColor::NoColor, 0 };   // empty tile


  //static_assert(sizeof(Piece) == 32, "size of Piece has changed");

  static_assert(!std::is_abstract<Piece>::value, "is abstract");
  static_assert(!std::is_trivially_constructible<Piece>::value, "must not be trivially constructible");
  static_assert(!std::is_constructible<Piece, Kind*, PieceColor*, unsigned int, unsigned int, unsigned int>::value, "is not constructible");
  static_assert(!std::is_assignable<Piece, Piece>::value, "is assignable");
  static_assert(Piece::NoTile != Piece::NoPiece, "Pieces can't be constexpr");

}
