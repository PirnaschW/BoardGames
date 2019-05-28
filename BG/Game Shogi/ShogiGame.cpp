#include "stdafx.h"

#include "ShogiGame.h"

namespace Shogi
{
  inline const Pawn    Pawn   ::ThePawn   {};
  inline const Knight  Knight ::TheKnight {};
  inline const Bishop  Bishop ::TheBishop {};
  inline const Rook    Rook   ::TheRook   {};
  inline const Lance   Lance  ::TheLance  {};
  inline const Silver  Silver ::TheSilver {};
  inline const Gold    Gold   ::TheGold   {};
  inline const King    King   ::TheKing   {};
  inline const PPawn   PPawn  ::ThePPawn  {};
  inline const PKnight PKnight::ThePKnight{};
  inline const PBishop PBishop::ThePBishop{};
  inline const PRook   PRook  ::ThePRook  {};
  inline const PLance  PLance ::ThePLance {};
  inline const PSilver PSilver::ThePSilver{};
  // S = Standard pieces, White
  inline const ShogiPiece ShogiPiece::ShogiSKW{ &King::TheKing,       &Color::White, &ShogiPiece::ShogiSKW, &ShogiPiece::ShogiSKW, IDB_SHOGI_SK_W, IDB_SHOGI_SK_S };
  inline const ShogiPiece ShogiPiece::ShogiSGW{ &Gold::TheGold,       &Color::White, &ShogiPiece::ShogiSGW, &ShogiPiece::ShogiSGW, IDB_SHOGI_SG_W, IDB_SHOGI_SG_S };
  inline const ShogiPiece ShogiPiece::ShogiSSW{ &Silver::TheSilver,   &Color::White, &ShogiPiece::ShogiPSW, &ShogiPiece::ShogiSSW, IDB_SHOGI_SS_W, IDB_SHOGI_SS_S };
  inline const ShogiPiece ShogiPiece::ShogiSBW{ &Bishop::TheBishop,   &Color::White, &ShogiPiece::ShogiPBW, &ShogiPiece::ShogiSBW, IDB_SHOGI_SB_W, IDB_SHOGI_SB_S };
  inline const ShogiPiece ShogiPiece::ShogiSRW{ &Rook::TheRook,       &Color::White, &ShogiPiece::ShogiPRW, &ShogiPiece::ShogiSRW, IDB_SHOGI_SR_W, IDB_SHOGI_SR_S };
  inline const ShogiPiece ShogiPiece::ShogiSNW{ &Knight::TheKnight,   &Color::White, &ShogiPiece::ShogiPNW, &ShogiPiece::ShogiSNW, IDB_SHOGI_SN_W, IDB_SHOGI_SN_S };
  inline const ShogiPiece ShogiPiece::ShogiSLW{ &Lance::TheLance,     &Color::White, &ShogiPiece::ShogiPLW, &ShogiPiece::ShogiSLW, IDB_SHOGI_SL_W, IDB_SHOGI_SL_S };
  inline const ShogiPiece ShogiPiece::ShogiSPW{ &Pawn::ThePawn,       &Color::White, &ShogiPiece::ShogiPPW, &ShogiPiece::ShogiSPW, IDB_SHOGI_SP_W, IDB_SHOGI_SP_S };
  // P = Promoted Pieces, White   
  inline const ShogiPiece ShogiPiece::ShogiPSW{ &PSilver::ThePSilver, &Color::White, &ShogiPiece::ShogiPSW, &ShogiPiece::ShogiSSW, IDB_SHOGI_PS_W, IDB_SHOGI_PS_S };
  inline const ShogiPiece ShogiPiece::ShogiPBW{ &PBishop::ThePBishop, &Color::White, &ShogiPiece::ShogiPBW, &ShogiPiece::ShogiSBW, IDB_SHOGI_PB_W, IDB_SHOGI_PB_S };
  inline const ShogiPiece ShogiPiece::ShogiPRW{ &PRook::ThePRook,     &Color::White, &ShogiPiece::ShogiPRW, &ShogiPiece::ShogiSRW, IDB_SHOGI_PR_W, IDB_SHOGI_PR_S };
  inline const ShogiPiece ShogiPiece::ShogiPNW{ &PKnight::ThePKnight, &Color::White, &ShogiPiece::ShogiPNW, &ShogiPiece::ShogiSNW, IDB_SHOGI_PN_W, IDB_SHOGI_PN_S };
  inline const ShogiPiece ShogiPiece::ShogiPLW{ &PLance::ThePLance,   &Color::White, &ShogiPiece::ShogiPLW, &ShogiPiece::ShogiSLW, IDB_SHOGI_PL_W, IDB_SHOGI_PL_S };
  inline const ShogiPiece ShogiPiece::ShogiPPW{ &PPawn::ThePPawn,     &Color::White, &ShogiPiece::ShogiPPW, &ShogiPiece::ShogiSPW, IDB_SHOGI_PP_W, IDB_SHOGI_PP_S };
  // S = Standard pieces, Black
  inline const ShogiPiece ShogiPiece::ShogiSKB{ &King::TheKing,       &Color::Black, &ShogiPiece::ShogiSKB, &ShogiPiece::ShogiSKB, IDB_SHOGI_SK_B, IDB_SHOGI_SK_S };
  inline const ShogiPiece ShogiPiece::ShogiSGB{ &Gold::TheGold,       &Color::Black, &ShogiPiece::ShogiSGB, &ShogiPiece::ShogiSGB, IDB_SHOGI_SG_B, IDB_SHOGI_SG_S };
  inline const ShogiPiece ShogiPiece::ShogiSSB{ &Silver::TheSilver,   &Color::Black, &ShogiPiece::ShogiPSB, &ShogiPiece::ShogiSSB, IDB_SHOGI_SS_B, IDB_SHOGI_SS_S };
  inline const ShogiPiece ShogiPiece::ShogiSBB{ &Bishop::TheBishop,   &Color::Black, &ShogiPiece::ShogiPBB, &ShogiPiece::ShogiSBB, IDB_SHOGI_SB_B, IDB_SHOGI_SB_S };
  inline const ShogiPiece ShogiPiece::ShogiSRB{ &Rook::TheRook,       &Color::Black, &ShogiPiece::ShogiPRB, &ShogiPiece::ShogiSRB, IDB_SHOGI_SR_B, IDB_SHOGI_SR_S };
  inline const ShogiPiece ShogiPiece::ShogiSNB{ &Knight::TheKnight,   &Color::Black, &ShogiPiece::ShogiPNB, &ShogiPiece::ShogiSNB, IDB_SHOGI_SN_B, IDB_SHOGI_SN_S };
  inline const ShogiPiece ShogiPiece::ShogiSLB{ &Lance::TheLance,     &Color::Black, &ShogiPiece::ShogiPLB, &ShogiPiece::ShogiSLB, IDB_SHOGI_SL_B, IDB_SHOGI_SL_S };
  inline const ShogiPiece ShogiPiece::ShogiSPB{ &Pawn::ThePawn,       &Color::Black, &ShogiPiece::ShogiPPB, &ShogiPiece::ShogiSPB, IDB_SHOGI_SP_B, IDB_SHOGI_SP_S };
  // P = Promoted pieces, Black   
  inline const ShogiPiece ShogiPiece::ShogiPSB{ &PSilver::ThePSilver, &Color::Black, &ShogiPiece::ShogiPSB, &ShogiPiece::ShogiSSB, IDB_SHOGI_PS_B, IDB_SHOGI_PS_S };
  inline const ShogiPiece ShogiPiece::ShogiPBB{ &PBishop::ThePBishop, &Color::Black, &ShogiPiece::ShogiPBB, &ShogiPiece::ShogiSBB, IDB_SHOGI_PB_B, IDB_SHOGI_PB_S };
  inline const ShogiPiece ShogiPiece::ShogiPRB{ &PRook::ThePRook,     &Color::Black, &ShogiPiece::ShogiPRB, &ShogiPiece::ShogiSRB, IDB_SHOGI_PR_B, IDB_SHOGI_PR_S };
  inline const ShogiPiece ShogiPiece::ShogiPNB{ &PKnight::ThePKnight, &Color::Black, &ShogiPiece::ShogiPNB, &ShogiPiece::ShogiSNB, IDB_SHOGI_PN_B, IDB_SHOGI_PN_S };
  inline const ShogiPiece ShogiPiece::ShogiPLB{ &PLance::ThePLance,   &Color::Black, &ShogiPiece::ShogiPLB, &ShogiPiece::ShogiSLB, IDB_SHOGI_PL_B, IDB_SHOGI_PL_S };
  const ShogiPiece ShogiPiece::ShogiPPB{ &PPawn::ThePPawn,     &Color::Black, &ShogiPiece::ShogiPPB, &ShogiPiece::ShogiSPB, IDB_SHOGI_PP_B, IDB_SHOGI_PP_S };

