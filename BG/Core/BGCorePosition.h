
namespace BoardGamesCore
{

  class Position
  {
  private:
    constexpr inline Position(void) noexcept = delete;

  protected:
    inline Position(const PieceMapP& p, Coordinate x, Coordinate y, const Piece* init = &Piece::NoPiece) noexcept
      : pMap{ p }, sizeX(x), sizeY(y), pieces(1ULL * x * y, pMap->GetIndex(init)) {}
    inline Position(const Position& p) noexcept : pMap{ p.pMap }, sizeX(p.sizeX), sizeY(p.sizeY), pieces{ p.pieces } {}
  public:
    inline virtual ~Position(void) noexcept {}

    constexpr inline Coordinate GetSizeX(void) const noexcept { return sizeX; }  // potentially needed for positional value calculations
    constexpr inline Coordinate GetSizeY(void) const noexcept { return sizeY; }  // only needed for positional value calculations
    constexpr inline const Piece* GetPiece(const Location& l) const noexcept { return l.Valid(sizeX, sizeY) ? pMap->GetPiece(pieces[l.Index(sizeX, sizeY)]) : nullptr; }
    inline bool HasPiece(const Piece* p) const noexcept { const PieceIndex z = pMap->GetIndex(p); for (const auto& pp : pieces) if (pp == z) return true; return false; }
    inline bool operator ==(const Position* p) const noexcept { return p->sizeX == sizeX && p->sizeY == sizeY && p->pieces == pieces; }
    inline bool operator !=(const Position* p) const noexcept { return !(*this == p); }

    virtual std::size_t GetHash(void) const noexcept;
    virtual void Serialize(CArchive* ar) const { for (auto& p : pieces) pMap->GetPiece(p)->Serialize(ar); }
    virtual const Piece* SetPiece(const Location& l, const Piece* p) noexcept { hash = 0; pieces[l.Index(sizeX, sizeY)] = pMap->GetIndex(p); return p; }
    virtual void SetPosition(std::vector<const Piece*> list);

  protected:
    const PieceMapP& pMap;
    const Coordinate sizeX;
    const Coordinate sizeY;
  private:
    std::vector<PieceIndex> pieces;
    mutable std::size_t hash{};
  };


  struct Dimension final  /// PoD structure to collect a game's layout dimensions
  {
  public:
    constexpr Dimension(
      Coordinate xc, Coordinate yc,                         // tile count in x and y directions
      unsigned int le, unsigned int te,                     // starting edge in x and y directions
      unsigned int xd, unsigned int yd,                     // tile size in x and y directions
      unsigned int xs = 0, unsigned int ys = 0) noexcept :  // extra distance between tiles in x and y directions
      xCount(xc), yCount(yc),
      xDim(xd), yDim(yd),
      xSkip(xs), ySkip(ys),
      lEdge(le), tEdge(te) {}

  public:
    const Coordinate xCount;
    const Coordinate yCount;
    const unsigned int xDim;
    const unsigned int yDim;
    const unsigned int xSkip;
    const unsigned int ySkip;
    const unsigned int lEdge;
    const unsigned int tEdge;
  };
  using Dimensions = std::array<Dimension, 3>;  // Main, Stock, Taken


  class AIContext;
  using AIContextP = std::shared_ptr<AIContext>;

  //class AIContextP;   // forward declaration needed for class MainPosition
  class MainPosition : public Position
  {
  public:
    using Depth = unsigned int;

  protected:
    inline MainPosition(const MainPosition& m) noexcept : Position(m),
      sequence(m.sequence), _stock(m._stock), _taken(m._taken), onTurn(m.onTurn), depth(m.depth),
      value(m.value), movelistW(m.movelistW), movelistB(m.movelistB) {}

  public:
    inline MainPosition(const PieceMapP& p, const Dimensions& d) noexcept : Position(p, d[0].xCount, d[0].yCount), _stock(p, d[1].xCount, d[1].yCount), _taken(p, d[2].xCount, d[2].yCount) {}
    ~MainPosition(void) noexcept override {}
    virtual MainPosition* Clone(void) const noexcept = 0;
    virtual inline bool operator ==(const MainPosition& p) const noexcept { return OnTurn() == p.OnTurn() && Position::operator==(&p); }
    virtual inline std::size_t GetHash(void) const noexcept { return Position::GetHash() + _taken.GetHash() + std::hash<const Color*>()(OnTurn()); }

    inline void SetOnTurn(const Color* c) noexcept { onTurn = c; }
    inline const Color* OnTurn(void) const noexcept { return onTurn; }
    virtual void NextPlayer(void) noexcept;
    virtual void PreviousPlayer(void) noexcept;  // needed for Undo

    virtual void GetAllMoves(void) noexcept;              // generate all moves and save list
    virtual Moves CollectMoves(void) const noexcept { Moves m{}; return m; }
    bool JumpsOnly(Moves& moves) const noexcept;
    constexpr inline Moves& GetMoveList(bool w) noexcept { return w ? movelistW : movelistB; }
    virtual bool AddIfLegal(Moves&, const Location, const Location) const noexcept { return false; };
    virtual void EvaluateStatically(void) noexcept;       // calculate position value and save
    virtual PositionValue Evaluate(AIContextP& plist, bool w, PositionValue alpha, PositionValue beta, unsigned int plies);
    inline PositionValue GetValue(bool w) const noexcept { return value.Relative(w); }
    virtual inline unsigned int GetMoveCountFactor(void) const noexcept { return 20; }
    inline Depth GetDepth(void) const noexcept { return depth; }
    inline Depth SetDepth(Depth d) noexcept { return depth = d; }
    inline PositionValue SetValue(bool w, PositionValue v) noexcept { return value = v.Relative(w); }
    virtual const Piece* GetPiece(const Location& l) const noexcept;
    virtual const Piece* SetPiece(const Location& l, const Piece* p) noexcept;
    const Location GetNextTakenL(const Color* c) const noexcept;
    inline bool IsTaken(const Location& l) const noexcept { return l.b_ == BoardPart::Taken; }
    virtual inline MoveP GetBestMove(bool w) const noexcept { return (w ? movelistW[0] : movelistB[0]); }
    virtual MainPosition* GetPosition(AIContextP& plist, MoveP m = nullptr) const noexcept;     // execute move, maintain in PList
    virtual void Execute(const Move& m) noexcept;
    virtual void Undo(const Move& m) noexcept;

  public:
    Moves sequence{};
    Position _stock;
    Position _taken;

  protected:
    const Color* onTurn{ &Color::White };
    Depth depth{ 0 };
    PositionValue value{ PositionValue::Undefined };                 // position value for White
    Moves movelistW{};
    Moves movelistB{};
  };

}
