#include "stdafx.h"

#include "CamGame.h"

namespace Cam
{
  constexpr inline int isqr(int x) noexcept { return x * x; }

  inline const Pawn   Pawn  ::ThePawn  {};
  inline const Knight Knight::TheKnight{};
  inline const CamPiece CamPiece::WP{ &Pawn::ThePawn, &Color::White, IDB_WPL, IDB_WPD, IDB_WPS };
  inline const CamPiece CamPiece::WN{ &Knight::TheKnight, &Color::White, IDB_WNL, IDB_WND, IDB_WNS };
  inline const CamPiece CamPiece::BP{ &Pawn::ThePawn,     &Color::Black, IDB_BPL, IDB_BPD, IDB_BPS };
  inline const CamPiece CamPiece::BN{ &Knight::TheKnight, &Color::Black, IDB_BNL, IDB_BND, IDB_BNS };

  void Pawn::CollectMoves(const MainPosition& p, const Location& l, Moves& moves) const
  {
    Steps s{};
    const CamPosition& pos = dynamic_cast<const CamPosition&>(p);
    pos.CollectJumps(l, s, false, &Color::NoColor, moves);
    const Piece* p0 = pos.GetPiece(l);                                    // piece that is moving
    for (auto& d : Offset::Qdirection)                                    // try slides all eight directions
    {
      const Piece* p1 = pos.GetPiece(l + d);                              // what is on the tile to move to?
      if (p1 == nullptr) continue;                                        // out of board
      if (p1 == &Piece::NoTile) continue;                                 // out of board
      if (!p1->IsBlank()) continue;                                       // tile occupied
      moves.push_back(std::make_shared<ComplexMove>(Steps(1,std::make_shared<ComplexStep>( Field{ l,p0 }, Field{ l + d,p0 } )))); // ok, so add move to move list
    }
  }

  void Knight::CollectMoves(const MainPosition& p, const Location& l, Moves& moves) const
  {
    Steps s{};
    const CamPosition& pos = dynamic_cast<const CamPosition&>(p);
    pos.CollectJumps(l, s, true, &Color::NoColor, moves);
    const Piece* p0 = pos.GetPiece(l);                                    // piece that is moving
    for (auto& d : Offset::Qdirection)                                    // try slides all eight directions
    {
      const Piece* p1 = pos.GetPiece(l + d);                              // what is on the tile to move to?
      if (p1 == nullptr) continue;                                        // out of board
      if (p1 == &Piece::NoTile) continue;                                 // out of board
      if (!p1->IsBlank()) continue;                                       // tile occupied
      moves.push_back(std::make_shared<ComplexMove>(Steps(1, std::make_shared<ComplexStep>(Field{ l,p0 }, Field{ l + d,p0 }))));  // ok, so add move to move list
    }
  }

  unsigned int CamPiece::GetValue(const MainPosition& p, const Location l) const noexcept
  {
    return kind->GetValue(p, l) + isqr(IsColor(&Color::White) ? p.GetSizeY() - 1 - l.y : l.y);
  }


