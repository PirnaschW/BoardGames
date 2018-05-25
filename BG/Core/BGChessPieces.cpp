#include "stdafx.h"

namespace BoardGamesChessPieces
{

  using BoardGamesCore::Offset;
  using BoardGamesCore::Color;

  const ChessPiece ChessPiece::WP{ &Pawn  ::ThePawn,   &Color::White, IDB_WPL, IDB_WPD, IDB_WPS };
  const ChessPiece ChessPiece::WB{ &Bishop::TheBishop, &Color::White, IDB_WBL, IDB_WBD, IDB_WBS };
  const ChessPiece ChessPiece::WN{ &Knight::TheKnight, &Color::White, IDB_WNL, IDB_WND, IDB_WNS };
  const ChessPiece ChessPiece::WR{ &Rook  ::TheRook,   &Color::White, IDB_WRL, IDB_WRD, IDB_WRS };
  const ChessPiece ChessPiece::WQ{ &Queen ::TheQueen,  &Color::White, IDB_WQL, IDB_WQD, IDB_WQS };
  const ChessPiece ChessPiece::WK{ &King  ::TheKing,   &Color::White, IDB_WKL, IDB_WKD, IDB_WKS };
  
  const ChessPiece ChessPiece::BP{ &Pawn  ::ThePawn,   &Color::Black, IDB_BPL, IDB_BPD, IDB_BPS };
  const ChessPiece ChessPiece::BB{ &Bishop::TheBishop, &Color::Black, IDB_BBL, IDB_BBD, IDB_BBS };
  const ChessPiece ChessPiece::BN{ &Knight::TheKnight, &Color::Black, IDB_BNL, IDB_BND, IDB_BNS };
  const ChessPiece ChessPiece::BR{ &Rook  ::TheRook,   &Color::Black, IDB_BRL, IDB_BRD, IDB_BRS };
  const ChessPiece ChessPiece::BQ{ &Queen ::TheQueen,  &Color::Black, IDB_BQL, IDB_BQD, IDB_BQS };
  const ChessPiece ChessPiece::BK{ &King  ::TheKing,   &Color::Black, IDB_BKL, IDB_BKD, IDB_BKS };

  void Pawn::CollectMoves(const MainPosition& p, const Location& l, std::vector<Move>& moves) const
  {
    p.AddIfLegal(moves, l, l + Offset(1, 1));
    p.AddIfLegal(moves, l, l + Offset(1, -1));
  }
  void Knight::CollectMoves(const MainPosition& p, const Location& l, std::vector<Move>& moves) const
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
  void Bishop::CollectMoves(const MainPosition& p, const Location& l, std::vector<Move>& moves) const
  {
    for (int z = 1; p.AddIfLegal(moves, l, l + Offset(+z, +z)); z++);
    for (int z = 1; p.AddIfLegal(moves, l, l + Offset(+z, -z)); z++);
    for (int z = 1; p.AddIfLegal(moves, l, l + Offset(-z, +z)); z++);
    for (int z = 1; p.AddIfLegal(moves, l, l + Offset(-z, -z)); z++);
  }
  void Rook::CollectMoves(const MainPosition& p, const Location& l, std::vector<Move>& moves) const
  {
    for (int z = 1; p.AddIfLegal(moves, l, l + Offset(+0, +z)); z++);
    for (int z = 1; p.AddIfLegal(moves, l, l + Offset(+0, -z)); z++);
    for (int z = 1; p.AddIfLegal(moves, l, l + Offset(+z, +0)); z++);
    for (int z = 1; p.AddIfLegal(moves, l, l + Offset(-z, -0)); z++);
  }
  void Queen::CollectMoves(const MainPosition& p, const Location& l, std::vector<Move>& moves) const
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
  void King::CollectMoves(const MainPosition& p, const Location& l, std::vector<Move>& moves) const
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
