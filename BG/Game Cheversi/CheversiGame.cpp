#include "pch.h"

#include "CheversiGame.h"

namespace Cheversi
{
  inline const Checker Checker::TheChecker{};
  inline const CheversiPiece CheversiPiece::CheversiPieceW{ Checker::TheChecker, Color::White, IDB_WCL, IDB_WCD, IDB_WCS };
  inline const CheversiPiece CheversiPiece::CheversiPieceB{ Checker::TheChecker, Color::Black, IDB_BCL, IDB_BCD, IDB_BCS };

  bool CheversiPosition::AddIfLegal(Moves& m, const Location& fr, const Location& to) const noexcept
  {
    const Piece& pf = GetPiece(fr);                                       // piece to move
    if (pf == Piece::NoTile) return false;                               // out of board
    if (pf.IsBlank()) return false;                                      // tile not occupied

    const Piece& pt = GetPiece(to);                                       // piece on target field
    if (pt == Piece::NoTile) return false;                               // out of board

    Actions a{};
    a.push_back(std::make_shared<ActionLift>(fr, pf));                    // pick piece up
    if (!pt.IsBlank())
    {
      a.push_back(std::make_shared<ActionLift>(to, pt));                    // pick opponent piece up
      a.push_back(std::make_shared<ActionDrop>(GetNextTakenL(pf.GetColor()), pt));                   // place it in Taken
    }
    a.push_back(std::make_shared<ActionDrop>(to, pf));                   // and place it on target
    m.push_back(std::make_shared<Move>(a));                               // add move to move list
    return false;
  };

  PositionValue CheversiPosition::EvaluateStatically(void) const noexcept
  {
    return MainPosition::EvaluateStatically();
    // ...
  }


  const VariantList& CheversiGame::GetVariants(void) noexcept
  {
    static VariantList v{ { Variant{ 8, 8, "Cheversi", 2, 20 } } };
    return v;
  }

  const PieceMapP& CheversiGame::GetPieces(void) noexcept
  {
    static const PieceMapP& p = std::make_shared<PieceMap>();
    p->Add(CheversiPiece::CheversiPieceW);
    p->Add(CheversiPiece::CheversiPieceB);
    return p;
  }

  const Dimensions& CheversiGame::GetDimensions(Coordinate x, Coordinate y) noexcept
  {
    static Dimensions d{
       Dimension(x, y, BoardStartX, BoardStartY, FieldSizeX, FieldSizeY, 1, 1),
       Dimension(2, 2, BoardStartX + FieldSizeX * (x + 1), BoardStartY + y * FieldSizeY + FieldSizeY / 2, FieldSizeX, FieldSizeY),
       Dimension(0, 0, BoardStartX + FieldSizeX * (x + 1), BoardStartY + FieldSizeSY, FieldSizeSX, FieldSizeSY, 0, FieldSizeY * x - FieldSizeSY * 4),
    };
    return d;
  }
}
