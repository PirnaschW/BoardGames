#include "stdafx.h"

#include "TemplateGame.h"

namespace Template
{
  inline const Checker Checker::TheChecker{};
  inline const TemplatePiece TemplatePiece::TemplatePieceW{ &Checker::TheChecker, &Color::White, IDB_WCL, IDB_WCD, IDB_WCS };
  inline const TemplatePiece TemplatePiece::TemplatePieceB{ &Checker::TheChecker, &Color::Black, IDB_BCL, IDB_BCD, IDB_BCS };

  bool TemplatePosition::AddIfLegal(std::vector<Move>& m, const Location fr, const Location to) const
  {
    const Piece* p = GetPiece(to);
    if (p == nullptr) return false;  // out of board
    if (p->IsColor(OnTurn())) return false;  // own piece

    const Step::StepType st = p->IsBlank() ? Step::StepType::Normal : Step::StepType::Take;
    m.push_back(Step{Field{fr,GetPiece(fr)}, Field{to,GetPiece(fr)},st,std::vector<Field>{Field{to,GetPiece(to)}}});
    return false;
  };

  void TemplatePosition::EvaluateStatically(void)
  {
    MainPosition::EvaluateStatically();
    // ...
  }


  TemplateGame::TemplateGame(Coordinate x, Coordinate y) noexcept : TemplateGame(
    new TemplatePosition(x, y), new TakenPosition(x, 2), new StockPosition(3, 1),
    new TemplateLayout(x, y), new TemplateTakenLayout(x, y), new TemplateStockLayout(x, y)) {}

  TemplateGame::TemplateGame(TemplatePosition* p, TakenPosition* t, StockPosition* s,
    TemplateLayout* l, TemplateTakenLayout* tl, TemplateStockLayout* sl) noexcept : Game{ p,t,s,l,tl,sl }
  {
    AddToStock(Location(0, 0), &TemplatePiece::TemplatePieceW);
    AddToStock(Location(1, 0), &TemplatePiece::TemplatePieceB);
  }

  const VariantList& TemplateGame::GetVariants(void) noexcept
  {
    static VariantList v{ { Variant{ 8, 8, nullptr, 2, 20 } } };
    return v;
  }

}
