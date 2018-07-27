#include "stdafx.h"

#include "TicTacToeGame.h"

namespace TicTacToe
{

  void TicTacToePosition::GetAllMoves(void) // collect all moves
  {
    assert(movelistW.empty());
    assert(movelistB.empty());
    for (Coordinate i = 0; i < sizeX; i++)
    {
      for (Coordinate j = 0; j < sizeY; j++)
      {
        const Location l{ i,j };
        const Piece* p = GetPiece(l);
        assert(p != nullptr);
        if (p->IsBlank())
        {
          movelistW.push_back(Step{ Field{ l,&Piece::NoPiece }, Field{ l,&TicTacToePiece::TicTacToePieceW },Step::StepType::Place });
          movelistB.push_back(Step{ Field{ l,&Piece::NoPiece }, Field{ l,&TicTacToePiece::TicTacToePieceB },Step::StepType::Place });
        }
      }
    }
  }


  //bool TicTacToePosition::AddIfLegal(std::vector<Move>& m, const Location fr, const Location to) const
  //{
  //  assert(fr == to);                   // this game allows only placements
  //  const Piece* p = GetPiece(fr);
  //  if (!p->IsBlank()) return false;    // field must be empty
  //  m.push_back(Step{ Field{ fr,&Piece::NoPiece }, Field{ fr,&TicTacToePiece::TicTacToePieceW },Step::StepType::Place });
  //  return false;
  //};

  void TicTacToePosition::EvaluateStatically(void)
  {
    int v1{ 0 };
    int v2{ 0 };

    char posi[13]{};
    for (Coordinate j = 0; j < sizeY; j++)
    {
      for (Coordinate i = 0; i < sizeX; i++)  // loop through all locations
      {
        const Location l{ i,j };
        const Piece* p = GetPiece(l);
        char c = '-';
        if (p->IsColor(&Color::White)) c = 'W';
        if (p->IsColor(&Color::Black)) c = 'B';
        posi[i + 4 * j] = c;
      }
      posi[sizeX + 4 * j] = '|';
    }

    GetAllMoves();                                                        // fill the move lists
    if (onTurn == &Color::White && movelistW.empty()) value = PositionValue::PValueType::Tie;        // if no more moves, game over
    else if (onTurn == &Color::Black && movelistB.empty()) value = PositionValue::PValueType::Tie;
    else
    {
      for (Coordinate j = 0; j < sizeY; j++)
      {
        for (Coordinate i = 0; i < sizeX; i++)  // loop through all locations
        {
          const Location l{ i,j };
          const Piece* p = GetPiece(l);
          if (p->IsColor(&Color::NoColor)) continue;  // nothing here, so no chain can start
          const bool w = p->IsColor(&Color::White);

          for (unsigned int k = 0; k < 4; k++)
          {
            const Offset& d = Offset::Qdirection[k];
            const Piece* pp{ GetPiece(l + d * -1) };
            if (pp != nullptr && pp->IsColor(p->GetColor())) continue;    // if same color is that direction, we counted it already, so move on
            if (pp != nullptr && pp->IsBlank()) (w ? v1 : v2) += 1;       // free field, give an extra point - much better than opponent's piece
            Location ll{ i,j };
            unsigned int z{ 1 };
            while ((pp = GetPiece(ll += d)) != nullptr)
            {
              if (pp->IsColor(p->GetColor())) z++;
              else
              {
                if (pp->IsColor(&Color::NoColor)) (w ? v1 : v2) += 1;     // if line ends with free field, give an extra point - much better than opponent's piece
                break;
              }
            }
            if (z >= sizeY || z >= sizeX)
            {
              value = w ? PositionValue::PValueType::Won : PositionValue::PValueType::Lost;
              return;
            }
            (w ? v1 : v2) += GetValue(z);
          }
        }
      }

      value = v1 - v2;
    }
  }

  unsigned int TicTacToePosition::GetChainValue(unsigned int z) const noexcept
  {
    switch (z)
    {
      case 0:  return  0;
      case 1:  return  1;
      case 2:  return  4;
      default: return  4;
    }
  }


  TicTacToeGame::TicTacToeGame(Coordinate x, Coordinate y) noexcept : TicTacToeGame(
    new TicTacToePosition(x, y), new TakenPosition(x, 2), new StockPosition(3, 1),
    new TicTacToeLayout(x, y), new TicTacToeTakenLayout(x, y), new TicTacToeStockLayout(x, y)) {}

  TicTacToeGame::TicTacToeGame(TicTacToePosition* p, TakenPosition* t, StockPosition* s,
    TicTacToeLayout* l, TicTacToeTakenLayout* tl, TicTacToeStockLayout* sl) noexcept : Game{ p,t,s,l,tl,sl,true }
  {
    AddToStock(Location(0, 0), &TicTacToePiece::TicTacToePieceW);
    AddToStock(Location(1, 0), &TicTacToePiece::TicTacToePieceB);
  }

  const VariantList& TicTacToeGame::GetVariants(void) noexcept
  {
    static VariantList v{ Variant{ 3, 3 } };
    return v;
  }

}
