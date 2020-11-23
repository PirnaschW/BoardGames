
namespace BoardGamesCore
{
  class AIContext;
  
  class Position
  {
  private:
    Position(void) noexcept = delete;

  protected:
    Position(const PieceMapP& p, Coordinate x, Coordinate y, const Piece& init = Piece::NoPiece) noexcept;
    Position(const Position& p) noexcept;
  public:
    virtual ~Position(void) noexcept {}

    constexpr Coordinate GetSizeX(void) const noexcept { return sizeX_; }  // potentially needed for positional value calculations
    constexpr Coordinate GetSizeY(void) const noexcept { return sizeY_; }  // only needed for positional value calculations
    const Piece& GetPiece(const Location& l) const noexcept;
    bool HasPiece(const Piece& p) const noexcept;
    bool operator ==(const Position* p) const noexcept;
    bool operator !=(const Position* p) const noexcept;

    virtual std::size_t GetHash(void) const noexcept;
    virtual void Serialize(Archive& ar) const;
    virtual const Piece& SetPiece(const Location& l, const Piece& p) noexcept;
    virtual void SetPosition(std::vector<const Piece*> list);

  protected:
    const PieceMapP& pMap_;                                               // map of all potential pieces in this game
    const Coordinate sizeX_;                                              // horizontal board size
    const Coordinate sizeY_;                                              // vertical board size
  private:
    std::vector<PieceIndex> pieces_;                                      // pMap-index of the Piece at each Location
    mutable std::size_t hash_{};                                          // mutable, so it can be lazy-filled
  };


  struct Dimension final                                                  // PoD structure to collect a game's layout dimensions
  {
  public:
    constexpr Dimension(
      Coordinate xc, Coordinate yc,                                       // tile count in x and y directions
      unsigned int le, unsigned int te,                                   // starting edge in x and y directions
      unsigned int xd, unsigned int yd,                                   // tile size in x and y directions
      unsigned int xs = 0, unsigned int ys = 0) noexcept :                // extra distance between tiles in x and y directions
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
  using Dimensions = std::array<Dimension, 3>;                            // Main, Stock, Taken

  class MainPosition : public Position
  {
  public:
    using Depth = unsigned int;

  protected:
    MainPosition(const MainPosition& m) noexcept;

  public:
    MainPosition(VariantCode c, const PieceMapP& p, const Dimensions& d) noexcept;
    ~MainPosition(void) noexcept override {}
    virtual MainPosition* Clone(void) const noexcept = 0;
    virtual bool operator ==(const MainPosition& p) const noexcept;
    virtual std::size_t GetHash(void) const noexcept;

    void SetOnTurn(const PieceColor& c) noexcept;
    const PieceColor& OnTurn(void) const noexcept;
    virtual void NextPlayer(void) noexcept;
    virtual void PreviousPlayer(void) noexcept;  // needed for Undo

    VariantCode GetVariant(void) const { return vCode_; }
    constexpr Moves& GetMoveList(bool w) const noexcept { return w ? movesW_ : movesB_; }
    virtual bool AddIfLegal(Moves&, const Location&, const Location&) const noexcept { return false; };
    [[ nodiscard ]] virtual PositionValue EvaluateStatically(void) const noexcept;       // calculate position value
    [[ nodiscard ]] virtual PositionValue EvaluateChainLengths(unsigned int max) const noexcept;        // calculate position value by chain lengths
    virtual unsigned int GetChainValue(unsigned int z) const noexcept { return 0; }
    virtual PositionValue Evaluate(AIContext& plist, bool w, PositionValue alpha, PositionValue beta, unsigned int plies) const noexcept;
    virtual PositionValue EvaluateBF(AIContext& plist, bool w, unsigned int plies) const noexcept;
    PositionValue GetValue(bool w) const noexcept;
    virtual unsigned int GetMoveCountFactor(void) const noexcept { return 20; }
    Depth GetDepth(void) const noexcept { return depth_; }
    Depth SetDepth(Depth d) const noexcept { return depth_ = d; }
    PositionValue SetValue(bool w, PositionValue v) const noexcept;
    virtual const Piece& GetPiece(const Location& l) const noexcept;
    virtual const Piece& SetPiece(const Location& l, const Piece& p) noexcept;
    const Location GetNextTakenL(const PieceColor& c) const noexcept;
    bool IsTaken(const Location& l) const noexcept;
    virtual MoveP GetBestMove(bool w) const noexcept;
    virtual MainPosition* GetPosition(AIContext& plist, MoveP m = nullptr) const noexcept;     // execute move, maintain in PList
    virtual void Execute(const Move& m) noexcept;
    virtual void Undo(const Move& m) noexcept;

    virtual void GetAllMoves(void) const noexcept;              // generate all moves and save list
    void JumpsOnly(Moves& moves) const noexcept;          // if there are any jumps, remove all non-jumps - jumping is mandatory

  private:
    [[ nodiscard ]] PositionValue EvaluateChainLength(Location l, unsigned int max) const noexcept;  // chain length for one location


  public:
    VariantCode vCode_;                                                   // rule variant of the game
    Moves sequence_{};                                                    // list of historic moves that led to this position
    Position stock_;                                                      // associated Stock position
    Position taken_;                                                      // associated Taken position

  protected:
    const PieceColor* onTurn_{ &PieceColor::White };                                // color (player) currently on turn

    //below members are mutable because adjusting them during the evaluation doesn't 'change' the position'.
    mutable PositionValue value_{ PositionValue::PValueType::Undefined }; // calculated position value (positiv is good for White)
    mutable Depth depth_{ 0 };                                            // evaluated depth of the position
    mutable Moves movesW_{};                                              // list of possible moves for White
    mutable Moves movesB_{};                                              // list of possible moves for Black
  };

}
