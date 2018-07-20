#include "stdafx.h"

#include "TicTacToeResource.h"
#include "TicTacToeGame.h"

namespace TicTacToe
{
  const TicTacToePiece TicTacToePiece::TicTacToePieceB{&Checker::TheChecker, &Color::Black, IDB_LOAPEGB, IDB_LOAPEGBF};
  const TicTacToePiece TicTacToePiece::TicTacToePieceW{&Checker::TheChecker, &Color::White, IDB_LOAPEGW, IDB_LOAPEGWF};


  TicTacToePosition::TicTacToePosition(unsigned int x, unsigned int y) noexcept : MainPosition(x, y)
  {
  }

  void TicTacToePosition::GetAllMoves(void) // collect all moves
  {
    assert(movelistW.empty());
    assert(movelistB.empty());
    for (unsigned int i = 0; i < sizeX; i++)
    {
      for (unsigned int j = 0; j < sizeY; j++)
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


  bool TicTacToePosition::AddIfLegal(std::vector<Move>& m, const Location fr, const Location to) const
  {
    assert(fr == to);                   // this game allows only placements
    const Piece* p = GetPiece(fr);
    if (!p->IsBlank()) return false;    // field must be empty
    m.push_back(Step{ Field{ fr,&Piece::NoPiece }, Field{ fr,&TicTacToePiece::TicTacToePieceW },Step::StepType::Place });
    return false;
  };

  void TicTacToePosition::EvaluateStatically(void)
  {
    int v1{ 0 };
    int v2{ 0 };

    char posi[13]{};
    for (unsigned int j = 0; j < sizeY; j++)
    {
      for (unsigned int i = 0; i < sizeX; i++)  // loop through all locations
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
      for (unsigned int j = 0; j < sizeY; j++)
      {
        for (unsigned int i = 0; i < sizeX; i++)  // loop through all locations
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

  unsigned int TicTacToePosition::GetValue(unsigned int z) const noexcept
  {
    switch (z)
    {
      case 0:  return  0;
      case 1:  return  1;
      case 2:  return  4;
      default: return  4;
    }
  }

  TicTacToeLayout::TicTacToeLayout(unsigned int x, unsigned int y) noexcept :
    MainLayout(Dimension(x, y, BoardStartX, BoardStartY, FieldSizeX, FieldSizeY, 1, 1), Layout::LayoutType::Light) {}

}
