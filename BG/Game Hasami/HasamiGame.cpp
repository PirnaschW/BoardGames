#include "stdafx.h"

#include "HasamiResource.h"
#include "HasamiGame.h"

namespace Hasami
{

  const HasamiPiece HasamiPiece::HasamiPieceB{&Checker::TheChecker, &Color::Black, IDB_HASAMI_B, IDB_HASAMI_BS};
  const HasamiPiece HasamiPiece::HasamiPieceW{&Checker::TheChecker, &Color::White, IDB_HASAMI_W, IDB_HASAMI_WS};


  void Checker::CollectMoves(const MainPosition& pos, const Location& l, std::vector<Move>& moves) const
  {
    for (auto& d : Offset::Rdirection)
    {
      const Piece * p1 = pos.GetPiece(l + d);
      if (p1 != nullptr)                    // on the board
      {
        if (p1->IsBlank())                  // free
        {
          // check for normal movement
          for (int z = 1; pos.AddIfLegal(moves, l, l + d * z); z++);
        }
        else
        {
          // check for jumps
          if (!p1->IsColor(pos.OnTurn()))   // not own piece
          {
            pos.AddIfLegal(moves, l, l + d * 2);
          }
        }
      }
    }
  }


  HasamiPosition::HasamiPosition(unsigned int x, unsigned int y) noexcept : MainPosition(x, y)
  {
    for (unsigned int i = 0; i < x; i++)
    {
      SetPiece(Location(i, 0), &HasamiPiece::HasamiPieceB);
      SetPiece(Location(i, 1), &HasamiPiece::HasamiPieceB);
      SetPiece(Location(i, y - 1), &HasamiPiece::HasamiPieceW);
      SetPiece(Location(i, y - 2), &HasamiPiece::HasamiPieceW);
    }
  }

  bool HasamiPosition::AddIfLegal(std::vector<Move>& m, const Location fr, const Location to) const
  {
    const Piece * p = GetPiece(to);
    if (p == nullptr) return false;          // out of board
    if (!p->IsBlank()) return false;         // occupied
    Step::StepType st{Step::StepType::Normal};

    std::vector<Field> taken{};
    for (auto& d : Offset::Rdirection)
    {
      std::vector<Field> t{};
      Location l = to;
      const Piece * pp{};
      while ((pp = GetPiece(l += d)) != nullptr)
      {
        if (pp->IsBlank()) break;           // nothing comes from this direction
        if (l == fr)                        // was this going backwards?
        {                                   
          if (t.size() == 1)                // if something was inbetween, it was a jump
            st = Step::StepType::Jump;
          break;                            // either way, stop looking into this direction
        }
        if (pp->IsColor(OnTurn()))          // own piece; this 'takes' all intermediate pieces
        {
          taken.insert(taken.end(), std::make_move_iterator(t.begin()), std::make_move_iterator(t.end()));
          break;
        }
        else t.push_back(Field(l, pp));     // opponents piece, add to potential taken list
      }
    }
    m.push_back(Step{Field{fr,GetPiece(fr)}, Field{to,GetPiece(fr)},st,taken});
    return true;
  }

  void HasamiPosition::EvaluateStatically(void)
  {
    GetAllMoves();                                                        // fill the move lists
    if (onTurn == &Color::White && movelistW.size() == 0) value = PositionValue::PValueType::Tie;        // if no more moves, game over
    else if (onTurn == &Color::Black && movelistB.size() == 0) value = PositionValue::PValueType::Tie;
    else
    {

      int v1{ 0 };
      int v2{ 0 };

      for (unsigned int j = 0; j < sizeY; j++)
      {
        const bool b1{ OnTurn() == &Color::White ? j < sizeY - 2 : j > 1 };  // limits for the player on turn
        const bool b2{ OnTurn() == &Color::White ? j > 1 : j < sizeY - 2 };  // limits for the player not on turn
        for (unsigned int i = 0; i < sizeX; i++)  // loop through all locations
        {
          const Location l{ i,j };
          const Piece* p = GetPiece(l);
          if (p->IsColor(&Color::NoColor)) continue;  // nothing here, so no chain can start
          const bool w = p->IsColor(&Color::White);

          for (unsigned int k = 0; k < 4; k++)
          {
            if (k == 1) continue; // horizontal is not allowed

            const Offset& d = Offset::Qdirection[k];
            const Piece * pp{ GetPiece(l + d * -1) };
            if (pp != nullptr && pp->IsColor(p->GetColor())) continue;    // if same color is that direction, we counted it already, so move on
            Location ll{ i,j };
            unsigned int z{ 0 };
            if (p->IsColor(OnTurn()) ? b1 : b2) z++;  // count the starting checker only if it is in the valid range

            while ((pp = GetPiece(ll += d)) != nullptr)
            {
              if (pp->IsColor(p->GetColor()))
              {
                if (p->IsColor(OnTurn()) ? b1 : b2) z++;   // count each checker only if it is in the valid range
              }
              else
              {
                if (pp->IsColor(&Color::NoColor))
                {
                  (p->IsColor(OnTurn()) ? v1 : v2) += 1;    // if line ends with free field, give an extra point - much better than opponent's piece
                }
                break;
              }
            }
            if (z >= 5)
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

  inline unsigned int HasamiPosition::GetValue(unsigned int z) const noexcept
  {
    switch (z)
    {
      case 0:  return   0;
      case 1:  return  10;
      case 2:  return  20;
      case 3:  return  60;
      case 4:  return 240;
      default: return   0;
    }
  }


  HasamiGame::HasamiGame(HasamiPosition* p, TakenPosition* t, StockPosition* s,
    HasamiLayout* l, HasamiTakenLayout* tl, HasamiStockLayout* sl) noexcept : Game{p,t,s,l,tl,sl}
  {
    AddToStock(Location(0, 0), &HasamiPiece::HasamiPieceW);
    AddToStock(Location(1, 0), &HasamiPiece::HasamiPieceB);
  }

}