  void Pawn::CollectMoves(const MainPosition& p, const Location& l, Moves& moves) const
  {
    const int dy = p.OnTurn() == &Color::White ? -1 : 1;
    p.AddIfLegal(moves, l, l + Offset(0, dy));
  }
  bool Pawn::CanDrop(const MainPosition* pos, const Location& l) const noexcept
  {
    if (!((pos->OnTurn() == &Color::White) ? (l._y < pos->GetSizeY() - 1) : (l._y > 1))) return false;   // drop anywhere except last row
    for (Coordinate j = 0; j < pos->GetSizeY(); j++)
    {
      const Piece* p = pos->GetPiece(Location{ l._x,j });
      if (p == ((pos->OnTurn() == &Color::White) ? &ShogiPiece::ShogiSPW : &ShogiPiece::ShogiSPB)) return false;
    }
    return true;
  }

  void Lance::CollectMoves(const MainPosition& p, const Location& l, Moves& moves) const
  {
    const int dy = p.OnTurn() == &Color::White ? -1 : 1;
    for (int z = 1; p.AddIfLegal(moves, l, l + Offset(0, z*dy)); z++);
  }
  bool Lance::CanDrop(const MainPosition* pos, const Location& l) const noexcept
  {
    return (pos->OnTurn() == &Color::White) ? (l._y < pos->GetSizeY() - 1) : (l._y > 1);   // drop anywhere except last row
  }

