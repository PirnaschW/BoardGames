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
    ChessVariantBoard<V>(const VariantChosen& v, const BoardPartDimensions& d) noexcept : ChessBoard(v, d) { SetDefaultStock(); }
    virtual Board* Clone() const noexcept override { return new ChessVariantBoard<V>(*this); }
    virtual void SetStartingBoard() noexcept override { ChessBoard::SetStartingBoard(); }
    virtual void GetAllMoves() const noexcept { ChessBoard::GetAllMoves(); }
    virtual bool AddIfLegal(Moves& m, const Location& fr, const Location& to) const noexcept override { return ChessBoard::AddIfLegal(m, fr, to); }
    constexpr virtual int GetMoveCountFactor() const noexcept override { return ChessBoard::GetMoveCountFactor(); }
    virtual void EvaluateStatically() const noexcept override { return ChessBoard::EvaluateStatically(); }

  private:
    Rule GetRule() const noexcept { return Castling | AllowMoves | AllowTakes | PawnsPromote | PawnsDoubleStep | EnPassant; }
    static bool ValidBoard(const std::vector<Coordinate>& c) noexcept { return true; }
  };


  //template <ChessVariant V>
  //class ChessVariantLayout : public ChessLayout
  //{
  //public:
  //  ChessVariantLayout(const BoardPartDimensions& d, LayoutType lt = LayoutType::Alternating) noexcept : ChessLayout(d, lt) {}
  //  virtual ~ChessVariantLayout() noexcept {}
  //  virtual void Draw(DC* pDC, const Board* board_, Mode mode_) const { ChessLayout::Draw(pDC, board_, mode_); }
  //};

  //#########################################
  // Specializations - those contain the variant specific code
  // note that sequence DOES matter in defining method specializations!
  // each method specialization must be defined BEFORE it is used / called


  //#######################################################
  // specializations for ChessVariant::Corner
  template <> inline Rule ChessVariantBoard<ChessVariant::Corner>::GetRule() const noexcept { return AllowMoves | AllowTakes | PawnsPromote | PawnsDoubleStep | EnPassant; }
  template <> inline bool ChessVariantBoard<ChessVariant::Corner>::ValidBoard(const std::vector<Coordinate>& c) noexcept { return c[3] % 2 != c[4] % 2; } // check for bishops on differently colored fields
  template <> inline void ChessVariantBoard<ChessVariant::Corner>::SetStartingBoard() noexcept
  {
    SetPawns(1U, PieceColor::Black);
    SetPawns(stage_.GetSizeY() - 2U, PieceColor::White);

    SetPiecesPSymmetrical(0U, 0U, ChessPiece::BK, ChessPiece::WK);

    std::vector<Coordinate> c;
    for (Coordinate i = 1; i < stage_.GetSizeX(); i++) c.push_back(i);
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


  //#######################################################
  // specializations for ChessVariant::Fortress
  template <> inline Rule ChessVariantBoard<ChessVariant::Fortress>::GetRule() const noexcept { return AllowMoves | AllowTakes | PawnsPromote | PawnsDoubleStep | EnPassant; }
  template <> inline bool ChessVariantBoard<ChessVariant::Fortress>::ValidBoard(const std::vector<Coordinate>& c) noexcept { return c[3] % 2 != c[4] % 2; } // check for bishops on differently colored fields
  template <> inline void ChessVariantBoard<ChessVariant::Fortress>::SetStartingBoard() noexcept
  {
    SetPawns(1U, PieceColor::Black);
    SetPawns(stage_.GetSizeY() - 2U, PieceColor::White);

    SetPiecesPSymmetrical(0U, 0U, ChessPiece::BK, ChessPiece::WK);

    std::vector<Coordinate> c;
    for (Coordinate i = 1; i < stage_.GetSizeX(); i++) c.push_back(i);
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


  //#######################################################
  // specializations for ChessVariant::Horde
  template <> inline void ChessVariantBoard<ChessVariant::Horde>::SetStartingBoard() noexcept
  {
    ChessBoard::SetStartingBoard();
    SetPawns(0U, PieceColor::Black);
    SetPawns(2U, PieceColor::Black);
    SetPawns(3U, PieceColor::Black);
    stage_.SetPieceIndex(PMap[ChessPiece::BP], 3U, 4U);
    stage_.SetPieceIndex(PMap[ChessPiece::BP], 4U, 4U);
    stage_.SetPieceIndex(PMap[Piece::NoPiece], 3U, 0U);
    stage_.SetPieceIndex(PMap[Piece::NoPiece], 4U, 0U);
  }
  template <> inline void ChessVariantBoard<ChessVariant::Horde>::EvaluateStatically() const noexcept
  {
    // TODO: evaluate Horde positions
    return ChessBoard::EvaluateStatically(); // temporary
  }


   //#######################################################
 // specializations for ChessVariant::Loop
  template <> inline Rule ChessVariantBoard<ChessVariant::Loop>::GetRule() const noexcept { return Castling | AllowMoves | AllowTakes | PawnsPromote | PawnsDoubleStep | EnPassant | DropTakenPieces; }


  //#######################################################
  // specializations for ChessVariant::Anti
  template <> inline Rule ChessVariantBoard<ChessVariant::Anti>::GetRule() const noexcept { return AllowMoves | AllowTakes | MustTake | PawnsPromote | PawnsDoubleStep | EnPassant; }
  template <> inline void ChessVariantBoard<ChessVariant::Anti>::EvaluateStatically() const noexcept
  {
    // TODO: evaluate Anti positions
    return ChessBoard::EvaluateStatically(); // temporary
  }


  //#######################################################
  // specializations for ChessVariant::Extinction
  template <> inline void ChessVariantBoard<ChessVariant::Extinction>::EvaluateStatically() const noexcept
  {
    // TODO: evaluate Extinction positions
    return ChessBoard::EvaluateStatically(); // temporary
  }

  
  //#######################################################
  // specializations for ChessVariant::Maharajah
  template <> inline ChessVariantBoard<ChessVariant::Maharajah>::ChessVariantBoard(const VariantChosen& v, const BoardPartDimensions& d) noexcept : ChessBoard(v, d)
  {
    // fill Stock
    int i = 0;
    auto append = [this, &i](const Piece& p) { stock_.SetPieceIndex(PMap[p], i / 2U, i % 2U); ++i; };
    append(ChessPiece::BP);  // Black Pawn
    append(ChessPiece::BN);  // Black Knight
    append(ChessPiece::BB);  // Black Bishop
    append(ChessPiece::BR);  // Black Rook
    append(ChessPiece::BQ);  // Black Queen
    append(ChessPiece::BK);  // Black King
    append(ChessPiece::WA);  // White Amazon   = Queen + Knight (also Maharajah)
  }
  template <> inline Rule ChessVariantBoard<ChessVariant::Maharajah>::GetRule() const noexcept { return Castling | AllowMoves | AllowTakes | PawnsDoubleStep; }
  template <> inline void ChessVariantBoard<ChessVariant::Maharajah>::SetStartingBoard() noexcept
  {
    const Coordinate sy = stage_.GetSizeY();
    ChessBoard::SetStartingBoard();
    for (Coordinate i = 0; i < stage_.GetSizeX(); i++)
    {
      stage_.SetPieceIndex(PMap[Piece::NoPiece], i, sy - 2U);
      stage_.SetPieceIndex(PMap[Piece::NoPiece], i, sy - 1U);
    }
    stage_.SetPieceIndex(PMap[ChessPiece::WA], 4U, sy - 1U);
  }


  //#######################################################
  // specializations for ChessVariant::ThreeChecks
  template <> inline void ChessVariantBoard<ChessVariant::ThreeChecks>::EvaluateStatically() const noexcept
  {
    // TODO: evaluate ThreeChecks positions
    return ChessBoard::EvaluateStatically(); // temporary
  }


  //#######################################################
  // specializations for ChessVariant::Dark
// TODO: Dark Chess

  //#######################################################
  // specializations for ChessVariant::Atomic
  template <> inline bool ChessVariantBoard<ChessVariant::Atomic>::AddIfLegal(Moves& m, const Location& fr, const Location& to) const noexcept
  {
    // TODO: AddifLegal Atomic chess
    return ChessBoard::AddIfLegal(m, fr, to);  // temporary
  }


  //#######################################################
  // specializations for ChessVariant::Janus
  template <> inline ChessVariantBoard<ChessVariant::Janus>::ChessVariantBoard(const VariantChosen& v, const BoardPartDimensions& d) noexcept : ChessBoard(v, d)
  {
    SetDefaultStock();
    int i = 8;  // keep first 8, start replacing there
    auto append = [this, &i](const Piece& p) { stock_.SetPieceIndex(PMap[p], i / 2U, i % 2U); ++i; };
    append(ChessPiece::WC);  // White Cardinal = Bishop + Knight (also: Archbishop, Princess, Janus)
    append(ChessPiece::BC);  // Black Cardinal = Bishop + Knight (also: Archbishop, Princess, Janus)
    append(ChessPiece::WQ);  // White Queen
    append(ChessPiece::BQ);  // Black Queen
    append(ChessPiece::WK);  // White King
    append(ChessPiece::BK);  // Black King
  }
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
  

  //#######################################################
  // specializations for ChessVariant::Embassy
  template <> inline ChessVariantBoard<ChessVariant::Embassy>::ChessVariantBoard(const VariantChosen& v, const BoardPartDimensions& d) noexcept : ChessBoard(v, d)
  {
    SetDefaultStock();
    int i = 8;  // keep first 8, start replacing there
    auto append = [this, &i](const Piece& p) { stock_.SetPieceIndex(PMap[p], i / 2U, i % 2U); ++i; };
    append(ChessPiece::WC);  // White Cardinal = Bishop + Knight (also: Archbishop, Princess, Janus)
    append(ChessPiece::BC);  // Black Cardinal = Bishop + Knight (also: Archbishop, Princess, Janus)
    append(ChessPiece::WM);  // White Marshall = Rook + Knight
    append(ChessPiece::BM);  // Black Marshall = Rook + Knight
    append(ChessPiece::WQ);  // White Queen
    append(ChessPiece::BQ);  // Black Queen
    append(ChessPiece::WK);  // White King
    append(ChessPiece::BK);  // Black King
  }
  template <> inline void ChessVariantBoard<ChessVariant::Embassy>::SetStartingBoard() noexcept
  {
    ChessBoard::SetStartingBoard();
    SetPiecesHSymmetrical(3U, 0U, ChessPiece::BQ, ChessPiece::WQ);
    SetPiecesHSymmetrical(4U, 0U, ChessPiece::BK, ChessPiece::WK);
    SetPiecesHSymmetrical(5U, 0U, ChessPiece::BM, ChessPiece::WM);
    SetPiecesHSymmetrical(6U, 0U, ChessPiece::BC, ChessPiece::WC);
  }
  

  //#######################################################
  // specializations for ChessVariant::Screen
  template <> inline void ChessVariantBoard<ChessVariant::Screen>::SetStartingBoard() noexcept {} // start blank
  // TODO: Screen chess


  //#######################################################
  // specializations for ChessVariant::CrazyScreen
  template <> inline void ChessVariantBoard<ChessVariant::CrazyScreen>::SetStartingBoard() noexcept {} // start blank
  // TODO: CrazyScreen chess

  
  //#######################################################
  // specializations for ChessVariant::Cylinder
  template <> inline bool ChessVariantBoard<ChessVariant::Cylinder>::AddIfLegal(Moves& m, const Location& fr, const Location& to) const noexcept
  {
    // TODO: AddifLegal Cylinder chess
    return ChessBoard::AddIfLegal(m, fr, to);  // temporary
  }


  //#######################################################
  // specializations for ChessVariant::Amazons
  template <> inline ChessVariantBoard<ChessVariant::Amazons>::ChessVariantBoard(const VariantChosen& v, const BoardPartDimensions& d) noexcept : ChessBoard(v, d)
  {
    SetDefaultStock();
    int i = 8;  // keep first 8, start replacing there
    auto append = [this, &i](const Piece& p) { stock_.SetPieceIndex(PMap[p], i / 2U, i % 2U); ++i; };
    append(ChessPiece::WA);  // White Amazon   = Queen + Knight (also: Maharajah)
    append(ChessPiece::BA);  // Black Amazon   = Queen + Knight (also: Maharajah)
  }
  template <> inline void ChessVariantBoard<ChessVariant::Amazons>::SetStartingBoard() noexcept
  {
    ChessBoard::SetStartingBoard();
    SetPiecesHSymmetrical(3U, 0U, ChessPiece::BA, ChessPiece::WA);
  }


  //#######################################################
  // specializations for ChessVariant::Berolina
  template <> inline Rule ChessVariantBoard<ChessVariant::Berolina>::GetRule() const noexcept { return Castling | AllowMoves | AllowTakes | PawnsPromote | PawnsDoubleStep | EnPassant | BerolinaPawns; }


  //#######################################################
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
    const Coordinate sx = stage_.GetSizeX();
    const Coordinate sy = stage_.GetSizeY();

    SetPawns(1U, PieceColor::Black);
    SetPawns(sy - 2U, PieceColor::White);

    std::vector<Coordinate> c;
    for (Coordinate i = 0; i < sx; i++) c.push_back(i);
      
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
  

  //#######################################################
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


  //#######################################################
  // specializations for ChessVariant::KnightRelay
  template <> inline Rule ChessVariantBoard<ChessVariant::KnightRelay>::GetRule() const noexcept { return Castling | AllowMoves | AllowTakes | PawnsPromote | PawnsDoubleStep; }
 // TODO: add KnightRelay chess


  //#######################################################
  // specializations for ChessVariant::Grand
  template <> inline ChessVariantBoard<ChessVariant::Grand>::ChessVariantBoard(const VariantChosen& v, const BoardPartDimensions& d) noexcept : ChessBoard(v, d)
  {
    SetDefaultStock();
    int i = 8;  // keep first 8, start replacing there
    auto append = [this, &i](const Piece& p) { stock_.SetPieceIndex(PMap[p], i / 2U, i % 2U); ++i; };
    append(ChessPiece::WC);  // White Cardinal = Bishop + Knight (also: Archbishop, Princess, Janus)
    append(ChessPiece::BC);  // Black Cardinal = Bishop + Knight (also: Archbishop, Princess, Janus)
    append(ChessPiece::WM);  // White Marshall = Rook + Knight
    append(ChessPiece::BM);  // Black Marshall = Rook + Knight
    append(ChessPiece::WQ);  // White Queen
    append(ChessPiece::BQ);  // Black Queen
    append(ChessPiece::WK);  // White King
    append(ChessPiece::BK);  // Black King
  }
  template <> inline void ChessVariantBoard<ChessVariant::Grand>::SetStartingBoard() noexcept
  {
    const Coordinate sy = stage_.GetSizeY();

    SetPawns(2U, PieceColor::Black);
    SetPawns(sy - 3U, PieceColor::White);

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


  //#######################################################
  // specializations for ChessVariant::CapablancaRandom
  template <> inline ChessVariantBoard<ChessVariant::CapablancaRandom>::ChessVariantBoard(const VariantChosen& v, const BoardPartDimensions& d) noexcept : ChessBoard(v, d)
  {
    SetDefaultStock();
    int i = 8;  // keep first 8, start replacing there
    auto append = [this, &i](const Piece& p) { stock_.SetPieceIndex(PMap[p], i / 2U, i % 2U); ++i; };
    append(ChessPiece::WC);  // White Cardinal = Bishop + Knight (also: Archbishop, Princess, Janus)
    append(ChessPiece::BC);  // Black Cardinal = Bishop + Knight (also: Archbishop, Princess, Janus)
    append(ChessPiece::WM);  // White Marshall = Rook + Knight
    append(ChessPiece::BM);  // Black Marshall = Rook + Knight
    append(ChessPiece::WQ);  // White Queen
    append(ChessPiece::BQ);  // Black Queen
    append(ChessPiece::WK);  // White King
    append(ChessPiece::BK);  // Black King
  }
  template <> inline bool ChessVariantBoard<ChessVariant::CapablancaRandom>::ValidBoard(const std::vector<Coordinate>& c) noexcept
  {
    if (c[3] % 2 == c[4] % 2) return false;       // bishops not on differently colored fields
    if (c[7] > c[1] && c[7] < c[2]) return true;  // king is between the rooks
    if (c[7] > c[2] && c[7] < c[1]) return true;  // king is between the rooks
    return false;
  }
  template <> inline void ChessVariantBoard<ChessVariant::CapablancaRandom>::SetStartingBoard() noexcept
  {
    const Coordinate sx = stage_.GetSizeX();
    const Coordinate sy = stage_.GetSizeY();
    SetPawns(1U, PieceColor::Black);
    SetPawns(sy - 2U, PieceColor::White);

    std::vector<Coordinate> c;
    for (Coordinate i = 0; i < sx; i++) c.push_back(i);

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


  //#######################################################
  // specializations for ChessVariant::LosAlamos
  template <> inline ChessVariantBoard<ChessVariant::LosAlamos>::ChessVariantBoard(const VariantChosen& v, const BoardPartDimensions& d) noexcept : ChessBoard(v, d)
  {
    int i = 0;  // keep first 8, start replacing there
    auto append = [this, &i](const Piece& p) { stock_.SetPieceIndex(PMap[p], i / 2U, i % 2U); ++i; };
    append(ChessPiece::WP);  // White Pawn
    append(ChessPiece::BP);  // Black Pawn
    append(ChessPiece::WN);  // White Knight
    append(ChessPiece::BN);  // Black Knight
    append(ChessPiece::WR);  // White Rook 
    append(ChessPiece::BR);  // Black Rook
    append(ChessPiece::WQ);  // White Queen
    append(ChessPiece::BQ);  // Black Queen
    append(ChessPiece::WK);  // White King 
    append(ChessPiece::BK);  // Black King
  }
  template <> inline Rule ChessVariantBoard<ChessVariant::LosAlamos>::GetRule() const noexcept { return AllowMoves | AllowTakes | PawnsPromote; }


  //#######################################################
  // specializations for ChessVariant::Ambiguous
  template <> inline Rule ChessVariantBoard<ChessVariant::Ambiguous>::GetRule() const noexcept { return AllowMoves | AllowTakes | PawnsPromote | PawnsDoubleStep | EnPassant; }
  // TODO: add Ambiguous chess


  //#######################################################
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
  

  //#######################################################
  // specializations for ChessVariant::Dice
  template <> inline void ChessVariantBoard<ChessVariant::Dice>::GetAllMoves() const noexcept // collect all moves for all pieces
  {
    ChessBoard::GetAllMoves(); // get all moves, and only then dabble around which numbers can be rolled
    ChessVariantData<ChessVariant::Dice>::side_ = PickRandomPiece();
    // TODO: Move list for blacks crashes
  }
  //template <> inline void ChessVariantLayout<ChessVariant::Dice>::Draw(DC* pDC, const Board* board_, Mode mode_) const
  //{
  //  ChessLayout::Draw(pDC, board_, mode_);

  //  // show the rolled die
  //  constexpr int x = 500;
  //  constexpr int y = 200;
  //  Rect r{ x,y,x + 20,y + 20 };

  //  auto pp{ down_cast<const ChessVariantBoard<ChessVariant::Dice>*>(board_) };

  //  Die::Sides[pp->ChessVariantData<ChessVariant::Dice>::side_].Draw(pDC, r);
  //}


  //#######################################################
  // specializations for ChessVariant::Recycle
  template <> inline Rule ChessVariantBoard<ChessVariant::Recycle>::GetRule() const noexcept { return Castling | AllowMoves | AllowTakes | TakeOwn | PawnsPromote | PawnsDoubleStep | DropTakenPieces; }


  //#######################################################
  // specializations for ChessVariant::IceAge
  template <> inline ChessVariantBoard<ChessVariant::IceAge>::ChessVariantBoard(const VariantChosen& v, const BoardPartDimensions& d) noexcept : ChessBoard(v, d)
  {
    SetDefaultStock();
    int i = 12;  // keep first 8, start replacing there
    auto append = [this, &i](const Piece& p) { stock_.SetPieceIndex(PMap[p], i / 2U, i % 2U); ++i; };
    append(ChessPiece::GI);  // (Gray) Ice
  }
  template <> inline void ChessVariantBoard<ChessVariant::IceAge>::SetStartingBoard() noexcept
  {
    const Coordinate sx = stage_.GetSizeX();
    const Coordinate sy = stage_.GetSizeY();
    ChessBoard::SetStartingBoard();
    for (Coordinate i = 0U; i < sx; i++)
    {
      for (Coordinate j = 2U; j < sy/2; j++)
      {
        SetPiecesHSymmetrical(i, j, ChessPiece::GI, ChessPiece::GI);
      }
    }
  }
  // TODO: add IceAge post-move freezing
  // note: freezing is known to AI / player


  //#######################################################
  // specializations for ChessVariant::Behemoth
  template <> inline ChessVariantBoard<ChessVariant::Behemoth>::ChessVariantBoard(const VariantChosen& v, const BoardPartDimensions& d) noexcept : ChessBoard(v, d)
  {
    SetDefaultStock();
    int i = 12;  // keep first 8, start replacing there
    auto append = [this, &i](const Piece& p) { stock_.SetPieceIndex(PMap[p], i / 2U, i % 2U); ++i; };
    append(ChessPiece::RB);  // (Red) Behemoth
  }
  template <> inline void ChessVariantBoard<ChessVariant::Behemoth>::SetStartingBoard() noexcept
  {
    ChessBoard::SetStartingBoard();
    stage_.SetPieceIndex(PMap[ChessPiece::RB], 3U, 4U);
  }
  // TODO: add Behemoth post-move killing
  // note: killing is NOT known to AI / player


  //#######################################################
  // specializations for ChessVariant::CheshireCat
  template <> inline Rule ChessVariantBoard<ChessVariant::CheshireCat>::GetRule() const noexcept { return AllowMoves | AllowTakes | PawnsPromote | PawnsDoubleStep | EnPassant; }
  template <> inline bool ChessVariantBoard<ChessVariant::CheshireCat>::AddIfLegal(Moves& m, const Location& fr, const Location& to) const noexcept
  {
    const PieceIndex pfI = GetPieceIndex(fr.x_,fr.y_,fr.b_);
    assert(pfI != PMap[Piece::NoTile]);                                             // start field must exist
    assert(pfI != PMap[Piece::NoPiece]);                                            // start field must have a piece

    const PieceIndex ptI = GetPieceIndex(to.x_,to.y_,to.b_);
    if (ptI == PMap[Piece::NoTile]) return false;                                   // out of board
    if (ptI == PMap[Piece::NoPiece])                                                // moving onto free field
    {
      if (HasRule(AllowMoves))
      {
        Actions a{};
        a.push_back(std::make_shared<ActionLift>(fr, pfI));                         // pick piece up
        a.push_back(std::make_shared<ActionEliminate>(fr, PMap[Piece::NoTile]));    // remove starting field
        a.push_back(std::make_shared<ActionDrop>(to, pfI));                         // and place it on target
        m.push_back(std::make_shared<Move>(a));                                     // add move to move list
      }
      return true;                                                                  // keep trying this direction
    }

    if (HasRule(AllowTakes))
    {
      if (PMap[ptI].IsColor(PMap[pfI]) && !HasRule(TakeOwn)) return false;          // own piece; don't keep trying this direction

      const Location lt{ BoardPartID::Taken,0,0 }; // first field in Taken - the real placement will be found when executed
      Actions a{};
      a.push_back(std::make_shared<ActionLift>(fr, pfI));                           // pick piece up
      a.push_back(std::make_shared<ActionEliminate>(fr, PMap[Piece::NoTile]));      // remove starting field
      a.push_back(std::make_shared<ActionLift>(to, ptI));                           // pick opponent piece up
      a.push_back(std::make_shared<ActionDrop>(lt, ptI));                           // place it in Taken
      a.push_back(std::make_shared<ActionDrop>(to, pfI));                           // and place it on target
      m.push_back(std::make_shared<Move>(a));                                       // add move to move list
    }
    return false;                                                                   // don't keep trying this direction
  }


 // TODO: add CheshireCat King first move as Queen

  //#######################################################
  // specializations for ChessVariant::Knightmate
  template <> inline Rule ChessVariantBoard<ChessVariant::Knightmate>::GetRule() const noexcept { return Castling | AllowMoves | AllowTakes | PawnsPromote | PawnsDoubleStep | TakeKing; }
  template <> inline void ChessVariantBoard<ChessVariant::Knightmate>::EvaluateStatically() const noexcept
  {
    // TODO: evaluate Knightmate positions
    return ChessBoard::EvaluateStatically(); // temporary
  }


  //#######################################################
  // specializations for ChessVariant::RacingKings
  template <> inline Rule ChessVariantBoard<ChessVariant::RacingKings>::GetRule() const noexcept { return AllowMoves | AllowTakes;; }
  template <> inline void ChessVariantBoard<ChessVariant::RacingKings>::SetStartingBoard() noexcept
  {
    const Coordinate sx = stage_.GetSizeX();
    const Coordinate sy = stage_.GetSizeY();

    stage_.SetPieceIndex(PMap[ChessPiece::BQ], 0U, sy - 1U);
    stage_.SetPieceIndex(PMap[ChessPiece::BK], 0U, sy - 2U);
    stage_.SetPieceIndex(PMap[ChessPiece::BR], 1U, sy - 1U);
    stage_.SetPieceIndex(PMap[ChessPiece::BR], 1U, sy - 2U);
    stage_.SetPieceIndex(PMap[ChessPiece::BB], 2U, sy - 1U);
    stage_.SetPieceIndex(PMap[ChessPiece::BB], 2U, sy - 2U);
    stage_.SetPieceIndex(PMap[ChessPiece::BN], 3U, sy - 1U);
    stage_.SetPieceIndex(PMap[ChessPiece::BN], 3U, sy - 2U);
    stage_.SetPieceIndex(PMap[ChessPiece::WQ], sx - 1U - 0U, sy - 1U);
    stage_.SetPieceIndex(PMap[ChessPiece::WK], sx - 1U - 0U, sy - 2U);
    stage_.SetPieceIndex(PMap[ChessPiece::WR], sx - 1U - 1U, sy - 1U);
    stage_.SetPieceIndex(PMap[ChessPiece::WR], sx - 1U - 1U, sy - 2U);
    stage_.SetPieceIndex(PMap[ChessPiece::WB], sx - 1U - 2U, sy - 1U);
    stage_.SetPieceIndex(PMap[ChessPiece::WB], sx - 1U - 2U, sy - 2U);
    stage_.SetPieceIndex(PMap[ChessPiece::WN], sx - 1U - 3U, sy - 1U);
    stage_.SetPieceIndex(PMap[ChessPiece::WN], sx - 1U - 3U, sy - 2U);
  }
  template <> inline void ChessVariantBoard<ChessVariant::RacingKings>::EvaluateStatically() const noexcept
  {
    // TODO: evaluate RacingKings positions
    return ChessBoard::EvaluateStatically(); // temporary
  }

  
  //#######################################################
  // specializations for ChessVariant::Dice10x10
  template <> inline void ChessVariantBoard<ChessVariant::Dice10x10>::SetStartingBoard() noexcept
  {
    const Coordinate sx = stage_.GetSizeX();
    ChessBoard::SetStartingBoard();
    SetPiecesHSymmetrical(          3U, 0U, ChessPiece::BQ, ChessPiece::WQ);
    SetPiecesHSymmetrical(          4U, 0U, ChessPiece::BK, ChessPiece::WK);
    SetPiecesHSymmetrical(sx - 1U - 3U, 0U, ChessPiece::BK, ChessPiece::WK);
    SetPiecesHSymmetrical(sx - 1U - 4U, 0U, ChessPiece::BK, ChessPiece::WK);
  }
  template <> inline void ChessVariantBoard<ChessVariant::Dice10x10>::GetAllMoves() const noexcept // collect all moves for all pieces
  {
    ChessBoard::GetAllMoves(); // get all moves, and only then dabble around which numbers can be rolled
    ChessVariantData<ChessVariant::Dice10x10>::side_ = PickRandomPiece();
  }
  //template <> inline void ChessVariantLayout<ChessVariant::Dice10x10>::Draw(DC* pDC, const Board* board_, Mode mode_) const
  //{
  //  ChessLayout::Draw(pDC, board_, mode_);

  //  // show the rolled die
  //  constexpr int x = 600;
  //  constexpr int y = 200;
  //  Rect r{ x,y,x + 20,y + 20 };

  //  auto pp{ down_cast<const ChessVariantBoard<ChessVariant::Dice10x10>*>(board_) };

  //  Die::Sides[pp->ChessVariantData<ChessVariant::Dice10x10>::side_].Draw(pDC, r);
  //}

  //#######################################################
  // specializations for ChessVariant::Massacre
  template <> inline ChessVariantBoard<ChessVariant::Massacre>::ChessVariantBoard(const VariantChosen& v, const BoardPartDimensions& d) noexcept : ChessBoard(v, d)
  {
    int i = 0;  // keep first 8, start replacing there
    auto append = [this, &i](const Piece& p) { stock_.SetPieceIndex(PMap[p], i / 2U, i % 2U); ++i; };
    append(ChessPiece::WN);  // White Knight
    append(ChessPiece::BN);  // Black Knight
    append(ChessPiece::WB);  // White Bishop
    append(ChessPiece::BB);  // Black Bishop
    append(ChessPiece::WR);  // White Rook 
    append(ChessPiece::BR);  // Black Rook
    append(ChessPiece::WQ);  // White Queen
    append(ChessPiece::BQ);  // Black Queen
  }
  template <> inline Rule ChessVariantBoard<ChessVariant::Massacre>::GetRule() const noexcept { return AllowTakes; }
  template <> inline constexpr virtual int ChessVariantBoard<ChessVariant::Massacre>::GetMoveCountFactor() const noexcept { return 1000; }
  template <> inline void ChessVariantBoard<ChessVariant::Massacre>::SetStartingBoard() noexcept
  {
    const Coordinate sx = stage_.GetSizeX();
    const Coordinate sy = stage_.GetSizeY();

    std::vector<Location> ll;
    for (Coordinate i = 0; i < sx; i++)
      for (Coordinate j = 0; j < sy; j++)
      {
        const Location l{ BoardPartID::Stage,  i,j };
        assert(GetPieceIndex(l.x_,l.y_,l.b_) == PMap[Piece::NoPiece]);
        ll.push_back(l);
      }
    assert(ll.size() == sx * sy);
    Math::Shuffle(ll);
    auto it = ll.begin();
    for (Coordinate z = 0; z < sx * sy / 8; z++)
    {
      SetPieceIndex(PMap[ChessPiece::WQ ],(*it).x_,(*it).y_,(*it).b_); it++;
      SetPieceIndex(PMap[ChessPiece::BQ ],(*it).x_,(*it).y_,(*it).b_); it++;
      SetPieceIndex(PMap[ChessPiece::WR ],(*it).x_,(*it).y_,(*it).b_); it++;
      SetPieceIndex(PMap[ChessPiece::BR ],(*it).x_,(*it).y_,(*it).b_); it++;
      SetPieceIndex(PMap[ChessPiece::WB ],(*it).x_,(*it).y_,(*it).b_); it++;
      SetPieceIndex(PMap[ChessPiece::BB ],(*it).x_,(*it).y_,(*it).b_); it++;
      SetPieceIndex(PMap[ChessPiece::WN ],(*it).x_,(*it).y_,(*it).b_); it++;
      SetPieceIndex(PMap[ChessPiece::BN ],(*it).x_,(*it).y_,(*it).b_); it++;
    }
    assert(it == ll.end());
  }


  Board* ChessGame::GetNewBoard(const VariantChosen& v, const BoardPartDimensions& d) noexcept
  {
    // this switch looks silly, but cannot be avoided - the compiler has no idea which variants would be possible, 
    // but needs to generate the code for each one. An explicit list is needed in some form!
    switch (static_cast<ChessVariant>(v.c))
    {
      case ChessVariant::Standard:          return new ChessVariantBoard<ChessVariant::Standard        >(v, d);     // Standard Chess          
      case ChessVariant::Corner:            return new ChessVariantBoard<ChessVariant::Corner          >(v, d);     // Corner Chess            
      case ChessVariant::Fortress:          return new ChessVariantBoard<ChessVariant::Fortress        >(v, d);     // Fortress Chess          
      case ChessVariant::Horde:             return new ChessVariantBoard<ChessVariant::Horde           >(v, d);     // Horde Chess             
      case ChessVariant::Loop:              return new ChessVariantBoard<ChessVariant::Loop            >(v, d);     // Loop Chess              
      case ChessVariant::Anti:              return new ChessVariantBoard<ChessVariant::Anti            >(v, d);     // Anti Chess              
      case ChessVariant::Extinction:        return new ChessVariantBoard<ChessVariant::Extinction      >(v, d);     // Extinction Chess        
      case ChessVariant::Maharajah:         return new ChessVariantBoard<ChessVariant::Maharajah       >(v, d);     // Maharajah Chess         
      case ChessVariant::ThreeChecks:       return new ChessVariantBoard<ChessVariant::ThreeChecks     >(v, d);     // Three Checks Chess      
      case ChessVariant::Dark:              return new ChessVariantBoard<ChessVariant::Dark            >(v, d);     // Dark Chess              
      case ChessVariant::Atomic:            return new ChessVariantBoard<ChessVariant::Atomic          >(v, d);     // Atomic Chess            
      case ChessVariant::Janus:             return new ChessVariantBoard<ChessVariant::Janus           >(v, d);     // Janus Chess             
      case ChessVariant::Embassy:           return new ChessVariantBoard<ChessVariant::Embassy         >(v, d);     // Embassy Chess           
      case ChessVariant::Screen:            return new ChessVariantBoard<ChessVariant::Screen          >(v, d);     // Screen Chess            
      case ChessVariant::CrazyScreen:       return new ChessVariantBoard<ChessVariant::CrazyScreen     >(v, d);     // Crazy Screen Chess      
      case ChessVariant::Cylinder:          return new ChessVariantBoard<ChessVariant::Cylinder        >(v, d);     // Cylinder Chess          
      case ChessVariant::Amazons:           return new ChessVariantBoard<ChessVariant::Amazons         >(v, d);     // Amazon Chess            
      case ChessVariant::Berolina:          return new ChessVariantBoard<ChessVariant::Berolina        >(v, d);     // Berolina Chess          
      case ChessVariant::FischerRandom:     return new ChessVariantBoard<ChessVariant::FischerRandom   >(v, d);     // Fischer Random Chess    
      case ChessVariant::Legan:             return new ChessVariantBoard<ChessVariant::Legan           >(v, d);     // Legan Chess             
      case ChessVariant::KnightRelay:       return new ChessVariantBoard<ChessVariant::KnightRelay     >(v, d);     // Knight Relay Chess      
      case ChessVariant::Grand:             return new ChessVariantBoard<ChessVariant::Grand           >(v, d);     // Grand Chess             
      case ChessVariant::CapablancaRandom:  return new ChessVariantBoard<ChessVariant::CapablancaRandom>(v, d);     // Capablanca Random Chess 
      case ChessVariant::LosAlamos:         return new ChessVariantBoard<ChessVariant::LosAlamos       >(v, d);     // Los Alamos Chess        
      case ChessVariant::Ambiguous:         return new ChessVariantBoard<ChessVariant::Ambiguous       >(v, d);     // Ambiguous Chess         
      case ChessVariant::Cheversi:          return new ChessVariantBoard<ChessVariant::Cheversi        >(v, d);     // Cheversi
      case ChessVariant::Dice:              return new ChessVariantBoard<ChessVariant::Dice            >(v, d);     // Dice Chess              
      case ChessVariant::Recycle:           return new ChessVariantBoard<ChessVariant::Recycle         >(v, d);     // Recycle Chess           
      case ChessVariant::IceAge:            return new ChessVariantBoard<ChessVariant::IceAge          >(v, d);     // Ice Age Chess           
      case ChessVariant::Behemoth:          return new ChessVariantBoard<ChessVariant::Behemoth        >(v, d);     // Behemoth Chess          
      case ChessVariant::CheshireCat:       return new ChessVariantBoard<ChessVariant::CheshireCat     >(v, d);     // Cheshire Cat Chess      
      case ChessVariant::Knightmate:        return new ChessVariantBoard<ChessVariant::Knightmate      >(v, d);     // Knightmate Chess        
      case ChessVariant::RacingKings:       return new ChessVariantBoard<ChessVariant::RacingKings     >(v, d);     // Racing Kings            
      case ChessVariant::Dice10x10:         return new ChessVariantBoard<ChessVariant::Dice10x10       >(v, d);     // Dice Chess 10x10        
      case ChessVariant::Massacre:          return new ChessVariantBoard<ChessVariant::Massacre        >(v, d);     // Massacre Chess
      default: return nullptr; // must not happen
    }
  }

  void ChessGame::Register() noexcept
  {
    PMap.Register(ChessPiece::WP);  // White Pawn
    PMap.Register(ChessPiece::WN);  // White Knight
    PMap.Register(ChessPiece::WB);  // White Bishop
    PMap.Register(ChessPiece::WR);  // White Rook 
    PMap.Register(ChessPiece::WC);  // White Cardinal = Bishop + Knight (also: Archbishop, Princess, Janus)
    PMap.Register(ChessPiece::WM);  // White Marshall = Rook + Knight
    PMap.Register(ChessPiece::WQ);  // White Queen
    PMap.Register(ChessPiece::WA);  // White Amazon   = Queen + Knight (also: Maharajah)
    PMap.Register(ChessPiece::WK);  // White King 

    PMap.Register(ChessPiece::BP);  // Black Pawn
    PMap.Register(ChessPiece::BN);  // Black Knight
    PMap.Register(ChessPiece::BB);  // Black Bishop
    PMap.Register(ChessPiece::BR);  // Black Rook
    PMap.Register(ChessPiece::BC);  // Black Cardinal = Bishop + Knight (also: Archbishop, Princess, Janus)
    PMap.Register(ChessPiece::BM);  // Black Marshall = Rook + Knight
    PMap.Register(ChessPiece::BQ);  // Black Queen
    PMap.Register(ChessPiece::BA);  // Black Amazon   = Queen + Knight (also: Maharajah)
    PMap.Register(ChessPiece::BK);  // Black King

    PMap.Register(ChessPiece::RB);  // (Red) Behemoth
    PMap.Register(ChessPiece::GI);  // (Gray) Ice

    Variants.Register(Variant(0, IDR_GAMETYPE_CHESS, "Standard Chess",          ChessVariant::Standard,          8,  8         ));
    Variants.Register(Variant(0, IDR_GAMETYPE_CHESS, "Corner Chess",            ChessVariant::Corner,            8,  8         ));
    Variants.Register(Variant(0, IDR_GAMETYPE_CHESS, "Fortress Chess",          ChessVariant::Fortress,          8,  8         ));
    Variants.Register(Variant(0, IDR_GAMETYPE_CHESS, "Horde Chess",             ChessVariant::Horde,             8,  8         ));
    Variants.Register(Variant(0, IDR_GAMETYPE_CHESS, "Loop Chess",              ChessVariant::Loop,              8,  8         ));
    Variants.Register(Variant(0, IDR_GAMETYPE_CHESS, "Anti Chess",              ChessVariant::Anti,              8,  8         ));
    Variants.Register(Variant(0, IDR_GAMETYPE_CHESS, "Extinction Chess",        ChessVariant::Extinction,        8,  8         ));
    Variants.Register(Variant(0, IDR_GAMETYPE_CHESS, "Maharajah Chess",         ChessVariant::Maharajah,         8,  8         ));
    Variants.Register(Variant(0, IDR_GAMETYPE_CHESS, "Three Checks Chess",      ChessVariant::ThreeChecks,       8,  8         ));
    Variants.Register(Variant(0, IDR_GAMETYPE_CHESS, "Dark Chess",              ChessVariant::Dark,              8,  8         ));
    Variants.Register(Variant(0, IDR_GAMETYPE_CHESS, "Atomic Chess",            ChessVariant::Atomic,            8,  8         ));
    Variants.Register(Variant(0, IDR_GAMETYPE_CHESS, "Janus Chess",             ChessVariant::Janus,            10,  8         ));
    Variants.Register(Variant(0, IDR_GAMETYPE_CHESS, "Embassy Chess",           ChessVariant::Embassy,          10,  8         ));
    Variants.Register(Variant(0, IDR_GAMETYPE_CHESS, "Screen Chess",            ChessVariant::Screen,            8,  8         ));
    Variants.Register(Variant(0, IDR_GAMETYPE_CHESS, "Crazy Screen Chess",      ChessVariant::CrazyScreen,       8,  8         ));
    Variants.Register(Variant(0, IDR_GAMETYPE_CHESS, "Cylinder Chess",          ChessVariant::Cylinder,          8,  8         ));
    Variants.Register(Variant(0, IDR_GAMETYPE_CHESS, "Amazon Chess",            ChessVariant::Amazons,           8,  8         ));
    Variants.Register(Variant(0, IDR_GAMETYPE_CHESS, "Berolina Chess",          ChessVariant::Berolina,          8,  8         ));
    Variants.Register(Variant(0, IDR_GAMETYPE_CHESS, "Fischer Random Chess",    ChessVariant::FischerRandom,     8,  8         ));
    Variants.Register(Variant(0, IDR_GAMETYPE_CHESS, "Legan Chess",             ChessVariant::Legan,             8,  8         ));
    Variants.Register(Variant(0, IDR_GAMETYPE_CHESS, "Knight Relay Chess",      ChessVariant::KnightRelay,       8,  8         ));
    Variants.Register(Variant(0, IDR_GAMETYPE_CHESS, "Grand Chess",             ChessVariant::Grand,            10, 10         ));
    Variants.Register(Variant(0, IDR_GAMETYPE_CHESS, "Capablanca Random Chess", ChessVariant::CapablancaRandom, 10,  8         ));
    Variants.Register(Variant(0, IDR_GAMETYPE_CHESS, "Los Alamos Chess",        ChessVariant::LosAlamos,         6,  6         ));
    Variants.Register(Variant(0, IDR_GAMETYPE_CHESS, "Ambiguous Chess",         ChessVariant::Ambiguous,         8,  8         ));
    Variants.Register(Variant(0, IDR_GAMETYPE_CHESS, "Cheversi",                ChessVariant::Cheversi,          8,  8         ));
    Variants.Register(Variant(0, IDR_GAMETYPE_CHESS, "Dice Chess",              ChessVariant::Dice,              8,  8         ));
    Variants.Register(Variant(0, IDR_GAMETYPE_CHESS, "Recycle Chess",           ChessVariant::Recycle,           8,  8         ));
    Variants.Register(Variant(0, IDR_GAMETYPE_CHESS, "Ice Age Chess",           ChessVariant::IceAge,            8,  8         ));
    Variants.Register(Variant(0, IDR_GAMETYPE_CHESS, "Behemoth Chess",          ChessVariant::Behemoth,          8,  8         ));
    Variants.Register(Variant(0, IDR_GAMETYPE_CHESS, "Cheshire Cat Chess",      ChessVariant::CheshireCat,       8,  8         ));
    Variants.Register(Variant(0, IDR_GAMETYPE_CHESS, "Knightmate Chess",        ChessVariant::Knightmate,        8,  8         ));
    Variants.Register(Variant(0, IDR_GAMETYPE_CHESS, "Racing Kings",            ChessVariant::RacingKings,       8,  8         ));
    Variants.Register(Variant(0, IDR_GAMETYPE_CHESS, "Dice Chess 10x10",        ChessVariant::Dice10x10,        10, 10         ));
    Variants.Register(Variant(0, IDR_GAMETYPE_CHESS, "Massacre Chess",          ChessVariant::Massacre,          8,  8,  2, 20 ));
  }


  //MainLayout* ChessGame::GetNewLayout(const VariantChosen& v, const BoardPartDimensions& d) noexcept
  //{
  //  // this switch looks silly, but cannot be avoided - the compiler has no idea which variants would be possible, 
  //  // but needs to generate the code for each one. An explicit list is needed in some form!
  //  switch (static_cast<ChessVariant>(v.c))
  //  {
  //    case ChessVariant::Standard:          return new ChessVariantLayout<ChessVariant::Standard        >(d);     // Standard Chess          
  //    case ChessVariant::Corner:            return new ChessVariantLayout<ChessVariant::Corner          >(d);     // Corner Chess            
  //    case ChessVariant::Fortress:          return new ChessVariantLayout<ChessVariant::Fortress        >(d);     // Fortress Chess          
  //    case ChessVariant::Horde:             return new ChessVariantLayout<ChessVariant::Horde           >(d);     // Horde Chess             
  //    case ChessVariant::Loop:              return new ChessVariantLayout<ChessVariant::Loop            >(d);     // Loop Chess              
  //    case ChessVariant::Anti:              return new ChessVariantLayout<ChessVariant::Anti            >(d);     // Anti Chess              
  //    case ChessVariant::Extinction:        return new ChessVariantLayout<ChessVariant::Extinction      >(d);     // Extinction Chess        
  //    case ChessVariant::Maharajah:         return new ChessVariantLayout<ChessVariant::Maharajah       >(d);     // Maharajah Chess         
  //    case ChessVariant::ThreeChecks:       return new ChessVariantLayout<ChessVariant::ThreeChecks     >(d);     // Three Checks Chess      
  //    case ChessVariant::Dark:              return new ChessVariantLayout<ChessVariant::Dark            >(d);     // Dark Chess              
  //    case ChessVariant::Atomic:            return new ChessVariantLayout<ChessVariant::Atomic          >(d);     // Atomic Chess            
  //    case ChessVariant::Janus:             return new ChessVariantLayout<ChessVariant::Janus           >(d);     // Janus Chess             
  //    case ChessVariant::Embassy:           return new ChessVariantLayout<ChessVariant::Embassy         >(d);     // Embassy Chess           
  //    case ChessVariant::Screen:            return new ChessVariantLayout<ChessVariant::Screen          >(d);     // Screen Chess            
  //    case ChessVariant::CrazyScreen:       return new ChessVariantLayout<ChessVariant::CrazyScreen     >(d);     // Crazy Screen Chess      
  //    case ChessVariant::Cylinder:          return new ChessVariantLayout<ChessVariant::Cylinder        >(d);     // Cylinder Chess          
  //    case ChessVariant::Amazons:           return new ChessVariantLayout<ChessVariant::Amazons         >(d);     // Amazon Chess            
  //    case ChessVariant::Berolina:          return new ChessVariantLayout<ChessVariant::Berolina        >(d);     // Berolina Chess          
  //    case ChessVariant::FischerRandom:     return new ChessVariantLayout<ChessVariant::FischerRandom   >(d);     // Fischer Random Chess    
  //    case ChessVariant::Legan:             return new ChessVariantLayout<ChessVariant::Legan           >(d);     // Legan Chess             
  //    case ChessVariant::KnightRelay:       return new ChessVariantLayout<ChessVariant::KnightRelay     >(d);     // Knight Relay Chess      
  //    case ChessVariant::Grand:             return new ChessVariantLayout<ChessVariant::Grand           >(d);     // Grand Chess             
  //    case ChessVariant::CapablancaRandom:  return new ChessVariantLayout<ChessVariant::CapablancaRandom>(d);     // Capablanca Random Chess 
  //    case ChessVariant::LosAlamos:         return new ChessVariantLayout<ChessVariant::LosAlamos       >(d);     // Los Alamos Chess        
  //    case ChessVariant::Ambiguous:         return new ChessVariantLayout<ChessVariant::Ambiguous       >(d);     // Ambiguous Chess         
  //    case ChessVariant::Cheversi:          return new ChessVariantLayout<ChessVariant::Cheversi        >(d);     // Cheversi
  //    case ChessVariant::Dice:              return new ChessVariantLayout<ChessVariant::Dice            >(d);     // Dice Chess              
  //    case ChessVariant::Recycle:           return new ChessVariantLayout<ChessVariant::Recycle         >(d);     // Recycle Chess           
  //    case ChessVariant::IceAge:            return new ChessVariantLayout<ChessVariant::IceAge          >(d);     // Ice Age Chess           
  //    case ChessVariant::Behemoth:          return new ChessVariantLayout<ChessVariant::Behemoth        >(d);     // Behemoth Chess          
  //    case ChessVariant::CheshireCat:       return new ChessVariantLayout<ChessVariant::CheshireCat     >(d);     // Cheshire Cat Chess      
  //    case ChessVariant::Knightmate:        return new ChessVariantLayout<ChessVariant::Knightmate      >(d);     // Knightmate Chess        
  //    case ChessVariant::RacingKings:       return new ChessVariantLayout<ChessVariant::RacingKings     >(d);     // Racing Kings            
  //    case ChessVariant::Dice10x10:         return new ChessVariantLayout<ChessVariant::Dice10x10       >(d);     // Dice Chess 10x10        
  //    case ChessVariant::Massacre:          return new ChessVariantLayout<ChessVariant::Massacre        >(d);     // Massacre Chess
  //    default: return nullptr; // must not happen
  //  }
  //}

}
