#include "Games.h"

#include "ChessGame.h"

namespace Chess
{

  const Pawn     Pawn    ::ThePawn    {};
  const Knight   Knight  ::TheKnight  {};
  const Bishop   Bishop  ::TheBishop  {};
  const Rook     Rook    ::TheRook    {};
  const Cardinal Cardinal::TheCardinal{};
  const Marshall Marshall::TheMarshall{};
  const Queen    Queen   ::TheQueen   {};
  const Amazon   Amazon  ::TheAmazon  {};
  const King     King    ::TheKing    {};

  const ChessPiece ChessPiece::WP{ Pawn    ::ThePawn,    PieceColor::White, IDB_WPL };  // White Pawn
  const ChessPiece ChessPiece::WN{ Knight  ::TheKnight,  PieceColor::White, IDB_WNL };  // White Knight
  const ChessPiece ChessPiece::WB{ Bishop  ::TheBishop,  PieceColor::White, IDB_WBL };  // White Bishop
  const ChessPiece ChessPiece::WR{ Rook    ::TheRook,    PieceColor::White, IDB_WRL };  // White Rook 
  const ChessPiece ChessPiece::WC{ Cardinal::TheCardinal,PieceColor::White, IDB_WCL };  // White Cardinal = Bishop + Knight (also: Archbishop, Princess, Janus)
  const ChessPiece ChessPiece::WM{ Marshall::TheMarshall,PieceColor::White, IDB_WML };  // White Marshall = Rook + Knight
  const ChessPiece ChessPiece::WQ{ Queen   ::TheQueen,   PieceColor::White, IDB_WQL };  // White Queen
  const ChessPiece ChessPiece::WA{ Amazon  ::TheAmazon,  PieceColor::White, IDB_WAL };  // White Amazon   = Queen + Knight
  const ChessPiece ChessPiece::WK{ King    ::TheKing,    PieceColor::White, IDB_WKL };  // White King 

  const ChessPiece ChessPiece::BP{ Pawn    ::ThePawn,    PieceColor::Black, IDB_BPL };  // Black Pawn
  const ChessPiece ChessPiece::BN{ Knight  ::TheKnight,  PieceColor::Black, IDB_BNL };  // Black Knight
  const ChessPiece ChessPiece::BB{ Bishop  ::TheBishop,  PieceColor::Black, IDB_BBL };  // Black Bishop
  const ChessPiece ChessPiece::BR{ Rook    ::TheRook,    PieceColor::Black, IDB_BRL };  // Black Rook
  const ChessPiece ChessPiece::BC{ Cardinal::TheCardinal,PieceColor::Black, IDB_BCL };  // Black Cardinal = Bishop + Knight (also: Archbishop, Princess, Janus)
  const ChessPiece ChessPiece::BM{ Marshall::TheMarshall,PieceColor::Black, IDB_BML };  // Black Marshall = Rook + Knight
  const ChessPiece ChessPiece::BQ{ Queen   ::TheQueen,   PieceColor::Black, IDB_BQL };  // Black Queen
  const ChessPiece ChessPiece::BA{ Amazon  ::TheAmazon,  PieceColor::Black, IDB_BAL };  // Black Amazon   = Queen + Knight
  const ChessPiece ChessPiece::BK{ King    ::TheKing,    PieceColor::Black, IDB_BKL };  // Black King 


