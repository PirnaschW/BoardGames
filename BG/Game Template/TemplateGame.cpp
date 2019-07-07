#include "stdafx.h"

#include "TemplateGame.h"

namespace Template
{
  inline const Checker Checker::TheChecker{};
  inline const TemplatePiece TemplatePiece::TemplatePieceW{ &Checker::TheChecker, &Color::White, IDB_WCL, IDB_WCD, IDB_WCS };
  inline const TemplatePiece TemplatePiece::TemplatePieceB{ &Checker::TheChecker, &Color::Black, IDB_BCL, IDB_BCD, IDB_BCS };

  bool TemplatePosition::AddIfLegal(Moves& m, const Location fr, const Location to) const
  {
    const Piece* p = GetPiece(to);
    if (p == nullptr) return false;  // out of board
    if (p->IsColor(OnTurn())) return false;  // own piece

    const StepSimple::StepType st = p->IsBlank() ? StepSimple::StepType::Normal : StepSimple::StepType::Take;
    m.push_back(std::make_shared<SimpleMove>(std::make_shared<StepSimple>(Field{ fr,GetPiece(fr) }, Field{ to,GetPiece(fr) }, st)));
    return false;
  };

  void TemplatePosition::EvaluateStatically(void)
  {
    MainPosition::EvaluateStatically();
    // ...
  }


  //TemplateGame::TemplateGame(const PieceMapP& m, Coordinate x, Coordinate y) noexcept : TemplateGame(m,
  //  new TemplatePosition(m, x, y), new TakenPosition(m, x, 2), new StockPosition(m, 3, 1),
  //  new TemplateLayout(x, y), new TemplateTakenLayout(x, y), new TemplateStockLayout(x, y)) {}
  TemplateGame::TemplateGame(const PieceMapP& m, Coordinate x, Coordinate y) noexcept : TemplateGame(m, new TemplatePosition(m, x, y), new TemplateLayout(x, y)) {}

  TemplateGame::TemplateGame(const PieceMapP& m, TemplatePosition* p, TemplateLayout* l) noexcept : Game{ m,p,l }
  {
    AddToStock(Location(BoardPart::Main, 0U, 0U), &TemplatePiece::TemplatePieceW);
    AddToStock(Location(BoardPart::Main, 1U, 0U), &TemplatePiece::TemplatePieceB);
  }

  const VariantList& TemplateGame::GetVariants(void) noexcept
  {
    static VariantList v{ { Variant{ 8, 8, nullptr, 2, 20 } } };
    return v;
  }

  const PieceMapP& TemplateGame::GetPieces(void) noexcept
  {
    static const PieceMapP& p = std::make_shared<PieceMap>();
    p->Add(&TemplatePiece::TemplatePieceW);
    p->Add(&TemplatePiece::TemplatePieceB);
    return p;
  }

}
