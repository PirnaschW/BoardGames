#include "Games.h"

#include "ChessGame.h"

namespace Chess
{

  // templates to create a subclass for each variant

  template <ChessVariant V> struct ChessVariantData {};  // helper to allow to add data for one variant only
  template <> struct ChessVariantData<ChessVariant::Dice>      { mutable Side side_; };  // rolled Die
  template <> struct ChessVariantData<ChessVariant::Dice10x10> { mutable Side side_; };  // rolled Die

  template <ChessVariant V>
  class ChessVariantBoard : public ChessBoard, public ChessVariantData<V>
  {
  protected:
    ChessVariantBoard<V>(const ChessVariantBoard<V>& m) noexcept : ChessBoard(m), ChessVariantData<V>(m) {}
  public:
    inline ChessVariantBoard<V>(const VariantChosen& v, const PieceMapP& p, const BoardPartDimensions& d) noexcept : ChessBoard(v, p, d) {}
    virtual Board* Clone() const noexcept override { return new ChessVariantBoard<V>(*this); }
    virtual void SetStartingBoard() noexcept override { ChessBoard::SetStartingBoard(); }
    virtual void GetAllMoves() const noexcept { ChessBoard::GetAllMoves(); }
    virtual bool AddIfLegal(Moves& m, const Location& fr, const Location& to) const noexcept override { return ChessBoard::AddIfLegal(m, fr, to); }
    constexpr virtual int GetMoveCountFactor() const noexcept override { return ChessBoard::GetMoveCountFactor(); }
    [[ nodiscard ]] virtual PositionValue EvaluateStatically() const noexcept override { return ChessBoard::EvaluateStatically(); }

  private:
    virtual Rule GetRule() const noexcept override { return Castling | AllowMoves | AllowTakes | PawnsPromote | PawnsDoubleStep | EnPassant; }
    static bool ValidBoard(const std::vector<Coordinate>& c) noexcept { return true; }
  };

  template <ChessVariant V>
  class ChessVariantLayout : public ChessLayout
  {
  public:
    ChessVariantLayout(const BoardPartDimensions& d, LayoutType lt = LayoutType::Alternating) noexcept : ChessLayout(d, lt) {}
    virtual ~ChessVariantLayout() noexcept {}
    virtual void Draw(DC* pDC, const Board* board_, Mode mode_) const { ChessLayout::Draw(pDC, board_, mode_); }
  };

  //#########################################
  // Specializations - those contain the variant specific code
  // note that sequence DOES matter in defining method specializations!
  // each method specialization must be defined BEFORE it is used / called


  // specializations for ChessVariant::Corner
  template <> inline Rule ChessVariantBoard<ChessVariant::Corner>::GetRule() const noexcept { return AllowMoves | AllowTakes | PawnsPromote | PawnsDoubleStep | EnPassant; }
  template <> inline bool ChessVariantBoard<ChessVariant::Corner>::ValidBoard(const std::vector<Coordinate>& c) noexcept { return c[3] % 2 != c[4] % 2; } // check for bishops on differently colored fields
  template <> inline void ChessVariantBoard<ChessVariant::Corner>::SetStartingBoard() noexcept
  {
    SetPawns(1U, PieceColor::Black);
    SetPawns(sizeY_ - 2U, PieceColor::White);

    SetPiecesPSymmetrical(0U, 0U, ChessPiece::BK, ChessPiece::WK);

    std::vector<Coordinate> c;
    for (Coordinate i = 1; i < sizeX_; i++) c.push_back(i);
    do Math::Shuffle(c);
    while (!ValidBoard(c));  // keep shuffling until the position is valid

    SetPiecesPSymmetrical(c[0], 0U, ChessPiece::BQ, ChessPiece::WQ);
    SetPiecesPSymmetrical(c[1], 0U, ChessPiece::BR, ChessPiece::WR);
    SetPiecesPSymmetrical(c[2], 0U, ChessPiece::BR, ChessPiece::WR);
    SetPiecesPSymmetrical(c[3], 0U, ChessPiece::BB, ChessPiece::WB);
    SetPiecesPSymmetrical(c[4], 0U, ChessPiece::BB, ChessPiece::WB);
    SetPiecesPSymmetrical(c[5], 0U, ChessPiece::BN, ChessPiece::WN);
    SetPiecesPSymmetrical(c[6], 0U, ChessPiece::BN, ChessPiece::WN);
  }


  // specializations for ChessVariant::Fortress
  template <> inline Rule ChessVariantBoard<ChessVariant::Fortress>::GetRule() const noexcept { return AllowMoves | AllowTakes | PawnsPromote | PawnsDoubleStep | EnPassant; }
  template <> inline bool ChessVariantBoard<ChessVariant::Fortress>::ValidBoard(const std::vector<Coordinate>& c) noexcept { return c[3] % 2 != c[4] % 2; } // check for bishops on differently colored fields
  template <> inline void ChessVariantBoard<ChessVariant::Fortress>::SetStartingBoard() noexcept
  {
    SetPawns(1U, PieceColor::Black);
    SetPawns(sizeY_ - 2U, PieceColor::White);

    SetPiecesPSymmetrical(0U, 0U, ChessPiece::BK, ChessPiece::WK);

    std::vector<Coordinate> c;
    for (Coordinate i = 1; i < sizeX_; i++) c.push_back(i);
    do Math::Shuffle(c);
    while (!ValidBoard(c));  // keep shuffling until the position is valid

    SetPiecesPSymmetrical(c[0], 0U, ChessPiece::BQ, ChessPiece::WQ);
    SetPiecesPSymmetrical(c[1], 0U, ChessPiece::BR, ChessPiece::WR);
    SetPiecesPSymmetrical(c[2], 0U, ChessPiece::BR, ChessPiece::WR);
    SetPiecesPSymmetrical(c[3], 0U, ChessPiece::BB, ChessPiece::WB);
    SetPiecesPSymmetrical(c[4], 0U, ChessPiece::BB, ChessPiece::WB);
    SetPiecesPSymmetrical(c[5], 0U, ChessPiece::BN, ChessPiece::WN);
    SetPiecesPSymmetrical(c[6], 0U, ChessPiece::BN, ChessPiece::WN);
    SetPiecesPSymmetrical(  0U, 2U, ChessPiece::BP, ChessPiece::WP);
    SetPiecesPSymmetrical(  1U, 2U, ChessPiece::BP, ChessPiece::WP);
    SetPiecesPSymmetrical(  2U, 2U, ChessPiece::BP, ChessPiece::WP);
  };


