
namespace BoardGamesCore
{

  class Position abstract
  {
  private:
    class PieceMap final  // local helper class, collects all the existing pieces for the game.
                          // Intention is to be able to save inside Positions a 'PieceIndex' (= unsigned char = 1 byte)
                          // instead of a 'Piece*' (= 4 or 8 byte)
    {
    public:
      using PieceIndex = unsigned char;
    public:
      constexpr inline PieceMap(void) noexcept {}
      constexpr inline PieceMap(const PieceMap& p) noexcept : used{ p.used }, map{ p.map } {}
      constexpr inline PieceIndex GetIndex(const Piece* p) noexcept { for (PieceIndex z = 0; z < used; z++) if (map[z] == p) return z; map[used] = p; return used++; }
      constexpr inline const Piece* GetPiece(PieceIndex i) noexcept { return map[i]; }

    private:
      constexpr static const PieceIndex max{ 32 };
      PieceIndex used{ 0 };
      std::array<const Piece*, max> map{};
    };

  private:
    constexpr inline Position(void) noexcept = delete;

  public:
    inline Position(Coordinate x, Coordinate y, const Piece* init = &Piece::NoPiece) noexcept
      : sizeX(x), sizeY(y), pm{}, pieces(1ULL * x * y, pm->GetIndex(init)) {}
    inline Position(const Position& p) noexcept : sizeX(p.sizeX), sizeY(p.sizeY), pm{ p.pm }, pieces{ p.pieces } {}
    inline virtual ~Position(void) noexcept {}

    constexpr inline Coordinate GetSizeX(void) const noexcept { return sizeX; }  // potentially needed for positional value calculations
    constexpr inline Coordinate GetSizeY(void) const noexcept { return sizeY; }  // only needed for positional value calculations
    constexpr inline const Piece* GetPiece(const Location& l) const noexcept { return l.Valid(sizeX, sizeY) ? pm->GetPiece(pieces[l.Index(sizeX, sizeY)]) : nullptr; }

    virtual bool operator ==(const Position* p) const noexcept;
    virtual std::size_t GetHash(void) const noexcept;
    virtual void Serialize(CArchive* ar) const { for (auto& p : pieces) pm->GetPiece(p)->Serialize(ar); }
    virtual inline const Piece* SetPiece(const Location& l, const Piece* p) noexcept { hash = 0; pieces[l.Index(sizeX, sizeY)] = pm->GetIndex(p); return p; }
    virtual void SetPosition(std::vector<const Piece*> list);

  protected:
    const Coordinate sizeX;
    const Coordinate sizeY;
  private:
    std::shared_ptr<PieceMap> pm;
    std::vector<PieceMap::PieceIndex> pieces;
    mutable std::size_t hash{};
  };


  class AIContext;   // forward declaration needed for class MainPosition
  class MainPosition : public Position
  {
  public:
    using Depth = unsigned int;

  protected:
    inline MainPosition(const MainPosition& m) noexcept : Position(m), sequence(m.sequence), onTurn(m.onTurn) {}

  public:
    inline MainPosition(Coordinate x, Coordinate y) noexcept : Position(x, y) {}
    ~MainPosition(void) noexcept override {}
    virtual MainPosition* Clone(void) const = 0;
    virtual inline bool operator ==(const MainPosition& p) const noexcept { return OnTurn() == p.OnTurn() && Position::operator==(&p); }
    virtual inline std::size_t GetHash(void) const noexcept { return Position::GetHash() + std::hash<const Color*>()(OnTurn()); }

    inline void SetOnTurn(const Color* c) noexcept { onTurn = c; }
    inline const Color* OnTurn(void) const noexcept { return onTurn; }
    virtual void NextPlayer(void) noexcept;
    virtual void PreviousPlayer(void) noexcept;  // needed for Undo

    virtual void GetAllMoves(void);              // generate all moves and save list
    virtual Moves CollectMoves(void) const { Moves m{}; return m; }
    bool JumpsOnly(Moves& moves) const noexcept;
    inline Moves& GetMoveList(bool w) { return w ? movelistW : movelistB; }
    virtual bool AddIfLegal(Moves&, const Location, const Location) const { return false; };
    virtual void EvaluateStatically(void);       // calculate position value and save
    virtual PositionValue Evaluate(AIContext& plist, bool w, PositionValue alpha, PositionValue beta, unsigned int plies);
    inline PositionValue GetValue(bool w) const noexcept { return value.Relative(w); }
    virtual inline unsigned int GetMoveCountFactor(void) const noexcept { return 20; }
    inline Depth GetDepth(void) const noexcept { return depth; }
    inline Depth SetDepth(Depth d) noexcept { return depth = d; }
    inline PositionValue SetValue(bool w, PositionValue v) noexcept { return value = v.Relative(w); }
    virtual inline MoveP GetBestMove(bool w) const { return (w ? movelistW[0] : movelistB[0]); }
    virtual MainPosition* GetPosition(AIContext& plist, MoveP m = nullptr) const;     // execute move, maintain in PList
    virtual const std::vector<const Piece*> Execute(MoveP m);
    virtual void Undo(const MoveP m);

  public:
    Moves sequence{};

  protected:
    const Color* onTurn{ &Color::White };
    Depth depth{ 0 };
    PositionValue value{ PositionValue::Undefined };                 // position value for White
    Moves movelistW{};
    Moves movelistB{};
  };

  class TakenPosition : public Position
  {
  public:
    inline TakenPosition(Coordinate x, Coordinate y) noexcept : Position(x, y, &Piece::NoTile) {}
    void Push(unsigned int player, const std::vector<const Piece*>& p) noexcept;
  };

  class StockPosition : public Position
  {
  public:
    inline StockPosition(Coordinate x, Coordinate y) noexcept : Position(x, y) {}
  };

}
