#include "Core.h"

namespace BoardGamesCore
{

  const Checker Checker::TheChecker{};
  const Pawn    Pawn   ::ThePawn   {};
  const Knight  Knight ::TheKnight {};
  const Bishop  Bishop ::TheBishop {};
  const Rook    Rook   ::TheRook   {};
  const Queen   Queen  ::TheQueen  {};
  const King    King   ::TheKing   {};
  
  const CorePiece CorePiece::WC{ Checker::TheChecker, PieceColor::White, IDB_WCL };
  const CorePiece CorePiece::WP{ Pawn   ::ThePawn,    PieceColor::White, IDB_WPL };
  const CorePiece CorePiece::WB{ Bishop ::TheBishop,  PieceColor::White, IDB_WBL };
  const CorePiece CorePiece::WN{ Knight ::TheKnight,  PieceColor::White, IDB_WNL };
  const CorePiece CorePiece::WR{ Rook   ::TheRook,    PieceColor::White, IDB_WRL };
  const CorePiece CorePiece::WQ{ Queen  ::TheQueen,   PieceColor::White, IDB_WQL };
  const CorePiece CorePiece::WK{ King   ::TheKing,    PieceColor::White, IDB_WKL };
                                                     
  const CorePiece CorePiece::BC{ Checker::TheChecker, PieceColor::Black, IDB_BCL };
  const CorePiece CorePiece::BP{ Pawn   ::ThePawn,    PieceColor::Black, IDB_BPL };
  const CorePiece CorePiece::BB{ Bishop ::TheBishop,  PieceColor::Black, IDB_BBL };
  const CorePiece CorePiece::BN{ Knight ::TheKnight,  PieceColor::Black, IDB_BNL };
  const CorePiece CorePiece::BR{ Rook   ::TheRook,    PieceColor::Black, IDB_BRL };
  const CorePiece CorePiece::BQ{ Queen  ::TheQueen,   PieceColor::Black, IDB_BQL };
  const CorePiece CorePiece::BK{ King   ::TheKing,    PieceColor::Black, IDB_BKL };

  void Checker::CollectMoves(const MainPosition& p, const Location& l, Moves& moves) const noexcept
  {
    for (const Offset& o : Offset::QDirection) p.AddIfLegal(moves, l, l + o);
  }

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
    for (const Offset& o : Offset::BDirection) for (int z = 1; p.AddIfLegal(moves, l, l + o); z++);
  }
  void Rook::CollectMoves(const MainPosition& p, const Location& l, Moves& moves) const noexcept
  {
    for (const Offset& o : Offset::RDirection) for (int z = 1; p.AddIfLegal(moves, l, l + o); z++);
  }
  void Queen::CollectMoves(const MainPosition& p, const Location& l, Moves& moves) const noexcept
  {
    for (const Offset& o : Offset::QDirection) for (int z = 1; p.AddIfLegal(moves, l, l + o); z++);
  }
  void King::CollectMoves(const MainPosition& p, const Location& l, Moves& moves) const noexcept
  {
    for (const Offset& o : Offset::QDirection) p.AddIfLegal(moves, l, l + o);
  }

}