  // specializations for ChessVariant::Horde
  template <> inline void ChessVariantBoard<ChessVariant::Horde>::SetStartingBoard() noexcept
  {
    ChessBoard::SetStartingBoard();
    SetPawns(0U, PieceColor::Black);
    SetPawns(2U, PieceColor::Black);
    SetPawns(3U, PieceColor::Black);
    SetPieceIndex({ BoardPartID::Stage, 3U, 4U }, ChessPiece::BP);
    SetPieceIndex({ BoardPartID::Stage, 4U, 4U }, ChessPiece::BP);
    SetPieceIndex({ BoardPartID::Stage, 3U, 0U }, Piece::NoPiece);
    SetPieceIndex({ BoardPartID::Stage, 4U, 0U }, Piece::NoPiece);
  }
  template <> inline PositionValue ChessVariantBoard<ChessVariant::Horde>::EvaluateStatically() const noexcept
  {
    // TODO: evaluate Horde positions
    return ChessBoard::EvaluateStatically(); // temporary
  }


  // specializations for ChessVariant::Loop
  template <> inline Rule ChessVariantBoard<ChessVariant::Loop>::GetRule() const noexcept { return Castling | AllowMoves | AllowTakes | PawnsPromote | PawnsDoubleStep | EnPassant | DropTakenPieces; }


  // specializations for ChessVariant::Anti
  template <> inline Rule ChessVariantBoard<ChessVariant::Anti>::GetRule() const noexcept { return AllowMoves | AllowTakes | MustTake | PawnsPromote | PawnsDoubleStep | EnPassant; }
  template <> inline PositionValue ChessVariantBoard<ChessVariant::Anti>::EvaluateStatically() const noexcept
  {
    // TODO: evaluate Anti positions
    return ChessBoard::EvaluateStatically(); // temporary
  }


  // specializations for ChessVariant::Extinction
  template <> inline PositionValue ChessVariantBoard<ChessVariant::Extinction>::EvaluateStatically() const noexcept
  {
    // TODO: evaluate Extinction positions
    return ChessBoard::EvaluateStatically(); // temporary
  }

  
  // specializations for ChessVariant::Maharajah
  template <> inline Rule ChessVariantBoard<ChessVariant::Maharajah>::GetRule() const noexcept { return Castling | AllowMoves | AllowTakes | PawnsDoubleStep; }
  template <> inline void ChessVariantBoard<ChessVariant::Maharajah>::SetStartingBoard() noexcept
  {
    ChessBoard::SetStartingBoard();
    for (Coordinate i = 0; i < sizeX_; i++)
    {
      SetPieceIndex({ BoardPartID::Stage, i, sizeY_ - 2U }, Piece::NoPiece);
      SetPieceIndex({ BoardPartID::Stage, i, sizeY_ - 1U }, Piece::NoPiece);
    }
    SetPieceIndex({ BoardPartID::Stage, 4U, sizeY_ - 1U }, ChessPiece::WA);
  }


  // specializations for ChessVariant::ThreeChecks
  template <> inline PositionValue ChessVariantBoard<ChessVariant::ThreeChecks>::EvaluateStatically() const noexcept
  {
    // TODO: evaluate ThreeChecks positions
    return ChessBoard::EvaluateStatically(); // temporary
  }


  // specializations for ChessVariant::Dark
// TODO: Dark Chess

  // specializations for ChessVariant::Atomic
  template <> inline bool ChessVariantBoard<ChessVariant::Atomic>::AddIfLegal(Moves& m, const Location& fr, const Location& to) const noexcept
  {
    // TODO: AddifLegal Atomic chess
    return ChessBoard::AddIfLegal(m, fr, to);  // temporary
  }


  // specializations for ChessVariant::Janus
  template <> inline void ChessVariantBoard<ChessVariant::Janus>::SetStartingBoard() noexcept
  {
    ChessBoard::SetStartingBoard();
    SetPiecesHSymmetrical(1U, 0U, ChessPiece::BC, ChessPiece::WC);
    SetPiecesHSymmetrical(2U, 0U, ChessPiece::BN, ChessPiece::WN);
    SetPiecesHSymmetrical(3U, 0U, ChessPiece::BB, ChessPiece::WB);
    SetPiecesHSymmetrical(6U, 0U, ChessPiece::BB, ChessPiece::WB);
    SetPiecesHSymmetrical(7U, 0U, ChessPiece::BN, ChessPiece::WN);
    SetPiecesHSymmetrical(8U, 0U, ChessPiece::BC, ChessPiece::WC);
  }
  

  // specializations for ChessVariant::Embassy
  template <> inline void ChessVariantBoard<ChessVariant::Embassy>::SetStartingBoard() noexcept
  {
    ChessBoard::SetStartingBoard();
    SetPiecesHSymmetrical(3U, 0U, ChessPiece::BQ, ChessPiece::WQ);
    SetPiecesHSymmetrical(4U, 0U, ChessPiece::BK, ChessPiece::WK);
    SetPiecesHSymmetrical(5U, 0U, ChessPiece::BM, ChessPiece::WM);
    SetPiecesHSymmetrical(6U, 0U, ChessPiece::BC, ChessPiece::WC);
  }
  

  // specializations for ChessVariant::Screen
  template <> inline void ChessVariantBoard<ChessVariant::Screen>::SetStartingBoard() noexcept {} // start blank
  // TODO: Screen chess


