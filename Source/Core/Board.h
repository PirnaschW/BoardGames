
namespace BoardGamesCore
{
  using Depth = unsigned char;         // evaluation depth, never going to go beyond 127...
  class Board
  {
  protected:
    Board(const VariantChosen& v, const BoardPartDimensions& d) noexcept;
    Board(const Board& m) noexcept;                    // copy constructor - needed for cloning in derived boards
    Board& operator=(const Board& b) = delete;         // copy assignment
    Board(Board&& m) noexcept = delete;                // move constructor
    Board& operator=(Board&& b) noexcept { assert(false); }     // move assignment

  public:
    virtual ~Board() noexcept = default;
    virtual Board* Clone() const noexcept = 0;  // each game must define its own clone function

    virtual std::size_t GetHash() const noexcept;
    virtual bool operator ==(const Board& p) const noexcept { return whiteOnTurn_ == p.whiteOnTurn_ && stage_ == p.stage_; } // if Stage is equal, Taken is automatically equal too!
    virtual bool operator !=(const Board& p) const noexcept { return !(*this == p); }

    virtual void Serialize(std::stringstream& s) const;
    virtual void SetStartingBoard() noexcept {};  // default is empty board; each game can define its specific starting setup
    void SetupBoard(const std::vector<PieceIndex>& list) noexcept;  // set up a board from a given Piece List

    [[TODO::Unittest]] constexpr PieceIndex GetPieceIndex(const Location& l) const noexcept { return GetPieceIndex(l.x_,l.y_,l.b_); }
    constexpr PieceIndex GetPieceIndex(Coordinate x, Coordinate y, BoardPartID b = BoardPartID::Stage) const noexcept
    {
      switch (b)
      {
        case BoardPartID::Stock: return stock_.GetPieceIndex(x, y);
        case BoardPartID::Taken: return taken_.GetPieceIndex(x, y);
        default:                 return stage_.GetPieceIndex(x, y);
      }
    }

    [[TODO::Unittest]] void SetPieceIndex (PieceIndex pI, const Location& l) noexcept { SetPieceIndex(pI, l.x_, l.y_, l.b_); }
    void SetPieceIndex(PieceIndex pI, Coordinate x, Coordinate y, BoardPartID b = BoardPartID::Stage) noexcept;

    constexpr PositionValue GetValue(bool whiteonplay) const noexcept { return value_.Relative(whiteonplay); }
    constexpr void SetValue(bool whiteonplay, PositionValue v) const noexcept { value_ = v.Relative(whiteonplay); }
    constexpr Depth GetDepth() const noexcept { return depth_; }
    constexpr void SetDepth(Depth d) const noexcept { depth_ = d; }

    constexpr Moves& GetMoveList(bool whiteonplay) const noexcept { return whiteonplay ? movesW_ : movesB_; }
    virtual int GetMoveCountFactor() const noexcept { return 20; }

    virtual void GetAllMoves() const noexcept;              // generate all moves and save list
    virtual void EvaluateStatically() const noexcept;       // calculate static position value
    
    [[TODO::Unittest]] void Execute(const MoveP& p) noexcept;
    [[deprecated]] void Undo(const Move& m) noexcept { assert(false); }  // TODO: Let Board Undo Moves

    constexpr bool WhiteOnTurn() const noexcept { return whiteOnTurn_; }

    virtual void Draw(DC* dc, bool showstock) const;                          // draw the complete board
    virtual void DrawSelected(DC* dc, const Location& l) const;   // highlight field (for selected / from / to)
    virtual bool GetLocationFromPoint(const Point& p, Location& l) const noexcept;

    constexpr virtual bool AddIfLegal(Moves& /*m*/, const Location& /*fr*/, const Location& /*to*/) const noexcept { return false; }

  protected:
    constexpr PValueBaseType GetChainValue(unsigned int z) const noexcept;           // value of a chain of length z
    virtual PositionValue EvaluateChainLengths(unsigned int towin) const noexcept;   // calculate position value of all chain lengths

    //[[TODO::Unittest]] [[deprecated]] VariantChosen GetVariant() const { return v_; }
    //[[TODO::Unittest]] [[deprecated]] bool HasRule(Rule r) const noexcept { return GetRule() & r; }
    //[[TODO::Unittest]] [[deprecated]] virtual Rule GetRule() const noexcept { return 0; }
    //[[TODO::Unittest]] [[deprecated]] void JumpsOnly(Moves& moves) const noexcept;          // if there are any jumps, remove all non-jumps - jumping is mandatory

    //[[TODO::Unittest]] [[deprecated]] const Location GetNextFreeTakenLocation(const PieceColor& c) const noexcept;
    //[[TODO::Unittest]] [[deprecated]] bool IsTaken(const Location& l) const noexcept;

    //[[TODO::Unittest]] bool EnsureValid(const Location& l) const noexcept { return GetPiece(l) != Piece::NoTile; }       // check if the location is a valid field
    //[[TODO::Unittest]] bool IsFree(const Location& l) const noexcept { return EnsureValid(l) && GetPiece(l).IsBlank(); } // check if the location is a valid field and is free
    //[[TODO::Unittest]] bool IsMyPiece(const Location& l, const PieceColor& c) const noexcept { return EnsureValid(l) && GetPiece(l).IsColor(c); }  // check if own piece is in the location 
    //[[TODO::Unittest]] bool IsOppPiece(const Location& l, const PieceColor& c) const noexcept { return EnsureValid(l) && !GetPiece(l).IsBlank() && !GetPiece(l).IsColor(c); }  // check if opponent's piece is in the location 

  private:
    PositionValue EvaluateChainLength(Location l, unsigned int towin) const noexcept;  // chain length for one location

  public:
    VariantChosen v_;                                                     // variant of the game (defines rule set)
    BoardPart stage_;                                                     // main playing board
    BoardPart stock_;                                                     // associated Stock position
    BoardPart taken_;                                                     // associated Taken position
    Moves sequence_{};                                                    // list of historic moves that led to this position

  protected:
    bool whiteOnTurn_{ true};                                             // player currently on turn

    //below members are mutable because adjusting them during the evaluation doesn't 'change' the board position'.
    mutable PositionValue value_{ PositionValue::PValueType::Undefined }; // calculated position value (positiv is good for White)
    mutable Depth depth_{ 0 };                                            // evaluated depth of the position
    mutable Moves movesW_{};                                              // list of possible moves for White
    mutable Moves movesB_{};                                              // list of possible moves for Black
  };

//  static_assert(sizeof(Board) == 360, "size of Board has changed");

}

template <> struct std::hash<BoardGamesCore::Board> { std::size_t operator()(const BoardGamesCore::Board& b) const noexcept { return b.GetHash(); } };
