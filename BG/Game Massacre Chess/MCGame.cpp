#include "stdafx.h"

#include "MCResource.h"
#include "MCGame.h"

namespace MassacreChess
{
  MCPosition::MCPosition(const PieceMapP& p, const Dimensions& d) noexcept : MainPosition(p, d)
  {
    //srand((unsigned)time(nullptr));
    std::srand(1);

    for (Coordinate z = 0; z < d[0].xCount_*d[0].yCount_ / 8; z++)
    {
      PlaceRandomly(ChessPiece::WQ);
      PlaceRandomly(ChessPiece::BQ);
      PlaceRandomly(ChessPiece::WR);
      PlaceRandomly(ChessPiece::BR);
      PlaceRandomly(ChessPiece::WB);
      PlaceRandomly(ChessPiece::BB);
      PlaceRandomly(ChessPiece::WN);
      PlaceRandomly(ChessPiece::BN);
    }
  }

  bool MCPosition::AddIfLegal(Moves& m, const Location& fr, const Location& to) const noexcept
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
    a.push_back(std::make_shared<ActionTake>(fr, pf));                    // pick piece up
    a.push_back(std::make_shared<ActionTake>(to, pt));                    // pick opponent piece up
    a.push_back(std::make_shared<ActionPlace>(GetNextTakenL(pf.GetColor()), pt));                   // place it in Taken
    a.push_back(std::make_shared<ActionPlace>(to, pf));                   // and place it on target
    m.push_back(std::make_shared<Move>(a));                               // add move to move list

    return false;                                                         // don't keep trying this direction
  };

  bool MCPosition::PlaceRandomly(const Piece& p)
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


  const VariantList& MCGame::GetVariants(void) noexcept
  {
    static VariantList v{ { Variant{ 8, 8, nullptr, 2, 20 } } };
    return v;
  }

  const PieceMapP& MCGame::GetPieces(void) noexcept
  {
    static const PieceMapP& p = std::make_shared<PieceMap>();
    p->Add(ChessPiece::WQ);
    p->Add(ChessPiece::WR);
    p->Add(ChessPiece::WB);
    p->Add(ChessPiece::WN);
    p->Add(ChessPiece::BQ);
    p->Add(ChessPiece::BR);
    p->Add(ChessPiece::BB);
    p->Add(ChessPiece::BN);
    return p;
  }


  const Dimensions& MCGame::GetDimensions(Coordinate x, Coordinate y) noexcept
  {
    static Dimensions d{
       Dimension(x, y, BoardStartX, BoardStartY, FieldSizeX, FieldSizeY, 1, 1),
       Dimension(5, 2, BoardStartX + FieldSizeX, BoardStartY + FieldSizeY * (y + 1), FieldSizeX, FieldSizeY),
       Dimension(x * y / 2, 2, BoardStartX + FieldSizeX * (x + 1), BoardStartY + FieldSizeSY, FieldSizeSX, FieldSizeSY, 0, FieldSizeY * y - FieldSizeSY * 4),
    };
    return d;
  }

}
