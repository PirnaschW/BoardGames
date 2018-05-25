#include "stdafx.h"

#include "ShogiResource.h"
#include "ShogiGame.h"

namespace Shogi
{

  // S = Standard pieces, White
  const ShogiPiece ShogiPiece::ShogiSKW{&King   ::TheKing,    &Color::White, &ShogiPiece::ShogiSKW, &ShogiPiece::ShogiSKW, IDB_SHOGI_SK_W, IDB_SHOGI_SK_S};
  const ShogiPiece ShogiPiece::ShogiSGW{&Gold   ::TheGold,    &Color::White, &ShogiPiece::ShogiSGW, &ShogiPiece::ShogiSGW, IDB_SHOGI_SG_W, IDB_SHOGI_SG_S};
  const ShogiPiece ShogiPiece::ShogiSSW{&Silver ::TheSilver,  &Color::White, &ShogiPiece::ShogiPSW, &ShogiPiece::ShogiSSW, IDB_SHOGI_SS_W, IDB_SHOGI_SS_S};
  const ShogiPiece ShogiPiece::ShogiSBW{&Bishop ::TheBishop,  &Color::White, &ShogiPiece::ShogiPBW, &ShogiPiece::ShogiSBW, IDB_SHOGI_SB_W, IDB_SHOGI_SB_S};
  const ShogiPiece ShogiPiece::ShogiSRW{&Rook   ::TheRook,    &Color::White, &ShogiPiece::ShogiPRW, &ShogiPiece::ShogiSRW, IDB_SHOGI_SR_W, IDB_SHOGI_SR_S};
  const ShogiPiece ShogiPiece::ShogiSNW{&Knight ::TheKnight,  &Color::White, &ShogiPiece::ShogiPNW, &ShogiPiece::ShogiSNW, IDB_SHOGI_SN_W, IDB_SHOGI_SN_S};
  const ShogiPiece ShogiPiece::ShogiSLW{&Lance  ::TheLance,   &Color::White, &ShogiPiece::ShogiPLW, &ShogiPiece::ShogiSLW, IDB_SHOGI_SL_W, IDB_SHOGI_SL_S};
  const ShogiPiece ShogiPiece::ShogiSPW{&Pawn   ::ThePawn,    &Color::White, &ShogiPiece::ShogiPPW, &ShogiPiece::ShogiSPW, IDB_SHOGI_SP_W, IDB_SHOGI_SP_S};
  // P = Promoted pieces, White         
  const ShogiPiece ShogiPiece::ShogiPSW{&PSilver::ThePSilver, &Color::White, &ShogiPiece::ShogiPSW, &ShogiPiece::ShogiSSW, IDB_SHOGI_PS_W, IDB_SHOGI_PS_S};
  const ShogiPiece ShogiPiece::ShogiPBW{&PBishop::ThePBishop, &Color::White, &ShogiPiece::ShogiPBW, &ShogiPiece::ShogiSBW, IDB_SHOGI_PB_W, IDB_SHOGI_PB_S};
  const ShogiPiece ShogiPiece::ShogiPRW{&PRook  ::ThePRook,   &Color::White, &ShogiPiece::ShogiPRW, &ShogiPiece::ShogiSRW, IDB_SHOGI_PR_W, IDB_SHOGI_PR_S};
  const ShogiPiece ShogiPiece::ShogiPNW{&PKnight::ThePKnight, &Color::White, &ShogiPiece::ShogiPNW, &ShogiPiece::ShogiSNW, IDB_SHOGI_PN_W, IDB_SHOGI_PN_S};
  const ShogiPiece ShogiPiece::ShogiPLW{&PLance ::ThePLance,  &Color::White, &ShogiPiece::ShogiPLW, &ShogiPiece::ShogiSLW, IDB_SHOGI_PL_W, IDB_SHOGI_PL_S};
  const ShogiPiece ShogiPiece::ShogiPPW{&PPawn  ::ThePPawn,   &Color::White, &ShogiPiece::ShogiPPW, &ShogiPiece::ShogiSPW, IDB_SHOGI_PP_W, IDB_SHOGI_PP_S};
                                        
