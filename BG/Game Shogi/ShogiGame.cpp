#include "stdafx.h"

#include "ShogiGame.h"

namespace Shogi
{

  void Pawn::CollectMoves(const MainPosition& p, const Location& l, std::vector<Move>& moves) const
  {
    const int dy = p.OnTurn() == &Color::White ? -1 : 1;
    p.AddIfLegal(moves, l, l + Offset(0, dy));
  }
  bool Pawn::CanDrop(const MainPosition* pos, const Location& l) const noexcept
  {
    if (!((pos->OnTurn() == &Color::White) ? (l.y < pos->GetSizeY() - 1) : (l.y > 1))) return false;   // drop anywhere except last row
    for (Coordinate j = 0; j < pos->GetSizeY(); j++)
    {
      const Piece* p = pos->GetPiece(Location{ l.x,j });
      if (p == ((pos->OnTurn() == &Color::White) ? &ShogiPiece::ShogiSPW : &ShogiPiece::ShogiSPB)) return false;
    }
    return true;
  }

  void Lance::CollectMoves(const MainPosition& p, const Location& l, std::vector<Move>& moves) const
  {
    const int dy = p.OnTurn() == &Color::White ? -1 : 1;
    for (int z = 1; p.AddIfLegal(moves, l, l + Offset(0, z*dy)); z++);
  }
  bool Lance::CanDrop(const MainPosition* pos, const Location& l) const noexcept
  {
    return (pos->OnTurn() == &Color::White) ? (l.y < pos->GetSizeY() - 1) : (l.y > 1);   // drop anywhere except last row
  }
  void Knight::CollectMoves(const MainPosition& p, const Location& l, std::vector<Move>& moves) const
  {
    const int dy = p.OnTurn() == &Color::White ? -1 : 1;
    p.AddIfLegal(moves, l, l + Offset(+1, 2 * dy));
    p.AddIfLegal(moves, l, l + Offset(-1, 2 * dy));
  }
  bool Knight::CanDrop(const MainPosition* pos, const Location& l) const noexcept
  {
    return (pos->OnTurn() == &Color::White) ? (l.y < pos->GetSizeY() - 2) : (l.y > 2);   // drop anywhere except last two rows
  }
  void Bishop::CollectMoves(const MainPosition& p, const Location& l, std::vector<Move>& moves) const
  {
    for (int z = 1; p.AddIfLegal(moves, l, l + Offset(+z, +z)); z++);
    for (int z = 1; p.AddIfLegal(moves, l, l + Offset(+z, -z)); z++);
    for (int z = 1; p.AddIfLegal(moves, l, l + Offset(-z, +z)); z++);
    for (int z = 1; p.AddIfLegal(moves, l, l + Offset(-z, -z)); z++);
  }

  void Rook::CollectMoves(const MainPosition& p, const Location& l, std::vector<Move>& moves) const
  {
    for (int z = 1; p.AddIfLegal(moves, l, l + Offset(+0, +z)); z++);
    for (int z = 1; p.AddIfLegal(moves, l, l + Offset(+0, -z)); z++);
    for (int z = 1; p.AddIfLegal(moves, l, l + Offset(+z, +0)); z++);
    for (int z = 1; p.AddIfLegal(moves, l, l + Offset(-z, +0)); z++);
  }

  void Silver::CollectMoves(const MainPosition& p, const Location& l, std::vector<Move>& moves) const
  {
    const int dy = p.OnTurn() == &Color::White ? -1 : 1;
    p.AddIfLegal(moves, l, l + Offset(+1, dy));
    p.AddIfLegal(moves, l, l + Offset(+0, dy));
    p.AddIfLegal(moves, l, l + Offset(-1, dy));
    p.AddIfLegal(moves, l, l + Offset(-1, -dy));
    p.AddIfLegal(moves, l, l + Offset(+1, -dy));
  }

