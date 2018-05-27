#include "stdafx.h"

#include "CamResource.h"
#include "CamGame.h"

namespace Cam
{
  const CamPiece CamPiece::WP{&Pawn  ::ThePawn,   &Color::White, IDB_WPL, IDB_WPD, IDB_WPS};
  const CamPiece CamPiece::WN{&Knight::TheKnight, &Color::White, IDB_WNL, IDB_WND, IDB_WNS};
  const CamPiece CamPiece::BP{&Pawn  ::ThePawn,   &Color::Black, IDB_BPL, IDB_BPD, IDB_BPS};
  const CamPiece CamPiece::BN{&Knight::TheKnight, &Color::Black, IDB_BNL, IDB_BND, IDB_BNS};

  void Pawn::CollectMoves(const MainPosition& p, const Location& l, std::vector<Move>& moves) const
  {
    std::vector<Step> s{};
    CollectMoves::CollectJumps(p, l, s, false, &Color::NoColor, moves);
    CollectMoves::CollectSlides(p, l, moves);
    moves = CollectMoves::EnforceJumps(moves);
  }
  void Knight::CollectMoves(const MainPosition& p, const Location& l, std::vector<Move>& moves) const
  {
    std::vector<Step> s{};
    CollectMoves::CollectJumps(p, l, s, true, &Color::NoColor, moves);
    CollectMoves::CollectSlides(p, l, moves);
    moves = CollectMoves::EnforceJumps(moves);
  }


  void CollectMoves::CollectSlides(const MainPosition& pos, const Location& fr, std::vector<Move>& m)
  {
    for (auto& d : Offset::Qdirection)              // try all eight directions
    {
      pos.AddIfLegal(m, fr, fr + d);
    }
  }

  bool CollectMoves::CollectJumps(const MainPosition& pos, const Location& fr, const std::vector<Step>& s, bool canter, const Color* c, std::vector<Move>& m)
  {
    bool any{false};                            // were any more jumps possible?
    for (auto& d : Offset::Qdirection)            // try all eight directions
    {
      std::vector<Step> s1{s};
      const Color* c1{c};
      const Location l1{fr + d};
      const Location l2{l1 + d};

      // check the jump-over tile
      const Piece * p1 = pos.GetPiece(l1);      // what is on the tile to jump over?
      if (p1 == nullptr) continue;              // tile is out of board, can't jump over it
      if (p1 == &Piece::NoTile) continue;       // tile is out of board, can't jump over it
      if (p1->IsBlank()) continue;              // tile is not occupied, can't jump over it

      Step::StepType st{Step::StepType::Jump};
      const Color* cp = p1->GetColor();
      if (cp != pos.OnTurn()) st = (Step::StepType) (Step::StepType::Jump | Step::StepType::Take);
      if (c1 == &Color::NoColor) c1 = cp;       // first jump - either one is allowed
      else if (c1 != cp)                        // trying to switch jumped color
      {
        if (c1 != pos.OnTurn()) continue;       // can't switch back to jumping own pieces
        if (!canter) continue;                  // can't switch to enemy pieces
      }

      // check the jump-to tile
      const Piece * p2 = pos.GetPiece(l2);      // what is on the jump-to tile
      if (p2 == nullptr) continue;              // tile is out of board, can't jump there
      if (p2 == &Piece::NoTile) continue;  // tile is out of board, can't jump over it
      if (!p2->IsBlank()) continue;             // tile is ccupied, can't jump there

      // check if the same jump was done before
      bool repeat{false};
      for (auto& ss : s)
      {
        if (ss.GetFr().GetLocation() == fr && ss.GetTo().GetLocation() == l2)
        {
          repeat = true;
          break;
        }
        if (ss.GetFr().GetLocation() == l2 && ss.GetTo().GetLocation() == fr)
        {
          repeat = true;
          break;
        }
      }
      if (repeat) continue;                 // don't make the same move again
      any = true;

      s1.push_back(Step(Field{fr, pos.GetPiece(fr)}, Field{l2,pos.GetPiece(l2)}, st, std::vector<Field>{Field{l1,p1}}));       // add the jump to the step list

      if (!CollectMoves::CollectJumps(pos, l2, s1, canter, c1, m) ||       // collect potential further jumps
        st == Step::StepType::Jump)
      {
        m.push_back(Move(s1));              // if it could not be extended, or was a jumpOver, add the step list as a move
      }
    }
    return any;
  }


  std::vector<Move> CollectMoves::EnforceJumps(std::vector<Move>& moves)
  {
    // if there are any takes (catching opponent pieces) possible, remove all non-takes
    bool takes{false};
    for (auto& m : moves) if (m.GetSteps().front().IsTake()) { takes = true; break; }
    if (takes)
    {
      std::vector<Move> allowed;  // here we will collect all take-moves
      for (auto& m : moves) if (m.IsTake()) allowed.push_back(m);
      return allowed;
    }
    return moves;
  }


  void CamPosition::GetAllMoves(void)  // collect all moves for all pieces
  {
    MainPosition::GetAllMoves();
    CollectMoves::EnforceJumps(movelistW);
    CollectMoves::EnforceJumps(movelistB);
  }

  bool CamPosition::AddIfLegal(std::vector<Move>& m, const Location fr, const Location to) const
  {
    const Piece * p = GetPiece(to);           // what is on the tile to move to?
    if (p == nullptr) return false;           // out of board
    if (p == &Piece::NoTile) return false;   // out of board
    if (!p->IsBlank()) return false;          // tile occupied

    m.push_back(Move{Field{fr,GetPiece(fr)}, Field{to,GetPiece(to)}});
    return false;
  };


