#include "Games.h"

#include "ChessGame.h"

namespace Chess
{

  // templates to create a subclass for each variant

  template <ChessVariant V> struct ChessVariantData {};  // helper to allow to add data for one variant nly
  template <> struct ChessVariantData<ChessVariant::Dice>      { mutable Side side_; };  // rolled Die
  template <> struct ChessVariantData<ChessVariant::Dice10x10> { mutable Side side_; };  // rolled Die

  template <ChessVariant V>
  class ChessVariantPosition : public ChessPosition, public ChessVariantData<V>
  {
  public:
    inline ChessVariantPosition<V>(const VariantChosen& v, const PieceMapP& p, const Dimensions& d) noexcept : ChessPosition(v, p, d) {}
    virtual MainPosition* Clone(void) const noexcept override { return new ChessVariantPosition<V>(*this); }
    virtual void SetStartingPosition() noexcept override { ChessPosition::SetStartingPosition(); }
    virtual void GetAllMoves(void) const noexcept { ChessPosition::GetAllMoves(); }
    virtual bool AddIfLegal(Moves& m, const Location& fr, const Location& to) const noexcept override { return ChessPosition::AddIfLegal(m, fr, to); }
    constexpr virtual unsigned int GetMoveCountFactor(void) const noexcept override { return ChessPosition::GetMoveCountFactor(); }
    [[ nodiscard ]] virtual PositionValue EvaluateStatically(void) const noexcept override { return ChessPosition::EvaluateStatically(); }

  private:
    virtual Rule GetRule() const noexcept override { return Castling | AllowMoves | AllowTakes | PawnsPromote | PawnsDoubleStep | EnPassant; }
    static bool ValidPosition(const std::vector<Coordinate>& c) noexcept { return true; }
  };

  template <ChessVariant V>
  class ChessVariantLayout : public ChessLayout
  {
  public:
    ChessVariantLayout(const Dimensions& d, LayoutType lt = LayoutType::Alternating) noexcept : ChessLayout(d, lt) {}
    virtual ~ChessVariantLayout() noexcept {}
    virtual void Draw(DC* pDC, const MainPosition* pos, _Mode mode) const { ChessLayout::Draw(pDC, pos, mode); }
  };

  //#########################################
  // Specializations - those contain thh variant specific code
  // note that sequence DOES matter in defining method specializations!
  // each method specialization must be defined BEFORE it is used / called


  // specializations for ChessVariant::Corner
  template <> inline Rule ChessVariantPosition<ChessVariant::Corner>::GetRule() const noexcept { return AllowMoves | AllowTakes | PawnsPromote | PawnsDoubleStep | EnPassant; }
  template <> inline bool ChessVariantPosition<ChessVariant::Corner>::ValidPosition(const std::vector<Coordinate>& c) noexcept { return c[3] % 2 != c[4] % 2; } // check for bishops on differently colored fields
  template <> inline void ChessVariantPosition<ChessVariant::Corner>::SetStartingPosition() noexcept
  {
    SetPawns(1U, PieceColor::Black);
    SetPawns(sizeY_ - 2U, PieceColor::White);

    SetPiecesPSymmetrical(0U, 0U, ChessPiece::BK, ChessPiece::WK);

    std::vector<Coordinate> c;
    for (Coordinate i = 1; i < sizeX_; i++) c.push_back(i);
    do Math::Shuffle(c);
    while (!ValidPosition(c));  // keep shuffling until the position is valid

    SetPiecesPSymmetrical(c[0], 0U, ChessPiece::BQ, ChessPiece::WQ);
    SetPiecesPSymmetrical(c[1], 0U, ChessPiece::BR, ChessPiece::WR);
    SetPiecesPSymmetrical(c[2], 0U, ChessPiece::BR, ChessPiece::WR);
    SetPiecesPSymmetrical(c[3], 0U, ChessPiece::BB, ChessPiece::WB);
    SetPiecesPSymmetrical(c[4], 0U, ChessPiece::BB, ChessPiece::WB);
    SetPiecesPSymmetrical(c[5], 0U, ChessPiece::BN, ChessPiece::WN);
    SetPiecesPSymmetrical(c[6], 0U, ChessPiece::BN, ChessPiece::WN);
  }


