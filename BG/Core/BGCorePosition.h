
namespace BoardGamesCore
{

  class Position
  {
  private:
    constexpr inline Position(void) noexcept = delete;

  protected:
    inline Position(const PieceMapP& p, Coordinate x, Coordinate y, const Piece* init = &Piece::NoPiece) noexcept
      : pMap_{ p }, sizeX_(x), sizeY_(y), pieces_(1ULL * x * y, pMap_->GetIndex(init)) {}
    inline Position(const Position& p) noexcept : pMap_{ p.pMap_ }, sizeX_(p.sizeX_), sizeY_(p.sizeY_), pieces_{ p.pieces_ } {}
  public:
    inline virtual ~Position(void) noexcept {}

    constexpr inline Coordinate GetSizeX(void) const noexcept { return sizeX_; }  // potentially needed for positional value calculations
    constexpr inline Coordinate GetSizeY(void) const noexcept { return sizeY_; }  // only needed for positional value calculations
    constexpr inline const Piece* GetPiece(const Location& l) const noexcept { return l.Valid(sizeX_, sizeY_) ? pMap_->GetPiece(pieces_[l.Index(sizeX_, sizeY_)]) : nullptr; }
    inline bool HasPiece(const Piece* p) const noexcept { const PieceIndex z = pMap_->GetIndex(p); for (const auto& pp : pieces_) if (pp == z) return true; return false; }
    inline bool operator ==(const Position* p) const noexcept { return p->sizeX_ == sizeX_ && p->sizeY_ == sizeY_ && p->pieces_ == pieces_; }
    inline bool operator !=(const Position* p) const noexcept { return !(*this == p); }

    virtual std::size_t GetHash(void) const noexcept;
    virtual void Serialize(CArchive* ar) const { for (auto& p : pieces_) pMap_->GetPiece(p)->Serialize(ar); }
    virtual const Piece* SetPiece(const Location& l, const Piece* p) noexcept { hash_ = 0; pieces_[l.Index(sizeX_, sizeY_)] = pMap_->GetIndex(p); return p; }
    virtual void SetPosition(std::vector<const Piece*> list);

  protected:
    const PieceMapP& pMap_;
    const Coordinate sizeX_;
    const Coordinate sizeY_;
  private:
    std::vector<PieceIndex> pieces_;
    mutable std::size_t hash_{};
  };


  struct Dimension final  /// PoD structure to collect a game's layout dimensions
  {
  public:
    constexpr Dimension(
      Coordinate xc, Coordinate yc,                         // tile count in x and y directions
      unsigned int le, unsigned int te,                     // starting edge in x and y directions
      unsigned int xd, unsigned int yd,                     // tile size in x and y directions
      unsigned int xs = 0, unsigned int ys = 0) noexcept :  // extra distance between tiles in x and y directions
      xCount_(xc), yCount_(yc),
      xDim_(xd), yDim_(yd),
      xSkip_(xs), ySkip_(ys),
      lEdge_(le), tEdge_(te) {}

  public:
    const Coordinate xCount_;
    const Coordinate yCount_;
    const unsigned int xDim_;
    const unsigned int yDim_;
    const unsigned int xSkip_;
    const unsigned int ySkip_;
    const unsigned int lEdge_;
    const unsigned int tEdge_;
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
      sequence_(m.sequence_), stock_(m.stock_), taken_(m.taken_), onTurn_(m.onTurn_), depth_(m.depth_),
      value_(m.value_), movesW_(m.movesW_), movesB_(m.movesB_) {}

  public:
    inline MainPosition(const PieceMapP& p, const Dimensions& d) noexcept : Position(p, d[0].xCount_, d[0].yCount_), stock_(p, d[1].xCount_, d[1].yCount_), taken_(p, d[2].xCount_, d[2].yCount_) {}
    ~MainPosition(void) noexcept override {}
    virtual MainPosition* Clone(void) const noexcept = 0;
    virtual inline bool operator ==(const MainPosition& p) const noexcept { return OnTurn() == p.OnTurn() && Position::operator==(&p); }
    virtual inline std::size_t GetHash(void) const noexcept { return Position::GetHash() + taken_.GetHash() + std::hash<const Color*>()(OnTurn()); }

    inline void SetOnTurn(const Color* c) noexcept { onTurn_ = c; }
    inline const Color* OnTurn(void) const noexcept { return onTurn_; }
    virtual void NextPlayer(void) noexcept;
    virtual void PreviousPlayer(void) noexcept;  // needed for Undo

    virtual void GetAllMoves(void) noexcept;              // generate all moves and save list
    virtual Moves CollectMoves(void) const noexcept { Moves m{}; return m; }
    void JumpsOnly(Moves& moves) const noexcept;          // if there are any jumps, remove all non-jumps - jumping is mandatory
    constexpr inline Moves& GetMoveList(bool w) noexcept { return w ? movesW_ : movesB_; }
    virtual bool AddIfLegal(Moves&, const Location, const Location) const noexcept { return false; };
    virtual void EvaluateStatically(void) noexcept;       // calculate position value and save
    virtual PositionValue Evaluate(AIContextP& plist, bool w, PositionValue alpha, PositionValue beta, unsigned int plies);
    inline PositionValue GetValue(bool w) const noexcept { return value_.Relative(w); }
    virtual inline unsigned int GetMoveCountFactor(void) const noexcept { return 20; }
    inline Depth GetDepth(void) const noexcept { return depth_; }
    inline Depth SetDepth(Depth d) noexcept { return depth_ = d; }
    inline PositionValue SetValue(bool w, PositionValue v) noexcept { return value_ = v.Relative(w); }
    virtual const Piece* GetPiece(const Location& l) const noexcept;
    virtual const Piece* SetPiece(const Location& l, const Piece* p) noexcept;
    const Location GetNextTakenL(const Color* c) const noexcept;
    inline bool IsTaken(const Location& l) const noexcept { return l.b_ == BoardPart::Taken; }
    virtual inline MoveP GetBestMove(bool w) const noexcept { return (w ? movesW_[0] : movesB_[0]); }
    virtual MainPosition* GetPosition(AIContextP& plist, MoveP m = nullptr) const noexcept;     // execute move, maintain in PList
    virtual void Execute(const Move& m) noexcept;
    virtual void Undo(const Move& m) noexcept;

  public:
    Moves sequence_{};
    Position stock_;
    Position taken_;

  protected:
    const Color* onTurn_{ &Color::White };
    Depth depth_{ 0 };
    PositionValue value_{ PositionValue::Undefined };                 // position value for White
    Moves movesW_{};
    Moves movesB_{};
  };

}