  // specializations for ChessVariant::CrazyScreen
  template <> inline void ChessVariantBoard<ChessVariant::CrazyScreen>::SetStartingBoard() noexcept {} // start blank
  // TODO: CrazyScreen chess

  
  // specializations for ChessVariant::Cylinder
  template <> inline bool ChessVariantBoard<ChessVariant::Cylinder>::AddIfLegal(Moves& m, const Location& fr, const Location& to) const noexcept
  {
    // TODO: AddifLegal Cylinder chess
    return ChessBoard::AddIfLegal(m, fr, to);  // temporary
  }


  // specializations for ChessVariant::Amazons
  template <> inline void ChessVariantBoard<ChessVariant::Amazons>::SetStartingBoard() noexcept
  {
    ChessBoard::SetStartingBoard();
    SetPiecesHSymmetrical(3U, 0U, ChessPiece::BA, ChessPiece::WA);
  }


  // specializations for ChessVariant::Berolina
  template <> inline Rule ChessVariantBoard<ChessVariant::Berolina>::GetRule() const noexcept { return Castling | AllowMoves | AllowTakes | PawnsPromote | PawnsDoubleStep | EnPassant | BerolinaPawns; }


  // specializations for ChessVariant::FischerRandom
  template <> inline bool ChessVariantBoard<ChessVariant::FischerRandom>::ValidBoard(const std::vector<Coordinate>& c) noexcept
  {
    if (c[3] % 2 == c[4] % 2) return false;       // bishops not on differently colored fields
    if (c[7] > c[1] && c[7] < c[2]) return true;  // king is between the rooks
    if (c[7] > c[2] && c[7] < c[1]) return true;  // king is between the rooks
    return false;
  }
  template <> inline void ChessVariantBoard<ChessVariant::FischerRandom>::SetStartingBoard() noexcept
  {
    SetPawns(1U, PieceColor::Black);
    SetPawns(sizeY_ - 2U, PieceColor::White);

    std::vector<Coordinate> c;
    for (Coordinate i = 0; i < sizeX_; i++) c.push_back(i);
      
    do Math::Shuffle(c); while (!ValidBoard(c));   // keep shuffling until position is valid

    SetPiecesHSymmetrical(c[0], 0U, ChessPiece::BQ, ChessPiece::WQ);
    SetPiecesHSymmetrical(c[1], 0U, ChessPiece::BR, ChessPiece::WR);
    SetPiecesHSymmetrical(c[2], 0U, ChessPiece::BR, ChessPiece::WR);
    SetPiecesHSymmetrical(c[3], 0U, ChessPiece::BB, ChessPiece::WB);
    SetPiecesHSymmetrical(c[4], 0U, ChessPiece::BB, ChessPiece::WB);
    SetPiecesHSymmetrical(c[5], 0U, ChessPiece::BN, ChessPiece::WN);
    SetPiecesHSymmetrical(c[6], 0U, ChessPiece::BN, ChessPiece::WN);
    SetPiecesHSymmetrical(c[7], 0U, ChessPiece::BK, ChessPiece::WK);
  }
  

  // specializations for ChessVariant::Legan
  template <> inline Rule ChessVariantBoard<ChessVariant::Legan>::GetRule() const noexcept { return AllowMoves | AllowTakes | PawnsPromote | LeganPawns; }
  template <> inline void ChessVariantBoard<ChessVariant::Legan>::SetStartingBoard() noexcept
  {
    SetPiecesPSymmetrical(0U, 0U, ChessPiece::BK, ChessPiece::WK);
    SetPiecesPSymmetrical(1U, 1U, ChessPiece::BQ, ChessPiece::WQ);
    SetPiecesPSymmetrical(0U, 3U, ChessPiece::BR, ChessPiece::WR);
    SetPiecesPSymmetrical(3U, 0U, ChessPiece::BR, ChessPiece::WR);
    SetPiecesPSymmetrical(0U, 1U, ChessPiece::BB, ChessPiece::WB);
    SetPiecesPSymmetrical(2U, 0U, ChessPiece::BB, ChessPiece::WB);
    SetPiecesPSymmetrical(0U, 2U, ChessPiece::BN, ChessPiece::WN);
    SetPiecesPSymmetrical(1U, 0U, ChessPiece::BN, ChessPiece::WN);

    SetPiecesPSymmetrical(1U, 2U, ChessPiece::BP, ChessPiece::WP);
    SetPiecesPSymmetrical(2U, 1U, ChessPiece::BP, ChessPiece::WP);
    SetPiecesPSymmetrical(4U, 0U, ChessPiece::BP, ChessPiece::WP);
    SetPiecesPSymmetrical(3U, 1U, ChessPiece::BP, ChessPiece::WP);
    SetPiecesPSymmetrical(2U, 2U, ChessPiece::BP, ChessPiece::WP);
    SetPiecesPSymmetrical(1U, 3U, ChessPiece::BP, ChessPiece::WP);
    SetPiecesPSymmetrical(0U, 4U, ChessPiece::BP, ChessPiece::WP);
    SetPiecesPSymmetrical(3U, 3U, ChessPiece::BP, ChessPiece::WP);
  }


  // specializations for ChessVariant::KnightRelay
  template <> inline Rule ChessVariantBoard<ChessVariant::KnightRelay>::GetRule() const noexcept { return Castling | AllowMoves | AllowTakes | PawnsPromote | PawnsDoubleStep; }
 // TODO: add KnightRelay chess


