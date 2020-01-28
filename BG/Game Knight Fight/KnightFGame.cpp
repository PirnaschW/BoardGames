#include "pch.h"

#include "KnightFGame.h"

namespace KnightF
{
  inline const Checker Checker::TheChecker{};
  inline const KnightFPiece KnightFPiece::KnightFPieceW{ Checker::TheChecker, Color::White, IDB_WCL, IDB_WCD, IDB_WCS };
  inline const KnightFPiece KnightFPiece::KnightFPieceB{ Checker::TheChecker, Color::Black, IDB_BCL, IDB_BCD, IDB_BCS };

  KnightFPosition::KnightFPosition(const PieceMapP& p, const Dimensions& d) noexcept : MainPosition(p, d)
  {
    const Coordinate max = d[0].xCount_ * d[0].yCount_;

    std::vector<Coordinate> available(max);
    for (Coordinate i = 0; i < max; ++i) available[i] = i;

    fValue_.resize(max);
    for (Coordinate z = 0; z < max; ++z)
    {
      const unsigned int i = rand() % available.size();
      fValue_[z] = available[i];
      available.erase(available.begin() + i);
    }
    assert(available.size() == 0);
  }

  bool KnightFPosition::AddIfLegal(Moves& m, const Location& fr, const Location& to) const noexcept
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

  PositionValue KnightFPosition::EvaluateStatically(void) const noexcept
  {
    return MainPosition::EvaluateStatically();
    // ...
  }


  const VariantList& KnightFGame::GetVariants(void) noexcept
  {
    static VariantList v{ { Variant{ 10, 10, "Knight Fight", 2, 20 } } };
    return v;
  }

  const PieceMapP& KnightFGame::GetPieces(void) noexcept
  {
    static const PieceMapP& p = std::make_shared<PieceMap>();
    p->Add(KnightFPiece::KnightFPieceW);
    p->Add(KnightFPiece::KnightFPieceB);
    return p;
  }

  const Dimensions& KnightFGame::GetDimensions(Coordinate x, Coordinate y) noexcept
  {
    static Dimensions d{
       Dimension(x, y, BoardStartX, BoardStartY, FieldSizeX, FieldSizeY, 1, 1),
       Dimension(2, 2, BoardStartX + FieldSizeX * (x + 1), BoardStartY + y * FieldSizeY + FieldSizeY / 2, FieldSizeX, FieldSizeY),
       Dimension(1, 1, BoardStartX + FieldSizeX * (x + 1), BoardStartY + FieldSizeSY, FieldSizeSX, FieldSizeSY, 0, FieldSizeY * x - FieldSizeSY * 4),
    };
    return d;
  }
}