  void Gold::CollectMoves(const MainPosition& p, const Location& l, std::vector<Move>& moves) const
  {
    Gold::CollectGoldMoves(p, l, moves);
  }
  void Gold::CollectGoldMoves(const MainPosition& p, const Location& l, std::vector<Move>& moves)
  {
    const int dy = p.OnTurn() == &Color::White ? -1 : 1;
    p.AddIfLegal(moves, l, l + Offset(+1, dy));
    p.AddIfLegal(moves, l, l + Offset(+0, dy));
    p.AddIfLegal(moves, l, l + Offset(-1, dy));
    p.AddIfLegal(moves, l, l + Offset(+1, +0));
    p.AddIfLegal(moves, l, l + Offset(-1, +0));
    p.AddIfLegal(moves, l, l + Offset(+0, -dy));
  }

  void King::CollectMoves(const MainPosition& p, const Location& l, std::vector<Move>& moves) const
  {
    p.AddIfLegal(moves, l, l + Offset(+1, +1));
    p.AddIfLegal(moves, l, l + Offset(+1, -1));
    p.AddIfLegal(moves, l, l + Offset(-1, +1));
    p.AddIfLegal(moves, l, l + Offset(-1, -1));
    p.AddIfLegal(moves, l, l + Offset(+0, +1));
    p.AddIfLegal(moves, l, l + Offset(+0, -1));
    p.AddIfLegal(moves, l, l + Offset(+1, +0));
    p.AddIfLegal(moves, l, l + Offset(-1, +0));
  }
  void PPawn::CollectMoves(const MainPosition& p, const Location& l, std::vector<Move>& moves) const
  {
    Gold::CollectGoldMoves(p, l, moves);
  }
  void PLance::CollectMoves(const MainPosition& p, const Location& l, std::vector<Move>& moves) const
  {
    Gold::CollectGoldMoves(p, l, moves);
  }
  void PKnight::CollectMoves(const MainPosition& p, const Location& l, std::vector<Move>& moves) const
  {
    Gold::CollectGoldMoves(p, l, moves);
  }
  void PBishop::CollectMoves(const MainPosition& p, const Location& l, std::vector<Move>& moves) const
  {
    p.AddIfLegal(moves, l, l + Offset(+1, +0));
    p.AddIfLegal(moves, l, l + Offset(-1, +0));
    p.AddIfLegal(moves, l, l + Offset(+0, +1));
    p.AddIfLegal(moves, l, l + Offset(+0, -1));
    for (int z = 1; p.AddIfLegal(moves, l, l + Offset(+z, +z)); z++);
    for (int z = 1; p.AddIfLegal(moves, l, l + Offset(+z, -z)); z++);
    for (int z = 1; p.AddIfLegal(moves, l, l + Offset(-z, +z)); z++);
    for (int z = 1; p.AddIfLegal(moves, l, l + Offset(-z, -z)); z++);
  }
  void PRook::CollectMoves(const MainPosition& p, const Location& l, std::vector<Move>& moves) const
  {
    p.AddIfLegal(moves, l, l + Offset(+1, +1));
    p.AddIfLegal(moves, l, l + Offset(-1, +1));
    p.AddIfLegal(moves, l, l + Offset(-1, -1));
    p.AddIfLegal(moves, l, l + Offset(+1, -1));
    for (int z = 1; p.AddIfLegal(moves, l, l + Offset(+0, +z)); z++);
    for (int z = 1; p.AddIfLegal(moves, l, l + Offset(+0, -z)); z++);
    for (int z = 1; p.AddIfLegal(moves, l, l + Offset(+z, +0)); z++);
    for (int z = 1; p.AddIfLegal(moves, l, l + Offset(-z, -0)); z++);
  }
  void PSilver::CollectMoves(const MainPosition& p, const Location& l, std::vector<Move>& moves) const
  {
    Gold::CollectGoldMoves(p, l, moves);
  }