  void Pawn::CollectMoves(const MainPosition& p, const Location& l, Moves& moves) const noexcept
  {
    p.AddIfLegal(moves, l, l + Offset(1, 1));
    p.AddIfLegal(moves, l, l + Offset(1, -1));
  }
  void Knight::CollectMoves(const MainPosition& p, const Location& l, Moves& moves) const noexcept
  {
    p.AddIfLegal(moves, l, l + Offset(+2, +1));
    p.AddIfLegal(moves, l, l + Offset(+1, +2));
    p.AddIfLegal(moves, l, l + Offset(-1, +2));
    p.AddIfLegal(moves, l, l + Offset(-2, +1));
    p.AddIfLegal(moves, l, l + Offset(-2, -1));
    p.AddIfLegal(moves, l, l + Offset(-1, -2));
    p.AddIfLegal(moves, l, l + Offset(+1, -2));
    p.AddIfLegal(moves, l, l + Offset(+2, -1));
  }
  void Bishop::CollectMoves(const MainPosition& p, const Location& l, Moves& moves) const noexcept
  {
    for (const Offset& o : Offset::BDirection) for (int z = 1; p.AddIfLegal(moves, l, l + o * z); z++);
  }
  void Rook::CollectMoves(const MainPosition& p, const Location& l, Moves& moves) const noexcept
  {
    for (const Offset& o : Offset::RDirection) for (int z = 1; p.AddIfLegal(moves, l, l + o * z); z++);
  }
  void Cardinal::CollectMoves(const MainPosition& p, const Location& l, Moves& moves) const noexcept
  {
    p.AddIfLegal(moves, l, l + Offset(+2, +1));
    p.AddIfLegal(moves, l, l + Offset(+1, +2));
    p.AddIfLegal(moves, l, l + Offset(-1, +2));
    p.AddIfLegal(moves, l, l + Offset(-2, +1));
    p.AddIfLegal(moves, l, l + Offset(-2, -1));
    p.AddIfLegal(moves, l, l + Offset(-1, -2));
    p.AddIfLegal(moves, l, l + Offset(+1, -2));
    p.AddIfLegal(moves, l, l + Offset(+2, -1));
    for (const Offset& o : Offset::BDirection) for (int z = 1; p.AddIfLegal(moves, l, l + o * z); z++);
  }
  void Marshall::CollectMoves(const MainPosition& p, const Location& l, Moves& moves) const noexcept
  {
    p.AddIfLegal(moves, l, l + Offset(+2, +1));
    p.AddIfLegal(moves, l, l + Offset(+1, +2));
    p.AddIfLegal(moves, l, l + Offset(-1, +2));
    p.AddIfLegal(moves, l, l + Offset(-2, +1));
    p.AddIfLegal(moves, l, l + Offset(-2, -1));
    p.AddIfLegal(moves, l, l + Offset(-1, -2));
    p.AddIfLegal(moves, l, l + Offset(+1, -2));
    p.AddIfLegal(moves, l, l + Offset(+2, -1));
    for (const Offset& o : Offset::RDirection) for (int z = 1; p.AddIfLegal(moves, l, l + o * z); z++);
  }
  void Queen::CollectMoves(const MainPosition& p, const Location& l, Moves& moves) const noexcept
  {
    for (const Offset& o : Offset::QDirection) for (int z = 1; p.AddIfLegal(moves, l, l + o * z); z++);
  }
  void Amazon::CollectMoves(const MainPosition& p, const Location& l, Moves& moves) const noexcept
  {
    p.AddIfLegal(moves, l, l + Offset(+2, +1));
    p.AddIfLegal(moves, l, l + Offset(+1, +2));
    p.AddIfLegal(moves, l, l + Offset(-1, +2));
    p.AddIfLegal(moves, l, l + Offset(-2, +1));
    p.AddIfLegal(moves, l, l + Offset(-2, -1));
    p.AddIfLegal(moves, l, l + Offset(-1, -2));
    p.AddIfLegal(moves, l, l + Offset(+1, -2));
    p.AddIfLegal(moves, l, l + Offset(+2, -1));
    for (const Offset& o : Offset::QDirection) for (int z = 1; p.AddIfLegal(moves, l, l + o * z); z++);
  }
  void King::CollectMoves(const MainPosition& p, const Location& l, Moves& moves) const noexcept
  {
    for (const Offset& o : Offset::QDirection) p.AddIfLegal(moves, l, l + o);
  }

  void ChessPosition::SetPawns(Coordinate row, const PieceColor& c) noexcept
  {
    const Piece& p{ c == PieceColor::White ? ChessPiece::WP : ChessPiece::BP };
    for (Coordinate i = 0; i < sizeX_; i++)
    {
      const Location& l{ BoardPart::Main, i, row };
      SetPiece(l, p);
    }
  }

