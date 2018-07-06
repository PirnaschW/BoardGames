#include "stdafx.h"

#include "LoAResource.h"
#include "LoAGame.h"

namespace LoA
{
  const LoAPiece LoAPiece::LoAPieceB{ &LoAPeg::ThePeg, &Color::Black, IDB_LOAPEGB, IDB_LOAPEGBF };
  const LoAPiece LoAPiece::LoAPieceW{ &LoAPeg::ThePeg, &Color::White, IDB_LOAPEGW, IDB_LOAPEGWF };


  void LoAPosition::EvaluateStatically(void)
  {
    if (IsConnected(false))
    {
      value = PositionValue::PValueType::Lost;  // opponent is connected -> loss
      return;
    }
    if (IsConnected(true))
    {
      value = PositionValue::PValueType::Won;   // player is connected -> win
      return;
    }

    GetAllMoves();                                                        // fill the move lists
    if (onTurn == &Color::White && movelistW.empty()) value = PositionValue::PValueType::Lost;        // if no more moves, game over
    else if (onTurn == &Color::Black && movelistB.empty()) value = PositionValue::PValueType::Won;
    else
    {

      int v1{ 0 };
      int v2{ 0 };

      for (unsigned int i = 0; i < sizeX; i++)
      {
        for (unsigned int j = 0; j < sizeY; j++)
        {
          const Piece* p = GetPiece(Location{ i,j });
          if (p->IsColor(&Color::NoColor)) continue;
          int d{ 0 };
          for (unsigned int z = 0; z < (sizeX - 1) / 2; z++)
          {
            if (i > z && i < sizeX - 1 - z) d++;
            if (j > z && j < sizeY - 1 - z) d++;
          }
          int v{ 0 };
          switch (d)
          {
            case 0: v += -5; break;
            case 1: v += -2; break;
            case 2: v += -1; break;
            case 3: v += 0; break;
            case 4: v += 1; break;
            case 5: v += 2; break;
            case 6: v += 4; break;
            default: v += 6; break;
          }
          if (p->IsColor(OnTurn())) v1 += v;
          else v2 += v;
        }
      }
      value = v1 - v2;
    }
  }


  LoALayout::LoALayout(unsigned int x, unsigned int y) noexcept :
    MainLayout(Dimension(x, y, BoardStartX, BoardStartY, FieldSizeX, FieldSizeY), LayoutType::Light) {}

  LoATakenLayout::LoATakenLayout(unsigned int x, unsigned int y) noexcept :
    TakenLayout(Dimension(2 * x, 2, FieldSizeX * (x + 1), BoardStartY + FieldSizeSY, FieldSizeSX, FieldSizeSY, 0, FieldSizeY * y - FieldSizeSY * 4)) {}

  LoAStockLayout::LoAStockLayout(unsigned int x, unsigned int y) noexcept :
    StockLayout(Dimension(3, 1, BoardStartX + FieldSizeX * (x + 1), BoardStartY + FieldSizeY / 2 + FieldSizeY * (y - 2), FieldSizeX, FieldSizeY)) {}

}