  ShogiPosition::ShogiPosition(unsigned int x, unsigned int y) noexcept : MainPosition(x, y)
  {
    assert((x == 9 && y == 9) || (x == 5 && y == 5));
    if (ShogiGame::IsFull(x, y))
    {
      SetPiece(Location(0, 0), &ShogiPiece::ShogiSLB);
      SetPiece(Location(1, 0), &ShogiPiece::ShogiSNB);
      SetPiece(Location(2, 0), &ShogiPiece::ShogiSSB);
      SetPiece(Location(3, 0), &ShogiPiece::ShogiSGB);
      SetPiece(Location(4, 0), &ShogiPiece::ShogiSKB);
      SetPiece(Location(5, 0), &ShogiPiece::ShogiSGB);
      SetPiece(Location(6, 0), &ShogiPiece::ShogiSSB);
      SetPiece(Location(7, 0), &ShogiPiece::ShogiSNB);
      SetPiece(Location(8, 0), &ShogiPiece::ShogiSLB);

      SetPiece(Location(1, 1), &ShogiPiece::ShogiSRB);
      SetPiece(Location(7, 1), &ShogiPiece::ShogiSBB);

      SetPiece(Location(0, 2), &ShogiPiece::ShogiSPB);
      SetPiece(Location(1, 2), &ShogiPiece::ShogiSPB);
      SetPiece(Location(2, 2), &ShogiPiece::ShogiSPB);
      SetPiece(Location(3, 2), &ShogiPiece::ShogiSPB);
      SetPiece(Location(4, 2), &ShogiPiece::ShogiSPB);
      SetPiece(Location(5, 2), &ShogiPiece::ShogiSPB);
      SetPiece(Location(6, 2), &ShogiPiece::ShogiSPB);
      SetPiece(Location(7, 2), &ShogiPiece::ShogiSPB);
      SetPiece(Location(8, 2), &ShogiPiece::ShogiSPB);


      SetPiece(Location(0, 6), &ShogiPiece::ShogiSPW);
      SetPiece(Location(1, 6), &ShogiPiece::ShogiSPW);
      SetPiece(Location(2, 6), &ShogiPiece::ShogiSPW);
      SetPiece(Location(3, 6), &ShogiPiece::ShogiSPW);
      SetPiece(Location(4, 6), &ShogiPiece::ShogiSPW);
      SetPiece(Location(5, 6), &ShogiPiece::ShogiSPW);
      SetPiece(Location(6, 6), &ShogiPiece::ShogiSPW);
      SetPiece(Location(7, 6), &ShogiPiece::ShogiSPW);
      SetPiece(Location(8, 6), &ShogiPiece::ShogiSPW);

      SetPiece(Location(1, 7), &ShogiPiece::ShogiSBW);
      SetPiece(Location(7, 7), &ShogiPiece::ShogiSRW);

      SetPiece(Location(0, 8), &ShogiPiece::ShogiSLW);
      SetPiece(Location(1, 8), &ShogiPiece::ShogiSNW);
      SetPiece(Location(2, 8), &ShogiPiece::ShogiSSW);
      SetPiece(Location(3, 8), &ShogiPiece::ShogiSGW);
      SetPiece(Location(4, 8), &ShogiPiece::ShogiSKW);
      SetPiece(Location(5, 8), &ShogiPiece::ShogiSGW);
      SetPiece(Location(6, 8), &ShogiPiece::ShogiSSW);
      SetPiece(Location(7, 8), &ShogiPiece::ShogiSNW);
      SetPiece(Location(8, 8), &ShogiPiece::ShogiSLW);
    }
    else
    {
      SetPiece(Location(0, 0), &ShogiPiece::ShogiSRB);
      SetPiece(Location(1, 0), &ShogiPiece::ShogiSBB);
      SetPiece(Location(2, 0), &ShogiPiece::ShogiSSB);
      SetPiece(Location(3, 0), &ShogiPiece::ShogiSGB);
      SetPiece(Location(4, 0), &ShogiPiece::ShogiSKB);
      SetPiece(Location(4, 1), &ShogiPiece::ShogiSPB);

      SetPiece(Location(4, 4), &ShogiPiece::ShogiSRW);
      SetPiece(Location(3, 4), &ShogiPiece::ShogiSBW);
      SetPiece(Location(2, 4), &ShogiPiece::ShogiSSW);
      SetPiece(Location(1, 4), &ShogiPiece::ShogiSGW);
      SetPiece(Location(0, 4), &ShogiPiece::ShogiSKW);
      SetPiece(Location(0, 3), &ShogiPiece::ShogiSPW);
    }
  }

