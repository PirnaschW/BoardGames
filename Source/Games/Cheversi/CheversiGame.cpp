#include "Games.h"

#include "CheversiGame.h"

namespace Cheversi
{
  CheversiPosition::CheversiPosition(VariantCode c, const PieceMapP& p, const Dimensions& d) noexcept : MainPosition(c, p, d)
  {
    SetPiece(Location{ BoardPart::Taken, 0, 1 }, CorePiece::WK);
    SetPiece(Location{ BoardPart::Taken, 1, 1 }, CorePiece::WQ);
    SetPiece(Location{ BoardPart::Taken, 2, 1 }, CorePiece::WR);
    SetPiece(Location{ BoardPart::Taken, 3, 1 }, CorePiece::WR);
    SetPiece(Location{ BoardPart::Taken, 4, 1 }, CorePiece::WB);
    SetPiece(Location{ BoardPart::Taken, 5, 1 }, CorePiece::WB);
    SetPiece(Location{ BoardPart::Taken, 6, 1 }, CorePiece::WN);
    SetPiece(Location{ BoardPart::Taken, 7, 1 }, CorePiece::WN);
    SetPiece(Location{ BoardPart::Taken, 0, 0 }, CorePiece::BK);
    SetPiece(Location{ BoardPart::Taken, 1, 0 }, CorePiece::BQ);
    SetPiece(Location{ BoardPart::Taken, 2, 0 }, CorePiece::BR);
    SetPiece(Location{ BoardPart::Taken, 3, 0 }, CorePiece::BR);
    SetPiece(Location{ BoardPart::Taken, 4, 0 }, CorePiece::BB);
    SetPiece(Location{ BoardPart::Taken, 5, 0 }, CorePiece::BB);
    SetPiece(Location{ BoardPart::Taken, 6, 0 }, CorePiece::BN);
    SetPiece(Location{ BoardPart::Taken, 7, 0 }, CorePiece::BN);
  }

  void CheversiPosition::GetAllMoves(void) const noexcept // collect all moves for all pieces
  {
    assert(movesW_.empty());
    assert(movesB_.empty());

    movesW_.reserve(20);
    movesB_.reserve(20);

    if (sequence_.size() == 0)  // first move?
    {
    }

    for (Coordinate i = 0; i < sizeX_; i++)
    {
      for (Coordinate j = 0; j < sizeY_; j++)
      {
        const Piece& p = GetPiece(Location(BoardPart::Main, i, j));
        if (!p.IsKind(noKind::NoKind))  // skip blank fields as well as nonexisting tiles
        {
          p.CollectMoves(*this, Location(BoardPart::Main, i, j), p.IsColor(PieceColor::White) ? movesW_ : movesB_);
        }
      }
    }
  }


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
    static VariantList v{
      { Variant{ nullptr, 'A', 8, 8, 2, 20 } },
    };
    return v;
  }

  const PieceMapP& CheversiGame::GetPieces(VariantCode c) noexcept
  {
    static const PieceMapP& p = std::make_shared<PieceMap>();
    p->Empty();
    p->Add(CorePiece::WK);
    p->Add(CorePiece::BK);
    p->Add(CorePiece::WQ);
    p->Add(CorePiece::BQ);
    p->Add(CorePiece::WR);
    p->Add(CorePiece::BR);
    p->Add(CorePiece::WB);
    p->Add(CorePiece::BB);
    p->Add(CorePiece::WN);
    p->Add(CorePiece::BN);
    return p;
  }

  const Dimensions& CheversiGame::GetDimensions(VariantCode c, Coordinate x, Coordinate y) noexcept
  {
    static Dimensions d{
       Dimension(x, y, BoardStartX, BoardStartY, FieldSizeX, FieldSizeY, 1, 1),
       Dimension(6, 2, BoardStartX + FieldSizeX * (x + 1), BoardStartY + y * FieldSizeY + FieldSizeY / 2, FieldSizeX, FieldSizeY),
       Dimension(8, 2, BoardStartX + FieldSizeX * (x + 1), BoardStartY + FieldSizeSY, FieldSizeSX, FieldSizeSY, 0, FieldSizeY * x - FieldSizeSY * 4),
    };
    return d;
  }
}
