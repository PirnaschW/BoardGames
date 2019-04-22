#include "stdafx.h"

#include "MCResource.h"
#include "MCGame.h"

namespace MassacreChess
{
  MCPosition::MCPosition(Coordinate x, Coordinate y) : MainPosition(x, y)
  {
    //srand((unsigned)time(NULL));
    std::srand(1);

    for (unsigned int z = 0; z < x*y / 8; z++)
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
    m.push_back(std::make_shared<SimpleMove>(std::make_shared < SimpleStep>( Field{fr,pf},Field{to,pf},SimpleStep::StepType::Take/*,std::vector<Field>{Field{to,pt}}*/ )));
    return false;                                                         // don't keep trying this direction
  };

  bool MCPosition::PlaceRandomly(const Piece* p)
  {
    std::vector<Location> ll;
    for (Coordinate i = 0; i < sizeX; i++)
      for (Coordinate j = 0; j < sizeY; j++)
      {
        const Location l{ i,j };
        if (GetPiece(l) == &Piece::NoPiece) ll.push_back(l);
      }
    if (ll.empty()) return false;
    const unsigned int z = rand() % ll.size();
    SetPiece(ll[z], p);
    return true;
  }


  MCGame::MCGame(Coordinate x, Coordinate y) noexcept : MCGame(
    new MCPosition(x, y), new TakenPosition(x*y / 2, 2), new StockPosition(5, 2),
    new MCLayout(x, y), new MCTakenLayout(x, y), new MCStockLayout(x, y)) {}

  MCGame::MCGame(MCPosition* p, TakenPosition* t, StockPosition* s,
    MCLayout* l, MCTakenLayout* tl, MCStockLayout* sl) noexcept : Game{ p,t,s,l,tl,sl }
  {
    AddToStock(Location(0, 0), &ChessPiece::WQ);
    AddToStock(Location(1, 0), &ChessPiece::WR);
    AddToStock(Location(2, 0), &ChessPiece::WB);
    AddToStock(Location(3, 0), &ChessPiece::WN);
    AddToStock(Location(0, 1), &ChessPiece::BQ);
    AddToStock(Location(1, 1), &ChessPiece::BR);
    AddToStock(Location(2, 1), &ChessPiece::BB);
    AddToStock(Location(3, 1), &ChessPiece::BN);
  }

  const VariantList& MCGame::GetVariants(void) noexcept
  {
    static VariantList v{ { Variant{ 8, 8, nullptr, 2, 20 } } };
    return v;
  }

}
