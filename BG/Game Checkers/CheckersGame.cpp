#include "stdafx.h"

#include "CheckersGame.h"

namespace Checkers
{
  inline const Checker Checker::TheChecker{};
  inline const King    King   ::TheKing   {};
  inline const Queen   Queen  ::TheQueen  {};

  inline const CheckersPiece CheckersPiece::CheckersPieceW{ &Checker::TheChecker, &Color::White, &CheckersQueenW, IDB_WCD, IDB_WCS };
  inline const CheckersPiece CheckersPiece::CheckersPieceB{ &Checker::TheChecker, &Color::Black, &CheckersQueenB, IDB_BCD, IDB_BCS };
  inline const CheckersPiece CheckersPiece::CheckersKingW { &King::TheKing,       &Color::White, NULL,            IDB_WKD, IDB_WKS };
  inline const CheckersPiece CheckersPiece::CheckersKingB { &King::TheKing,       &Color::Black, NULL,            IDB_BKD, IDB_BKS };
  inline const CheckersPiece CheckersPiece::CheckersQueenW{ &Queen::TheQueen,     &Color::White, NULL,            IDB_WQD, IDB_WQS };
  inline const CheckersPiece CheckersPiece::CheckersQueenB{ &Queen::TheQueen,     &Color::Black, NULL,            IDB_BQD, IDB_BQS };


  void Checker::CollectMoves(const MainPosition& p, const Location& l, Moves& moves) const
  {
    const CheckersPosition& pos = dynamic_cast<const CheckersPosition&>(p);
    const int dy = p.OnTurn() == &Color::White ? -1 : 1;
    pos.AddIfLegal(moves, l, l + Offset(1, dy));
    pos.AddIfLegal(moves, l, l + Offset(-1, dy));
    pos.AddIfLegalJump(moves, false, Steps{}, l);  // check for jump moves
  }


  void King::CollectMoves(const MainPosition& p, const Location& l, Moves& moves) const
  {
    const CheckersPosition& pos = dynamic_cast<const CheckersPosition&>(p);
    for (auto& d : Offset::Bdirection)
      pos.AddIfLegal(moves, l, l + d);  // check for slide moves
    pos.AddIfLegalJump(moves, false, Steps{}, l);  // check for jump moves
  }


  void Queen::CollectMoves(const MainPosition& p, const Location& l, Moves& moves) const
  {
    const CheckersPosition& pos = dynamic_cast<const CheckersPosition&>(p);
    for (auto& d : Offset::Bdirection)
      for (int z = 1; pos.AddIfLegal(moves, l, l + d * z); z++);          // check for slide moves
    pos.AddIfLegalJump(moves, true, Steps{}, l);                    // check for jump moves
  }


  CheckersPosition::CheckersPosition(Coordinate x, Coordinate y) noexcept : MainPosition(x, y)
  {
    //SetPiece(Location(1, 2), &CheckersPiece::CheckersPieceB);
    //SetPiece(Location(3, 2), &CheckersPiece::CheckersPieceB);
    //SetPiece(Location(5, 2), &CheckersPiece::CheckersPieceB);
    //SetPiece(Location(5, 6), &CheckersPiece::CheckersPieceB);
    //SetPiece(Location(2, 5), &CheckersPiece::CheckersPieceB);
    //SetPiece(Location(4, 7), &CheckersPiece::CheckersQueenW);
    //return;
    for (Coordinate j = 0; j < y / 2 - 1; j++)
    {
      for (Coordinate i = 0; i < x; i++)
      {
        if ((i + j) % 2) SetPiece(Location(i, j), &CheckersPiece::CheckersPieceB);
        else SetPiece(Location(i, y - 1 - j), &CheckersPiece::CheckersPieceW);
      }
    }
  }

  bool CheckersPosition::AddIfLegal(Moves& m, const Location fr, const Location to) const
  {
    const Piece* p = GetPiece(to);
    if (p == nullptr) return false;                                       // out of board
    if (!p->IsBlank()) return false;                                      // field is not empty

    const Piece * p2 = CanPromote(to) ? GetPiece(fr)->Promote(true) : GetPiece(fr);
    m.push_back(std::make_shared<ComplexMove>(Steps(1, std::make_shared<ComplexStep>(Field{ fr,GetPiece(fr) }, Field{ to,p2 }, Step::StepType::Normal, std::vector<Field>{Field{ to,GetPiece(to) }}))));
    return true;
  }