  CamPosition::CamPosition(Coordinate x, Coordinate y) noexcept : MainPosition(x, y)
  {
    if (x == 12) // full Camelot
    {
      // Black Knights
      SetPiece(Location(2, 5), &CamPiece::BN);
      SetPiece(Location(3, 6), &CamPiece::BN);
      SetPiece(Location(8, 6), &CamPiece::BN);
      SetPiece(Location(9, 5), &CamPiece::BN);

      // Black Pawns
      SetPiece(Location(3, 5), &CamPiece::BP);
      SetPiece(Location(4, 5), &CamPiece::BP);
      SetPiece(Location(5, 5), &CamPiece::BP);
      SetPiece(Location(6, 5), &CamPiece::BP);
      SetPiece(Location(7, 5), &CamPiece::BP);
      SetPiece(Location(8, 5), &CamPiece::BP);

      SetPiece(Location(4, 6), &CamPiece::BP);
      SetPiece(Location(5, 6), &CamPiece::BP);
      SetPiece(Location(6, 6), &CamPiece::BP);
      SetPiece(Location(7, 6), &CamPiece::BP);

      // White Knights
      SetPiece(Location(2, 10), &CamPiece::WN);
      SetPiece(Location(3, 9), &CamPiece::WN);
      SetPiece(Location(8, 9), &CamPiece::WN);
      SetPiece(Location(9, 10), &CamPiece::WN);

      // White Pawns
      SetPiece(Location(3, 10), &CamPiece::WP);
      SetPiece(Location(4, 10), &CamPiece::WP);
      SetPiece(Location(5, 10), &CamPiece::WP);
      SetPiece(Location(6, 10), &CamPiece::WP);
      SetPiece(Location(7, 10), &CamPiece::WP);
      SetPiece(Location(8, 10), &CamPiece::WP);

      SetPiece(Location(4, 9), &CamPiece::WP);
      SetPiece(Location(5, 9), &CamPiece::WP);
      SetPiece(Location(6, 9), &CamPiece::WP);
      SetPiece(Location(7, 9), &CamPiece::WP);

      // Illegal tiles
      SetPiece(Location(0, 0), &Piece::NoTile);
      SetPiece(Location(1, 0), &Piece::NoTile);
      SetPiece(Location(2, 0), &Piece::NoTile);
      SetPiece(Location(3, 0), &Piece::NoTile);
      SetPiece(Location(4, 0), &Piece::NoTile);
      SetPiece(Location(7, 0), &Piece::NoTile);
      SetPiece(Location(8, 0), &Piece::NoTile);
      SetPiece(Location(9, 0), &Piece::NoTile);
      SetPiece(Location(10, 0), &Piece::NoTile);
      SetPiece(Location(11, 0), &Piece::NoTile);

      SetPiece(Location(0, 1), &Piece::NoTile);
      SetPiece(Location(1, 1), &Piece::NoTile);
      SetPiece(Location(10, 1), &Piece::NoTile);
      SetPiece(Location(11, 1), &Piece::NoTile);

      SetPiece(Location(0, 2), &Piece::NoTile);
      SetPiece(Location(11, 2), &Piece::NoTile);

      SetPiece(Location(0, 13), &Piece::NoTile);
      SetPiece(Location(11, 13), &Piece::NoTile);

      SetPiece(Location(0, 14), &Piece::NoTile);
      SetPiece(Location(1, 14), &Piece::NoTile);
      SetPiece(Location(10, 14), &Piece::NoTile);
      SetPiece(Location(11, 14), &Piece::NoTile);

      SetPiece(Location(0, 15), &Piece::NoTile);
      SetPiece(Location(1, 15), &Piece::NoTile);
      SetPiece(Location(2, 15), &Piece::NoTile);
      SetPiece(Location(3, 15), &Piece::NoTile);
      SetPiece(Location(4, 15), &Piece::NoTile);
      SetPiece(Location(7, 15), &Piece::NoTile);
      SetPiece(Location(8, 15), &Piece::NoTile);
      SetPiece(Location(9, 15), &Piece::NoTile);
      SetPiece(Location(10, 15), &Piece::NoTile);
      SetPiece(Location(11, 15), &Piece::NoTile);
    }

    else  // Mini Cam position
    {
      // Black Knights
      SetPiece(Location(2, 3), &CamPiece::BN);
      SetPiece(Location(4, 3), &CamPiece::BN);

      // Black Pawns
      //SetPiece(Location(1, 4), &CamPiece::BP);
      //SetPiece(Location(2, 4), &CamPiece::BP);
      //SetPiece(Location(3, 4), &CamPiece::BP);
      //SetPiece(Location(4, 4), &CamPiece::BP);
      //SetPiece(Location(5, 4), &CamPiece::BP);

      // White Knights
      SetPiece(Location(2, 9), &CamPiece::WN);
      SetPiece(Location(4, 9), &CamPiece::WN);

      // White Pawns
      //SetPiece(Location(1, 8), &CamPiece::WP);
      //SetPiece(Location(2, 8), &CamPiece::WP);
      //SetPiece(Location(3, 8), &CamPiece::WP);
      //SetPiece(Location(4, 8), &CamPiece::WP);
      //SetPiece(Location(5, 8), &CamPiece::WP);

      // Illegal tiles
      SetPiece(Location(0, 0), &Piece::NoTile);
      SetPiece(Location(1, 0), &Piece::NoTile);
      SetPiece(Location(2, 0), &Piece::NoTile);
      SetPiece(Location(4, 0), &Piece::NoTile);
      SetPiece(Location(5, 0), &Piece::NoTile);
      SetPiece(Location(6, 0), &Piece::NoTile);

      SetPiece(Location(0, 1), &Piece::NoTile);
      SetPiece(Location(1, 1), &Piece::NoTile);
      SetPiece(Location(5, 1), &Piece::NoTile);
      SetPiece(Location(6, 1), &Piece::NoTile);

      SetPiece(Location(0, 2), &Piece::NoTile);
      SetPiece(Location(6, 2), &Piece::NoTile);

      SetPiece(Location(0, 10), &Piece::NoTile);
      SetPiece(Location(6, 10), &Piece::NoTile);

      SetPiece(Location(0, 11), &Piece::NoTile);
      SetPiece(Location(1, 11), &Piece::NoTile);
      SetPiece(Location(5, 11), &Piece::NoTile);
      SetPiece(Location(6, 11), &Piece::NoTile);

      SetPiece(Location(0, 12), &Piece::NoTile);
      SetPiece(Location(1, 12), &Piece::NoTile);
      SetPiece(Location(2, 12), &Piece::NoTile);
      SetPiece(Location(4, 12), &Piece::NoTile);
      SetPiece(Location(5, 12), &Piece::NoTile);
      SetPiece(Location(6, 12), &Piece::NoTile);
    }
  }