  // specializations for ChessVariant::Grand
  template <> inline void ChessVariantBoard<ChessVariant::Grand>::SetStartingBoard() noexcept
  {
    SetPawns(2U, PieceColor::Black);
    SetPawns(sizeY_ - 3U, PieceColor::White);

    SetPiecesHSymmetrical(0U, 0U, ChessPiece::BR, ChessPiece::WR);
    SetPiecesHSymmetrical(1U, 1U, ChessPiece::BN, ChessPiece::WN);
    SetPiecesHSymmetrical(2U, 1U, ChessPiece::BB, ChessPiece::WB);
    SetPiecesHSymmetrical(3U, 1U, ChessPiece::BQ, ChessPiece::WQ);
    SetPiecesHSymmetrical(4U, 1U, ChessPiece::BK, ChessPiece::WK);
    SetPiecesHSymmetrical(5U, 1U, ChessPiece::BM, ChessPiece::WM);
    SetPiecesHSymmetrical(6U, 1U, ChessPiece::BC, ChessPiece::WC);
    SetPiecesHSymmetrical(7U, 1U, ChessPiece::BB, ChessPiece::WB);
    SetPiecesHSymmetrical(8U, 1U, ChessPiece::BN, ChessPiece::WN);
    SetPiecesHSymmetrical(9U, 0U, ChessPiece::BR, ChessPiece::WR);
  }


  // specializations for ChessVariant::CapablancaRandom
  template <> inline bool ChessVariantBoard<ChessVariant::CapablancaRandom>::ValidBoard(const std::vector<Coordinate>& c) noexcept
  {
    if (c[3] % 2 == c[4] % 2) return false;       // bishops not on differently colored fields
    if (c[7] > c[1] && c[7] < c[2]) return true;  // king is between the rooks
    if (c[7] > c[2] && c[7] < c[1]) return true;  // king is between the rooks
    return false;
  }
  template <> inline void ChessVariantBoard<ChessVariant::CapablancaRandom>::SetStartingBoard() noexcept
  {
    SetPawns(1U, PieceColor::Black);
    SetPawns(sizeY_ - 2U, PieceColor::White);

    std::vector<Coordinate> c;
    for (Coordinate i = 0; i < sizeX_; i++) c.push_back(i);

    do Math::Shuffle(c); while (!ValidBoard(c));   // keep shuffling until position is valid

    SetPiecesHSymmetrical(c[0], 0U, ChessPiece::BQ, ChessPiece::WQ);
    SetPiecesHSymmetrical(c[1], 0U, ChessPiece::BR, ChessPiece::WR);
    SetPiecesHSymmetrical(c[2], 0U, ChessPiece::BR, ChessPiece::WR);
    SetPiecesHSymmetrical(c[3], 0U, ChessPiece::BB, ChessPiece::WB);
    SetPiecesHSymmetrical(c[4], 0U, ChessPiece::BB, ChessPiece::WB);
    SetPiecesHSymmetrical(c[5], 0U, ChessPiece::BN, ChessPiece::WN);
    SetPiecesHSymmetrical(c[6], 0U, ChessPiece::BN, ChessPiece::WN);
    SetPiecesHSymmetrical(c[7], 0U, ChessPiece::BK, ChessPiece::WK);
    SetPiecesHSymmetrical(c[8], 0U, ChessPiece::BC, ChessPiece::WC);
    SetPiecesHSymmetrical(c[9], 0U, ChessPiece::BM, ChessPiece::WM);
  }


  // specializations for ChessVariant::LosAlamos
  template <> inline Rule ChessVariantBoard<ChessVariant::LosAlamos>::GetRule() const noexcept { return AllowMoves | AllowTakes | PawnsPromote; }


  // specializations for ChessVariant::Ambiguous
  template <> inline Rule ChessVariantBoard<ChessVariant::Ambiguous>::GetRule() const noexcept { return AllowMoves | AllowTakes | PawnsPromote | PawnsDoubleStep | EnPassant; }
  // TODO: add Ambiguous chess


  // specializations for ChessVariant::Cheversi
  template <> inline Rule ChessVariantBoard<ChessVariant::Cheversi>::GetRule() const noexcept { return None; }
  template <> inline void ChessVariantBoard<ChessVariant::Cheversi>::SetStartingBoard() noexcept {}  // start empty
  template <> inline void ChessVariantBoard<ChessVariant::Cheversi>::GetAllMoves() const noexcept // collect all moves for all pieces
  {
    assert(movesW_.empty());
    assert(movesB_.empty());

    movesW_.reserve(20);
    movesB_.reserve(20);

  // TODO: add GetAllMoves for Cheversi
    if (sequence_.size() == 0)  // first move?
    {
    }
  }
  

  // specializations for ChessVariant::Dice
  template <> inline void ChessVariantBoard<ChessVariant::Dice>::GetAllMoves() const noexcept // collect all moves for all pieces
  {
    ChessBoard::GetAllMoves(); // get all moves, and only then dabble around which numbers can be rolled
    ChessVariantData<ChessVariant::Dice>::side_ = PickRandomPiece();
    // TODO: Move list for blacks crashes
  }
  template <> inline void ChessVariantLayout<ChessVariant::Dice>::Draw(DC* pDC, const Board* board_, Mode mode_) const
  {
    ChessLayout::Draw(pDC, board_, mode_);

    // show the rolled die
    constexpr int x = 500;
    constexpr int y = 200;
    Rect r{ x,y,x + 20,y + 20 };

    auto pp{ down_cast<const ChessVariantBoard<ChessVariant::Dice>*>(board_) };

    Die::Sides[pp->ChessVariantData<ChessVariant::Dice>::side_].Draw(pDC, r);
  }


  // specializations for ChessVariant::Recycle
  template <> inline Rule ChessVariantBoard<ChessVariant::Recycle>::GetRule() const noexcept { return Castling | AllowMoves | AllowTakes | TakeOwn | PawnsPromote | PawnsDoubleStep | DropTakenPieces; }


  // specializations for ChessVariant::IceAge
  template <> inline void ChessVariantBoard<ChessVariant::IceAge>::SetStartingBoard() noexcept
  {
    ChessBoard::SetStartingBoard();
    for (Coordinate i = 0U; i < sizeX_; i++)
    {
      for (Coordinate j = 2U; j < sizeY_/2; j++)
      {
        SetPiecesHSymmetrical(i, j, ChessPiece::GI, ChessPiece::GI);
      }
    }
  }
  // TODO: add IceAge post-move freezing
  // note: freezing is known to AI / player


