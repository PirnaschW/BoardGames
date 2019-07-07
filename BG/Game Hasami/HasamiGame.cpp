#include "stdafx.h"

#include "HasamiGame.h"

namespace Hasami
{
  inline const Checker Checker::TheChecker{};
  inline const HasamiPiece HasamiPiece::HasamiPieceB{ &Checker::TheChecker, &Color::Black, IDB_HASAMI_B, IDB_HASAMI_BS };
  inline const HasamiPiece HasamiPiece::HasamiPieceW{ &Checker::TheChecker, &Color::White, IDB_HASAMI_W, IDB_HASAMI_WS };

  void Checker::CollectMoves(const MainPosition& pos, const Location& l, Moves& moves) const
  {
    for (auto& d : Offset::Rdirection)
    {
      const Piece* p1 = pos.GetPiece(l + d);
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


  HasamiPosition::HasamiPosition(const PieceMapP& p, Coordinate x, Coordinate y) noexcept : MainPosition(p, x, y)
  {
    for (Coordinate i = 0; i < x; i++)
    {
      SetPiece(Location(BoardPart::Main, i, 0U), &HasamiPiece::HasamiPieceB);
      SetPiece(Location(BoardPart::Main, i, 1U), &HasamiPiece::HasamiPieceB);
      SetPiece(Location(BoardPart::Main, i, y - 1), &HasamiPiece::HasamiPieceW);
      SetPiece(Location(BoardPart::Main, i, y - 2), &HasamiPiece::HasamiPieceW);
    }
  }

  bool HasamiPosition::AddIfLegal(Moves& m, const Location fr, const Location to) const
  {
    const Piece* p = GetPiece(to);
    if (p == nullptr) return false;          // out of board
    if (!p->IsBlank()) return false;         // occupied
    StepSimple::StepType st{ StepSimple::StepType::Normal };

    Fields taken{};
    for (auto& d : Offset::Rdirection)
    {
      Fields t{};
      Location l = to;
      const Piece* pp{};
      while ((pp = GetPiece(l += d)) != nullptr)
      {
        if (pp->IsBlank()) break;           // nothing comes from this direction
        if (l == fr)                        // was this going backwards?
        {
          if (t.size() == 1)                // if something was inbetween, it was a jump
            st = StepSimple::StepType::Jump;
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
    m.push_back(std::make_shared<SimpleMove>(std::make_shared<StepSimple>(Field{ fr,GetPiece(fr) }, Field{ to,GetPiece(fr) }, st)));
    return true;
  }

  void HasamiPosition::EvaluateStatically(void)
  {
    GetAllMoves();                                                        // fill the move lists
    if (onTurn == &Color::White && movelistW.empty()) value = PositionValue::PValueType::Tie;        // if no more moves, game over
    else if (onTurn == &Color::Black && movelistB.empty()) value = PositionValue::PValueType::Tie;
    else
    {

      int v1{ 0 };
      int v2{ 0 };

      for (Coordinate j = 0; j < sizeY; j++)
      {
        const bool b1{ OnTurn() == &Color::White ? j < sizeY - 2 : j > 1 };  // limits for the player on turn
        const bool b2{ OnTurn() == &Color::White ? j > 1 : j < sizeY - 2 };  // limits for the player not on turn
        for (Coordinate i = 0; i < sizeX; i++)  // loop through all locations
        {
          const Location l{ BoardPart::Main, i,j };
          const Piece* p = GetPiece(l);
          if (p->IsColor(&Color::NoColor)) continue;  // nothing here, so no chain can start
          const bool w = p->IsColor(&Color::White);

          for (unsigned int k = 0; k < 4; k++)
          {
            if (k == 1) continue; // horizontal is not allowed

            const Offset& d = Offset::Qdirection[k];
            const Piece* pp{ GetPiece(l + d * -1) };
            if (pp != nullptr && pp->IsColor(p->GetColor())) continue;    // if same color is that direction, we counted it already, so move on
            Location ll{ BoardPart::Main,  i,j };
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
            (w ? v1 : v2) += GetChainValue(z);
          }
        }
      }

      value = v1 - v2;
    }
  }

  inline unsigned int HasamiPosition::GetChainValue(unsigned int z) const noexcept
  {
    switch (z)
    {
      case 0:  return    0;
      case 1:  return  100;
      case 2:  return  200;
      case 3:  return  600;
      case 4:  return 2400;
      default: return    0;
    }
  }

  //HasamiGame::HasamiGame(const PieceMapP& m, Coordinate x, Coordinate y) noexcept : HasamiGame(m,
  //  new HasamiPosition(m, x, y), new HasamiTakenPosition(m, x, y), new StockPosition(m, 3, 1),
  //  new HasamiLayout(x, y), new HasamiTakenLayout(x, y), new HasamiStockLayout(x, y)) {}
  HasamiGame::HasamiGame(const PieceMapP& m, Coordinate x, Coordinate y) noexcept : HasamiGame(m, new HasamiPosition(m, x, y), new HasamiLayout(x, y)) {}

  HasamiGame::HasamiGame(const PieceMapP& m, HasamiPosition* p, HasamiLayout* l) noexcept : Game{ m,p,l }
  {
    AddToStock(Location(BoardPart::Stock, 0U, 0U), &HasamiPiece::HasamiPieceW);
    AddToStock(Location(BoardPart::Stock, 1U, 0U), &HasamiPiece::HasamiPieceB);
  }

  const VariantList& HasamiGame::GetVariants(void) noexcept
  {
    static VariantList v{ { Variant{ 9, 9, nullptr, 2, 20, 5, 20 } } };
    return v;
  }

  const PieceMapP& HasamiGame::GetPieces(void) noexcept
  {
    static const PieceMapP& p = std::make_shared<PieceMap>();
    p->Add(&HasamiPiece::HasamiPieceW);
    p->Add(&HasamiPiece::HasamiPieceB);
    return p;
  }

}
