
namespace BoardGamesCore
{
  class Kind;
  class MainPosition;

  class Piece // an object that can be on a Location - could be checkers, a wall, or nothing
  {
  protected:
    Piece(const Kind& k, const PieceColor& c, unsigned int ID) noexcept;
  public:
    Piece(void) noexcept = delete;
    Piece(const Piece&) noexcept = delete;            // delete copy constructor
    Piece& operator=(const Piece&) = delete;          // delete assignment operator
    virtual ~Piece(void) noexcept {}

    bool operator ==(const Piece& p) const noexcept;
    bool operator !=(const Piece& p) const noexcept;
    bool IsBlank(void) const noexcept;
    size_t GetHash(void) const noexcept;
    void CollectMoves(const MainPosition& p, const Location& l, Moves& m) const;
    bool IsKind(const Kind& k) const noexcept;
    bool IsColor(const PieceColor& c) const noexcept;
    constexpr const PieceColor& GetColor(void) const noexcept { return color_; }

    virtual void Serialize(Archive& ar) const;
    virtual unsigned int GetValue(const MainPosition& p, const Location& l) const noexcept;
    virtual bool CanTake(const Piece& p) const noexcept;
    virtual bool IsPromotable(void) const noexcept { return false; }                        // by default: no promotions
    virtual const Piece& Promote(bool /*up*/) const noexcept { return *this; };             // by default: no promotions
    virtual void Draw(DC* pDC, const Rect& r) const;

    static const std::unordered_map<std::string, const Piece&>& GetHTMLPieceMap(void) noexcept;
    static std::vector<const Piece*> ListFromHTML(std::string s, const std::unordered_map<std::string, const Piece&>&);

  public:
    static const Piece NoTile;    // used to indicate 'not part of the board'
    static const Piece NoPiece;   // used to indicate 'empty field'

  protected:
    const Kind& kind_;            // static shared object ('Singleton')
    const PieceColor& color_;     // static shared object ('Singleton')

  private:
    Bitmap bmp_;
  };

  static_assert(!std::is_abstract<Piece>::value, "is abstract");
  static_assert(!std::is_trivially_constructible<Piece>::value, "must not be trivially constructible");
  static_assert(!std::is_constructible<Piece, Kind*, PieceColor*, unsigned int, unsigned int, unsigned int>::value, "is not constructible");
  static_assert(!std::is_assignable<Piece, Piece>::value, "is assignable");

  using PieceIndex = unsigned char;
  class PieceMap final  // collects all the existing pieces for the game.
                        // Intention is to be able to save inside Positions a 'PieceIndex' (= unsigned char = 1 byte)
                        // instead of a 'Piece*' (= 4 or 8 byte)
  {
  public:
    constexpr PieceMap(void) noexcept {};
    constexpr PieceMap(const PieceMap& p) noexcept : used_{ p.used_ }, map_{ p.map_ } {}
    constexpr PieceIndex GetIndex(const Piece& p) const { for (PieceIndex z = 0; z < used_; z++) if (map_[z] == &p) return z; throw; }
    constexpr void Add(const Piece& p) noexcept { map_[used_++] = &p; }
    constexpr void Empty(void) noexcept { used_ = 0; }
    constexpr PieceIndex GetCount(void) const noexcept { return used_; }
    constexpr const Piece& GetPiece(PieceIndex i) const noexcept { return *map_[i]; }

  private:
    constexpr static const PieceIndex max_{ 32 };                         // no game has more than 32 different Pieces
    PieceIndex used_{ 0 };                                                // current usage of the map
    std::array<const Piece*, max_> map_{};                                // maps index -> Piece*
  };

  using PieceMapP = std::shared_ptr<PieceMap>;

}
