#include "Core.h"

namespace BoardGamesChessPieces
{
  using namespace BoardGamesCore;

  using BoardGamesCore::Offset;
  using BoardGamesCore::PieceColor;

  const Pawn   Pawn  ::ThePawn  {};
  const Knight Knight::TheKnight{};
  const Bishop Bishop::TheBishop{};
  const Rook   Rook  ::TheRook  {};
  const Queen  Queen ::TheQueen {};
  const King   King  ::TheKing  {};
  
  const ChessPiece ChessPiece::WP{ Pawn  ::ThePawn,   PieceColor::White, IDB_WPL };
  const ChessPiece ChessPiece::WB{ Bishop::TheBishop, PieceColor::White, IDB_WBL };
  const ChessPiece ChessPiece::WN{ Knight::TheKnight, PieceColor::White, IDB_WNL };
  const ChessPiece ChessPiece::WR{ Rook  ::TheRook,   PieceColor::White, IDB_WRL };
  const ChessPiece ChessPiece::WQ{ Queen ::TheQueen,  PieceColor::White, IDB_WQL };
  const ChessPiece ChessPiece::WK{ King  ::TheKing,   PieceColor::White, IDB_WKL };

  const ChessPiece ChessPiece::BP{ Pawn  ::ThePawn,   PieceColor::Black, IDB_BPL };
  const ChessPiece ChessPiece::BB{ Bishop::TheBishop, PieceColor::Black, IDB_BBL };
  const ChessPiece ChessPiece::BN{ Knight::TheKnight, PieceColor::Black, IDB_BNL };
  const ChessPiece ChessPiece::BR{ Rook  ::TheRook,   PieceColor::Black, IDB_BRL };
  const ChessPiece ChessPiece::BQ{ Queen ::TheQueen,  PieceColor::Black, IDB_BQL };
  const ChessPiece ChessPiece::BK{ King  ::TheKing,   PieceColor::Black, IDB_BKL };

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
    for (int z = 1; p.AddIfLegal(moves, l, l + Offset(+z, +z)); z++);
    for (int z = 1; p.AddIfLegal(moves, l, l + Offset(+z, -z)); z++);
    for (int z = 1; p.AddIfLegal(moves, l, l + Offset(-z, +z)); z++);
    for (int z = 1; p.AddIfLegal(moves, l, l + Offset(-z, -z)); z++);
  }
  void Rook::CollectMoves(const MainPosition& p, const Location& l, Moves& moves) const noexcept
  {
    for (int z = 1; p.AddIfLegal(moves, l, l + Offset(+0, +z)); z++);
    for (int z = 1; p.AddIfLegal(moves, l, l + Offset(+0, -z)); z++);
    for (int z = 1; p.AddIfLegal(moves, l, l + Offset(+z, +0)); z++);
    for (int z = 1; p.AddIfLegal(moves, l, l + Offset(-z, -0)); z++);
  }
  void Queen::CollectMoves(const MainPosition& p, const Location& l, Moves& moves) const noexcept
  {
    for (int z = 1; p.AddIfLegal(moves, l, l + Offset(+z, +z)); z++);
    for (int z = 1; p.AddIfLegal(moves, l, l + Offset(+z, -z)); z++);
    for (int z = 1; p.AddIfLegal(moves, l, l + Offset(-z, +z)); z++);
    for (int z = 1; p.AddIfLegal(moves, l, l + Offset(-z, -z)); z++);
    for (int z = 1; p.AddIfLegal(moves, l, l + Offset(+0, +z)); z++);
    for (int z = 1; p.AddIfLegal(moves, l, l + Offset(+0, -z)); z++);
    for (int z = 1; p.AddIfLegal(moves, l, l + Offset(+z, +0)); z++);
    for (int z = 1; p.AddIfLegal(moves, l, l + Offset(-z, -0)); z++);
  }
  void King::CollectMoves(const MainPosition& p, const Location& l, Moves& moves) const noexcept
  {
    p.AddIfLegal(moves, l, l + Offset(+1, +1));
    p.AddIfLegal(moves, l, l + Offset(+1, -1));
    p.AddIfLegal(moves, l, l + Offset(-1, +1));
    p.AddIfLegal(moves, l, l + Offset(-1, -1));
    p.AddIfLegal(moves, l, l + Offset(+0, +1));
    p.AddIfLegal(moves, l, l + Offset(+0, -1));
    p.AddIfLegal(moves, l, l + Offset(+1, +0));
    p.AddIfLegal(moves, l, l + Offset(-1, -0));
  }

}
