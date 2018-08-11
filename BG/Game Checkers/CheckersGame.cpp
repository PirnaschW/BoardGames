#include "stdafx.h"

#include "CheckersGame.h"

namespace Checkers
{

  void Checker::CollectMoves(const MainPosition& p, const Location& l, std::vector<Move>& moves) const
  {
    const CheckersPosition& pos = dynamic_cast<const CheckersPosition&>(p);
    const int dy = p.OnTurn() == &Color::White ? -1 : 1;
    pos.AddIfLegal(moves, l, l + Offset(1, dy));
    pos.AddIfLegal(moves, l, l + Offset(-1, dy));
    pos.AddIfLegalJump(moves, std::vector<Step>{}, l);  // check for jump moves
  }


  void King::CollectMoves(const MainPosition& p, const Location& l, std::vector<Move>& moves) const
  {
    const CheckersPosition& pos = dynamic_cast<const CheckersPosition&>(p);
    for (auto& d : Offset::Bdirection)
      pos.AddIfLegal(moves, l, l + d);  // check for slide moves
    pos.AddIfLegalJump(moves, std::vector<Step>{}, l);  // check for jump moves
  }


  void Queen::CollectMoves(const MainPosition& p, const Location& l, std::vector<Move>& moves) const
  {
    const CheckersPosition& pos = dynamic_cast<const CheckersPosition&>(p);
    for (auto& d : Offset::Bdirection)
      for (int z = 1; pos.AddIfLegal(moves, l, l + d * z); z++);          // check for slide moves
    pos.AddIfLegalJump(moves, std::vector<Step>{}, l);                    // check for jump moves
  }


  CheckersPosition::CheckersPosition(Coordinate x, Coordinate y) noexcept : MainPosition(x, y)
  {
    SetPiece(Location(2, 1), &CheckersPiece::CheckersPieceB);
    SetPiece(Location(2, 3), &CheckersPiece::CheckersPieceB);
    SetPiece(Location(2, 5), &CheckersPiece::CheckersPieceB);
    SetPiece(Location(3, 6), &CheckersPiece::CheckersPieceW);
    return;
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
    if (p == nullptr) return false;                                       // out of board
    if (!p->IsBlank()) return false;                                      // field is not empty

    const Step::StepType st = p->IsBlank() ? Step::StepType::Normal : Step::StepType::Take;
    m.push_back(Step{Field{fr,GetPiece(fr)}, Field{to,GetPiece(fr)},st,std::vector<Field>{Field{to,GetPiece(to)}}});
    return false;
  }


  bool CheckersPosition::AddIfLegalJump(std::vector<Move>& m, const std::vector<Step>& s, const Location fr) const
  {
    const Piece* p0 = s.empty() ? GetPiece(fr) : s.front().GetFr().GetPiece(); // the piece that is moving
    assert(p0 != nullptr);
    assert(p0 != &Piece::NoTile);
    assert(!p0->IsBlank());

    bool any{ false };                                                    // were any more jumps possible?

    for (auto& d : Offset::Bdirection)
    {
      std::vector<Step> s1{ s };                                          // copy the previous jump sequence, so we can extend it
      const Location l1{ fr + d };                                        // location to jump over
      const Location l2{ l1 + d };                                        // location to jump to

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
      if (p0->IsColor(c1)) continue;                                      // can't jump own pieces

      // check if the same jump was done before (can't jump back and forth or in circles, and can't jump the same opponent piece twice)
      bool repeat{ false };
      for (auto& ss : s)
      {
        if (ss.GetFr().GetLocation() == fr && ss.GetTo().GetLocation() == l2)
        {
          repeat = true;  // same jump was done before
          break;
        }
        if (ss.GetFr().GetLocation() == l2 && ss.GetTo().GetLocation() == fr)
        {
          repeat = true;  // reverse jump was done before
          break;
        }
        for (const auto& t : ss.GetTake())
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
      std::vector<Field> f{};
      f.push_back(Field{ l1,p1 });
      // add the jump to the step list
      s1.push_back(Step(Field{ fr, p0 }, Field{ l2,p0 }, (Step::StepType) (Step::StepType::Jump | Step::StepType::Take), f));

      if (!AddIfLegalJump(m, s1, l2))                                     // collect potential further jumps
        m.push_back(Move(s1));                                            // if it could not be extended, or was a jump over an own piece, add the step list as a move

    }
    return any;
  }

  void CheckersPosition::EvaluateStatically(void)
  {
    MainPosition::EvaluateStatically();
    // ...
  }

  void CheckersPosition::GetAllMoves(void)  // collect all moves for all pieces
  {
    MainPosition::GetAllMoves();
    JumpsOnly(movelistW);
    JumpsOnly(movelistB);
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
