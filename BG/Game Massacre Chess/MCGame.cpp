#include "stdafx.h"

#include "MCResource.h"
#include "MCGame.h"

namespace MassacreChess
{
  MCPosition::MCPosition(const PieceMapP& p, const Dimensions& d) noexcept : MainPosition(p, d)
  {
    //srand((unsigned)time(NULL));
    std::srand(1);

    for (Coordinate z = 0; z < d[0].xCount*d[0].yCount / 8; z++)
    {
      PlaceRandomly(&ChessPiece::WQ);
      PlaceRandomly(&ChessPiece::BQ);
      PlaceRandomly(&ChessPiece::WR);
      PlaceRandomly(&ChessPiece::BR);
      PlaceRandomly(&ChessPiece::WB);
      PlaceRandomly(&ChessPiece::BB);
      PlaceRandomly(&ChessPiece::WN);
      PlaceRandomly(&ChessPiece::BN);
    }
  }

  bool MCPosition::AddIfLegal(Moves& m, const Location fr, const Location to) const
  {
    const Piece* pf = GetPiece(fr);
    const Piece* pt = GetPiece(to);
    assert(pf != nullptr);                                                // start field must exist
    if (pt == nullptr) return false;                                      // out of board
    assert(!pf->IsBlank());                                               // start field must be a piece
    if (pt->IsBlank()) return true;                                       // not a move, but keep trying this direction
    if (pt->GetColor() == pf->GetColor()) return false;                   // own piece; don't keep trying this direction

    // valid move, save into collection
    m.push_back(std::make_shared<SimpleMove>(std::make_shared < StepSimple>( Field{fr,pf},Field{to,pf},StepSimple::StepType::Take/*,Fields{Field{to,pt}}*/ )));
    return false;                                                         // don't keep trying this direction
  };

  bool MCPosition::PlaceRandomly(const Piece* p)
  {
    std::vector<Location> ll;
    for (Coordinate i = 0; i < sizeX; i++)
      for (Coordinate j = 0; j < sizeY; j++)
      {
        const Location l{ BoardPart::Main,  i,j };
        if (GetPiece(l) == &Piece::NoPiece) ll.push_back(l);
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
    p->Add(&ChessPiece::WQ);
    p->Add(&ChessPiece::WR);
    p->Add(&ChessPiece::WB);
    p->Add(&ChessPiece::WN);
    p->Add(&ChessPiece::BQ);
    p->Add(&ChessPiece::BR);
    p->Add(&ChessPiece::BB);
    p->Add(&ChessPiece::BN);
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