  void ChessPosition::SetStartingPosition() noexcept
  {
    SetPawns(1U, PieceColor::Black);
    SetPawns(sizeY_ - 2U, PieceColor::White);

    SetPiece({ BoardPart::Main,          0U,          0U }, ChessPiece::BR);
    SetPiece({ BoardPart::Main,          1U,          0U }, ChessPiece::BN);
    SetPiece({ BoardPart::Main,          2U,          0U }, ChessPiece::BB);
    SetPiece({ BoardPart::Main,          3U,          0U }, ChessPiece::BQ);
    SetPiece({ BoardPart::Main, sizeX_ - 4U,          0U }, ChessPiece::BK);
    SetPiece({ BoardPart::Main, sizeX_ - 3U,          0U }, ChessPiece::BB);
    SetPiece({ BoardPart::Main, sizeX_ - 2U,          0U }, ChessPiece::BN);
    SetPiece({ BoardPart::Main, sizeX_ - 1U,          0U }, ChessPiece::BR);

    SetPiece({ BoardPart::Main,          0U, sizeY_ - 1U }, ChessPiece::WR);
    SetPiece({ BoardPart::Main,          1U, sizeY_ - 1U }, ChessPiece::WN);
    SetPiece({ BoardPart::Main,          2U, sizeY_ - 1U }, ChessPiece::WB);
    SetPiece({ BoardPart::Main,          3U, sizeY_ - 1U }, ChessPiece::WQ);
    SetPiece({ BoardPart::Main, sizeX_ - 4U, sizeY_ - 1U }, ChessPiece::WK);
    SetPiece({ BoardPart::Main, sizeX_ - 3U, sizeY_ - 1U }, ChessPiece::WB);
    SetPiece({ BoardPart::Main, sizeX_ - 2U, sizeY_ - 1U }, ChessPiece::WN);
    SetPiece({ BoardPart::Main, sizeX_ - 1U, sizeY_ - 1U }, ChessPiece::WR);
  }

  bool ChessPosition::AddIfLegal(Moves& m, const Location& fr, const Location& to) const noexcept
  {
    const Piece& pf = GetPiece(fr);
    assert(pf != Piece::NoTile);                                                // start field must exist
    assert(!pf.IsBlank());                                               // start field must be a piece

    const Piece& pt = GetPiece(to);
    if (pt == Piece::NoTile) return false;                                      // out of board
    if (pt.IsBlank()) return true;                                       // not a move, but keep trying this direction
    if (pt.GetColor() == pf.GetColor()) return false;                   // own piece; don't keep trying this direction

    // valid move, save into collection
    Actions a{};
    a.push_back(std::make_shared<ActionLift>(fr, pf));                    // pick piece up
    a.push_back(std::make_shared<ActionLift>(to, pt));                    // pick opponent piece up
    a.push_back(std::make_shared<ActionDrop>(GetNextTakenL(pf.GetColor()), pt));                   // place it in Taken
    a.push_back(std::make_shared<ActionDrop>(to, pf));                   // and place it on target
    m.push_back(std::make_shared<Move>(a));                               // add move to move list

    return false;                                                         // don't keep trying this direction
  };


