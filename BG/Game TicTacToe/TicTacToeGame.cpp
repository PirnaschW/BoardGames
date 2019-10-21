#include "stdafx.h"

#include "TicTacToeGame.h"

namespace TicTacToe
{
  inline const Checker Checker::TheChecker;
  inline const TicTacToePiece TicTacToePiece::TicTacToePieceB{ &Checker::TheChecker, &Color::Black, IDB_BCL, IDB_BCD, IDB_BCS };
  inline const TicTacToePiece TicTacToePiece::TicTacToePieceW{ &Checker::TheChecker, &Color::White, IDB_WCL, IDB_WCD, IDB_WCS };

  void TicTacToePosition::GetAllMoves(void) noexcept                      // collect all moves
  {
    assert(movesW_.empty());
    assert(movesB_.empty());

    Actions aw0{};
    Actions ab0{};
    aw0.push_back(std::make_shared<ActionTake>(Location{ BoardPart::Stock, 0U, 0U }, TicTacToePiece::TicTacToePieceW));
    ab0.push_back(std::make_shared<ActionTake>(Location{ BoardPart::Stock, 0U, 1U }, TicTacToePiece::TicTacToePieceB));
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
          aw.push_back(std::make_shared<ActionPlace>(l, TicTacToePiece::TicTacToePieceW));
          ab.push_back(std::make_shared<ActionPlace>(l, TicTacToePiece::TicTacToePieceB));
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

  void TicTacToePosition::EvaluateStatically(void) noexcept
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
        if (p.IsColor(&Color::White)) c = 'W';
        if (p.IsColor(&Color::Black)) c = 'B';
        posi[i + 4 * j] = c;
      }
      posi[sizeX_ + 4 * j] = '|';
    }

    GetAllMoves();                                                        // fill the move lists
    if (onTurn_ == &Color::White && movesW_.empty()) value_ = PositionValue::PValueType::Tie;        // if no more moves, game over
    else if (onTurn_ == &Color::Black && movesB_.empty()) value_ = PositionValue::PValueType::Tie;
    else
    {
      for (Coordinate j = 0; j < sizeY_; j++)
      {
        for (Coordinate i = 0; i < sizeX_; i++)  // loop through all locations
        {
          const Location l{ BoardPart::Main,i,j };
          const Piece& p = GetPiece(l);
          if (p.IsColor(&Color::NoColor)) continue;  // nothing here, so no chain can start
          const bool w = p.IsColor(&Color::White);

          for (unsigned int k = 0; k < 4; k++)
          {
            const Offset& d = Offset::Qdirection[k];
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
                if (pp->IsColor(&Color::NoColor)) (w ? v1 : v2) += 1;     // if line ends with free field, give an extra point - much better than opponent's piece
                break;
              }
            }
            if (z >= sizeY_ || z >= sizeX_)
            {
              value_ = w ? PositionValue::PValueType::Won : PositionValue::PValueType::Lost;
              return;
            }
            (w ? v1 : v2) += GetChainValue(z);
          }
        }
      }

      value_ = v1 - v2;
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


  const VariantList& TicTacToeGame::GetVariants(void) noexcept
  {
    static VariantList v{ Variant{ 3, 3 } };
    return v;
  }

  const PieceMapP& TicTacToeGame::GetPieces(void) noexcept
  {
    static const PieceMapP& p = std::make_shared<PieceMap>();
    p->Empty();
    p->Add(TicTacToePiece::TicTacToePieceW);
    p->Add(TicTacToePiece::TicTacToePieceB);
    return p;
  }

  const Dimensions& TicTacToeGame::GetDimensions(Coordinate x, Coordinate y) noexcept
  {
    static Dimensions d{
       Dimension(x, y, BoardStartX, BoardStartY, FieldSizeX, FieldSizeY, 1, 1),
       Dimension(2, 2, BoardStartX + FieldSizeX * (x + 1), BoardStartY + FieldSizeY / 2 + FieldSizeY * (y - 2), FieldSizeX, FieldSizeY),
       Dimension(1, 1, FieldSizeX * (x + 1), BoardStartY + FieldSizeSY, FieldSizeSX, FieldSizeSY, 0, FieldSizeY * y - FieldSizeSY * 4),
    };
    return d;
  }

}
