#include "Games.h"

#include "KnightFGame.h"

namespace KnightF
{
  const Checker Checker::TheChecker{};
  const KnightFPiece KnightFPiece::KnightFPieceW{ Checker::TheChecker, PieceColor::White, IDB_WCL };
  const KnightFPiece KnightFPiece::KnightFPieceB{ Checker::TheChecker, PieceColor::Black, IDB_BCL };

  void KnightFBoard::SetStartingBoard() noexcept
  {
    const Coordinate max = sizeX_ * sizeY_;

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

  bool KnightFBoard::AddIfLegal(Moves& m, const Location& fr, const Location& to) const noexcept
  {
    const Piece& pf = GetPieceIndex(fr);                                       // piece to move
    if (pf == Piece::NoTile) return false;                               // out of board
    if (pf.IsBlank()) return false;                                      // tile not occupied

    const Piece& pt = GetPieceIndex(to);                                       // piece on target field
    if (pt == Piece::NoTile) return false;                               // out of board

    Actions a{};
    a.push_back(std::make_shared<ActionLift>(fr, pf));                    // pick piece up
    if (!pt.IsBlank())
    {
      a.push_back(std::make_shared<ActionLift>(to, pt));                    // pick opponent piece up
      a.push_back(std::make_shared<ActionDrop>(GetNextFreeTakenLocation(pf.GetColor()), pt));                   // place it in Taken
    }
    a.push_back(std::make_shared<ActionDrop>(to, pf));                   // and place it on target
    m.push_back(std::make_shared<Move>(a));                               // add move to move list
    return false;
  };

  void KnightFBoard::EvaluateStatically() const noexcept
  {
    Board::EvaluateStatically();
    // ...
  }


  const VariantList& KnightFGame::GetVariants() noexcept
  {
    static VariantList v{
      { Variant{ nullptr, 'A', 10, 10 } },
    };
    return v;
  }

  const PieceMapP& KnightFGame::GetPieces(const VariantChosen& v) noexcept
  {
    static const PieceMapP& p = std::make_shared<PieceMap>();
    p->Empty();
    p->Add(KnightFPiece::KnightFPieceW);
    p->Add(KnightFPiece::KnightFPieceB);
    return p;
  }

  const BoardPartDimensions KnightFGame::GetDimensions(const VariantChosen& v) noexcept
  {
    BoardPartDimensions d{
       BoardPartDimension(v.x, v.y, BoardStartX, BoardStartY, FieldSizeX, FieldSizeY, 1, 1),
       BoardPartDimension(2, 2, BoardStartX + FieldSizeX * (v.x + 1), BoardStartY + v.y * FieldSizeY + FieldSizeY / 2, FieldSizeX, FieldSizeY),
       BoardPartDimension(1, 1, BoardStartX + FieldSizeX * (v.x + 1), BoardStartY + FieldSizeSY, FieldSizeSX, FieldSizeSY, 0, FieldSizeY * v.x - FieldSizeSY * 4),
    };
    return d;
  }
}
