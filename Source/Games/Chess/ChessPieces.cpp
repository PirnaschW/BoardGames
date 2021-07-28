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
  const Behemoth Behemoth::TheBehemoth{};
  const Ice      Ice     ::TheIce     {};

  const ChessPiece ChessPiece::WP{ Pawn    ::ThePawn,    PieceColor::White,   IDB_WPL };  // White Pawn
  const ChessPiece ChessPiece::WN{ Knight  ::TheKnight,  PieceColor::White,   IDB_WNL };  // White Knight
  const ChessPiece ChessPiece::WB{ Bishop  ::TheBishop,  PieceColor::White,   IDB_WBL };  // White Bishop
  const ChessPiece ChessPiece::WR{ Rook    ::TheRook,    PieceColor::White,   IDB_WRL };  // White Rook 
  const ChessPiece ChessPiece::WC{ Cardinal::TheCardinal,PieceColor::White,   IDB_WCL };  // White Cardinal = Bishop + Knight (also: Archbishop, Princess, Janus)
  const ChessPiece ChessPiece::WM{ Marshall::TheMarshall,PieceColor::White,   IDB_WML };  // White Marshall = Rook + Knight
  const ChessPiece ChessPiece::WQ{ Queen   ::TheQueen,   PieceColor::White,   IDB_WQL };  // White Queen
  const ChessPiece ChessPiece::WA{ Amazon  ::TheAmazon,  PieceColor::White,   IDB_WAL };  // White Amazon   = Queen + Knight
  const ChessPiece ChessPiece::WK{ King    ::TheKing,    PieceColor::White,   IDB_WKL };  // White King 

  const ChessPiece ChessPiece::BP{ Pawn    ::ThePawn,    PieceColor::Black,   IDB_BPL };  // Black Pawn
  const ChessPiece ChessPiece::BN{ Knight  ::TheKnight,  PieceColor::Black,   IDB_BNL };  // Black Knight
  const ChessPiece ChessPiece::BB{ Bishop  ::TheBishop,  PieceColor::Black,   IDB_BBL };  // Black Bishop
  const ChessPiece ChessPiece::BR{ Rook    ::TheRook,    PieceColor::Black,   IDB_BRL };  // Black Rook
  const ChessPiece ChessPiece::BC{ Cardinal::TheCardinal,PieceColor::Black,   IDB_BCL };  // Black Cardinal = Bishop + Knight (also: Archbishop, Princess, Janus)
  const ChessPiece ChessPiece::BM{ Marshall::TheMarshall,PieceColor::Black,   IDB_BML };  // Black Marshall = Rook + Knight
  const ChessPiece ChessPiece::BQ{ Queen   ::TheQueen,   PieceColor::Black,   IDB_BQL };  // Black Queen
  const ChessPiece ChessPiece::BA{ Amazon  ::TheAmazon,  PieceColor::Black,   IDB_BAL };  // Black Amazon   = Queen + Knight
  const ChessPiece ChessPiece::BK{ King    ::TheKing,    PieceColor::Black,   IDB_BKL };  // Black King

  const ChessPiece ChessPiece::RB{ Behemoth::TheBehemoth,PieceColor::NoColor, IDB_RBL };  // (Red) Behemoth
  const ChessPiece ChessPiece::GI{ Ice     ::TheIce,     PieceColor::NoColor, IDB_GIL };  // (Gray) Ice

  void Pawn::CollectMoves(const MainPosition& p, const Location& l, Moves& moves) const noexcept
  {
    const PieceColor& c{ p.GetPiece(l).GetColor()};
    const int dy = c == PieceColor::White ? -1 : 1;

    if (p.HasRule(AllowTakes))
    {
      const Location& lr{ l + Offset(+1, dy) };  // right forward
      const Location& ll{ l + Offset(-1, dy) };  // left forward
      const PieceColor cr{ p.GetPiece(lr).GetColor() };
      const PieceColor cl{ p.GetPiece(ll).GetColor() };
      // if there is a piece there, allow the pawn to take diagonally
      if (cr == ~c || (p.HasRule(TakeOwn) && cr == c)) p.AddIfLegal(moves, l, lr);
      if (cl == ~c || (p.HasRule(TakeOwn) && cl == c)) p.AddIfLegal(moves, l, ll);
    }

    if (p.HasRule(AllowMoves))
    {
    // if the field ahead is free, pawn can move there
      if (p.GetPiece(l + Offset(0, dy)).IsBlank())
      {
        bool step = p.AddIfLegal(moves, l, l + Offset(0, dy));
        if (step && p.HasRule(PawnsDoubleStep))
        {
          Coordinate z = c == PieceColor::White ? p.GetSizeY() - 2U : 1U;
          // if the next field ahead is free too, pawn can move there too
          if (l.y_ == z) if (p.GetPiece(l + Offset(0, dy * 2)).IsBlank()) p.AddIfLegal(moves, l, l + Offset(0, dy * 2));
        }
      }
    }
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

}