  void ShogiPosition::GetAllMoves(void)              // generate all moves and save list
  {
    MainPosition::GetAllMoves();    // standard: get moves for all pieces on the board

    // add all potential placement moves
    for (unsigned int i = 0; i < sizeX; i++)
      for (unsigned int j = 0; j < sizeY; j++)
      {
        const Location l{ i,j };
        if (GetPiece(l)->IsBlank())
        {
          const Piece* p{ nullptr };
          for (unsigned int z = 0; (p = tpos->GetPiece(Location(z, 0))) != &Piece::NoTile; z++)
          {
            const ShogiPiece* pp = dynamic_cast<const ShogiPiece*>(p); // must be a shogipiece, verify it
            if (pp->CanDrop(this,l))
              movelistB.push_back(Step{ Field{ Location(z, 0),nullptr }, Field{ l,p },Step::StepType::Drop,std::vector<Field>{} });
          }
        }
      }
  }
  
  bool ShogiPosition::AddIfLegal(std::vector<Move>& m, const Location fr, const Location to) const
  {
    const Piece* pt = GetPiece(to);
    if (pt == nullptr) return false;  // out of board
    if (pt->IsColor(OnTurn())) return false;  // own piece

    const ShogiPiece* pf = dynamic_cast<const ShogiPiece*>(GetPiece(fr));

    Step::StepType st = pt->IsBlank() ? Step::StepType::Normal : Step::StepType::Take;
    if (pf->CanDrop(this, to))  // 'can drop' is also 'can move there'
    {
      m.push_back(Step{ Field{fr,pf}, Field{to,pf},st,std::vector<Field>{Field{to,pt}} });
    }
    if ((CanPromote(fr) || CanPromote(to)) && GetPiece(fr)->IsPromotable())
    {
      st = static_cast<Step::StepType>(st | Step::StepType::Promote);
      m.push_back(Step{ Field{fr,GetPiece(fr)}, Field{to,pf->Promote(true)},st,std::vector<Field>{Field{to,pt}} });
    }
    return pt->IsBlank();   // if free tile, keep trying this direction
  }

  void ShogiPosition::EvaluateStatically(void)
  {
    GetAllMoves();                                                        // fill the move lists
    if (onTurn == &Color::White && movelistW.empty()) value = PositionValue::PValueType::Lost;        // if no more moves, game over
    else if (onTurn == &Color::Black && movelistB.empty()) value = PositionValue::PValueType::Won;
    else
    {
      int v{ 0 };
      for (unsigned int i = 0; i < sizeX; i++)
      {
        for (unsigned int j = 0; j < sizeY; j++)
        {
          const Piece* p = GetPiece(Location{ i,j });
          if ((p == &Piece::NoTile) || (p == &Piece::NoPiece)) continue;
          v += ((p->IsColor(OnTurn())) ? 1 : -1) * p->GetValue();
        }
      }
      value = v;
    }
  }

  //PositionValue ShogiPosition::EvaluateWin(void) const
  //{
  //  bool kw{false};
  //  bool kb{false};

