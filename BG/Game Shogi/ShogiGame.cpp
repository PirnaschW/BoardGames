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
  inline const ShogiPiece ShogiPiece::ShogiSKW{ &King::TheKing,       &Color::White, &ShogiPiece::ShogiSKW, &ShogiPiece::ShogiSKB, IDB_SHOGI_SK_W, IDB_SHOGI_SK_S };
  inline const ShogiPiece ShogiPiece::ShogiSGW{ &Gold::TheGold,       &Color::White, &ShogiPiece::ShogiSGW, &ShogiPiece::ShogiSGB, IDB_SHOGI_SG_W, IDB_SHOGI_SG_S };
  inline const ShogiPiece ShogiPiece::ShogiSSW{ &Silver::TheSilver,   &Color::White, &ShogiPiece::ShogiPSW, &ShogiPiece::ShogiSSB, IDB_SHOGI_SS_W, IDB_SHOGI_SS_S };
  inline const ShogiPiece ShogiPiece::ShogiSBW{ &Bishop::TheBishop,   &Color::White, &ShogiPiece::ShogiPBW, &ShogiPiece::ShogiSBB, IDB_SHOGI_SB_W, IDB_SHOGI_SB_S };
  inline const ShogiPiece ShogiPiece::ShogiSRW{ &Rook::TheRook,       &Color::White, &ShogiPiece::ShogiPRW, &ShogiPiece::ShogiSRB, IDB_SHOGI_SR_W, IDB_SHOGI_SR_S };
  inline const ShogiPiece ShogiPiece::ShogiSNW{ &Knight::TheKnight,   &Color::White, &ShogiPiece::ShogiPNW, &ShogiPiece::ShogiSNB, IDB_SHOGI_SN_W, IDB_SHOGI_SN_S };
  inline const ShogiPiece ShogiPiece::ShogiSLW{ &Lance::TheLance,     &Color::White, &ShogiPiece::ShogiPLW, &ShogiPiece::ShogiSLB, IDB_SHOGI_SL_W, IDB_SHOGI_SL_S };
  inline const ShogiPiece ShogiPiece::ShogiPWS{ &Pawn::ThePawn,       &Color::White, &ShogiPiece::ShogiPPW, &ShogiPiece::ShogiPBS, IDB_SHOGI_PWS_L, IDB_SHOGI_PWS_S };
  // P = Promoted Pieces, White   
  inline const ShogiPiece ShogiPiece::ShogiPSW{ &PSilver::ThePSilver, &Color::White, &ShogiPiece::ShogiPSW, &ShogiPiece::ShogiSSB, IDB_SHOGI_PS_W, IDB_SHOGI_PS_S };
  inline const ShogiPiece ShogiPiece::ShogiPBW{ &PBishop::ThePBishop, &Color::White, &ShogiPiece::ShogiPBW, &ShogiPiece::ShogiSBB, IDB_SHOGI_PB_W, IDB_SHOGI_PB_S };
  inline const ShogiPiece ShogiPiece::ShogiPRW{ &PRook::ThePRook,     &Color::White, &ShogiPiece::ShogiPRW, &ShogiPiece::ShogiSRB, IDB_SHOGI_PR_W, IDB_SHOGI_PR_S };
  inline const ShogiPiece ShogiPiece::ShogiPNW{ &PKnight::ThePKnight, &Color::White, &ShogiPiece::ShogiPNW, &ShogiPiece::ShogiSNB, IDB_SHOGI_PN_W, IDB_SHOGI_PN_S };
  inline const ShogiPiece ShogiPiece::ShogiPLW{ &PLance::ThePLance,   &Color::White, &ShogiPiece::ShogiPLW, &ShogiPiece::ShogiSLB, IDB_SHOGI_PL_W, IDB_SHOGI_PL_S };
  inline const ShogiPiece ShogiPiece::ShogiPPW{ &PPawn::ThePPawn,     &Color::White, &ShogiPiece::ShogiPPW, &ShogiPiece::ShogiPBS, IDB_SHOGI_PP_W, IDB_SHOGI_PP_S };
  // S = Standard pieces, Black
  inline const ShogiPiece ShogiPiece::ShogiSKB{ &King::TheKing,       &Color::Black, &ShogiPiece::ShogiSKB, &ShogiPiece::ShogiSKW, IDB_SHOGI_SK_B, IDB_SHOGI_SK_S };
  inline const ShogiPiece ShogiPiece::ShogiSGB{ &Gold::TheGold,       &Color::Black, &ShogiPiece::ShogiSGB, &ShogiPiece::ShogiSGW, IDB_SHOGI_SG_B, IDB_SHOGI_SG_S };
  inline const ShogiPiece ShogiPiece::ShogiSSB{ &Silver::TheSilver,   &Color::Black, &ShogiPiece::ShogiPSB, &ShogiPiece::ShogiSSW, IDB_SHOGI_SS_B, IDB_SHOGI_SS_S };
  inline const ShogiPiece ShogiPiece::ShogiSBB{ &Bishop::TheBishop,   &Color::Black, &ShogiPiece::ShogiPBB, &ShogiPiece::ShogiSBW, IDB_SHOGI_SB_B, IDB_SHOGI_SB_S };
  inline const ShogiPiece ShogiPiece::ShogiSRB{ &Rook::TheRook,       &Color::Black, &ShogiPiece::ShogiPRB, &ShogiPiece::ShogiSRW, IDB_SHOGI_SR_B, IDB_SHOGI_SR_S };
  inline const ShogiPiece ShogiPiece::ShogiSNB{ &Knight::TheKnight,   &Color::Black, &ShogiPiece::ShogiPNB, &ShogiPiece::ShogiSNW, IDB_SHOGI_SN_B, IDB_SHOGI_SN_S };
  inline const ShogiPiece ShogiPiece::ShogiSLB{ &Lance::TheLance,     &Color::Black, &ShogiPiece::ShogiPLB, &ShogiPiece::ShogiSLW, IDB_SHOGI_SL_B, IDB_SHOGI_SL_S };
  inline const ShogiPiece ShogiPiece::ShogiPBS{ &Pawn::ThePawn,       &Color::Black, &ShogiPiece::ShogiPPB, &ShogiPiece::ShogiPWS, IDB_SHOGI_PBS_L, IDB_SHOGI_PBS_S };
  // P = Promoted pieces, Black   
  inline const ShogiPiece ShogiPiece::ShogiPSB{ &PSilver::ThePSilver, &Color::Black, &ShogiPiece::ShogiPSB, &ShogiPiece::ShogiSSW, IDB_SHOGI_PS_B, IDB_SHOGI_PS_S };
  inline const ShogiPiece ShogiPiece::ShogiPBB{ &PBishop::ThePBishop, &Color::Black, &ShogiPiece::ShogiPBB, &ShogiPiece::ShogiSBW, IDB_SHOGI_PB_B, IDB_SHOGI_PB_S };
  inline const ShogiPiece ShogiPiece::ShogiPRB{ &PRook::ThePRook,     &Color::Black, &ShogiPiece::ShogiPRB, &ShogiPiece::ShogiSRW, IDB_SHOGI_PR_B, IDB_SHOGI_PR_S };
  inline const ShogiPiece ShogiPiece::ShogiPNB{ &PKnight::ThePKnight, &Color::Black, &ShogiPiece::ShogiPNB, &ShogiPiece::ShogiSNW, IDB_SHOGI_PN_B, IDB_SHOGI_PN_S };
  inline const ShogiPiece ShogiPiece::ShogiPLB{ &PLance::ThePLance,   &Color::Black, &ShogiPiece::ShogiPLB, &ShogiPiece::ShogiSLW, IDB_SHOGI_PL_B, IDB_SHOGI_PL_S };
         const ShogiPiece ShogiPiece::ShogiPPB{ &PPawn::ThePPawn,     &Color::Black, &ShogiPiece::ShogiPPB, &ShogiPiece::ShogiPWS, IDB_SHOGI_PP_B, IDB_SHOGI_PP_S };

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
      const Piece* p = pos->GetPiece(Location{ BoardPart::Main, l._x,j });
      if (p == ((pos->OnTurn() == &Color::White) ? &ShogiPiece::ShogiPWS : &ShogiPiece::ShogiPBS)) return false;
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
  bool Lance::CanMove(const MainPosition* pos, const Location& l) const noexcept
  {
    return (pos->OnTurn() == &Color::White) ? (l._y < pos->GetSizeY() - 1) : (l._y > 1);   // move anywhere except last row
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
  bool Knight::CanMove(const MainPosition* pos, const Location& l) const noexcept
  {
    return (pos->OnTurn() == &Color::White) ? (l._y < pos->GetSizeY() - 2) : (l._y > 2);   // move anywhere except last two rows
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


  ShogiPosition::ShogiPosition(const PieceMapP& p, const Dimensions& d) noexcept : MainPosition(p, d)
  {
    assert((d[0].xCount == 9 && d[0].yCount == 9) || (d[0].xCount == 5 && d[0].yCount == 5));
    if (ShogiGame::IsFull(d[0].xCount, d[0].yCount))
    {
      SetPiece(Location( BoardPart::Main, 0U, 0U), &ShogiPiece::ShogiSLB);
      SetPiece(Location( BoardPart::Main, 1U, 0U), &ShogiPiece::ShogiSNB);
      SetPiece(Location( BoardPart::Main, 2U, 0U), &ShogiPiece::ShogiSSB);
      SetPiece(Location( BoardPart::Main, 3U, 0U), &ShogiPiece::ShogiSGB);
      SetPiece(Location( BoardPart::Main, 4U, 0U), &ShogiPiece::ShogiSKB);
      SetPiece(Location( BoardPart::Main, 5U, 0U), &ShogiPiece::ShogiSGB);
      SetPiece(Location( BoardPart::Main, 6U, 0U), &ShogiPiece::ShogiSSB);
      SetPiece(Location( BoardPart::Main, 7U, 0U), &ShogiPiece::ShogiSNB);
      SetPiece(Location( BoardPart::Main, 8U, 0U), &ShogiPiece::ShogiSLB);
                         
      SetPiece(Location( BoardPart::Main, 1U, 1U), &ShogiPiece::ShogiSRB);
      SetPiece(Location( BoardPart::Main, 7U, 1U), &ShogiPiece::ShogiSBB);
                        
      SetPiece(Location( BoardPart::Main, 0U, 2U), &ShogiPiece::ShogiPBS);
      SetPiece(Location( BoardPart::Main, 1U, 2U), &ShogiPiece::ShogiPBS);
      SetPiece(Location( BoardPart::Main, 2U, 2U), &ShogiPiece::ShogiPBS);
      SetPiece(Location( BoardPart::Main, 3U, 2U), &ShogiPiece::ShogiPBS);
      SetPiece(Location( BoardPart::Main, 4U, 2U), &ShogiPiece::ShogiPBS);
      SetPiece(Location( BoardPart::Main, 5U, 2U), &ShogiPiece::ShogiPBS);
      SetPiece(Location( BoardPart::Main, 6U, 2U), &ShogiPiece::ShogiPBS);
      SetPiece(Location( BoardPart::Main, 7U, 2U), &ShogiPiece::ShogiPBS);
      SetPiece(Location( BoardPart::Main, 8U, 2U), &ShogiPiece::ShogiPBS);
                         
                         
      SetPiece(Location( BoardPart::Main, 0U, 6U), &ShogiPiece::ShogiPWS);
      SetPiece(Location( BoardPart::Main, 1U, 6U), &ShogiPiece::ShogiPWS);
      SetPiece(Location( BoardPart::Main, 2U, 6U), &ShogiPiece::ShogiPWS);
      SetPiece(Location( BoardPart::Main, 3U, 6U), &ShogiPiece::ShogiPWS);
      SetPiece(Location( BoardPart::Main, 4U, 6U), &ShogiPiece::ShogiPWS);
      SetPiece(Location( BoardPart::Main, 5U, 6U), &ShogiPiece::ShogiPWS);
      SetPiece(Location( BoardPart::Main, 6U, 6U), &ShogiPiece::ShogiPWS);
      SetPiece(Location( BoardPart::Main, 7U, 6U), &ShogiPiece::ShogiPWS);
      SetPiece(Location( BoardPart::Main, 8U, 6U), &ShogiPiece::ShogiPWS);
                         
      SetPiece(Location( BoardPart::Main, 1U, 7U), &ShogiPiece::ShogiSBW);
      SetPiece(Location( BoardPart::Main, 7U, 7U), &ShogiPiece::ShogiSRW);
                         
      SetPiece(Location( BoardPart::Main, 0U, 8U), &ShogiPiece::ShogiSLW);
      SetPiece(Location( BoardPart::Main, 1U, 8U), &ShogiPiece::ShogiSNW);
      SetPiece(Location( BoardPart::Main, 2U, 8U), &ShogiPiece::ShogiSSW);
      SetPiece(Location( BoardPart::Main, 3U, 8U), &ShogiPiece::ShogiSGW);
      SetPiece(Location( BoardPart::Main, 4U, 8U), &ShogiPiece::ShogiSKW);
      SetPiece(Location( BoardPart::Main, 5U, 8U), &ShogiPiece::ShogiSGW);
      SetPiece(Location( BoardPart::Main, 6U, 8U), &ShogiPiece::ShogiSSW);
      SetPiece(Location( BoardPart::Main, 7U, 8U), &ShogiPiece::ShogiSNW);
      SetPiece(Location( BoardPart::Main, 8U, 8U), &ShogiPiece::ShogiSLW);
    }
    else
    {
      SetPiece(Location( BoardPart::Main, 0U, 0U), &ShogiPiece::ShogiSRB);
      SetPiece(Location( BoardPart::Main, 1U, 0U), &ShogiPiece::ShogiSBB);
      SetPiece(Location( BoardPart::Main, 2U, 0U), &ShogiPiece::ShogiSSB);
      SetPiece(Location( BoardPart::Main, 3U, 0U), &ShogiPiece::ShogiSGB);
      SetPiece(Location( BoardPart::Main, 4U, 0U), &ShogiPiece::ShogiSKB);
      SetPiece(Location( BoardPart::Main, 4U, 1U), &ShogiPiece::ShogiPBS);
                         
      SetPiece(Location( BoardPart::Main, 4U, 4U), &ShogiPiece::ShogiSRW);
      SetPiece(Location( BoardPart::Main, 3U, 4U), &ShogiPiece::ShogiSBW);
      SetPiece(Location( BoardPart::Main, 2U, 4U), &ShogiPiece::ShogiSSW);
      SetPiece(Location( BoardPart::Main, 1U, 4U), &ShogiPiece::ShogiSGW);
      SetPiece(Location( BoardPart::Main, 0U, 4U), &ShogiPiece::ShogiSKW);
      SetPiece(Location( BoardPart::Main, 0U, 3U), &ShogiPiece::ShogiPWS);
    }
  }

  void ShogiPosition::GetAllMoves(void)                                   // generate all moves and save list
  {
    MainPosition::GetAllMoves();                                          // standard: get moves for all pieces on the board

    // add all potential drop moves
    for (Coordinate y = 0; true; y++)
    {
      const Location l(BoardPart::Taken, 0U, y);
      const Piece* p = _taken.GetPiece(Location(BoardPart::Taken, 0U, y));
      if (p == nullptr) break;                                            // end of Taken reached
      if (p->IsBlank()) continue;                                         // no piece here to drop, try next Location
      const ShogiPiece* pp = dynamic_cast<const ShogiPiece*>(p);          // must be a shogipiece,
      assert(pp != nullptr);                                              // verify it

      Actions a{ std::make_shared<ActionTake>(l,pp) };
      for (Coordinate i = 0; i < sizeX; i++)
        for (Coordinate j = 0; j < sizeY; j++)
        {
          const Location ll{ BoardPart::Main, i, j };
          if (GetPiece(ll)->IsBlank() && pp->CanDrop(this, ll))
          {
            Actions aa{ a };
            aa.push_back(std::make_shared<ActionPlace>(ll, pp));
            movelistB.push_back(std::make_shared<Move>(aa));
          }
        }
    }
  }
  
  bool ShogiPosition::AddIfLegal(Moves& m, const Location fr, const Location to) const // returns true if further tries in this direction are allowed
  {
    const Piece* pt = GetPiece(to);                                       // what is on the target field
    if (pt == nullptr) return false;                                      // out of board
    if (IsTaken(fr) && !pt->IsBlank()) return false;                      // can't drop onto existing piece

    const ShogiPiece* pf = dynamic_cast<const ShogiPiece*>(GetPiece(fr));
    if (pt->IsColor(pf->GetColor())) return false;                        // own piece

    if (IsTaken(fr) && !pf->CanDrop(this, to)) return false;

    Actions a{ std::make_shared<ActionTake>(fr, pf) };
    if (!pt->IsBlank()) // if something is there, take it and place in Taken
    {
      a.push_back(std::make_shared<ActionTake>(to, pt));
      const Piece* pt0 = pt->Promote(false);  // demote and change color/owner
      a.push_back(std::make_shared<ActionPlace>(GetNextTakenL(pt->GetColor()), pt0));
    }
    // if this is not a drop, check if promotion is possible
    if (!IsTaken(fr) && (CanPromote(pf->GetColor(), fr) || CanPromote(pf->GetColor(), to)) && pf->IsPromotable())
    {
      Actions aa{ a }; // promote is optional, so we make a copy and add as second move
      aa.push_back(std::make_shared<ActionPlace>(to, pf->Promote(true)));
      m.push_back(std::make_shared<Move>(aa));
    }
    if (pf->CanMove(this, to)) // if no promotion, verify if this piece can ever move/drop there
    {
      a.push_back(std::make_shared<ActionPlace>(to, pf));  // unpromoted placement
      m.push_back(std::make_shared<Move>(a));
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

  inline bool ShogiPosition::CanPromote(const Color* c, const Location& l) const noexcept
  {
    const bool f = ShogiGame::IsFull(sizeX, sizeY);
    return c == &Color::White ? (l._y < (f ? 3U : 1U)) : (l._y > (f ? 8U : 3U));
  }
    
  const VariantList& ShogiGame::GetVariants(void) noexcept
  {
    static VariantList v{ { Variant{ 9, 9, "Shogi" },{ Variant{ 5, 5, "Mini Shogi" } } } };
    return v;
  }

  const PieceMapP& ShogiGame::GetPieces(void) noexcept
  {
    static const PieceMapP& p = std::make_shared<PieceMap>();
    p->Add(&ShogiPiece::ShogiSKW);   // Standard King White
    p->Add(&ShogiPiece::ShogiSKB);   // Standard King Black
    p->Add(&ShogiPiece::ShogiSGW);
    p->Add(&ShogiPiece::ShogiSGB);
    p->Add(&ShogiPiece::ShogiSSW);
    p->Add(&ShogiPiece::ShogiSSB);
    p->Add(&ShogiPiece::ShogiSBW);
    p->Add(&ShogiPiece::ShogiSBB);
    p->Add(&ShogiPiece::ShogiSRW);
    p->Add(&ShogiPiece::ShogiSRB);
    p->Add(&ShogiPiece::ShogiSNW);
    p->Add(&ShogiPiece::ShogiSNB);
    p->Add(&ShogiPiece::ShogiSLW);
    p->Add(&ShogiPiece::ShogiSLB);
    p->Add(&ShogiPiece::ShogiPWS);   // Pawn White Standard
    p->Add(&ShogiPiece::ShogiPBS);   // Pawn Black Standard

    p->Add(&ShogiPiece::ShogiPSW);   // Promoted Silver White
    p->Add(&ShogiPiece::ShogiPSB);   // Promoted Silver Black
    p->Add(&ShogiPiece::ShogiPBW);
    p->Add(&ShogiPiece::ShogiPBB);
    p->Add(&ShogiPiece::ShogiPRW);
    p->Add(&ShogiPiece::ShogiPRB);
    p->Add(&ShogiPiece::ShogiPNW);
    p->Add(&ShogiPiece::ShogiPNB);
    p->Add(&ShogiPiece::ShogiPLW);
    p->Add(&ShogiPiece::ShogiPLB);
    p->Add(&ShogiPiece::ShogiPPW);
    p->Add(&ShogiPiece::ShogiPPB);

    return p;
  }


  const Dimensions& ShogiGame::GetDimensions(Coordinate x, Coordinate y) noexcept
  {
    static Dimensions d{
       Dimension(x, y, BoardStartX, BoardStartY, FieldSizeX, FieldSizeY, 1, 1),
       Dimension(15, 2, BoardStartX + FieldSizeX * (x + 1), BoardStartY + y * FieldSizeY + FieldSizeY / 2, FieldSizeX, FieldSizeY),
       Dimension(3 * x, 2, BoardStartX + FieldSizeX * (x + 1), BoardStartY + FieldSizeSY, FieldSizeSX, FieldSizeSY, 0, FieldSizeY * x - FieldSizeSY * 4),
    };
    return d;
  }

}