  // S = Standard pieces, Black         
  const ShogiPiece ShogiPiece::ShogiSKB{&King   ::TheKing,    &Color::Black, &ShogiPiece::ShogiSKB, &ShogiPiece::ShogiSKB, IDB_SHOGI_SK_B, IDB_SHOGI_SK_S};
  const ShogiPiece ShogiPiece::ShogiSGB{&Gold   ::TheGold,    &Color::Black, &ShogiPiece::ShogiSGB, &ShogiPiece::ShogiSGB, IDB_SHOGI_SG_B, IDB_SHOGI_SG_S};
  const ShogiPiece ShogiPiece::ShogiSSB{&Silver ::TheSilver,  &Color::Black, &ShogiPiece::ShogiPSB, &ShogiPiece::ShogiSSB, IDB_SHOGI_SS_B, IDB_SHOGI_SS_S};
  const ShogiPiece ShogiPiece::ShogiSBB{&Bishop ::TheBishop,  &Color::Black, &ShogiPiece::ShogiPBB, &ShogiPiece::ShogiSBB, IDB_SHOGI_SB_B, IDB_SHOGI_SB_S};
  const ShogiPiece ShogiPiece::ShogiSRB{&Rook   ::TheRook,    &Color::Black, &ShogiPiece::ShogiPRB, &ShogiPiece::ShogiSRB, IDB_SHOGI_SR_B, IDB_SHOGI_SR_S};
  const ShogiPiece ShogiPiece::ShogiSNB{&Knight ::TheKnight,  &Color::Black, &ShogiPiece::ShogiPNB, &ShogiPiece::ShogiSNB, IDB_SHOGI_SN_B, IDB_SHOGI_SN_S};
  const ShogiPiece ShogiPiece::ShogiSLB{&Lance  ::TheLance,   &Color::Black, &ShogiPiece::ShogiPLB, &ShogiPiece::ShogiSLB, IDB_SHOGI_SL_B, IDB_SHOGI_SL_S};
  const ShogiPiece ShogiPiece::ShogiSPB{&Pawn   ::ThePawn,    &Color::Black, &ShogiPiece::ShogiPPB, &ShogiPiece::ShogiSPB, IDB_SHOGI_SP_B, IDB_SHOGI_SP_S};
  // P = Promoted pieces, Black         
  const ShogiPiece ShogiPiece::ShogiPSB{&PSilver::ThePSilver, &Color::Black, &ShogiPiece::ShogiPSB, &ShogiPiece::ShogiSSB, IDB_SHOGI_PS_B, IDB_SHOGI_PS_S};
  const ShogiPiece ShogiPiece::ShogiPBB{&PBishop::ThePBishop, &Color::Black, &ShogiPiece::ShogiPBB, &ShogiPiece::ShogiSBB, IDB_SHOGI_PB_B, IDB_SHOGI_PB_S};
  const ShogiPiece ShogiPiece::ShogiPRB{&PRook  ::ThePRook,   &Color::Black, &ShogiPiece::ShogiPRB, &ShogiPiece::ShogiSRB, IDB_SHOGI_PR_B, IDB_SHOGI_PR_S};
  const ShogiPiece ShogiPiece::ShogiPNB{&PKnight::ThePKnight, &Color::Black, &ShogiPiece::ShogiPNB, &ShogiPiece::ShogiSNB, IDB_SHOGI_PN_B, IDB_SHOGI_PN_S};
  const ShogiPiece ShogiPiece::ShogiPLB{&PLance ::ThePLance,  &Color::Black, &ShogiPiece::ShogiPLB, &ShogiPiece::ShogiSLB, IDB_SHOGI_PL_B, IDB_SHOGI_PL_S};
  const ShogiPiece ShogiPiece::ShogiPPB{&PPawn  ::ThePPawn,   &Color::Black, &ShogiPiece::ShogiPPB, &ShogiPiece::ShogiSPB, IDB_SHOGI_PP_B, IDB_SHOGI_PP_S};