  void Knight::CollectMoves(const MainPosition& p, const Location& l, Moves& moves) const
  {
    const int dy = p.OnTurn() == &Color::White ? -1 : 1;
    p.AddIfLegal(moves, l, l + Offset(+1, 2 * dy));
    p.AddIfLegal(moves, l, l + Offset(-1, 2 * dy));
  }
  bool Knight::CanDrop(const MainPosition* pos, const Location& l) const noexcept
  {
    return (pos->OnTurn() == &Color::White) ? (l._y < pos->GetSizeY() - 2) : (l._y > 2);   // drop anywhere except last two rows
  }

  void Bishop::CollectMoves(const MainPosition& p, const Location& l, Moves& moves) const
  {
    for (auto& d : Offset::Bdirection)
      for (int z = 1; p.AddIfLegal(moves, l, l + d * z); z++);
  }

  void Rook::CollectMoves(const MainPosition& p, const Location& l, Moves& moves) const
  {
    for (auto& d : Offset::Rdirection)
      for (int z = 1; p.AddIfLegal(moves, l, l + d * z); z++);
  }

  void Silver::CollectMoves(const MainPosition& p, const Location& l, Moves& moves) const
  {
    const int dy = p.OnTurn() == &Color::White ? -1 : 1;
    p.AddIfLegal(moves, l, l + Offset(+1, dy));
    p.AddIfLegal(moves, l, l + Offset(+0, dy));
    p.AddIfLegal(moves, l, l + Offset(-1, dy));
    p.AddIfLegal(moves, l, l + Offset(-1, -dy));
    p.AddIfLegal(moves, l, l + Offset(+1, -dy));
  }

  void Gold::CollectMoves(const MainPosition& p, const Location& l, Moves& moves) const
  {
    Gold::CollectGoldMoves(p, l, moves);
  }
  void Gold::CollectGoldMoves(const MainPosition& p, const Location& l, Moves& moves)
  {
    const int dy = p.OnTurn() == &Color::White ? -1 : 1;
    p.AddIfLegal(moves, l, l + Offset(+1, dy));
    p.AddIfLegal(moves, l, l + Offset(+0, dy));
    p.AddIfLegal(moves, l, l + Offset(-1, dy));
    p.AddIfLegal(moves, l, l + Offset(+1, +0));
    p.AddIfLegal(moves, l, l + Offset(-1, +0));
    p.AddIfLegal(moves, l, l + Offset(+0, -dy));
  }

