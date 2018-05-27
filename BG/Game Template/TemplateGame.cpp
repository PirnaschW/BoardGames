#include "stdafx.h"

#include "TemplateResource.h"
#include "TemplateGame.h"

namespace Template
{
  const TemplatePiece TemplatePiece::TemplatePieceB{&Checker::TheChecker, &Color::Black, IDB_LOAPEGB, IDB_LOAPEGBF};
  const TemplatePiece TemplatePiece::TemplatePieceW{&Checker::TheChecker, &Color::White, IDB_LOAPEGW, IDB_LOAPEGWF};


  void Checker::CollectMoves(const MainPosition& p, const Location& l, std::vector<Move>& moves) const
  {
    p.AddIfLegal(moves, l, l + Offset(+1, +1));
  }


  TemplatePosition::TemplatePosition(unsigned int x, unsigned int y) noexcept : MainPosition(x, y)
  {
    for (unsigned int i = 0; i < x; i++)
      for (unsigned int j = 0; j < y; j++)
      {
        if (((i == 0) || (i == x - 1)) && (j != 0) && (j != y - 1))  // left or right border, but not top or bottom corner
        {
          SetPiece(Location(i, j), &TemplatePiece::TemplatePieceW);
        }
        else if (((j == 0) || (j == y - 1)) && (i != 0) && (i != x - 1))  // top or bottom border, but not left or right corner
        {
          SetPiece(Location(i, j), &TemplatePiece::TemplatePieceB);
        }
      }
  }

  bool TemplatePosition::AddIfLegal(std::vector<Move>& m, const Location fr, const Location to) const
  {
    const Piece * p = GetPiece(to);
    if (p == nullptr) return false;  // out of board
    if (p->IsColor(OnTurn())) return false;  // own piece

    const Step::StepType st = p->IsBlank() ? Step::StepType::Normal : Step::StepType::Take;
    m.push_back(Move{Field{fr,GetPiece(fr)}, Field{to,GetPiece(to)},st,std::vector<Field>{Field{to,GetPiece(to)}}});
    return false;
  };

  Move::PositionValue TemplatePosition::EvaluateStatically(void)
  {
    int v1{0};
    int v2{0};

    for (unsigned int i = 0; i < sizeX; i++)
    {
      for (unsigned int j = 0; j < sizeY; j++)
      {
        const Piece* p = GetPiece(Location{i,j});
        if (p->IsColor(&Color::NoColor)) continue;
        if (p->IsColor(OnTurn())) v1 += 1;
        else v2 += 1;
      }
    }
    return v1 - v2;
  }

}