  void Pawn::CollectMoves(const MainPosition& p, const Location& l, std::vector<Move>& moves) const
  {
    int dy = p.OnTurn() == &Color::White ? -1 : 1;
    p.AddIfLegal(moves, l, l + Offset(0, dy));
  }
  void Lance::CollectMoves(const MainPosition& p, const Location& l, std::vector<Move>& moves) const
  {
    int dy = p.OnTurn() == &Color::White ? -1 : 1;
    for (int z = 1; p.AddIfLegal(moves, l, l + Offset(0, z*dy)); z++);
  }
  void Knight::CollectMoves(const MainPosition& p, const Location& l, std::vector<Move>& moves) const
  {
    int dy = p.OnTurn() == &Color::White ? -1 : 1;
    p.AddIfLegal(moves, l, l + Offset(+1, 2 * dy));
    p.AddIfLegal(moves, l, l + Offset(-1, 2 * dy));
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
    int dy = p.OnTurn() == &Color::White ? -1 : 1;
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
    int dy = p.OnTurn() == &Color::White ? -1 : 1;
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


  bool ShogiPosition::AddIfLegal(std::vector<Move>& m, const Location fr, const Location to) const
  {
    const Piece * p = GetPiece(to);
    if (p == nullptr) return false;  // out of board
    if (p->IsColor(OnTurn())) return false;  // own piece

    Step::StepType st = p->IsBlank() ? Step::StepType::Normal : Step::StepType::Take;
    m.push_back(Move{Field{fr,GetPiece(fr)}, Field{to,GetPiece(to)},st,std::vector<Field>{Field{to,GetPiece(to)}}});
    if (CanPromote(to) && GetPiece(fr)->IsPromotable())
    {
      st = static_cast<Step::StepType>(st | Step::StepType::Promote);
      m.push_back(Move{Field{fr,GetPiece(fr)}, Field{to,GetPiece(to)},st,std::vector<Field>{Field{to,GetPiece(to)}}});
    }
    return p->IsBlank();   // if free tile, keep trying this direction
  }
  
  Move::PositionValue ShogiPosition::EvaluateStatically(void)
  {
    int v{0};
    for (unsigned int i = 0; i < sizeX; i++)
    {
      for (unsigned int j = 0; j < sizeY; j++)
      {
        const Piece* p = GetPiece(Location{i,j});
        if ((p == &Piece::NoTile) || (p == &Piece::NoPiece)) continue;
        v += ((p->IsColor(OnTurn())) ? 1 : -1) * p->GetValue();
      }
    }
    return v;
  }

  //Move::PositionValue ShogiPosition::EvaluateWin(void) const
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
  //  if (!kw) return -Move::win * dy;  // no white king - opponent wins
  //  if (!kb) return Move::win * dy;  // no black king - player wins
  //  return 0;
  //}

  bool MiniShogiPosition::CanPromote(const Location &l) const noexcept
  {
    return (OnTurn() == &Color::White && (l.y == 0)) || (OnTurn() != &Color::White && (l.y == 4));
  }

  bool FullShogiPosition::CanPromote(const Location &l) const noexcept
  {
    return (OnTurn() == &Color::White && (l.y < 3)) || (OnTurn() != &Color::White && (l.y > 8));
  }


  ShogiGame::ShogiGame(
    ShogiPosition* p, TakenPosition* t, StockPosition* s,
    ShogiLayout* l, ShogiTakenLayout* tl, ShogiStockLayout* sl) noexcept : Game{p,t,s,l,tl,sl}
  {
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

  MiniShogiPosition::MiniShogiPosition(unsigned int x, unsigned int y) noexcept : ShogiPosition(x, y)
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

  FullShogiPosition::FullShogiPosition(unsigned int x, unsigned int y) noexcept : ShogiPosition(x, y)
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

}
