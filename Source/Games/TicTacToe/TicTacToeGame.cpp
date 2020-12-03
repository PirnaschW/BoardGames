#include "Games.h"

#include "TicTacToeGame.h"

namespace TicTacToe
{
  const Checker Checker::TheChecker;
  const TicTacToePiece TicTacToePiece::TicTacToePieceB{ Checker::TheChecker, PieceColor::Black, IDB_BCL };
  const TicTacToePiece TicTacToePiece::TicTacToePieceW{ Checker::TheChecker, PieceColor::White, IDB_WCL };

  void TicTacToePosition::GetAllMoves(void) const noexcept                      // collect all moves
  {
    assert(movesW_.empty());
    assert(movesB_.empty());

    Actions aw0{};
    Actions ab0{};
    aw0.push_back(std::make_shared<ActionLift>(Location{ BoardPart::Stock, 0U, 0U }, TicTacToePiece::TicTacToePieceW));
    ab0.push_back(std::make_shared<ActionLift>(Location{ BoardPart::Stock, 0U, 1U }, TicTacToePiece::TicTacToePieceB));
    for (Coordinate i = 0; i < sizeX_; i++)
    {
      for (Coordinate j = 0; j < sizeY_; j++)
      {
        const Location l{ BoardPart::Main, i, j };
        const Piece& p = GetPiece(l);
        assert(p != Piece::NoTile);
        if (p.IsBlank())
        {
          Actions aw{ aw0 };
          Actions ab{ ab0 };
          aw.push_back(std::make_shared<ActionDrop>(l, TicTacToePiece::TicTacToePieceW));
          ab.push_back(std::make_shared<ActionDrop>(l, TicTacToePiece::TicTacToePieceB));
          movesW_.push_back(std::make_shared<Move>(aw));
          movesB_.push_back(std::make_shared<Move>(ab));
        }
      }
    }
  }


  //bool TicTacToePosition::AddIfLegal(Moves& m, const Location fr, const Location to) const
  //{
  //  assert(fr == to);                   // this game allows only placements
  //  const Piece& p = GetPiece(fr);
  //  if (!p->IsBlank()) return false;    // field must be empty
  //  m.push_back(StepSimple{ Field{ fr,&Piece::NoPiece }, Field{ fr,&TicTacToePiece::TicTacToePieceW },StepSimple::StepType::Place });
  //  return false;
  //};

  PositionValue TicTacToePosition::EvaluateStatically(void) const noexcept
  {
    int v1{ 0 };
    int v2{ 0 };

    char posi[13]{};
    for (Coordinate j = 0; j < sizeY_; j++)
    {
      for (Coordinate i = 0; i < sizeX_; i++)  // loop through all locations
      {
        const Location l{ BoardPart::Main,i,j };
        const Piece& p = GetPiece(l);
        char c = '-';
        if (p.IsColor(PieceColor::White)) c = 'W';
        if (p.IsColor(PieceColor::Black)) c = 'B';
        posi[i + 4 * j] = c;
      }
      posi[sizeX_ + 4 * j] = '|';
    }

    GetAllMoves();                                                        // fill the move lists
    if (onTurn_ == &PieceColor::White && movesW_.empty()) return PositionValue::PValueType::Tie;        // if no more moves, game over
    if (onTurn_ == &PieceColor::Black && movesB_.empty()) return PositionValue::PValueType::Tie;
    for (Coordinate j = 0; j < sizeY_; j++)
    {
      for (Coordinate i = 0; i < sizeX_; i++)  // loop through all locations
      {
        const Location l{ BoardPart::Main,i,j };
        const Piece& p = GetPiece(l);
        if (p.IsColor(PieceColor::NoColor)) continue;  // nothing here, so no chain can start
        const bool w = p.IsColor(PieceColor::White);

        for (const Offset& d : Offset::Qdirection)
        {
          const Piece* pp{ &GetPiece(l + d * -1) };
          if (pp != &Piece::NoTile && pp->IsColor(p.GetColor())) continue;    // if same color is that direction, we counted it already, so move on
          if (pp != &Piece::NoTile && pp->IsBlank()) (w ? v1 : v2) += 1;       // free field, give an extra point - much better than opponent's piece
          Location ll{ BoardPart::Main,i,j };
          unsigned int z{ 1 };
          while ((pp = &GetPiece(ll += d)) != nullptr)
          {
            if (pp->IsColor(p.GetColor())) z++;
            else
            {
              if (pp->IsColor(PieceColor::NoColor)) (w ? v1 : v2) += 1;     // if line ends with free field, give an extra point - much better than opponent's piece
              break;
            }
          }
          if (z >= sizeY_ || z >= sizeX_) return w ? PositionValue::PValueType::Won : PositionValue::PValueType::Lost;
          (w ? v1 : v2) += GetChainValue(z);
        }
      }
    }

    return v1 - v2;
  }

  unsigned int TicTacToePosition::GetChainValue(unsigned int z) const noexcept
  {
    switch (z)
    {
      case 0:  return  1;
      case 1:  return  1;
      case 2:  return  4;
      default: return  4;
    }
  }


  const VariantList& TicTacToeGame::GetVariants(void) noexcept
  {
    static VariantList v{
      { Variant{ nullptr, 'A', 3 } },
    };
    return v;
  }

  const PieceMapP& TicTacToeGame::GetPieces(VariantCode c) noexcept
  {
    static const PieceMapP& p = std::make_shared<PieceMap>();
    p->Empty();
    p->Add(TicTacToePiece::TicTacToePieceW);
    p->Add(TicTacToePiece::TicTacToePieceB);
    return p;
  }

  const Dimensions& TicTacToeGame::GetDimensions(const VariantChosen& v) noexcept
  {
    static Dimensions d{
       Dimension(v.x, v.y, BoardStartX, BoardStartY, FieldSizeX, FieldSizeY, 1, 1),
       Dimension(2, 2, BoardStartX + FieldSizeX * (v.x + 1), BoardStartY + FieldSizeY / 2 + FieldSizeY * (v.y - 2), FieldSizeX, FieldSizeY),
       Dimension(1, 1, FieldSizeX * (v.x + 1), BoardStartY + FieldSizeSY, FieldSizeSX, FieldSizeSY, 0, FieldSizeY * v.y - FieldSizeSY * 4),
    };
    return d;
  }

}