  bool CamPosition::CollectJumps(const Location& fr, const Steps& s, bool charge, const Color* c, Moves& m) const
  {
    const Piece* p0 = s.empty() ? GetPiece(fr) : s.front()->GetFr().GetPiece(); // the piece that is moving
    assert(p0 != nullptr);
    assert(p0 != &Piece::NoTile);
    assert(!p0->IsBlank());

    const Color* c0 = p0->GetColor();                                     // color of the piece moving
    bool any{ false };                                                    // were any more jumps possible?

    for (auto& d : Offset::Qdirection)                                    // try all eight directions
    {
      Steps s1{ s };                                            // copy the previous jump sequence, so we can extend it
      const Color* cc{ c };                                                 // color last jumped over (can switch once if charge)
      const Location l1{ fr + d };                                          // location to jump over
      const Location l2{ l1 + d };                                          // location to jump to

      // check the jumped-over tile                                         
      const Piece* p1 = GetPiece(l1);                                     // what is on the tile to jump over?
      if (p1 == nullptr) continue;                                        // tile is out of board, can't jump over it
      if (p1 == &Piece::NoTile) continue;                                 // tile is not existing, can't jump over it
      if (p1->IsBlank()) continue;                                        // tile is not occupied, can't jump over it
      const Color* c1 = p1->GetColor();                                   // color of jumped-over piece

      // check the jump-to tile                                           
      const Piece* p2 = GetPiece(l2);                                     // what is on the jump-to tile
      if (p2 == nullptr) continue;                                        // tile is out of board, can't jump there
      if (p2 == &Piece::NoTile) continue;                                 // tile is not existing, can't jump there
      if (!p2->IsBlank()) continue;                                       // tile is occupied, can't jump there

      // check the jump is allowed
      if (cc == &Color::NoColor) cc = c1;                                 // first jump - either one is allowed
      else if (cc != c1)                                                  // trying to switch jumped color
      {
        if (cc != c0) continue;                                           // can't switch back to jumping own pieces
        if (!charge) continue;                                            // can't switch to enemy pieces
      }

      // check if the same jump was done before (can't jump back and forth or in circles, and can't jump the same opponent piece twice)
      bool repeat{ false };
      for (auto& ss : s)
      {
        if (ss->GetFr().GetLocation() == fr && ss->GetTo().GetLocation() == l2)
        {
          repeat = true;  // same jump was done before
          break;
        }
        if (ss->GetFr().GetLocation() == l2 && ss->GetTo().GetLocation() == fr)
        {
          repeat = true;  // reverse jump was done before
          break;
        }
        for (const auto& t : ss->GetTakes())
        {
          if (t.GetLocation() == l1)
          {
            repeat = true;  // this piece was taken before
            break;
          }
        }
      }
      if (repeat) continue;                                               // don't allow the same move again

      // a legal jump was found
      any = true;
      Step::StepType st{ Step::StepType::Jump };
      std::vector<Field> f{};
      if (c1 != c0)                                                       // jump was over an enemy piece
      {
        st = (Step::StepType) (Step::StepType::Jump | SimpleStep::StepType::Take);
        f.push_back(Field{ l1,p1 });
      }

      s1.push_back(std::make_shared<ComplexStep>(Field{ fr, p0 }, Field{ l2,p0 }, st, f));         // add the jump to the SimpleStep list

      if (!CollectJumps(l2, s1, charge, cc, m) ||                         // collect potential further jumps
        st == Step::StepType::Jump)
      {
        m.push_back(std::make_shared<ComplexMove>(s1));                          // if it could not be extended, or was a jump over an own piece, add the SimpleStep list as a move
      }
    }
    return any;
  }