  Move::PositionValue CamPosition::EvaluateStatically(void)
  {
    int v{0};

    for (unsigned int i = 0; i < sizeX; i++)
    {
      for (unsigned int j = 0; j < sizeY; j++)
      {
        const Piece* p = GetPiece(Location{i,j});
        if ((p == &Piece::NoTile) || (p == &Piece::NoPiece)) continue;
        v += ((p->IsColor(OnTurn())) ? 1 : -1) * (p->GetValue() + j);
      }
    }
    return v;
  }

  //Move::PositionValue CamPosition::EvaluateWin(void) const
  //{
  //  bool wfree{false};
  //  bool bfree{false};

  //  for (unsigned int i = 0; i < sizeX; i++)
  //  {
  //    const Piece* pw = GetPiece(Location(i, 0));
  //    const Piece* pb = GetPiece(Location(i, sizeY - 1));
  //    if (pw != &Piece::NoTile && !pw->IsColor(&Color::White)) wfree = true;
  //    if (pb != &Piece::NoTile && !pb->IsColor(&Color::Black)) bfree = true;
  //  }

  //  int dy = OnTurn() == &Color::White ? 1 : -1;
  //  if (!bfree) return -Move::win * dy;
  //  if (!wfree) return Move::win * dy;
  //  return 0;
  //}


  CamGame::CamGame(CamPosition* p, TakenPosition* t, StockPosition* s,
    CamLayout* l, CamTakenLayout* tl, CamStockLayout* sl) noexcept : Game{p,t,s,l,tl,sl}
  {
    AddToStock(Location(0, 0), &CamPiece::WN);
    AddToStock(Location(1, 0), &CamPiece::WP);
    AddToStock(Location(0, 1), &CamPiece::BP);
    AddToStock(Location(1, 1), &CamPiece::BN);
  }

  MiniCamPosition::MiniCamPosition(unsigned int x, unsigned int y) noexcept : CamPosition(x, y)
  {
    // Black Knights
    SetPiece(Location(2, 3), &CamPiece::BN);
    SetPiece(Location(4, 3), &CamPiece::BN);

    // Black Pawns
    SetPiece(Location(1, 4), &CamPiece::BP);
    SetPiece(Location(2, 4), &CamPiece::BP);
    SetPiece(Location(3, 4), &CamPiece::BP);
    SetPiece(Location(4, 4), &CamPiece::BP);
    SetPiece(Location(5, 4), &CamPiece::BP);

    // White Knights
    SetPiece(Location(2, 9), &CamPiece::WN);
    SetPiece(Location(4, 9), &CamPiece::WN);

    // White Pawns
    SetPiece(Location(1, 8), &CamPiece::WP);
    SetPiece(Location(2, 8), &CamPiece::WP);
    SetPiece(Location(3, 8), &CamPiece::WP);
    SetPiece(Location(4, 8), &CamPiece::WP);
    SetPiece(Location(5, 8), &CamPiece::WP);

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

    SetPiece(Location(0,10), &Piece::NoTile);
    SetPiece(Location(6,10), &Piece::NoTile);

    SetPiece(Location(0,11), &Piece::NoTile);
    SetPiece(Location(1,11), &Piece::NoTile);
    SetPiece(Location(5,11), &Piece::NoTile);
    SetPiece(Location(6,11), &Piece::NoTile);

    SetPiece(Location(0,12), &Piece::NoTile);
    SetPiece(Location(1,12), &Piece::NoTile);
    SetPiece(Location(2,12), &Piece::NoTile);
    SetPiece(Location(4,12), &Piece::NoTile);
    SetPiece(Location(5,12), &Piece::NoTile);
    SetPiece(Location(6,12), &Piece::NoTile);
  }

  FullCamPosition::FullCamPosition(unsigned int x, unsigned int y) noexcept : CamPosition(x, y)
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
    SetPiece(Location(10,0), &Piece::NoTile);
    SetPiece(Location(11,0), &Piece::NoTile);

    SetPiece(Location(0, 1), &Piece::NoTile);
    SetPiece(Location(1, 1), &Piece::NoTile);
    SetPiece(Location(10,1), &Piece::NoTile);
    SetPiece(Location(11,1), &Piece::NoTile);

    SetPiece(Location(0, 2), &Piece::NoTile);
    SetPiece(Location(11,2), &Piece::NoTile);

    SetPiece(Location(0, 13), &Piece::NoTile);
    SetPiece(Location(11,13), &Piece::NoTile);

    SetPiece(Location(0, 14), &Piece::NoTile);
    SetPiece(Location(1, 14), &Piece::NoTile);
    SetPiece(Location(10,14), &Piece::NoTile);
    SetPiece(Location(11,14), &Piece::NoTile);

    SetPiece(Location(0, 15), &Piece::NoTile);
    SetPiece(Location(1, 15), &Piece::NoTile);
    SetPiece(Location(2, 15), &Piece::NoTile);
    SetPiece(Location(3, 15), &Piece::NoTile);
    SetPiece(Location(4, 15), &Piece::NoTile);
    SetPiece(Location(7, 15), &Piece::NoTile);
    SetPiece(Location(8, 15), &Piece::NoTile);
    SetPiece(Location(9, 15), &Piece::NoTile);
    SetPiece(Location(10,15), &Piece::NoTile);
    SetPiece(Location(11,15), &Piece::NoTile);
  }

}
