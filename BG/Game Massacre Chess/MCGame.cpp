#include "stdafx.h"

#include "MCResource.h"
#include "MCGame.h"

namespace MassacreChess
{
  MCPosition::MCPosition(unsigned int x, unsigned int y) : MainPosition(x, y)
  {
    if (x == x)
    {
      SetPiece(Location{ 0,0 }, &ChessPiece::WR);
      SetPiece(Location{ 0,1 }, &ChessPiece::BR);
      SetPiece(Location{ 0,2 }, &ChessPiece::BQ);
      SetPiece(Location{ 0,3 }, &ChessPiece::BB);
      SetPiece(Location{ 1,2 }, &ChessPiece::WB);
      SetPiece(Location{ 2,0 }, &ChessPiece::WN);
      SetPiece(Location{ 3,0 }, &ChessPiece::WN);
      SetPiece(Location{ 3,3 }, &ChessPiece::BN);
      return;
    }

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

  bool MCPosition::AddIfLegal(std::vector<Move>& m, const Location fr, const Location to) const
  {
    const Piece * pf = GetPiece(fr);
    const Piece * pt = GetPiece(to);
    assert(pf != nullptr);                                                // start field must exist
    if (pt == nullptr) return false;                                      // out of board
    assert(!pf->IsBlank());                                               // start field must be a piece
    if (pt->IsBlank()) return true;                                       // not a move, but keep trying this direction
    if (pt->GetColor() == pf->GetColor()) return false;                   // own piece; don't keep trying this direction

    // valid move, save into collection
    m.push_back(Step{ Field{fr,pf},Field{to,pf},Step::StepType::Take,std::vector<Field>{Field{to,pt}} });
    return false;                                                         // don't keep trying this direction
  };

  bool MCPosition::PlaceRandomly(const Piece* p)
  {
    std::vector<Location> ll;
    for (unsigned int i = 0; i < sizeX; i++)
      for (unsigned int j = 0; j < sizeY; j++)
      {
        const Location l{ i,j };
        if (GetPiece(l) == &Piece::NoPiece) ll.push_back(l);
      }
    if (ll.size() == 0) return false;
    const unsigned int z = rand() % ll.size();
    SetPiece(ll[z], p);
    return true;
  }

  void MCPosition::EvaluateStatically(void)   // as seen from White
  {
    // default evaluation: count all material, and add difference of move count. Overwrite for each game as needed
    GetAllMoves();                                                        // fill the move lists
    if (onTurn == &Color::White && movelistW.size() == 0) value = PositionValue::PValueType::Lost;        // if no more moves, game over
    else if (onTurn == &Color::Black && movelistB.size() == 0) value = PositionValue::PValueType::Won;
    else
    {
      value = (movelistW.size() - movelistB.size()) * 1000; // slightly more than a Q
      for (unsigned int j = 0; j < sizeY; j++)
      {
        for (unsigned int i = 0; i < sizeX; i++)                          // loop through all locations
        {                                                               
          const Piece* p = GetPiece(Location{ i,j });                   
          if (p == nullptr) continue;                                     // field does not exist
          if (p->IsColor(&Color::NoColor)) continue;                      // nothing here
          value += (p->IsColor(&Color::White) ? 1 : -1) * p->GetValue();
        }
      }
    }
  }


  MCGame::MCGame(unsigned int x, unsigned int y) : MCGame(
    new MCPosition(x, y), new TakenPosition(x*y / 2, 2), new StockPosition(5, 2),
    new MCLayout(x, y), new MCTakenLayout(x, y), new MCStockLayout(x, y)) {}

  MCGame::MCGame(MCPosition* p, TakenPosition* t, StockPosition* s,
    MCLayout* l, MCTakenLayout* tl, MCStockLayout* sl) : Game{ p,t,s,l,tl,sl }
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

}
