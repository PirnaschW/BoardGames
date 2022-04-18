#include "Games.h"

#include "HasamiResource.h"
#include "HasamiGame.h"

namespace Hasami
{
  const Checker Checker::TheChecker{};
  const HasamiPiece HasamiPiece::HasamiPieceB{ Checker::TheChecker, PieceColor::Black, IDB_HASAMI_B };
  const HasamiPiece HasamiPiece::HasamiPieceW{ Checker::TheChecker, PieceColor::White, IDB_HASAMI_W };

  void Checker::CollectMoves(const Board& board_, const Location& l, Moves& moves) const noexcept
  {
    for (auto& d : Offset::RDirection)
    {
      const Piece& p1 = board_.GetPieceIndex(l + d);
      if (p1 != Piece::NoTile)                    // on the board
      {
        if (p1.IsBlank())                  // free
        {
          // check for normal movement
          for (int z = 1; board_.AddIfLegal(moves, l, l + d * z); z++);
        }
        else
        {
          // check for jumps
          if (!p1.IsColor(board_.OnTurn()))   // not own piece
          {
            board_.AddIfLegal(moves, l, l + d * 2);
          }
        }
      }
    }
  }


  void HasamiBoard::SetStartingBoard() noexcept
  {
    for (Coordinate i = 0; i < sizeX_; i++)
    {
      SetPieceIndex(Location(BoardPartID::Stage, i, 0U        ), HasamiPiece::HasamiPieceB);
      SetPieceIndex(Location(BoardPartID::Stage, i, 1U        ), HasamiPiece::HasamiPieceB);
      SetPieceIndex(Location(BoardPartID::Stage, i, sizeY_ - 1), HasamiPiece::HasamiPieceW);
      SetPieceIndex(Location(BoardPartID::Stage, i, sizeY_ - 2), HasamiPiece::HasamiPieceW);
    }
  }

  bool HasamiBoard::AddIfLegal(Moves& m, const Location& fr, const Location& to) const noexcept
  {
    const Piece& p = GetPieceIndex(to);
    if (p == Piece::NoTile) return false;                                // out of board
    if (!p.IsBlank()) return false;                                      // occupied

    Actions a{};
    a.push_back(std::make_shared<ActionLift>(fr, p));                     // pick piece up

    for (auto& d : Offset::RDirection)
    {
      Actions ad{};
      Location l = to;
      const Piece* pp{ &Piece::NoTile };
      while ((pp = &(GetPieceIndex(l += d))) != &Piece::NoTile)
      {
        if (pp->IsBlank()) break;                                         // nothing comes from this direction
        if (l == fr) break;                                               // was this going backwards?
        if (pp->GetColor() != p.GetColor())                              // opponent's piece? Take it tentatively
        {
          ad.push_back(std::make_shared<ActionLift>(l, *pp));              // pick opponents piece up
          ad.push_back(std::make_shared<ActionDrop>(GetNextFreeTakenLocation(p.GetColor()), *pp));            // and place it to Taken
        }
        else a.insert(a.end(),ad.begin(),ad.end());                       // own piece; tentative takes become real
      }
    }

    a.push_back(std::make_shared<ActionDrop>(to, p));                    // and place it on target
    m.push_back(std::make_shared<Move>(a));                               // add move to move list
    return true;
  }

  PositionValue HasamiBoard::EvaluateStatically() const noexcept
  {
    GetAllMoves();                                                        // fill the move lists
    if (onTurn_ == &PieceColor::White && movesW_.empty()) return PositionValue::PValueType::Tie;        // if no more moves, game over
    if (onTurn_ == &PieceColor::Black && movesB_.empty()) return PositionValue::PValueType::Tie;

    int v1{ 0 };
    int v2{ 0 };

    for (Coordinate j = 0; j < sizeY_; j++)
    {
      const bool b1{ OnTurn() == PieceColor::White ? j < sizeY_ - 2 : j > 1 };  // limits for the player on turn
      const bool b2{ OnTurn() == PieceColor::White ? j > 1 : j < sizeY_ - 2 };  // limits for the player not on turn
      for (Coordinate i = 0; i < sizeX_; i++)  // loop through all locations
      {
        const Location l{ BoardPartID::Stage, i,j };
        const Piece& p = GetPieceIndex(l);
        if (p.IsColor(PieceColor::NoColor)) continue;  // nothing here, so no chain can start
        const bool w = p.IsColor(PieceColor::White);

        for (unsigned int k = 0; k < 4; k++)
        {
          if (k == 1) continue; // horizontal is not allowed

          const Offset& d = Offset::QDirection[k];
          const Piece* pp{ &GetPieceIndex(l + d * -1) };
          if (*pp != Piece::NoTile && pp->IsColor(p.GetColor())) continue;    // if same color is that direction, we counted it already, so move on
          Location ll{ BoardPartID::Stage,  i,j };
          unsigned int z{ 0 };
          if (p.IsColor(OnTurn()) ? b1 : b2) z++;  // count the starting checker only if it is in the valid range

          while ((pp = &GetPieceIndex(ll += d)) != nullptr)
          {
            if (pp->IsColor(p.GetColor()))
            {
              if (p.IsColor(OnTurn()) ? b1 : b2) z++;   // count each checker only if it is in the valid range
            }
            else
            {
              if (pp->IsColor(PieceColor::NoColor))
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

  const VariantList& HasamiGame::GetVariants() noexcept
  {
    static VariantList v{
      { Variant{ nullptr, 'A', 9, 9, 2, 20, 5, 20 } },
    };
    return v;
  }

  const PieceMapP& HasamiGame::GetPieces(const VariantChosen& v) noexcept
  {
    static const PieceMapP& p = std::make_shared<PieceMap>();
    p->Add(HasamiPiece::HasamiPieceW);
    p->Add(HasamiPiece::HasamiPieceB);
    return p;
  }

  const BoardPartDimensions HasamiGame::GetDimensions(const VariantChosen& v) noexcept
  {
    BoardPartDimensions d{
       BoardPartDimension(v.x, v.y, BoardStartX, BoardStartY, FieldSizeX, FieldSizeY, 1, 1),
       BoardPartDimension(3, 1, BoardStartX + FieldSizeX * (v.x + 1), BoardStartY + FieldSizeY / 2 + FieldSizeY * (v.y - 2), FieldSizeX, FieldSizeY),
       BoardPartDimension(2 * v.x, 2, FieldSizeX * (v.x + 1), BoardStartY + FieldSizeSY, FieldSizeSX, FieldSizeSY, 0, FieldSizeY * v.y - FieldSizeSY * 4),
    };
    return d;
  }

}
