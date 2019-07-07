#include "stdafx.h"

#include "MCResource.h"
#include "MCGame.h"

namespace MassacreChess
{
  MCPosition::MCPosition(const PieceMapP& p, Coordinate x, Coordinate y) : MainPosition(p, x, y)
  {
    //srand((unsigned)time(NULL));
    std::srand(1);

    for (unsigned int z = 0; z < x*y / 8; z++)
    {
      PlaceRandomly(&ChessPiece::WQ);
      PlaceRandomly(&ChessPiece::BQ);
      PlaceRandomly(&ChessPiece::WR);
      PlaceRandomly(&ChessPiece::BR);
      PlaceRandomly(&ChessPiece::WB);
      PlaceRandomly(&ChessPiece::BB);
      PlaceRandomly(&ChessPiece::WN);
      PlaceRandomly(&ChessPiece::BN);
    }
  }

  bool MCPosition::AddIfLegal(Moves& m, const Location fr, const Location to) const
  {
    const Piece* pf = GetPiece(fr);
    const Piece* pt = GetPiece(to);
    assert(pf != nullptr);                                                // start field must exist
    if (pt == nullptr) return false;                                      // out of board
    assert(!pf->IsBlank());                                               // start field must be a piece
    if (pt->IsBlank()) return true;                                       // not a move, but keep trying this direction
    if (pt->GetColor() == pf->GetColor()) return false;                   // own piece; don't keep trying this direction

    // valid move, save into collection
    m.push_back(std::make_shared<SimpleMove>(std::make_shared < StepSimple>( Field{fr,pf},Field{to,pf},StepSimple::StepType::Take/*,Fields{Field{to,pt}}*/ )));
    return false;                                                         // don't keep trying this direction
  };

  bool MCPosition::PlaceRandomly(const Piece* p)
  {
    std::vector<Location> ll;
    for (Coordinate i = 0; i < sizeX; i++)
      for (Coordinate j = 0; j < sizeY; j++)
      {
        const Location l{ BoardPart::Main,  i,j };
        if (GetPiece(l) == &Piece::NoPiece) ll.push_back(l);
      }
    if (ll.empty()) return false;
    const unsigned int z = rand() % ll.size();
    SetPiece(ll[z], p);
    return true;
  }


  //MCGame::MCGame(const PieceMapP& m, Coordinate x, Coordinate y) noexcept : MCGame(m,
  //  new MCPosition(m, x, y), new TakenPosition(m, x* y / 2, 2), new StockPosition(m, 5, 2),
  //  new MCLayout(x, y), new MCTakenLayout(x, y), new MCStockLayout(x, y)) {}
  MCGame::MCGame(const PieceMapP& m, Coordinate x, Coordinate y) noexcept : MCGame(m, new MCPosition(m, x, y), new MCLayout(x, y)) {}

  MCGame::MCGame(const PieceMapP& m, MCPosition* p, MCLayout* l) noexcept : Game{ m,p,l }
  {
    AddToStock(Location(BoardPart::Main, 0U, 0U), &ChessPiece::WQ);
    AddToStock(Location(BoardPart::Main, 1U, 0U), &ChessPiece::WR);
    AddToStock(Location(BoardPart::Main, 2U, 0U), &ChessPiece::WB);
    AddToStock(Location(BoardPart::Main, 3U, 0U), &ChessPiece::WN);
    AddToStock(Location(BoardPart::Main, 0U, 1U), &ChessPiece::BQ);
    AddToStock(Location(BoardPart::Main, 1U, 1U), &ChessPiece::BR);
    AddToStock(Location(BoardPart::Main, 2U, 1U), &ChessPiece::BB);
    AddToStock(Location(BoardPart::Main, 3U, 1U), &ChessPiece::BN);
  }

  const VariantList& MCGame::GetVariants(void) noexcept
  {
    static VariantList v{ { Variant{ 8, 8, nullptr, 2, 20 } } };
    return v;
  }

  const PieceMapP& MCGame::GetPieces(void) noexcept
  {
    static const PieceMapP& p = std::make_shared<PieceMap>();
    p->Add(&ChessPiece::WQ);
    p->Add(&ChessPiece::WR);
    p->Add(&ChessPiece::WB);
    p->Add(&ChessPiece::WN);
    p->Add(&ChessPiece::BQ);
    p->Add(&ChessPiece::BR);
    p->Add(&ChessPiece::BB);
    p->Add(&ChessPiece::BN);
    return p;
  }

}