  void King::CollectMoves(const MainPosition& p, const Location& l, Moves& moves) const
  {
    for (auto& d : Offset::Qdirection)
      p.AddIfLegal(moves, l, l + d);
  }

  void PPawn::CollectMoves(const MainPosition& p, const Location& l, Moves& moves) const
  {
    Gold::CollectGoldMoves(p, l, moves);
  }

  void PLance::CollectMoves(const MainPosition& p, const Location& l, Moves& moves) const
  {
    Gold::CollectGoldMoves(p, l, moves);
  }

  void PKnight::CollectMoves(const MainPosition& p, const Location& l, Moves& moves) const
  {
    Gold::CollectGoldMoves(p, l, moves);
  }

  void PBishop::CollectMoves(const MainPosition& p, const Location& l, Moves& moves) const
  {
    for (auto& d : Offset::Bdirection)
      for (int z = 1; p.AddIfLegal(moves, l, l + d * z); z++);
    for (auto& d : Offset::Rdirection)
      p.AddIfLegal(moves, l, l + d);
  }

  void PRook::CollectMoves(const MainPosition& p, const Location& l, Moves& moves) const
  {
    for (auto& d : Offset::Rdirection)
      for (int z = 1; p.AddIfLegal(moves, l, l + d * z); z++);
    for (auto& d : Offset::Bdirection)
      p.AddIfLegal(moves, l, l + d);
  }

  void PSilver::CollectMoves(const MainPosition& p, const Location& l, Moves& moves) const
  {
    Gold::CollectGoldMoves(p, l, moves);
  }


  ShogiPosition::ShogiPosition(const PieceMapP& p, Coordinate x, Coordinate y) noexcept : MainPosition(p, x, y)
  {
    assert((x == 9 && y == 9) || (x == 5 && y == 5));
    if (ShogiGame::IsFull(x, y))
    {
      SetPiece(Location(0U, 0U), &ShogiPiece::ShogiSLB);
      SetPiece(Location(1U, 0U), &ShogiPiece::ShogiSNB);
      SetPiece(Location(2U, 0U), &ShogiPiece::ShogiSSB);
      SetPiece(Location(3U, 0U), &ShogiPiece::ShogiSGB);
      SetPiece(Location(4U, 0U), &ShogiPiece::ShogiSKB);
      SetPiece(Location(5U, 0U), &ShogiPiece::ShogiSGB);
      SetPiece(Location(6U, 0U), &ShogiPiece::ShogiSSB);
      SetPiece(Location(7U, 0U), &ShogiPiece::ShogiSNB);
      SetPiece(Location(8U, 0U), &ShogiPiece::ShogiSLB);

      SetPiece(Location(1U, 1U), &ShogiPiece::ShogiSRB);
      SetPiece(Location(7U, 1U), &ShogiPiece::ShogiSBB);

      SetPiece(Location(0U, 2U), &ShogiPiece::ShogiSPB);
      SetPiece(Location(1U, 2U), &ShogiPiece::ShogiSPB);
      SetPiece(Location(2U, 2U), &ShogiPiece::ShogiSPB);
      SetPiece(Location(3U, 2U), &ShogiPiece::ShogiSPB);
      SetPiece(Location(4U, 2U), &ShogiPiece::ShogiSPB);
      SetPiece(Location(5U, 2U), &ShogiPiece::ShogiSPB);
      SetPiece(Location(6U, 2U), &ShogiPiece::ShogiSPB);
      SetPiece(Location(7U, 2U), &ShogiPiece::ShogiSPB);
      SetPiece(Location(8U, 2U), &ShogiPiece::ShogiSPB);


      SetPiece(Location(0U, 6U), &ShogiPiece::ShogiSPW);
      SetPiece(Location(1U, 6U), &ShogiPiece::ShogiSPW);
      SetPiece(Location(2U, 6U), &ShogiPiece::ShogiSPW);
      SetPiece(Location(3U, 6U), &ShogiPiece::ShogiSPW);
      SetPiece(Location(4U, 6U), &ShogiPiece::ShogiSPW);
      SetPiece(Location(5U, 6U), &ShogiPiece::ShogiSPW);
      SetPiece(Location(6U, 6U), &ShogiPiece::ShogiSPW);
      SetPiece(Location(7U, 6U), &ShogiPiece::ShogiSPW);
      SetPiece(Location(8U, 6U), &ShogiPiece::ShogiSPW);

      SetPiece(Location(1U, 7U), &ShogiPiece::ShogiSBW);
      SetPiece(Location(7U, 7U), &ShogiPiece::ShogiSRW);

      SetPiece(Location(0U, 8U), &ShogiPiece::ShogiSLW);
      SetPiece(Location(1U, 8U), &ShogiPiece::ShogiSNW);
      SetPiece(Location(2U, 8U), &ShogiPiece::ShogiSSW);
      SetPiece(Location(3U, 8U), &ShogiPiece::ShogiSGW);
      SetPiece(Location(4U, 8U), &ShogiPiece::ShogiSKW);
      SetPiece(Location(5U, 8U), &ShogiPiece::ShogiSGW);
      SetPiece(Location(6U, 8U), &ShogiPiece::ShogiSSW);
      SetPiece(Location(7U, 8U), &ShogiPiece::ShogiSNW);
      SetPiece(Location(8U, 8U), &ShogiPiece::ShogiSLW);
    }
    else
    {
      SetPiece(Location(0U, 0U), &ShogiPiece::ShogiSRB);
      SetPiece(Location(1U, 0U), &ShogiPiece::ShogiSBB);
      SetPiece(Location(2U, 0U), &ShogiPiece::ShogiSSB);
      SetPiece(Location(3U, 0U), &ShogiPiece::ShogiSGB);
      SetPiece(Location(4U, 0U), &ShogiPiece::ShogiSKB);
      SetPiece(Location(4U, 1U), &ShogiPiece::ShogiSPB);

      SetPiece(Location(4U, 4U), &ShogiPiece::ShogiSRW);
      SetPiece(Location(3U, 4U), &ShogiPiece::ShogiSBW);
      SetPiece(Location(2U, 4U), &ShogiPiece::ShogiSSW);
      SetPiece(Location(1U, 4U), &ShogiPiece::ShogiSGW);
      SetPiece(Location(0U, 4U), &ShogiPiece::ShogiSKW);
      SetPiece(Location(0U, 3U), &ShogiPiece::ShogiSPW);
    }
  }

