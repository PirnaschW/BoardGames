#include "pch.h"

#include "HasamiGame.h"

namespace Hasami
{
  inline const Checker Checker::TheChecker{};
  inline const HasamiPiece HasamiPiece::HasamiPieceB{ Checker::TheChecker, Color::Black, IDB_HASAMI_B, IDB_HASAMI_BS };
  inline const HasamiPiece HasamiPiece::HasamiPieceW{ Checker::TheChecker, Color::White, IDB_HASAMI_W, IDB_HASAMI_WS };

  void Checker::CollectMoves(const MainPosition& pos, const Location& l, Moves& moves) const noexcept
  {
    for (auto& d : Offset::Rdirection)
    {
      const Piece& p1 = pos.GetPiece(l + d);
      if (p1 != Piece::NoTile)                    // on the board
      {
        if (p1.IsBlank())                  // free
        {
          // check for normal movement
          for (int z = 1; pos.AddIfLegal(moves, l, l + d * z); z++);
        }
        else
        {
          // check for jumps
          if (!p1.IsColor(pos.OnTurn()))   // not own piece
          {
            pos.AddIfLegal(moves, l, l + d * 2);
          }
        }
      }
    }
  }


  HasamiPosition::HasamiPosition(const PieceMapP& p, const Dimensions& d) noexcept : MainPosition(p, d)
  {
    for (Coordinate i = 0; i < d[0].xCount_; i++)
    {
      SetPiece(Location(BoardPart::Main, i, 0U), HasamiPiece::HasamiPieceB);
      SetPiece(Location(BoardPart::Main, i, 1U), HasamiPiece::HasamiPieceB);
      SetPiece(Location(BoardPart::Main, i, d[0].yCount_ - 1), HasamiPiece::HasamiPieceW);
      SetPiece(Location(BoardPart::Main, i, d[0].yCount_ - 2), HasamiPiece::HasamiPieceW);
    }
  }

  bool HasamiPosition::AddIfLegal(Moves& m, const Location& fr, const Location& to) const noexcept
  {
    const Piece& p = GetPiece(to);
    if (p == Piece::NoTile) return false;                                // out of board
    if (!p.IsBlank()) return false;                                      // occupied

    Actions a{};
    a.push_back(std::make_shared<ActionLift>(fr, p));                     // pick piece up

    for (auto& d : Offset::Rdirection)
    {
      Actions ad{};
      Location l = to;
      const Piece* pp{ &Piece::NoTile };
      while ((pp = &(GetPiece(l += d))) != &Piece::NoTile)
      {
        if (pp->IsBlank()) break;                                         // nothing comes from this direction
        if (l == fr) break;                                               // was this going backwards?
        if (pp->GetColor() != p.GetColor())                              // opponent's piece? Take it tentatively
        {
          ad.push_back(std::make_shared<ActionLift>(l, *pp));              // pick opponents piece up
          ad.push_back(std::make_shared<ActionDrop>(GetNextTakenL(p.GetColor()), *pp));            // and place it to Taken
        }
        else a.insert(a.end(),ad.begin(),ad.end());                       // own piece; tentative takes become real
      }
    }

    a.push_back(std::make_shared<ActionDrop>(to, p));                    // and place it on target
    m.push_back(std::make_shared<Move>(a));                               // add move to move list
    return true;
  }

  PositionValue HasamiPosition::EvaluateStatically(void) const noexcept
  {
    GetAllMoves();                                                        // fill the move lists
    if (onTurn_ == &Color::White && movesW_.empty()) return PositionValue::PValueType::Tie;        // if no more moves, game over
    if (onTurn_ == &Color::Black && movesB_.empty()) return PositionValue::PValueType::Tie;

    int v1{ 0 };
    int v2{ 0 };

    for (Coordinate j = 0; j < sizeY_; j++)
    {
      const bool b1{ OnTurn() == Color::White ? j < sizeY_ - 2 : j > 1 };  // limits for the player on turn
      const bool b2{ OnTurn() == Color::White ? j > 1 : j < sizeY_ - 2 };  // limits for the player not on turn
      for (Coordinate i = 0; i < sizeX_; i++)  // loop through all locations
      {
        const Location l{ BoardPart::Main, i,j };
        const Piece& p = GetPiece(l);
        if (p.IsColor(Color::NoColor)) continue;  // nothing here, so no chain can start
        const bool w = p.IsColor(Color::White);

        for (unsigned int k = 0; k < 4; k++)
        {
          if (k == 1) continue; // horizontal is not allowed

          const Offset& d = Offset::Qdirection[k];
          const Piece* pp{ &GetPiece(l + d * -1) };
          if (*pp != Piece::NoTile && pp->IsColor(p.GetColor())) continue;    // if same color is that direction, we counted it already, so move on
          Location ll{ BoardPart::Main,  i,j };
          unsigned int z{ 0 };
          if (p.IsColor(OnTurn()) ? b1 : b2) z++;  // count the starting checker only if it is in the valid range

          while ((pp = &GetPiece(ll += d)) != nullptr)
          {
            if (pp->IsColor(p.GetColor()))
            {
              if (p.IsColor(OnTurn()) ? b1 : b2) z++;   // count each checker only if it is in the valid range
            }
            else
            {
              if (pp->IsColor(Color::NoColor))
              {
                (p.IsColor(OnTurn()) ? v1 : v2) += 1;    // if line ends with free field, give an extra point - much better than opponent's piece
              }
              break;
            }
          }
          if (z >= 5) return w ? PositionValue::PValueType::Won : PositionValue::PValueType::Lost;
          (w ? v1 : v2) += GetChainValue(z);
        }
      }
    }
    return v1 - v2;
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

  const VariantList& HasamiGame::GetVariants(void) noexcept
  {
    static VariantList v{ { Variant{ 9, 9, nullptr, 2, 20, 5, 20 } } };
    return v;
  }

  const PieceMapP& HasamiGame::GetPieces(void) noexcept
  {
    static const PieceMapP& p = std::make_shared<PieceMap>();
    p->Add(HasamiPiece::HasamiPieceW);
    p->Add(HasamiPiece::HasamiPieceB);
    return p;
  }

  const Dimensions& HasamiGame::GetDimensions(Coordinate x, Coordinate y) noexcept
  {
    static Dimensions d{
       Dimension(x, y, BoardStartX, BoardStartY, FieldSizeX, FieldSizeY, 1, 1),
       Dimension(3, 1, BoardStartX + FieldSizeX * (x + 1), BoardStartY + FieldSizeY / 2 + FieldSizeY * (y - 2), FieldSizeX, FieldSizeY),
       Dimension(2 * x, 2, FieldSizeX * (x + 1), BoardStartY + FieldSizeSY, FieldSizeSX, FieldSizeSY, 0, FieldSizeY * y - FieldSizeSY * 4),
    };
    return d;
  }

}