  // specializations for ChessVariant::Behemoth
  template <> inline void ChessVariantBoard<ChessVariant::Behemoth>::SetStartingBoard() noexcept
  {
    ChessBoard::SetStartingBoard();
    SetPieceIndex({ BoardPartID::Stage,3U, 4U }, ChessPiece::RB);
  }
  // TODO: add Behemoth post-move killing
  // note: killing is NOT known to AI / player


  // specializations for ChessVariant::CheshireCat
  template <> inline Rule ChessVariantBoard<ChessVariant::CheshireCat>::GetRule() const noexcept { return AllowMoves | AllowTakes | PawnsPromote | PawnsDoubleStep | EnPassant; }
  template <> inline bool ChessVariantBoard<ChessVariant::CheshireCat>::AddIfLegal(Moves& m, const Location& fr, const Location& to) const noexcept
  {
    const Piece& pf = GetPieceIndex(fr);
    assert(pf != Piece::NoTile);                                                    // start field must exist
    assert(!pf.IsBlank());                                                          // start field must be a piece

    const Piece& pt = GetPieceIndex(to);
    if (pt == Piece::NoTile) return false;                                          // out of board
    if (pt.IsBlank())                                                               // moving onto free field
    {
      if (HasRule(AllowMoves))
      {
        Actions a{};
        a.push_back(std::make_shared<ActionLift>(fr, pf));                          // pick piece up
        a.push_back(std::make_shared<ActionEliminate>(fr, Piece::NoTile));          // remove starting field
        a.push_back(std::make_shared<ActionDrop>(to, pf));                          // and place it on target
        m.push_back(std::make_shared<Move>(a));                                     // add move to move list
      }
      return true;                                                                  // keep trying this direction
    }

    if (HasRule(AllowTakes))
    {
      if (pt.GetColor() == pf.GetColor() && !HasRule(TakeOwn)) return false;        // own piece; don't keep trying this direction

      // valid take move, save into collection
      Actions a{};
      a.push_back(std::make_shared<ActionLift>(fr, pf));                            // pick piece up
      a.push_back(std::make_shared<ActionEliminate>(fr, Piece::NoTile));            // remove starting field
      a.push_back(std::make_shared<ActionLift>(to, pt));                            // pick opponent piece up
      a.push_back(std::make_shared<ActionDrop>(GetNextFreeTakenLocation(pf.GetColor()), pt));  // place it in Taken
      a.push_back(std::make_shared<ActionDrop>(to, pf));                            // and place it on target
      m.push_back(std::make_shared<Move>(a));                                       // add move to move list
    }

    return false;                                                                   // don't keep trying this direction
  }
 // TODO: add CheshireCat King first move as Queen


  // specializations for ChessVariant::Knightmate
  template <> inline Rule ChessVariantBoard<ChessVariant::Knightmate>::GetRule() const noexcept { return Castling | AllowMoves | AllowTakes | PawnsPromote | PawnsDoubleStep | TakeKing; }
  template <> inline PositionValue ChessVariantBoard<ChessVariant::Knightmate>::EvaluateStatically() const noexcept
  {
    // TODO: evaluate Knightmate positions
    return ChessBoard::EvaluateStatically(); // temporary
  }


  // specializations for ChessVariant::RacingKings
  template <> inline Rule ChessVariantBoard<ChessVariant::RacingKings>::GetRule() const noexcept { return AllowMoves | AllowTakes;; }
  template <> inline void ChessVariantBoard<ChessVariant::RacingKings>::SetStartingBoard() noexcept
  {
    SetPieceIndex({ BoardPartID::Stage,              0U,sizeY_ - 1U }, ChessPiece::BQ);
    SetPieceIndex({ BoardPartID::Stage,              0U,sizeY_ - 2U }, ChessPiece::BK);
    SetPieceIndex({ BoardPartID::Stage,              1U,sizeY_ - 1U }, ChessPiece::BR);
    SetPieceIndex({ BoardPartID::Stage,              1U,sizeY_ - 2U }, ChessPiece::BR);
    SetPieceIndex({ BoardPartID::Stage,              2U,sizeY_ - 1U }, ChessPiece::BB);
    SetPieceIndex({ BoardPartID::Stage,              2U,sizeY_ - 2U }, ChessPiece::BB);
    SetPieceIndex({ BoardPartID::Stage,              3U,sizeY_ - 1U }, ChessPiece::BN);
    SetPieceIndex({ BoardPartID::Stage,              3U,sizeY_ - 2U }, ChessPiece::BN);

    SetPieceIndex({ BoardPartID::Stage,sizeX_ - 1U - 0U,sizeY_ - 1U }, ChessPiece::WQ);
    SetPieceIndex({ BoardPartID::Stage,sizeX_ - 1U - 0U,sizeY_ - 2U }, ChessPiece::WK);
    SetPieceIndex({ BoardPartID::Stage,sizeX_ - 1U - 1U,sizeY_ - 1U }, ChessPiece::WR);
    SetPieceIndex({ BoardPartID::Stage,sizeX_ - 1U - 1U,sizeY_ - 2U }, ChessPiece::WR);
    SetPieceIndex({ BoardPartID::Stage,sizeX_ - 1U - 2U,sizeY_ - 1U }, ChessPiece::WB);
    SetPieceIndex({ BoardPartID::Stage,sizeX_ - 1U - 2U,sizeY_ - 2U }, ChessPiece::WB);
    SetPieceIndex({ BoardPartID::Stage,sizeX_ - 1U - 3U,sizeY_ - 1U }, ChessPiece::WN);
    SetPieceIndex({ BoardPartID::Stage,sizeX_ - 1U - 3U,sizeY_ - 2U }, ChessPiece::WN);
  }
  template <> inline PositionValue ChessVariantBoard<ChessVariant::RacingKings>::EvaluateStatically() const noexcept
  {
    // TODO: evaluate RacingKings positions
    return ChessBoard::EvaluateStatically(); // temporary
  }

  
  // specializations for ChessVariant::Dice10x10
  template <> inline void ChessVariantBoard<ChessVariant::Dice10x10>::SetStartingBoard() noexcept
  {
    ChessBoard::SetStartingBoard();
    SetPiecesHSymmetrical(              3U, 0U, ChessPiece::BQ, ChessPiece::WQ);
    SetPiecesHSymmetrical(              4U, 0U, ChessPiece::BK, ChessPiece::WK);
    SetPiecesHSymmetrical(sizeX_ - 1U - 3U, 0U, ChessPiece::BK, ChessPiece::WK);
    SetPiecesHSymmetrical(sizeX_ - 1U - 4U, 0U, ChessPiece::BK, ChessPiece::WK);
  }
  template <> inline void ChessVariantBoard<ChessVariant::Dice10x10>::GetAllMoves() const noexcept // collect all moves for all pieces
  {
    ChessBoard::GetAllMoves(); // get all moves, and only then dabble around which numbers can be rolled
    ChessVariantData<ChessVariant::Dice10x10>::side_ = PickRandomPiece();
  }
  template <> inline void ChessVariantLayout<ChessVariant::Dice10x10>::Draw(DC* pDC, const Board* board_, Mode mode_) const
  {
    ChessLayout::Draw(pDC, board_, mode_);

    // show the rolled die
    constexpr int x = 600;
    constexpr int y = 200;
    Rect r{ x,y,x + 20,y + 20 };

    auto pp{ down_cast<const ChessVariantBoard<ChessVariant::Dice10x10>*>(board_) };

    Die::Sides[pp->ChessVariantData<ChessVariant::Dice10x10>::side_].Draw(pDC, r);
  }