  void ShogiPosition::GetAllMoves(void)              // generate all moves and save list
  {
    MainPosition::GetAllMoves();    // standard: get moves for all pieces on the board

    // add all potential placement moves
    for (Coordinate i = 0; i < sizeX; i++)
      for (Coordinate j = 0; j < sizeY; j++)
      {
        const Location l{ i,j };
        if (GetPiece(l)->IsBlank())
        {
          const Piece* p{ nullptr };
          for (unsigned int z = 0; (p = tpos->GetPiece(Location(z, 0U))) != &Piece::NoTile; z++)
          {
            const ShogiPiece* pp = dynamic_cast<const ShogiPiece*>(p); // must be a shogipiece, verify it
            if (pp->CanDrop(this, l))
            {
              movelistB.push_back(std::make_shared<SimpleMove>(std::make_shared<SimpleStep>(Field{ Location(z, 0U),nullptr }, Field{ l,p }, SimpleStep::StepType::Drop)));
            }
          }
        }
      }
  }
  
  bool ShogiPosition::AddIfLegal(Moves& m, const Location fr, const Location to) const
  {
    const Piece* pt = GetPiece(to);
    if (pt == nullptr) return false;  // out of board
    if (pt->IsColor(OnTurn())) return false;  // own piece

    const ShogiPiece* pf = dynamic_cast<const ShogiPiece*>(GetPiece(fr));

    SimpleStep::StepType st = pt->IsBlank() ? SimpleStep::StepType::Normal : SimpleStep::StepType::Take;
//    if (pf->CanDrop(this, to))  // 'can drop' is also 'can move there'
//    {
    m.push_back(std::make_shared<SimpleMove>(std::make_shared<SimpleStep>(Field{ fr,pf }, Field{ to,pf }, st)));
//    }
    if ((CanPromote(fr) || CanPromote(to)) && GetPiece(fr)->IsPromotable())
    {
      st = static_cast<SimpleStep::StepType>(st | SimpleStep::StepType::Promote);
      m.push_back(std::make_shared<SimpleMove>(std::make_shared<SimpleStep>(Field{ fr,GetPiece(fr) }, Field{ to,pf->Promote(true) }, st)));
    }
    return pt->IsBlank();   // if free tile, keep trying this direction
  }

