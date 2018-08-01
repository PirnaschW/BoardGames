#include "stdafx.h"

#include "CheckersGame.h"

namespace Checkers
{
  CheckersPosition::CheckersPosition(Coordinate x, Coordinate y) noexcept : MainPosition(x, y)
  {
    for (Coordinate j = 0; j < y / 2 - 1; j++)
    {
      for (Coordinate i = 0; i < x; i++)
      {
        if ((i + j) % 2) SetPiece(Location(i, j), &CheckersPiece::CheckersPieceB);
        else SetPiece(Location(i, y - 1 - j), &CheckersPiece::CheckersPieceW);
      }
    }
  }

  bool CheckersPosition::AddIfLegal(std::vector<Move>& m, const Location fr, const Location to) const
  {
    const Piece* p = GetPiece(to);
    if (p == nullptr) return false;  // out of board
    if (p->IsColor(OnTurn())) return false;  // own piece

    const Step::StepType st = p->IsBlank() ? Step::StepType::Normal : Step::StepType::Take;
    m.push_back(Step{Field{fr,GetPiece(fr)}, Field{to,GetPiece(fr)},st,std::vector<Field>{Field{to,GetPiece(to)}}});
    return false;
  };

  void CheckersPosition::EvaluateStatically(void)
  {
    MainPosition::EvaluateStatically();
    // ...
  }


  CheckersGame::CheckersGame(Coordinate x, Coordinate y) noexcept : CheckersGame(
    new CheckersPosition(x, y), new TakenPosition(x, 2), new StockPosition(3, 1),
    new CheckersLayout(x, y), new CheckersTakenLayout(x, y), new CheckersStockLayout(x, y)) {}

  CheckersGame::CheckersGame(CheckersPosition* p, TakenPosition* t, StockPosition* s,
    CheckersLayout* l, CheckersTakenLayout* tl, CheckersStockLayout* sl) noexcept : Game{ p,t,s,l,tl,sl }
  {
    AddToStock(Location(0, 0), &CheckersPiece::CheckersPieceW);
    AddToStock(Location(1, 0), &CheckersPiece::CheckersPieceB);
  }

  const VariantList& CheckersGame::GetVariants(void) noexcept
  {
    static VariantList v{ { Variant{ 8, 8, nullptr, 2, 20 } } };
    return v;
  }

}