  // specializations for ChessVariant::Massacre
  template <> inline Rule ChessVariantBoard<ChessVariant::Massacre>::GetRule() const noexcept { return AllowTakes; }
  template <> inline constexpr virtual unsigned int ChessVariantBoard<ChessVariant::Massacre>::GetMoveCountFactor() const noexcept { return 1000; }
  template <> inline void ChessVariantBoard<ChessVariant::Massacre>::SetStartingBoard() noexcept
  {
    std::vector<Location> ll;
    for (Coordinate i = 0; i < sizeX_; i++)
      for (Coordinate j = 0; j < sizeY_; j++)
      {
        const Location l{ BoardPartID::Stage,  i,j };
        assert(GetPieceIndex(l) == Piece::NoPiece);
        ll.push_back(l);
      }
    assert(ll.size() == sizeX_ * sizeY_);
    Math::Shuffle(ll);
    auto it = ll.begin();
    for (Coordinate z = 0; z < sizeX_ * sizeY_ / 8; z++)
    {
      SetPieceIndex(*it++, ChessPiece::WQ);
      SetPieceIndex(*it++, ChessPiece::BQ);
      SetPieceIndex(*it++, ChessPiece::WR);
      SetPieceIndex(*it++, ChessPiece::BR);
      SetPieceIndex(*it++, ChessPiece::WB);
      SetPieceIndex(*it++, ChessPiece::BB);
      SetPieceIndex(*it++, ChessPiece::WN);
      SetPieceIndex(*it++, ChessPiece::BN);
    }
    assert(it == ll.end());
  }
    

  const VariantList& ChessGame::GetVariants() noexcept
  {
    static VariantList v{
      { Variant{ "Standard Chess",          VC(ChessVariant::Standard),           8,  8 } },
      { Variant{ "Corner Chess",            VC(ChessVariant::Corner),             8,  8 } },
      { Variant{ "Fortress Chess",          VC(ChessVariant::Fortress),           8,  8 } },
      { Variant{ "Horde Chess",             VC(ChessVariant::Horde),              8,  8 } },
      { Variant{ "Loop Chess",              VC(ChessVariant::Loop),               8,  8 } },
      { Variant{ "Anti Chess",              VC(ChessVariant::Anti),               8,  8 } },
      { Variant{ "Extinction Chess",        VC(ChessVariant::Extinction),         8,  8 } },
      { Variant{ "Maharajah Chess",         VC(ChessVariant::Maharajah),          8,  8 } },
      { Variant{ "Three Checks Chess",      VC(ChessVariant::ThreeChecks),        8,  8 } },
      { Variant{ "Dark Chess",              VC(ChessVariant::Dark),               8,  8 } },
      { Variant{ "Atomic Chess",            VC(ChessVariant::Atomic),             8,  8 } },
      { Variant{ "Janus Chess",             VC(ChessVariant::Janus),             10,  8 } },
      { Variant{ "Embassy Chess",           VC(ChessVariant::Embassy),           10,  8 } },
      { Variant{ "Screen Chess",            VC(ChessVariant::Screen),             8,  8 } },
      { Variant{ "Crazy Screen Chess",      VC(ChessVariant::CrazyScreen),        8,  8 } },
      { Variant{ "Cylinder Chess",          VC(ChessVariant::Cylinder),           8,  8 } },
      { Variant{ "Amazon Chess",            VC(ChessVariant::Amazons),            8,  8 } },
      { Variant{ "Berolina Chess",          VC(ChessVariant::Berolina),           8,  8 } },
      { Variant{ "Fischer Random Chess",    VC(ChessVariant::FischerRandom),      8,  8 } },
      { Variant{ "Legan Chess",             VC(ChessVariant::Legan),              8,  8 } },
      { Variant{ "Knight Relay Chess",      VC(ChessVariant::KnightRelay),        8,  8 } },
      { Variant{ "Grand Chess",             VC(ChessVariant::Grand),             10, 10 } },
      { Variant{ "Capablanca Random Chess", VC(ChessVariant::CapablancaRandom),  10,  8 } },
      { Variant{ "Los Alamos Chess",        VC(ChessVariant::LosAlamos),          6,  6 } },
      { Variant{ "Ambiguous Chess",         VC(ChessVariant::Ambiguous),          8,  8 } },
      { Variant{ "Cheversi",                VC(ChessVariant::Cheversi),           8,  8 } },
      { Variant{ "Dice Chess",              VC(ChessVariant::Dice),               8,  8 } },
      { Variant{ "Recycle Chess",           VC(ChessVariant::Recycle),            8,  8 } },
      { Variant{ "Ice Age Chess",           VC(ChessVariant::IceAge),             8,  8 } },
      { Variant{ "Behemoth Chess",          VC(ChessVariant::Behemoth),           8,  8 } },
      { Variant{ "Cheshire Cat Chess",      VC(ChessVariant::CheshireCat),        8,  8 } },
      { Variant{ "Knightmate Chess",        VC(ChessVariant::Knightmate),         8,  8 } },
      { Variant{ "Racing Kings",            VC(ChessVariant::RacingKings),        8,  8 } },
      { Variant{ "Dice Chess 10x10",        VC(ChessVariant::Dice10x10),         10, 10 } },
      { Variant{ "Massacre Chess",          VC(ChessVariant::Massacre),           8,  8,  2, 20 } },
    };
    return v;
  }


