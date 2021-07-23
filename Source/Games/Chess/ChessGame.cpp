#include "Games.h"

#include "ChessGame.h"

namespace Chess
{
  ChessPosition::ChessPosition(const VariantChosen& v, const PieceMapP& p, const Dimensions& d) noexcept : MainPosition(v, p, d)
  {
    for (Coordinate z = 0; z < d[0].xCount_*d[0].yCount_ / 8; z++)
    {
      PlaceRandomly(CorePiece::WQ);
      PlaceRandomly(CorePiece::BQ);
      PlaceRandomly(CorePiece::WR);
      PlaceRandomly(CorePiece::BR);
      PlaceRandomly(CorePiece::WB);
      PlaceRandomly(CorePiece::BB);
      PlaceRandomly(CorePiece::WN);
      PlaceRandomly(CorePiece::BN);
    }
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

  bool ChessPosition::PlaceRandomly(const Piece& p)
  {
    std::vector<Location> ll;
    for (Coordinate i = 0; i < sizeX_; i++)
      for (Coordinate j = 0; j < sizeY_; j++)
      {
        const Location l{ BoardPart::Main,  i,j };
        if (GetPiece(l) == Piece::NoPiece) ll.push_back(l);
      }
    if (ll.empty()) return false;
    const unsigned int z = rand() % ll.size();
    SetPiece(ll[z], p);
    return true;
  }


  const VariantList& ChessGame::GetVariants(void) noexcept
  {
    static VariantList v{
      { Variant{ nullptr, 'A', 8, 8, 2, 20 } },
    };
    return v;
  }

  const PieceMapP& ChessGame::GetPieces(const VariantChosen& v) noexcept
  {
    static const PieceMapP& p = std::make_shared<PieceMap>();
    p->Add(CorePiece::WQ);
    p->Add(CorePiece::WR);
    p->Add(CorePiece::WB);
    p->Add(CorePiece::WN);
    p->Add(CorePiece::BQ);
    p->Add(CorePiece::BR);
    p->Add(CorePiece::BB);
    p->Add(CorePiece::BN);
    return p;
  }


  const Dimensions ChessGame::GetDimensions(const VariantChosen& v) noexcept
  {
    Dimensions d{
       Dimension(v.x, v.y, BoardStartX, BoardStartY, FieldSizeX, FieldSizeY, 1, 1),
       Dimension(5, 2, BoardStartX + FieldSizeX, BoardStartY + FieldSizeY * (v.y + 1), FieldSizeX, FieldSizeY),
       Dimension(v.x * v.y / 2, 2, BoardStartX + FieldSizeX * (v.x + 1), BoardStartY + FieldSizeSY, FieldSizeSX, FieldSizeSY, 0, FieldSizeY * v.y - FieldSizeSY * 4),
    };
    return d;
  }

}
