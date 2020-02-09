#include "pch.h"

namespace BoardGamesChessPieces
{
  using BoardGamesCore::Offset;
  using BoardGamesCore::PieceColor;

  inline const Pawn   Pawn  ::ThePawn  {};
  inline const Knight Knight::TheKnight{};
  inline const Bishop Bishop::TheBishop{};
  inline const Rook   Rook  ::TheRook  {};
  inline const Queen  Queen ::TheQueen {};
  inline const King   King  ::TheKing  {};
  
  inline const ChessPiece ChessPiece::WP{ Pawn  ::ThePawn,   PieceColor::White, IDB_WPL };
  inline const ChessPiece ChessPiece::WB{ Bishop::TheBishop, PieceColor::White, IDB_WBL };
  inline const ChessPiece ChessPiece::WN{ Knight::TheKnight, PieceColor::White, IDB_WNL };
  inline const ChessPiece ChessPiece::WR{ Rook  ::TheRook,   PieceColor::White, IDB_WRL };
  inline const ChessPiece ChessPiece::WQ{ Queen ::TheQueen,  PieceColor::White, IDB_WQL };
  inline const ChessPiece ChessPiece::WK{ King  ::TheKing,   PieceColor::White, IDB_WKL };

  inline const ChessPiece ChessPiece::BP{ Pawn  ::ThePawn,   PieceColor::Black, IDB_BPL };
  inline const ChessPiece ChessPiece::BB{ Bishop::TheBishop, PieceColor::Black, IDB_BBL };
  inline const ChessPiece ChessPiece::BN{ Knight::TheKnight, PieceColor::Black, IDB_BNL };
  inline const ChessPiece ChessPiece::BR{ Rook  ::TheRook,   PieceColor::Black, IDB_BRL };
  inline const ChessPiece ChessPiece::BQ{ Queen ::TheQueen,  PieceColor::Black, IDB_BQL };
  inline const ChessPiece ChessPiece::BK{ King  ::TheKing,   PieceColor::Black, IDB_BKL };

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
