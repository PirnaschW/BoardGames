#include "Games.h"

#include "TicTacToeGame.h"

namespace TicTacToe
{

  void TicTacToePosition::GetAllMoves(void) const noexcept                      // collect all moves
  {
    assert(movesW_.empty());
    assert(movesB_.empty());

    Actions aw0{};
    Actions ab0{};
    aw0.push_back(std::make_shared<ActionLift>(Location{ BoardPart::Stock, 0U, 0U }, CorePiece::WC));
    ab0.push_back(std::make_shared<ActionLift>(Location{ BoardPart::Stock, 0U, 1U }, CorePiece::BC));
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
          aw.push_back(std::make_shared<ActionDrop>(l, CorePiece::WC));
          ab.push_back(std::make_shared<ActionDrop>(l, CorePiece::BC));
          movesW_.push_back(std::make_shared<Move>(aw));
          movesB_.push_back(std::make_shared<Move>(ab));
        }
      }
    }
  }

  PositionValue TicTacToePosition::EvaluateStatically(void) const noexcept
  {
    assert(movesW_.empty());
    assert(movesB_.empty());

    GetAllMoves();                                                        // fill the move lists
    depth_ = 1;
    if (onTurn_ == &PieceColor::White && movesW_.empty()) return PositionValue::PValueType::Tie;  // if no more moves, game over
    if (onTurn_ == &PieceColor::Black && movesB_.empty()) return PositionValue::PValueType::Tie;
    return EvaluateChainLengths(3);
  }

  unsigned int TicTacToePosition::GetChainValue(unsigned int z) const noexcept
  {
    switch (z)
    {
      case 1:  return  1;
      case 2:  return  9;
      default: return  0;
    }
  }


  const VariantList& TicTacToeGame::GetVariants(void) noexcept
  {
    static VariantList v{
      { Variant{ nullptr, 'A', 3 } },
    };
    return v;
  }

  const PieceMapP& TicTacToeGame::GetPieces(const VariantChosen& v) noexcept
  {
    static const PieceMapP& p = std::make_shared<PieceMap>();
    p->Empty();
    p->Add(CorePiece::WC);
    p->Add(CorePiece::BC);
    return p;
  }

  const Dimensions TicTacToeGame::GetDimensions(const VariantChosen& v) noexcept
  {
    Dimensions d{
       Dimension(v.x, v.y, BoardStartX, BoardStartY, FieldSizeX, FieldSizeY, 1, 1),
       Dimension(2, 2, BoardStartX + FieldSizeX * (v.x + 1), BoardStartY + FieldSizeY / 2 + FieldSizeY * (v.y - 2), FieldSizeX, FieldSizeY),
       Dimension(1, 1, FieldSizeX * (v.x + 1), BoardStartY + FieldSizeSY, FieldSizeSX, FieldSizeSY, 0, FieldSizeY * v.y - FieldSizeSY * 4),
    };
    return d;
  }

}