  // specializations for ChessVariant::Fortress
  template <> inline Rule ChessVariantPosition<ChessVariant::Fortress>::GetRule() const noexcept { return AllowMoves | AllowTakes | PawnsPromote | PawnsDoubleStep | EnPassant; }
  template <> inline bool ChessVariantPosition<ChessVariant::Fortress>::ValidPosition(const std::vector<Coordinate>& c) noexcept { return c[3] % 2 != c[4] % 2; } // check for bishops on differently colored fields
  template <> inline void ChessVariantPosition<ChessVariant::Fortress>::SetStartingPosition() noexcept
  {
    SetPawns(1U, PieceColor::Black);
    SetPawns(sizeY_ - 2U, PieceColor::White);

    SetPiecesPSymmetrical(0U, 0U, ChessPiece::BK, ChessPiece::WK);

    std::vector<Coordinate> c;
    for (Coordinate i = 1; i < sizeX_; i++) c.push_back(i);
    do Math::Shuffle(c);
    while (!ValidPosition(c));  // keep shuffling until the position is valid

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
  template <> inline void ChessVariantPosition<ChessVariant::Horde>::SetStartingPosition() noexcept
  {
    ChessPosition::SetStartingPosition();
    SetPawns(0U, PieceColor::Black);
    SetPawns(2U, PieceColor::Black);
    SetPawns(3U, PieceColor::Black);
    SetPiece({ BoardPart::Main, 3U, 4U }, ChessPiece::BP);
    SetPiece({ BoardPart::Main, 4U, 4U }, ChessPiece::BP);
    SetPiece({ BoardPart::Main, 3U, 0U }, Piece::NoPiece);
    SetPiece({ BoardPart::Main, 4U, 0U }, Piece::NoPiece);
  }
  template <> inline PositionValue ChessVariantPosition<ChessVariant::Horde>::EvaluateStatically(void) const noexcept
  {
    // TODO: evaluate Horde positions
    return ChessPosition::EvaluateStatically(); // temporary
  }


  // specializations for ChessVariant::Loop
  template <> inline Rule ChessVariantPosition<ChessVariant::Loop>::GetRule() const noexcept { return Castling | AllowMoves | AllowTakes | PawnsPromote | PawnsDoubleStep | EnPassant | DropTakenPieces; }


  // specializations for ChessVariant::Anti
  template <> inline Rule ChessVariantPosition<ChessVariant::Anti>::GetRule() const noexcept { return AllowMoves | AllowTakes | MustTake | PawnsPromote | PawnsDoubleStep | EnPassant; }
  template <> inline PositionValue ChessVariantPosition<ChessVariant::Anti>::EvaluateStatically(void) const noexcept
  {
    // TODO: evaluate Anti positions
    return ChessPosition::EvaluateStatically(); // temporary
  }


  // specializations for ChessVariant::Extinction
  template <> inline PositionValue ChessVariantPosition<ChessVariant::Extinction>::EvaluateStatically(void) const noexcept
  {
    // TODO: evaluate Extinction positions
    return ChessPosition::EvaluateStatically(); // temporary
  }

  
  // specializations for ChessVariant::Maharajah
  template <> inline Rule ChessVariantPosition<ChessVariant::Maharajah>::GetRule() const noexcept { return Castling | AllowMoves | AllowTakes | PawnsDoubleStep; }
  template <> inline void ChessVariantPosition<ChessVariant::Maharajah>::SetStartingPosition() noexcept
  {
    ChessPosition::SetStartingPosition();
    for (Coordinate i = 0; i < sizeX_; i++)
    {
      SetPiece({ BoardPart::Main, i, sizeY_ - 2U }, Piece::NoPiece);
      SetPiece({ BoardPart::Main, i, sizeY_ - 1U }, Piece::NoPiece);
    }
    SetPiece({ BoardPart::Main, 4U, sizeY_ - 1U }, ChessPiece::WA);
  }


  // specializations for ChessVariant::ThreeChecks
  template <> inline PositionValue ChessVariantPosition<ChessVariant::ThreeChecks>::EvaluateStatically(void) const noexcept
  {
    // TODO: evaluate ThreeChecks positions
    return ChessPosition::EvaluateStatically(); // temporary
  }


  // specializations for ChessVariant::Dark
// TODO: Dark Chess

  // specializations for ChessVariant::Atomic
  template <> inline bool ChessVariantPosition<ChessVariant::Atomic>::AddIfLegal(Moves& m, const Location& fr, const Location& to) const noexcept
  {
    // TODO: AddifLegal Atomic chess
    return ChessPosition::AddIfLegal(m, fr, to);  // temporary
  }


  // specializations for ChessVariant::Janus
  template <> inline void ChessVariantPosition<ChessVariant::Janus>::SetStartingPosition() noexcept
  {
    ChessPosition::SetStartingPosition();
    SetPiecesHSymmetrical(1U, 0U, ChessPiece::BC, ChessPiece::WC);
    SetPiecesHSymmetrical(2U, 0U, ChessPiece::BN, ChessPiece::WN);
    SetPiecesHSymmetrical(3U, 0U, ChessPiece::BB, ChessPiece::WB);
    SetPiecesHSymmetrical(6U, 0U, ChessPiece::BB, ChessPiece::WB);
    SetPiecesHSymmetrical(7U, 0U, ChessPiece::BN, ChessPiece::WN);
    SetPiecesHSymmetrical(8U, 0U, ChessPiece::BC, ChessPiece::WC);
  }
  

  // specializations for ChessVariant::Embassy
  template <> inline void ChessVariantPosition<ChessVariant::Embassy>::SetStartingPosition() noexcept
  {
    ChessPosition::SetStartingPosition();
    SetPiecesHSymmetrical(3U, 0U, ChessPiece::BQ, ChessPiece::WQ);
    SetPiecesHSymmetrical(4U, 0U, ChessPiece::BK, ChessPiece::WK);
    SetPiecesHSymmetrical(5U, 0U, ChessPiece::BM, ChessPiece::WM);
    SetPiecesHSymmetrical(6U, 0U, ChessPiece::BC, ChessPiece::WC);
  }
  

  // specializations for ChessVariant::Screen
  template <> inline void ChessVariantPosition<ChessVariant::Screen>::SetStartingPosition() noexcept {} // start blank
  // TODO: Screen chess


  // specializations for ChessVariant::CrazyScreen
  template <> inline void ChessVariantPosition<ChessVariant::CrazyScreen>::SetStartingPosition() noexcept {} // start blank
  // TODO: CrazyScreen chess

  
  // specializations for ChessVariant::Cylinder
  template <> inline bool ChessVariantPosition<ChessVariant::Cylinder>::AddIfLegal(Moves& m, const Location& fr, const Location& to) const noexcept
  {
    // TODO: AddifLegal Cylinder chess
    return ChessPosition::AddIfLegal(m, fr, to);  // temporary
  }


  // specializations for ChessVariant::Amazons
  template <> inline void ChessVariantPosition<ChessVariant::Amazons>::SetStartingPosition() noexcept
  {
    ChessPosition::SetStartingPosition();
    SetPiecesHSymmetrical(3U, 0U, ChessPiece::BA, ChessPiece::WA);
  }


  // specializations for ChessVariant::Berolina
  template <> inline Rule ChessVariantPosition<ChessVariant::Berolina>::GetRule() const noexcept { return Castling | AllowMoves | AllowTakes | PawnsPromote | PawnsDoubleStep | EnPassant | BerolinaPawns; }


  // specializations for ChessVariant::FischerRandom
  template <> inline bool ChessVariantPosition<ChessVariant::FischerRandom>::ValidPosition(const std::vector<Coordinate>& c) noexcept
  {
    if (c[3] % 2 == c[4] % 2) return false;       // bishops not on differently colored fields
    if (c[7] > c[1] && c[7] < c[2]) return true;  // king is between the rooks
    if (c[7] > c[2] && c[7] < c[1]) return true;  // king is between the rooks
    return false;
  }
  template <> inline void ChessVariantPosition<ChessVariant::FischerRandom>::SetStartingPosition() noexcept
  {
    SetPawns(1U, PieceColor::Black);
    SetPawns(sizeY_ - 2U, PieceColor::White);

    std::vector<Coordinate> c;
    for (Coordinate i = 0; i < sizeX_; i++) c.push_back(i);
      
    do Math::Shuffle(c); while (!ValidPosition(c));   // keep shuffling until position is valid

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
  template <> inline Rule ChessVariantPosition<ChessVariant::Legan>::GetRule() const noexcept { return AllowMoves | AllowTakes | PawnsPromote | LeganPawns; }
  template <> inline void ChessVariantPosition<ChessVariant::Legan>::SetStartingPosition() noexcept
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
  template <> inline Rule ChessVariantPosition<ChessVariant::KnightRelay>::GetRule() const noexcept { return Castling | AllowMoves | AllowTakes | PawnsPromote | PawnsDoubleStep; }
 // TODO: add KnightRelay chess


  // specializations for ChessVariant::Grand
  template <> inline void ChessVariantPosition<ChessVariant::Grand>::SetStartingPosition() noexcept
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
  template <> inline bool ChessVariantPosition<ChessVariant::CapablancaRandom>::ValidPosition(const std::vector<Coordinate>& c) noexcept
  {
    if (c[3] % 2 == c[4] % 2) return false;       // bishops not on differently colored fields
    if (c[7] > c[1] && c[7] < c[2]) return true;  // king is between the rooks
    if (c[7] > c[2] && c[7] < c[1]) return true;  // king is between the rooks
    return false;
  }
  template <> inline void ChessVariantPosition<ChessVariant::CapablancaRandom>::SetStartingPosition() noexcept
  {
    SetPawns(1U, PieceColor::Black);
    SetPawns(sizeY_ - 2U, PieceColor::White);

    std::vector<Coordinate> c;
    for (Coordinate i = 0; i < sizeX_; i++) c.push_back(i);

    do Math::Shuffle(c); while (!ValidPosition(c));   // keep shuffling until position is valid

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
  template <> inline Rule ChessVariantPosition<ChessVariant::LosAlamos>::GetRule() const noexcept { return AllowMoves | AllowTakes | PawnsPromote; }


  // specializations for ChessVariant::Ambiguous
  template <> inline Rule ChessVariantPosition<ChessVariant::Ambiguous>::GetRule() const noexcept { return AllowMoves | AllowTakes | PawnsPromote | PawnsDoubleStep | EnPassant; }
  // TODO: add Ambiguous chess


  // specializations for ChessVariant::Cheversi
  template <> inline Rule ChessVariantPosition<ChessVariant::Cheversi>::GetRule() const noexcept { return None; }
  template <> inline void ChessVariantPosition<ChessVariant::Cheversi>::SetStartingPosition() noexcept {}  // start empty
  template <> inline void ChessVariantPosition<ChessVariant::Cheversi>::GetAllMoves(void) const noexcept // collect all moves for all pieces
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
  template <> inline void ChessVariantPosition<ChessVariant::Dice>::GetAllMoves(void) const noexcept // collect all moves for all pieces
  {
    ChessPosition::GetAllMoves(); // get all moves, and only then dabble around which numbers can be rolled

    static const std::array<const Kind*,6> Kinds{
      &Pawn::ThePawn,    
      &Knight::TheKnight,
      &Bishop::TheBishop,
      &Rook::TheRook,    
      &Queen::TheQueen,  
      &King::TheKing,    
    };

    if (movesW_.size() == 0) return;

    std::array<Moves, 6> moves_{};

    auto Find = [&](const Piece& p) -> int { for (int i = 0; i < 6; i++)  if (p.IsKind(*Kinds[i])) return i; return -1; };

    for (auto m : movesW_)
    {
      int z = Find(m->GetActions()[0]->GetPiece());
      assert(z >= 0);
      moves_[z].push_back(m);
    }

    int z{};
    do z = Math::D6() - 1; while (moves_[z].size() == 0);  // find a random Kind to move
    movesW_ = moves_[z];
    ChessVariantData::side_ = z;
  }
  template <> inline void ChessVariantLayout<ChessVariant::Dice>::Draw(DC* pDC, const MainPosition* pos, _Mode mode) const
  {
    ChessLayout::Draw(pDC, pos, mode);

    // show the rolled die
    constexpr int x = 500;
    constexpr int y = 200;
    Rect r{ x,y,x + 20,y + 20 };

    auto pp{ dynamic_cast<const ChessVariantPosition<ChessVariant::Dice>*>(pos) };

    Die::Sides[pp->ChessVariantData<ChessVariant::Dice>::side_].Draw(pDC, r);
  }

  // specializations for ChessVariant::Recycle
  template <> inline Rule ChessVariantPosition<ChessVariant::Recycle>::GetRule() const noexcept { return Castling | AllowMoves | AllowTakes | TakeOwn | PawnsPromote | PawnsDoubleStep | DropTakenPieces; }


  // specializations for ChessVariant::IceAge
  template <> inline void ChessVariantPosition<ChessVariant::IceAge>::SetStartingPosition() noexcept
  {
    ChessPosition::SetStartingPosition();
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
  template <> inline void ChessVariantPosition<ChessVariant::Behemoth>::SetStartingPosition() noexcept
  {
    ChessPosition::SetStartingPosition();
    SetPiece({ BoardPart::Main,3U, 4U }, ChessPiece::RB);
  }
  // TODO: add Behemoth post-move killing
  // note: killing is NOT known to AI / player


  // specializations for ChessVariant::CheshireCat
  template <> inline Rule ChessVariantPosition<ChessVariant::CheshireCat>::GetRule() const noexcept { return AllowMoves | AllowTakes | PawnsPromote | PawnsDoubleStep | EnPassant; }
 // TODO: add CheshireCat field removal
 // TODO: add CheshireCat King first move as Queen


  // specializations for ChessVariant::Knightmate
  template <> inline Rule ChessVariantPosition<ChessVariant::Knightmate>::GetRule() const noexcept { return Castling | AllowMoves | AllowTakes | PawnsPromote | PawnsDoubleStep | TakeKing; }
  template <> inline PositionValue ChessVariantPosition<ChessVariant::Knightmate>::EvaluateStatically(void) const noexcept
  {
    // TODO: evaluate Knightmate positions
    return ChessPosition::EvaluateStatically(); // temporary
  }


  // specializations for ChessVariant::RacingKings
  template <> inline Rule ChessVariantPosition<ChessVariant::RacingKings>::GetRule() const noexcept { return AllowMoves | AllowTakes;; }
  template <> inline void ChessVariantPosition<ChessVariant::RacingKings>::SetStartingPosition() noexcept
  {
    SetPiece({ BoardPart::Main,              0U,sizeY_ - 1U }, ChessPiece::BQ);
    SetPiece({ BoardPart::Main,              0U,sizeY_ - 2U }, ChessPiece::BK);
    SetPiece({ BoardPart::Main,              1U,sizeY_ - 1U }, ChessPiece::BR);
    SetPiece({ BoardPart::Main,              1U,sizeY_ - 2U }, ChessPiece::BR);
    SetPiece({ BoardPart::Main,              2U,sizeY_ - 1U }, ChessPiece::BB);
    SetPiece({ BoardPart::Main,              2U,sizeY_ - 2U }, ChessPiece::BB);
    SetPiece({ BoardPart::Main,              3U,sizeY_ - 1U }, ChessPiece::BN);
    SetPiece({ BoardPart::Main,              3U,sizeY_ - 2U }, ChessPiece::BN);

    SetPiece({ BoardPart::Main,sizeX_ - 1U - 0U,sizeY_ - 1U }, ChessPiece::WQ);
    SetPiece({ BoardPart::Main,sizeX_ - 1U - 0U,sizeY_ - 2U }, ChessPiece::WK);
    SetPiece({ BoardPart::Main,sizeX_ - 1U - 1U,sizeY_ - 1U }, ChessPiece::WR);
    SetPiece({ BoardPart::Main,sizeX_ - 1U - 1U,sizeY_ - 2U }, ChessPiece::WR);
    SetPiece({ BoardPart::Main,sizeX_ - 1U - 2U,sizeY_ - 1U }, ChessPiece::WB);
    SetPiece({ BoardPart::Main,sizeX_ - 1U - 2U,sizeY_ - 2U }, ChessPiece::WB);
    SetPiece({ BoardPart::Main,sizeX_ - 1U - 3U,sizeY_ - 1U }, ChessPiece::WN);
    SetPiece({ BoardPart::Main,sizeX_ - 1U - 3U,sizeY_ - 2U }, ChessPiece::WN);
  }
  template <> inline PositionValue ChessVariantPosition<ChessVariant::RacingKings>::EvaluateStatically(void) const noexcept
  {
    // TODO: evaluate RacingKings positions
    return ChessPosition::EvaluateStatically(); // temporary
  }

  
  // specializations for ChessVariant::Dice10x10
  template <> inline void ChessVariantPosition<ChessVariant::Dice10x10>::SetStartingPosition() noexcept
  {
    ChessPosition::SetStartingPosition();
    SetPiecesHSymmetrical(              3U, 0U, ChessPiece::BQ, ChessPiece::WQ);
    SetPiecesHSymmetrical(              4U, 0U, ChessPiece::BK, ChessPiece::WK);
    SetPiecesHSymmetrical(sizeX_ - 1U - 3U, 0U, ChessPiece::BK, ChessPiece::WK);
    SetPiecesHSymmetrical(sizeX_ - 1U - 4U, 0U, ChessPiece::BK, ChessPiece::WK);
  }
  template <> inline void ChessVariantPosition<ChessVariant::Dice10x10>::GetAllMoves(void) const noexcept // collect all moves for all pieces
  {
    // TODO: roll dice for Dice10x10
    // TODO: GetAllMoves for Dice10x10

    ChessPosition::GetAllMoves();  // temporary
  }


  // specializations for ChessVariant::Massacre
  template <> inline Rule ChessVariantPosition<ChessVariant::Massacre>::GetRule() const noexcept { return AllowTakes; }
  template <> inline constexpr virtual unsigned int ChessVariantPosition<ChessVariant::Massacre>::GetMoveCountFactor(void) const noexcept { return 1000; }
  template <> inline void ChessVariantPosition<ChessVariant::Massacre>::SetStartingPosition() noexcept
  {
    std::vector<Location> ll;
    for (Coordinate i = 0; i < sizeX_; i++)
      for (Coordinate j = 0; j < sizeY_; j++)
      {
        const Location l{ BoardPart::Main,  i,j };
        assert(GetPiece(l) == Piece::NoPiece);
        ll.push_back(l);
      }
    assert(ll.size() == sizeX_ * sizeY_);
    Math::Shuffle(ll);
    auto it = ll.begin();
    for (Coordinate z = 0; z < sizeX_ * sizeY_ / 8; z++)
    {
      SetPiece(*it++, ChessPiece::WQ);
      SetPiece(*it++, ChessPiece::BQ);
      SetPiece(*it++, ChessPiece::WR);
      SetPiece(*it++, ChessPiece::BR);
      SetPiece(*it++, ChessPiece::WB);
      SetPiece(*it++, ChessPiece::BB);
      SetPiece(*it++, ChessPiece::WN);
      SetPiece(*it++, ChessPiece::BN);
    }
    assert(it == ll.end());
  }
    

  const VariantList& ChessGame::GetVariants(void) noexcept
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


  MainPosition* ChessGame::GetNewPosition(const VariantChosen& v, const PieceMapP& m, const Dimensions& d) noexcept
  {
    // this switch looks silly, but cannot be avoided - the compiler has no idea which variants would be possible, 
    // but needs to generate the code for each one. An explicit list is needed in some form!
    switch (static_cast<ChessVariant>(v.c))
    {
      case ChessVariant::Standard:          return new ChessVariantPosition<ChessVariant::Standard        >(v, m, d);     // Standard Chess          
      case ChessVariant::Corner:            return new ChessVariantPosition<ChessVariant::Corner          >(v, m, d);     // Corner Chess            
      case ChessVariant::Fortress:          return new ChessVariantPosition<ChessVariant::Fortress        >(v, m, d);     // Fortress Chess          
      case ChessVariant::Horde:             return new ChessVariantPosition<ChessVariant::Horde           >(v, m, d);     // Horde Chess             
      case ChessVariant::Loop:              return new ChessVariantPosition<ChessVariant::Loop            >(v, m, d);     // Loop Chess              
      case ChessVariant::Anti:              return new ChessVariantPosition<ChessVariant::Anti            >(v, m, d);     // Anti Chess              
      case ChessVariant::Extinction:        return new ChessVariantPosition<ChessVariant::Extinction      >(v, m, d);     // Extinction Chess        
      case ChessVariant::Maharajah:         return new ChessVariantPosition<ChessVariant::Maharajah       >(v, m, d);     // Maharajah Chess         
      case ChessVariant::ThreeChecks:       return new ChessVariantPosition<ChessVariant::ThreeChecks     >(v, m, d);     // Three Checks Chess      
      case ChessVariant::Dark:              return new ChessVariantPosition<ChessVariant::Dark            >(v, m, d);     // Dark Chess              
      case ChessVariant::Atomic:            return new ChessVariantPosition<ChessVariant::Atomic          >(v, m, d);     // Atomic Chess            
      case ChessVariant::Janus:             return new ChessVariantPosition<ChessVariant::Janus           >(v, m, d);     // Janus Chess             
      case ChessVariant::Embassy:           return new ChessVariantPosition<ChessVariant::Embassy         >(v, m, d);     // Embassy Chess           
      case ChessVariant::Screen:            return new ChessVariantPosition<ChessVariant::Screen          >(v, m, d);     // Screen Chess            
      case ChessVariant::CrazyScreen:       return new ChessVariantPosition<ChessVariant::CrazyScreen     >(v, m, d);     // Crazy Screen Chess      
      case ChessVariant::Cylinder:          return new ChessVariantPosition<ChessVariant::Cylinder        >(v, m, d);     // Cylinder Chess          
      case ChessVariant::Amazons:           return new ChessVariantPosition<ChessVariant::Amazons         >(v, m, d);     // Amazon Chess            
      case ChessVariant::Berolina:          return new ChessVariantPosition<ChessVariant::Berolina        >(v, m, d);     // Berolina Chess          
      case ChessVariant::FischerRandom:     return new ChessVariantPosition<ChessVariant::FischerRandom   >(v, m, d);     // Fischer Random Chess    
      case ChessVariant::Legan:             return new ChessVariantPosition<ChessVariant::Legan           >(v, m, d);     // Legan Chess             
      case ChessVariant::KnightRelay:       return new ChessVariantPosition<ChessVariant::KnightRelay     >(v, m, d);     // Knight Relay Chess      
      case ChessVariant::Grand:             return new ChessVariantPosition<ChessVariant::Grand           >(v, m, d);     // Grand Chess             
      case ChessVariant::CapablancaRandom:  return new ChessVariantPosition<ChessVariant::CapablancaRandom>(v, m, d);     // Capablanca Random Chess 
      case ChessVariant::LosAlamos:         return new ChessVariantPosition<ChessVariant::LosAlamos       >(v, m, d);     // Los Alamos Chess        
      case ChessVariant::Ambiguous:         return new ChessVariantPosition<ChessVariant::Ambiguous       >(v, m, d);     // Ambiguous Chess         
      case ChessVariant::Cheversi:          return new ChessVariantPosition<ChessVariant::Cheversi        >(v, m, d);     // Cheversi
      case ChessVariant::Dice:              return new ChessVariantPosition<ChessVariant::Dice            >(v, m, d);     // Dice Chess              
      case ChessVariant::Recycle:           return new ChessVariantPosition<ChessVariant::Recycle         >(v, m, d);     // Recycle Chess           
      case ChessVariant::IceAge:            return new ChessVariantPosition<ChessVariant::IceAge          >(v, m, d);     // Ice Age Chess           
      case ChessVariant::Behemoth:          return new ChessVariantPosition<ChessVariant::Behemoth        >(v, m, d);     // Behemoth Chess          
      case ChessVariant::CheshireCat:       return new ChessVariantPosition<ChessVariant::CheshireCat     >(v, m, d);     // Cheshire Cat Chess      
      case ChessVariant::Knightmate:        return new ChessVariantPosition<ChessVariant::Knightmate      >(v, m, d);     // Knightmate Chess        
      case ChessVariant::RacingKings:       return new ChessVariantPosition<ChessVariant::RacingKings     >(v, m, d);     // Racing Kings            
      case ChessVariant::Dice10x10:         return new ChessVariantPosition<ChessVariant::Dice10x10       >(v, m, d);     // Dice Chess 10x10        
      case ChessVariant::Massacre:          return new ChessVariantPosition<ChessVariant::Massacre        >(v, m, d);     // Massacre Chess
      default: return nullptr; // must not happen
    }
  }


  MainLayout* ChessGame::GetNewLayout(const VariantChosen& v, const Dimensions& d) noexcept
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