  Board* ChessGame::GetNewBoard(const VariantChosen& v, const PieceMapP& m, const BoardPartDimensions& d) noexcept
  {
    // this switch looks silly, but cannot be avoided - the compiler has no idea which variants would be possible, 
    // but needs to generate the code for each one. An explicit list is needed in some form!
    switch (static_cast<ChessVariant>(v.c))
    {
      case ChessVariant::Standard:          return new ChessVariantBoard<ChessVariant::Standard        >(v, m, d);     // Standard Chess          
      case ChessVariant::Corner:            return new ChessVariantBoard<ChessVariant::Corner          >(v, m, d);     // Corner Chess            
      case ChessVariant::Fortress:          return new ChessVariantBoard<ChessVariant::Fortress        >(v, m, d);     // Fortress Chess          
      case ChessVariant::Horde:             return new ChessVariantBoard<ChessVariant::Horde           >(v, m, d);     // Horde Chess             
      case ChessVariant::Loop:              return new ChessVariantBoard<ChessVariant::Loop            >(v, m, d);     // Loop Chess              
      case ChessVariant::Anti:              return new ChessVariantBoard<ChessVariant::Anti            >(v, m, d);     // Anti Chess              
      case ChessVariant::Extinction:        return new ChessVariantBoard<ChessVariant::Extinction      >(v, m, d);     // Extinction Chess        
      case ChessVariant::Maharajah:         return new ChessVariantBoard<ChessVariant::Maharajah       >(v, m, d);     // Maharajah Chess         
      case ChessVariant::ThreeChecks:       return new ChessVariantBoard<ChessVariant::ThreeChecks     >(v, m, d);     // Three Checks Chess      
      case ChessVariant::Dark:              return new ChessVariantBoard<ChessVariant::Dark            >(v, m, d);     // Dark Chess              
      case ChessVariant::Atomic:            return new ChessVariantBoard<ChessVariant::Atomic          >(v, m, d);     // Atomic Chess            
      case ChessVariant::Janus:             return new ChessVariantBoard<ChessVariant::Janus           >(v, m, d);     // Janus Chess             
      case ChessVariant::Embassy:           return new ChessVariantBoard<ChessVariant::Embassy         >(v, m, d);     // Embassy Chess           
      case ChessVariant::Screen:            return new ChessVariantBoard<ChessVariant::Screen          >(v, m, d);     // Screen Chess            
      case ChessVariant::CrazyScreen:       return new ChessVariantBoard<ChessVariant::CrazyScreen     >(v, m, d);     // Crazy Screen Chess      
      case ChessVariant::Cylinder:          return new ChessVariantBoard<ChessVariant::Cylinder        >(v, m, d);     // Cylinder Chess          
      case ChessVariant::Amazons:           return new ChessVariantBoard<ChessVariant::Amazons         >(v, m, d);     // Amazon Chess            
      case ChessVariant::Berolina:          return new ChessVariantBoard<ChessVariant::Berolina        >(v, m, d);     // Berolina Chess          
      case ChessVariant::FischerRandom:     return new ChessVariantBoard<ChessVariant::FischerRandom   >(v, m, d);     // Fischer Random Chess    
      case ChessVariant::Legan:             return new ChessVariantBoard<ChessVariant::Legan           >(v, m, d);     // Legan Chess             
      case ChessVariant::KnightRelay:       return new ChessVariantBoard<ChessVariant::KnightRelay     >(v, m, d);     // Knight Relay Chess      
      case ChessVariant::Grand:             return new ChessVariantBoard<ChessVariant::Grand           >(v, m, d);     // Grand Chess             
      case ChessVariant::CapablancaRandom:  return new ChessVariantBoard<ChessVariant::CapablancaRandom>(v, m, d);     // Capablanca Random Chess 
      case ChessVariant::LosAlamos:         return new ChessVariantBoard<ChessVariant::LosAlamos       >(v, m, d);     // Los Alamos Chess        
      case ChessVariant::Ambiguous:         return new ChessVariantBoard<ChessVariant::Ambiguous       >(v, m, d);     // Ambiguous Chess         
      case ChessVariant::Cheversi:          return new ChessVariantBoard<ChessVariant::Cheversi        >(v, m, d);     // Cheversi
      case ChessVariant::Dice:              return new ChessVariantBoard<ChessVariant::Dice            >(v, m, d);     // Dice Chess              
      case ChessVariant::Recycle:           return new ChessVariantBoard<ChessVariant::Recycle         >(v, m, d);     // Recycle Chess           
      case ChessVariant::IceAge:            return new ChessVariantBoard<ChessVariant::IceAge          >(v, m, d);     // Ice Age Chess           
      case ChessVariant::Behemoth:          return new ChessVariantBoard<ChessVariant::Behemoth        >(v, m, d);     // Behemoth Chess          
      case ChessVariant::CheshireCat:       return new ChessVariantBoard<ChessVariant::CheshireCat     >(v, m, d);     // Cheshire Cat Chess      
      case ChessVariant::Knightmate:        return new ChessVariantBoard<ChessVariant::Knightmate      >(v, m, d);     // Knightmate Chess        
      case ChessVariant::RacingKings:       return new ChessVariantBoard<ChessVariant::RacingKings     >(v, m, d);     // Racing Kings            
      case ChessVariant::Dice10x10:         return new ChessVariantBoard<ChessVariant::Dice10x10       >(v, m, d);     // Dice Chess 10x10        
      case ChessVariant::Massacre:          return new ChessVariantBoard<ChessVariant::Massacre        >(v, m, d);     // Massacre Chess
      default: return nullptr; // must not happen
    }
  }