  //PositionValue ShogiPosition::EvaluateWin(void) const
  //{
  //  bool kw{false};
  //  bool kb{false};

  //  for (Coordinate i = 0; i < sizeX; i++)
  //  {
  //    for (Coordinate j = 0; j < sizeY; j++)
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
    return (OnTurn() == &Color::White && (l._y < (f?3U:1U))) || (OnTurn() != &Color::White && (l._y > (f?8U:3U)));
  }
    

  ShogiGame::ShogiGame(const PieceMapP& m, Coordinate x, Coordinate y) noexcept : ShogiGame(m,
    new ShogiPosition(m, x, y), new ShogiTakenPosition(m, x, y), new StockPosition(m, 15, 2),
    new ShogiLayout(x, y), new ShogiTakenLayout(x, y), new ShogiStockLayout(x, y)) {}

  ShogiGame::ShogiGame(const PieceMapP& m,
    ShogiPosition* p, TakenPosition* t, StockPosition* s,
    ShogiLayout* l, ShogiTakenLayout* tl, ShogiStockLayout* sl) noexcept : Game{ m,p,t,s,l,tl,sl }
  {
    p->SetTPos(t);  // stores the Taken position inide the board position, so the move generator can access it for drops
    AddToStock(Location(0U, 0U), &ShogiPiece::ShogiSKW);
    AddToStock(Location(1U, 0U), &ShogiPiece::ShogiSGW);
    AddToStock(Location(2U, 0U), &ShogiPiece::ShogiSSW);
    AddToStock(Location(3U, 0U), &ShogiPiece::ShogiSBW);
    AddToStock(Location(4U, 0U), &ShogiPiece::ShogiSRW);
    AddToStock(Location(5U, 0U), &ShogiPiece::ShogiSNW);
    AddToStock(Location(6U, 0U), &ShogiPiece::ShogiSLW);
    AddToStock(Location(7U, 0U), &ShogiPiece::ShogiSPW);
                         
    AddToStock(Location(8U, 0U), &ShogiPiece::ShogiPSW);
    AddToStock(Location(9U, 0U), &ShogiPiece::ShogiPBW);
    AddToStock(Location(10U, 0U), &ShogiPiece::ShogiPRW);
    AddToStock(Location(11U, 0U), &ShogiPiece::ShogiPNW);
    AddToStock(Location(12U, 0U), &ShogiPiece::ShogiPLW);
    AddToStock(Location(13U, 0U), &ShogiPiece::ShogiPPW);

    AddToStock(Location(0U, 1U), &ShogiPiece::ShogiSKW);
    AddToStock(Location(1U, 1U), &ShogiPiece::ShogiSGW);
    AddToStock(Location(2U, 1U), &ShogiPiece::ShogiSSW);
    AddToStock(Location(3U, 1U), &ShogiPiece::ShogiSBW);
    AddToStock(Location(4U, 1U), &ShogiPiece::ShogiSRW);
    AddToStock(Location(5U, 1U), &ShogiPiece::ShogiSNW);
    AddToStock(Location(6U, 1U), &ShogiPiece::ShogiSLW);
    AddToStock(Location(7U, 1U), &ShogiPiece::ShogiSPW);
                             
    AddToStock(Location(8U, 1U), &ShogiPiece::ShogiPSW);
    AddToStock(Location(9U, 1U), &ShogiPiece::ShogiPBW);
    AddToStock(Location(10U, 1U), &ShogiPiece::ShogiPRW);
    AddToStock(Location(11U, 1U), &ShogiPiece::ShogiPNW);
    AddToStock(Location(12U, 1U), &ShogiPiece::ShogiPLW);
    AddToStock(Location(13U, 1U), &ShogiPiece::ShogiPPW);
  }

  const VariantList& ShogiGame::GetVariants(void) noexcept
  {
    static VariantList v{ { Variant{ 9, 9, "Shogi" },{ Variant{ 5, 5, "Mini Shogi" } } } };
    return v;
  }

}