  void CamPosition::GetAllMoves(void)  // collect all moves for all pieces
  {
    MainPosition::GetAllMoves();
    JumpsOnly(movelistW);
    JumpsOnly(movelistB);
  }

  //PositionValue CamPosition::EvaluateWin(void) const
  //{
  //  bool wfree{false};
  //  bool bfree{false};

  //  for (Coordinate i = 0; i < sizeX; i++)
  //  {
  //    const Piece* pw = GetPiece(Location(i, 0));
  //    const Piece* pb = GetPiece(Location(i, sizeY - 1));
  //    if (pw != &Piece::NoTile && !pw->IsColor(&Color::White)) wfree = true;
  //    if (pb != &Piece::NoTile && !pb->IsColor(&Color::Black)) bfree = true;
  //  }

  //  int dy = OnTurn() == &Color::White ? 1 : -1;
  //  if (!bfree) return PositionValue::PValueType::Lost * dy;
  //  if (!wfree) return PositionValue::PValueType::Won * dy;
  //  return 0;
  //}

  CamGame::CamGame(Coordinate x, Coordinate y) noexcept : CamGame(
    new CamPosition(x, y), new CamTakenPosition(x, y), new StockPosition(3, 2),
    new CamLayout(x, y), new CamTakenLayout(x, y), new CamStockLayout(x, y)) {}

  CamGame::CamGame(CamPosition* p, TakenPosition* t, StockPosition* s,
    CamLayout* l, CamTakenLayout* tl, CamStockLayout* sl) noexcept : Game{ p,t,s,l,tl,sl }
  {
    AddToStock(Location(0, 0), &CamPiece::WN);
    AddToStock(Location(1, 0), &CamPiece::WP);
    AddToStock(Location(0, 1), &CamPiece::BP);
    AddToStock(Location(1, 1), &CamPiece::BN);
  }

  const VariantList& CamGame::GetVariants(void) noexcept
  {
    static VariantList v{ { Variant{ 12, 16, "Camelot" },{ Variant{ 7, 13, "Cam" } } } };
    return v;
  }

}