  MainLayout* ChessGame::GetNewLayout(const VariantChosen& v, const BoardPartDimensions& d) noexcept
  {
    // this switch looks silly, but cannot be avoided - the compiler has no idea which variants would be possible, 
    // but needs to generate the code for each one. An explicit list is needed in some form!
    switch (static_cast<ChessVariant>(v.c))
    {
      case ChessVariant::Standard:          return new ChessVariantLayout<ChessVariant::Standard        >(d);     // Standard Chess          
      case ChessVariant::Corner:            return new ChessVariantLayout<ChessVariant::Corner          >(d);     // Corner Chess            
      case ChessVariant::Fortress:          return new ChessVariantLayout<ChessVariant::Fortress        >(d);     // Fortress Chess          
      case ChessVariant::Horde:             return new ChessVariantLayout<ChessVariant::Horde           >(d);     // Horde Chess             
      case ChessVariant::Loop:              return new ChessVariantLayout<ChessVariant::Loop            >(d);     // Loop Chess              
      case ChessVariant::Anti:              return new ChessVariantLayout<ChessVariant::Anti            >(d);     // Anti Chess              
      case ChessVariant::Extinction:        return new ChessVariantLayout<ChessVariant::Extinction      >(d);     // Extinction Chess        
      case ChessVariant::Maharajah:         return new ChessVariantLayout<ChessVariant::Maharajah       >(d);     // Maharajah Chess         
      case ChessVariant::ThreeChecks:       return new ChessVariantLayout<ChessVariant::ThreeChecks     >(d);     // Three Checks Chess      
      case ChessVariant::Dark:              return new ChessVariantLayout<ChessVariant::Dark            >(d);     // Dark Chess              
      case ChessVariant::Atomic:            return new ChessVariantLayout<ChessVariant::Atomic          >(d);     // Atomic Chess            
      case ChessVariant::Janus:             return new ChessVariantLayout<ChessVariant::Janus           >(d);     // Janus Chess             
      case ChessVariant::Embassy:           return new ChessVariantLayout<ChessVariant::Embassy         >(d);     // Embassy Chess           
      case ChessVariant::Screen:            return new ChessVariantLayout<ChessVariant::Screen          >(d);     // Screen Chess            
      case ChessVariant::CrazyScreen:       return new ChessVariantLayout<ChessVariant::CrazyScreen     >(d);     // Crazy Screen Chess      
      case ChessVariant::Cylinder:          return new ChessVariantLayout<ChessVariant::Cylinder        >(d);     // Cylinder Chess          
      case ChessVariant::Amazons:           return new ChessVariantLayout<ChessVariant::Amazons         >(d);     // Amazon Chess            
      case ChessVariant::Berolina:          return new ChessVariantLayout<ChessVariant::Berolina        >(d);     // Berolina Chess          
      case ChessVariant::FischerRandom:     return new ChessVariantLayout<ChessVariant::FischerRandom   >(d);     // Fischer Random Chess    
      case ChessVariant::Legan:             return new ChessVariantLayout<ChessVariant::Legan           >(d);     // Legan Chess             
      case ChessVariant::KnightRelay:       return new ChessVariantLayout<ChessVariant::KnightRelay     >(d);     // Knight Relay Chess      
      case ChessVariant::Grand:             return new ChessVariantLayout<ChessVariant::Grand           >(d);     // Grand Chess             
      case ChessVariant::CapablancaRandom:  return new ChessVariantLayout<ChessVariant::CapablancaRandom>(d);     // Capablanca Random Chess 
      case ChessVariant::LosAlamos:         return new ChessVariantLayout<ChessVariant::LosAlamos       >(d);     // Los Alamos Chess        
      case ChessVariant::Ambiguous:         return new ChessVariantLayout<ChessVariant::Ambiguous       >(d);     // Ambiguous Chess         
      case ChessVariant::Cheversi:          return new ChessVariantLayout<ChessVariant::Cheversi        >(d);     // Cheversi
      case ChessVariant::Dice:              return new ChessVariantLayout<ChessVariant::Dice            >(d);     // Dice Chess              
      case ChessVariant::Recycle:           return new ChessVariantLayout<ChessVariant::Recycle         >(d);     // Recycle Chess           
      case ChessVariant::IceAge:            return new ChessVariantLayout<ChessVariant::IceAge          >(d);     // Ice Age Chess           
      case ChessVariant::Behemoth:          return new ChessVariantLayout<ChessVariant::Behemoth        >(d);     // Behemoth Chess          
      case ChessVariant::CheshireCat:       return new ChessVariantLayout<ChessVariant::CheshireCat     >(d);     // Cheshire Cat Chess      
      case ChessVariant::Knightmate:        return new ChessVariantLayout<ChessVariant::Knightmate      >(d);     // Knightmate Chess        
      case ChessVariant::RacingKings:       return new ChessVariantLayout<ChessVariant::RacingKings     >(d);     // Racing Kings            
      case ChessVariant::Dice10x10:         return new ChessVariantLayout<ChessVariant::Dice10x10       >(d);     // Dice Chess 10x10        
      case ChessVariant::Massacre:          return new ChessVariantLayout<ChessVariant::Massacre        >(d);     // Massacre Chess
      default: return nullptr; // must not happen
    }
  }

}