  const VariantList& ChessGame::GetVariants(void) noexcept
  {
    static VariantList v{
      { Variant{ "Standard Chess",          Chess::Standard,           8,  8 } },
      { Variant{ "Corner Chess",            Chess::Corner,             8,  8 } },
      { Variant{ "Fortress Chess",          Chess::Fortress,           8,  8 } },
      { Variant{ "Horde Chess",             Chess::Horde,              8,  8 } },
      { Variant{ "Loop Chess",              Chess::Loop,               8,  8 } },
      { Variant{ "Anti Chess",              Chess::Anti,               8,  8 } },
      { Variant{ "Extinction Chess",        Chess::Extinction,         8,  8 } },
      { Variant{ "Maharajah Chess",         Chess::Maharajah,          8,  8 } },
      { Variant{ "Three Checks Chess",      Chess::ThreeChecks,        8,  8 } },
      { Variant{ "Dark Chess",              Chess::Dark,               8,  8 } },
      { Variant{ "Atomic Chess",            Chess::Atomic,             8,  8 } },
      { Variant{ "Janus Chess",             Chess::Janus,             10,  8 } },
      { Variant{ "Embassy Chess",           Chess::Embassy,           10,  8 } },
      { Variant{ "Screen Chess",            Chess::Screen,             8,  8 } },
      { Variant{ "Crazy Screen Chess",      Chess::CrazyScreen,        8,  8 } },
      { Variant{ "Cylinder Chess",          Chess::Cylinder,           8,  8 } },
      { Variant{ "Amazon Chess",            Chess::Amazons,            8,  8 } },
      { Variant{ "Berolina Chess",          Chess::Berolina,           8,  8 } },
      { Variant{ "Fischer Random Chess",    Chess::FischerRandom,      8,  8 } },
      { Variant{ "Legan Chess",             Chess::Legan,              8,  8 } },
      { Variant{ "Knight Relay Chess",      Chess::KnightRelay,        8,  8 } },
      { Variant{ "Grand Chess",             Chess::Grand,             10, 10 } },
      { Variant{ "Capablanca Random Chess", Chess::CapablancaRandom,  10,  8 } },
      { Variant{ "Los Alamos Chess",        Chess::LosAlamos,          6,  6 } },
      { Variant{ "Ambiguous Chess",         Chess::Ambiguous,          8,  8 } },
      { Variant{ "Cheversi",                Chess::Cheversi,           8,  8 } },
      { Variant{ "Dice Chess",              Chess::Dice,               8,  8 } },
      { Variant{ "Recycle Chess",           Chess::Recycle,            8,  8 } },
      { Variant{ "Ice Age Chess",           Chess::IceAge,             8,  8 } },
      { Variant{ "Behemoth Chess",          Chess::Behemoth,           8,  8 } },
      { Variant{ "Cheshire Cat Chess",      Chess::CheshireCat,        8,  8 } },
      { Variant{ "Knightmate Chess",        Chess::Knightmate,         8,  8 } },
      { Variant{ "Racing Kings",            Chess::RacingKings,        8,  8 } },
      { Variant{ "Dice Chess 10x10",        Chess::Dice10x10,         10, 10 } },
      { Variant{ "Massacre Chess",          Chess::Massacre,           8,  8 } },
    };
    return v;
  }

  const PieceMapP& ChessGame::GetPieces(const VariantChosen& v) noexcept
  {
    static const PieceMapP& p = std::make_shared<PieceMap>();
    p->Empty();
    p->Add(ChessPiece::WP);    p->Add(ChessPiece::BP);
    p->Add(ChessPiece::WN);    p->Add(ChessPiece::BN);
    p->Add(ChessPiece::WB);    p->Add(ChessPiece::BB);
    p->Add(ChessPiece::WR);    p->Add(ChessPiece::BR);
    p->Add(ChessPiece::WC);    p->Add(ChessPiece::BC);
    p->Add(ChessPiece::WM);    p->Add(ChessPiece::BM);
    p->Add(ChessPiece::WQ);    p->Add(ChessPiece::BQ);
    p->Add(ChessPiece::WA);    p->Add(ChessPiece::BA);
    p->Add(ChessPiece::WK);    p->Add(ChessPiece::BK);
    return p;
  }


  const Dimensions ChessGame::GetDimensions(const VariantChosen& v) noexcept
  {
    Dimensions d{
       Dimension(v.x, v.y, BoardStartX, BoardStartY, FieldSizeX, FieldSizeY, 1, 1),
       Dimension(10, 2, BoardStartX + FieldSizeX, BoardStartY + FieldSizeY * (v.y + 1), FieldSizeX, FieldSizeY),
       Dimension(v.x * v.y / 2, 2, BoardStartX + FieldSizeX * (v.x + 1), BoardStartY + FieldSizeSY, FieldSizeSX, FieldSizeSY, 0, FieldSizeY * v.y - FieldSizeSY * 4),
    };
    return d;
  }

}
