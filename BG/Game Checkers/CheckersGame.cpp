#include "stdafx.h"

#include "CheckersGame.h"

namespace Checkers
{
  inline const Checker Checker::TheChecker{};
  inline const King    King   ::TheKing   {};
  inline const Queen   Queen  ::TheQueen  {};

  inline const CheckersPiece CheckersPiece::CheckersPieceW{ &Checker::TheChecker, &Color::White, &CheckersQueenW, IDB_WCD, IDB_WCS };
  inline const CheckersPiece CheckersPiece::CheckersPieceB{ &Checker::TheChecker, &Color::Black, &CheckersQueenB, IDB_BCD, IDB_BCS };
  inline const CheckersPiece CheckersPiece::CheckersKingW { &King::TheKing,       &Color::White, &CheckersKingW,  IDB_WKD, IDB_WKS };
  inline const CheckersPiece CheckersPiece::CheckersKingB { &King::TheKing,       &Color::Black, &CheckersKingB,  IDB_BKD, IDB_BKS };
  inline const CheckersPiece CheckersPiece::CheckersQueenW{ &Queen::TheQueen,     &Color::White, &CheckersQueenW, IDB_WQD, IDB_WQS };
  inline const CheckersPiece CheckersPiece::CheckersQueenB{ &Queen::TheQueen,     &Color::Black, &CheckersQueenB, IDB_BQD, IDB_BQS };


  void Checker::CollectMoves(const MainPosition& p, const Location& l, Moves& moves) const noexcept
  {
    const CheckersPosition& pos = dynamic_cast<const CheckersPosition&>(p);
    const int dy = pos.GetPiece(l)->IsColor(&Color::White) ? -1 : 1;
    pos.AddIfLegal(moves, l, l + Offset(1, dy));                          // check for slide moves
    pos.AddIfLegal(moves, l, l + Offset(-1, dy));
//    pos.AddIfLegalJump(moves, false, Actions{}, l);                       // check for jump moves
  }


  void King::CollectMoves(const MainPosition& p, const Location& l, Moves& moves) const noexcept
  {
    const CheckersPosition& pos = dynamic_cast<const CheckersPosition&>(p);
    for (auto& d : Offset::Bdirection)
      pos.AddIfLegal(moves, l, l + d);                                    // check for slide moves
    pos.AddIfLegalJump(moves, false, Actions{}, l);                       // check for jump moves
  }


  void Queen::CollectMoves(const MainPosition& p, const Location& l, Moves& moves) const noexcept
  {
    const CheckersPosition& pos = dynamic_cast<const CheckersPosition&>(p);
    for (auto& d : Offset::Bdirection)
      for (int z = 1; pos.AddIfLegal(moves, l, l + d * z); z++);          // check for slide moves
//    pos.AddIfLegalJump(moves, true, Actions{}, l);                        // check for jump moves
  }


  CheckersPosition::CheckersPosition(const PieceMapP& p, const Dimensions& d) noexcept : MainPosition(p, d)
  {
    for (Coordinate j = 0; j < d[0].yCount / 2 - 1; j++)
    {
      for (Coordinate i = 0; i < d[0].xCount; i++)
      {
        if ((i + j) % 2) SetPiece(Location(BoardPart::Main, i, j), &CheckersPiece::CheckersPieceB);
        else SetPiece(Location(BoardPart::Main, i, d[0].yCount - 1 - j), &CheckersPiece::CheckersPieceW);
      }
    }
  }

  bool CheckersPosition::AddIfLegal(Moves& m, const Location fr, const Location to) const noexcept
  {
    const Piece* pf = GetPiece(fr);
    assert(pf != nullptr);
    assert(!pf->IsBlank());
    const Piece* pt = GetPiece(to);
    if (pt == nullptr) return false;                                       // out of board
    if (!pt->IsBlank()) return false;                                      // field is not empty

    const Piece * pf2 = CanPromote(to,pf) ? pf->Promote(true) : pf;

    Actions a{};
    a.push_back(std::make_shared<ActionTake>(fr, pf));          // pick piece up
    a.push_back(std::make_shared<ActionPlace>(to, pf2));                   // and place it on target
    m.push_back(std::make_shared<Move>(a));                               // add move to move list
    return true;
  }


  bool CheckersPosition::AddIfLegalJump(Moves& m, bool longjumps, const Actions& a, const Location fr) const noexcept
  {
    const Location l0{ fr };
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
          if (a.IsRepeat(p0, l0, l2)) break;                              // don't allow the same move again

          // a legal jump was found
          any = true;
          // add the jump to the StepSimple list
          Actions a0{ a };
          a0.push_back(std::make_shared<ActionTake>(fr, p0));             // pick piece up
          a0.push_back(std::make_shared<ActionTake>(l1, p1));             // pick jumped piece up
          a0.push_back(std::make_shared<ActionPlace>(GetNextTakenL(p1->GetColor()), p1)); // place it in taken
          a0.push_back(std::make_shared<ActionPlace>(l2, p0));            // and place it on target
          if (!AddIfLegalJump(m, longjumps, a0, l2))                      // collect potential further jumps
            m.push_back(std::make_shared<Move>(a));                       // if it could not be extended, add the list as a move
        }
        break;
      }
    }
    return any;
  }

  void CheckersPosition::EvaluateStatically(void) noexcept
  {
    MainPosition::EvaluateStatically();
    // ...
  }

  void CheckersPosition::GetAllMoves(void) noexcept                       // collect all moves for all pieces
  {
    MainPosition::GetAllMoves();
    JumpsOnly(movelistW);
    JumpsOnly(movelistB);
  }

  bool CheckersPosition::CanPromote(const Location& l, const Piece* p) const noexcept
  {
    return (p->IsColor(&Color::White) && l.y_ == 0) || (p->IsColor(&Color::Black) && l.y_ == sizeY - 1);
  }


  const VariantList& CheckersGame::GetVariants(void) noexcept
  {
    static VariantList v{ { Variant{ 8, 8, nullptr, 2, 20 } } };
    return v;
  }

  const PieceMapP& CheckersGame::GetPieces(void) noexcept
  {
    static const PieceMapP& p = std::make_shared<PieceMap>();
    p->Empty();
    p->Add(&CheckersPiece::CheckersPieceW);
    p->Add(&CheckersPiece::CheckersPieceB);
    p->Add(&CheckersPiece::CheckersKingW);
    p->Add(&CheckersPiece::CheckersKingB);
    p->Add(&CheckersPiece::CheckersQueenW);
    p->Add(&CheckersPiece::CheckersQueenB);
    return p;
  }

  const Dimensions& CheckersGame::GetDimensions(Coordinate x, Coordinate y) noexcept
  {
    static Dimensions d{
       Dimension(x, y, BoardStartX, BoardStartY, FieldSizeX, FieldSizeY, 1, 1),
       Dimension(4, 2, BoardStartX + FieldSizeX * (x + 1), BoardStartY + FieldSizeY / 2 + FieldSizeY * (y - 2), FieldSizeX, FieldSizeY),
       Dimension(x, 2, FieldSizeX * (x + 1), BoardStartY + FieldSizeSY, FieldSizeSX, FieldSizeSY, 0, FieldSizeY * y - FieldSizeSY * 4),
    };
    return d;
  }

}