  bool CheckersPosition::AddIfLegalJump(Moves& m, bool longjumps, const Steps& s, const Location fr) const
  {
    const Location l0{ s.empty() ? fr : s.front()->GetFr().GetLocation() };
    const Piece* p0 = GetPiece(l0);                                       // the piece that is moving
    assert(p0 != nullptr);
    assert(p0 != &Piece::NoTile);
    assert(!p0->IsBlank());

    bool any{ false };                                                    // were any more jumps possible?

    for (auto& d : Offset::Bdirection)
    {
      for (int z1 = 1; longjumps || z1 == 1; z1++)
      {
        const Location l1{ fr + d * z1 };                                 // location to jump over
     // check the jumped-over tile                                       
        const Piece* p1{ l1 == l0 ? &Piece::NoPiece : GetPiece(l1) };     // what is on the tile to jump over?
        if (p1 == nullptr) break;                                         // tile is out of board, can't jump over it
        if (p1 == &Piece::NoTile) break;                                  // tile is not existing, can't jump over it
        if (p1->IsBlank()) continue;                                      // tile is not occupied, keep going (not a jump yet)

        const Color* c1 = p1->GetColor();                                 // color of jumped-over piece
        if (p0->IsColor(c1)) break;                                       // can't jump own pieces

        for (int z2 = 1; longjumps || z2 == 1; z2++)
        {
          const Location l2{ l1 + d * z2 };                               // location to jump to

          // check the jump-to tile                                       
          const Piece* p2{ l2 == l0 ? &Piece::NoPiece : GetPiece(l2) };   // what is on the jump-to tile
          if (p2 == nullptr) break;                                       // tile is out of board, can't jump there
          if (p2 == &Piece::NoTile) break;                                // tile is not existing, can't jump there
          if (!p2->IsBlank()) break;                                      // tile is occupied, can't jump there

          // check if the same jump was done before (can't jump back and forth or in circles, and can't jump the same opponent piece twice)
          bool repeat{ false };
          for (auto& ss : s)
          {
            for (const auto& t : ss->GetTakes())
            {
              if (t.GetLocation() == l1)
              {
                repeat = true;  // this piece was taken before
                break;
              }
            }
          }
          if (repeat) break;                                              // don't allow the same move again

          // a legal jump was found
          any = true;
          std::vector<Field> f{};
          f.push_back(Field{ l1,p1 });
          Steps s1{ s };                                      // copy the previous jump sequence, so we can extend it
          // add the jump to the SimpleStep list
          s1.push_back(std::make_shared<ComplexStep>(Field{ fr, p0 }, Field{ l2,p0 }, (Step::StepType) (Step::StepType::Jump | Step::StepType::Take), f));

          if (!AddIfLegalJump(m, longjumps, s1, l2))                      // collect potential further jumps
            m.push_back(std::make_shared<ComplexMove>(s1));                      // if it could not be extended, or was a jump over an own piece, add the SimpleStep list as a move
        }
        break;
      }
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

  bool CheckersPosition::CanPromote(const Location& l) const noexcept
  {
    return (OnTurn() == &Color::White && l._y == 0) || (OnTurn() != &Color::White && l._y == sizeY-1);
  }


  CheckersGame::CheckersGame(Coordinate x, Coordinate y) noexcept : CheckersGame(
    new CheckersPosition(x, y), new TakenPosition(3*x/2, 2), new StockPosition(4, 2),
    new CheckersLayout(x, y), new CheckersTakenLayout(x, y), new CheckersStockLayout(x, y)) {}

  CheckersGame::CheckersGame(CheckersPosition* p, TakenPosition* t, StockPosition* s,
    CheckersLayout* l, CheckersTakenLayout* tl, CheckersStockLayout* sl) noexcept : Game{ p,t,s,l,tl,sl }
  {
    AddToStock(Location(0U, 0U), &CheckersPiece::CheckersPieceW);
    AddToStock(Location(1U, 0U), &CheckersPiece::CheckersKingW);
    AddToStock(Location(2U, 0U), &CheckersPiece::CheckersQueenW);
    AddToStock(Location(0U, 1U), &CheckersPiece::CheckersPieceB);
    AddToStock(Location(1U, 1U), &CheckersPiece::CheckersKingB);
    AddToStock(Location(2U, 1U), &CheckersPiece::CheckersQueenB);
  }

  const VariantList& CheckersGame::GetVariants(void) noexcept
  {
    static VariantList v{ { Variant{ 8, 8, nullptr, 2, 20 } } };
    return v;
  }

}