  //  for (unsigned int i = 0; i < sizeX; i++)
  //  {
  //    for (unsigned int j = 0; j < sizeY; j++)
  //    {
  //      const Piece* p = GetPiece(Location{i,j});
  //      if ((p == &ShogiPiece::ShogiSKW)) kw = true;
  //      if ((p == &ShogiPiece::ShogiSKB)) kb = true;
  //    }
  //  }

  //  int dy = OnTurn() == &Color::White ? 1 : -1;
  //  if (!kw) return PositionValue::PValueType::Lost * dy;  // no white king - opponent wins
  //  if (!kb) return PositionValue::PValueType::Won * dy;  // no black king - player wins
  //  return 0;
  //}

  inline bool ShogiPosition::CanPromote(const Location& l) const noexcept
  {
    bool f = ShogiGame::IsFull(sizeX, sizeY);
    return (OnTurn() == &Color::White && (l.y < (f?3U:1U))) || (OnTurn() != &Color::White && (l.y > (f?8U:3U)));
  }
    

  ShogiGame::ShogiGame(unsigned int x, unsigned int y) noexcept : ShogiGame(
    new ShogiPosition(x, y),
    IsFull(x, y) ? new TakenPosition(27, 2) : new TakenPosition(15, 2),
    new StockPosition(15, 2),
    new ShogiLayout(x, y),
    new ShogiTakenLayout(x, y),
    new ShogiStockLayout(x, y)) {}

  ShogiGame::ShogiGame(
    ShogiPosition* p, TakenPosition* t, StockPosition* s,
    ShogiLayout* l, ShogiTakenLayout* tl, ShogiStockLayout* sl) noexcept : Game{ p,t,s,l,tl,sl }
  {
    p->SetTPos(t);
    AddToStock(Location(0, 0), &ShogiPiece::ShogiSKW);
    AddToStock(Location(1, 0), &ShogiPiece::ShogiSGW);
    AddToStock(Location(2, 0), &ShogiPiece::ShogiSSW);
    AddToStock(Location(3, 0), &ShogiPiece::ShogiSBW);
    AddToStock(Location(4, 0), &ShogiPiece::ShogiSRW);
    AddToStock(Location(5, 0), &ShogiPiece::ShogiSNW);
    AddToStock(Location(6, 0), &ShogiPiece::ShogiSLW);
    AddToStock(Location(7, 0), &ShogiPiece::ShogiSPW);

    AddToStock(Location(8, 0), &ShogiPiece::ShogiPSW);
    AddToStock(Location(9, 0), &ShogiPiece::ShogiPBW);
    AddToStock(Location(10, 0), &ShogiPiece::ShogiPRW);
    AddToStock(Location(11, 0), &ShogiPiece::ShogiPNW);
    AddToStock(Location(12, 0), &ShogiPiece::ShogiPLW);
    AddToStock(Location(13, 0), &ShogiPiece::ShogiPPW);

    AddToStock(Location(0, 1), &ShogiPiece::ShogiSKW);
    AddToStock(Location(1, 1), &ShogiPiece::ShogiSGW);
    AddToStock(Location(2, 1), &ShogiPiece::ShogiSSW);
    AddToStock(Location(3, 1), &ShogiPiece::ShogiSBW);
    AddToStock(Location(4, 1), &ShogiPiece::ShogiSRW);
    AddToStock(Location(5, 1), &ShogiPiece::ShogiSNW);
    AddToStock(Location(6, 1), &ShogiPiece::ShogiSLW);
    AddToStock(Location(7, 1), &ShogiPiece::ShogiSPW);

    AddToStock(Location(8, 1), &ShogiPiece::ShogiPSW);
    AddToStock(Location(9, 1), &ShogiPiece::ShogiPBW);
    AddToStock(Location(10, 1), &ShogiPiece::ShogiPRW);
    AddToStock(Location(11, 1), &ShogiPiece::ShogiPNW);
    AddToStock(Location(12, 1), &ShogiPiece::ShogiPLW);
    AddToStock(Location(13, 1), &ShogiPiece::ShogiPPW);
  }

}
