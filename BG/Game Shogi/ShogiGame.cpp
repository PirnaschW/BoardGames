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
      const Piece* p = pos->GetPiece(Location{ BoardPart::Main, l._x,j });
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


  ShogiPosition::ShogiPosition(const PieceMapP& p, Coordinate x, Coordinate y) noexcept : MainPosition(p, x, y)
  {
    assert((x == 9 && y == 9) || (x == 5 && y == 5));
    if (ShogiGame::IsFull(x, y))
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
                        
      SetPiece(Location( BoardPart::Main, 0U, 2U), &ShogiPiece::ShogiSPB);
      SetPiece(Location( BoardPart::Main, 1U, 2U), &ShogiPiece::ShogiSPB);
      SetPiece(Location( BoardPart::Main, 2U, 2U), &ShogiPiece::ShogiSPB);
      SetPiece(Location( BoardPart::Main, 3U, 2U), &ShogiPiece::ShogiSPB);
      SetPiece(Location( BoardPart::Main, 4U, 2U), &ShogiPiece::ShogiSPB);
      SetPiece(Location( BoardPart::Main, 5U, 2U), &ShogiPiece::ShogiSPB);
      SetPiece(Location( BoardPart::Main, 6U, 2U), &ShogiPiece::ShogiSPB);
      SetPiece(Location( BoardPart::Main, 7U, 2U), &ShogiPiece::ShogiSPB);
      SetPiece(Location( BoardPart::Main, 8U, 2U), &ShogiPiece::ShogiSPB);
                         
                         
      SetPiece(Location( BoardPart::Main, 0U, 6U), &ShogiPiece::ShogiSPW);
      SetPiece(Location( BoardPart::Main, 1U, 6U), &ShogiPiece::ShogiSPW);
      SetPiece(Location( BoardPart::Main, 2U, 6U), &ShogiPiece::ShogiSPW);
      SetPiece(Location( BoardPart::Main, 3U, 6U), &ShogiPiece::ShogiSPW);
      SetPiece(Location( BoardPart::Main, 4U, 6U), &ShogiPiece::ShogiSPW);
      SetPiece(Location( BoardPart::Main, 5U, 6U), &ShogiPiece::ShogiSPW);
      SetPiece(Location( BoardPart::Main, 6U, 6U), &ShogiPiece::ShogiSPW);
      SetPiece(Location( BoardPart::Main, 7U, 6U), &ShogiPiece::ShogiSPW);
      SetPiece(Location( BoardPart::Main, 8U, 6U), &ShogiPiece::ShogiSPW);
                         
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
      SetPiece(Location( BoardPart::Main, 4U, 1U), &ShogiPiece::ShogiSPB);
                         
      SetPiece(Location( BoardPart::Main, 4U, 4U), &ShogiPiece::ShogiSRW);
      SetPiece(Location( BoardPart::Main, 3U, 4U), &ShogiPiece::ShogiSBW);
      SetPiece(Location( BoardPart::Main, 2U, 4U), &ShogiPiece::ShogiSSW);
      SetPiece(Location( BoardPart::Main, 1U, 4U), &ShogiPiece::ShogiSGW);
      SetPiece(Location( BoardPart::Main, 0U, 4U), &ShogiPiece::ShogiSKW);
      SetPiece(Location( BoardPart::Main, 0U, 3U), &ShogiPiece::ShogiSPW);
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
    if (pt->IsColor(OnTurn())) return false;                              // own piece
    if (IsTaken(fr) && !pt->IsBlank()) return false;                      // can't drop onto existing piece

    const ShogiPiece* pf = dynamic_cast<const ShogiPiece*>(GetPiece(fr));


    if (fr._b == BoardPart::Taken ? pf->CanDrop(this, to) : pf->CanMove(this, to))  // checks for legal drop / move
    {
      Actions a{ std::make_shared<ActionTake>(fr, pf) };
      if (!pt->IsBlank()) // if something is there, take it and place in Taken
      {
        a.push_back(std::make_shared<ActionTake>(to, pt));
        a.push_back(std::make_shared<ActionPlace>(GetNextTakenL(pt), pt));
      }
      if (!IsTaken(fr) && (CanPromote(fr) || CanPromote(to)) && pf->IsPromotable())
      {
        Actions aa{ a }; // promote is optional, so we make a copy and add as second move
        aa.push_back(std::make_shared<ActionPlace>(to, pf->Promote(true)));
        m.push_back(std::make_shared<Move>(aa));
      }
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

  inline bool ShogiPosition::CanPromote(const Location& l) const noexcept
  {
    bool f = ShogiGame::IsFull(sizeX, sizeY);
    return (OnTurn() == &Color::White && (l._y < (f?3U:1U))) || (OnTurn() != &Color::White && (l._y > (f?8U:3U)));
  }
    

  //ShogiGame::ShogiGame(const PieceMapP& m, Coordinate x, Coordinate y) noexcept : ShogiGame(m,
  //  new ShogiPosition(m, x, y), new ShogiTakenPosition(m, x, y), new StockPosition(m, 15, 2),
  //  new ShogiLayout(x, y), new ShogiTakenLayout(x, y), new ShogiStockLayout(x, y)) {}
  ShogiGame::ShogiGame(const PieceMapP& m, Coordinate x, Coordinate y) noexcept : ShogiGame(m, new ShogiPosition(m, x, y), new ShogiLayout(x, y)) {}

  ShogiGame::ShogiGame(const PieceMapP& m, ShogiPosition* p, ShogiLayout* l) noexcept : Game{ m,p,l }
  {
//    p->SetTPos(t);  // stores the Taken position inside the board position, so the move generator can access it for drops
    //AddToStock(Location(BoardPart::Main, 0U, 0U), &ShogiPiece::ShogiSKW);
    //AddToStock(Location(BoardPart::Main, 1U, 0U), &ShogiPiece::ShogiSGW);
    //AddToStock(Location(BoardPart::Main, 2U, 0U), &ShogiPiece::ShogiSSW);
    //AddToStock(Location(BoardPart::Main, 3U, 0U), &ShogiPiece::ShogiSBW);
    //AddToStock(Location(BoardPart::Main, 4U, 0U), &ShogiPiece::ShogiSRW);
    //AddToStock(Location(BoardPart::Main, 5U, 0U), &ShogiPiece::ShogiSNW);
    //AddToStock(Location(BoardPart::Main, 6U, 0U), &ShogiPiece::ShogiSLW);
    //AddToStock(Location(BoardPart::Main, 7U, 0U), &ShogiPiece::ShogiSPW);
    //                    
    //AddToStock(Location(BoardPart::Main, 8U, 0U), &ShogiPiece::ShogiPSW);
    //AddToStock(Location(BoardPart::Main, 9U, 0U), &ShogiPiece::ShogiPBW);
    //AddToStock(Location(BoardPart::Main, 10U, 0U), &ShogiPiece::ShogiPRW);
    //AddToStock(Location(BoardPart::Main, 11U, 0U), &ShogiPiece::ShogiPNW);
    //AddToStock(Location(BoardPart::Main, 12U, 0U), &ShogiPiece::ShogiPLW);
    //AddToStock(Location(BoardPart::Main, 13U, 0U), &ShogiPiece::ShogiPPW);

    //AddToStock(Location(BoardPart::Main, 0U, 1U), &ShogiPiece::ShogiSKW);
    //AddToStock(Location(BoardPart::Main, 1U, 1U), &ShogiPiece::ShogiSGW);
    //AddToStock(Location(BoardPart::Main, 2U, 1U), &ShogiPiece::ShogiSSW);
    //AddToStock(Location(BoardPart::Main, 3U, 1U), &ShogiPiece::ShogiSBW);
    //AddToStock(Location(BoardPart::Main, 4U, 1U), &ShogiPiece::ShogiSRW);
    //AddToStock(Location(BoardPart::Main, 5U, 1U), &ShogiPiece::ShogiSNW);
    //AddToStock(Location(BoardPart::Main, 6U, 1U), &ShogiPiece::ShogiSLW);
    //AddToStock(Location(BoardPart::Main, 7U, 1U), &ShogiPiece::ShogiSPW);
    //                         
    //AddToStock(Location(BoardPart::Main, 8U, 1U), &ShogiPiece::ShogiPSW);
    //AddToStock(Location(BoardPart::Main, 9U, 1U), &ShogiPiece::ShogiPBW);
    //AddToStock(Location(BoardPart::Main, 10U, 1U), &ShogiPiece::ShogiPRW);
    //AddToStock(Location(BoardPart::Main, 11U, 1U), &ShogiPiece::ShogiPNW);
    //AddToStock(Location(BoardPart::Main, 12U, 1U), &ShogiPiece::ShogiPLW);
    //AddToStock(Location(BoardPart::Main, 13U, 1U), &ShogiPiece::ShogiPPW);
    for (PieceIndex i = 0; i < m->GetCount(); i++)
      p->SetPiece(Location(BoardPart::Stock, i / 2U, i % 2U), m->GetPiece(i));  // expects respective Pieces with alternating colors
  }

  const VariantList& ShogiGame::GetVariants(void) noexcept
  {
    static VariantList v{ { Variant{ 9, 9, "Shogi" },{ Variant{ 5, 5, "Mini Shogi" } } } };
    return v;
  }

  const PieceMapP& ShogiGame::GetPieces(void) noexcept
  {
    static const PieceMapP& p = std::make_shared<PieceMap>();
    p->Add(&ShogiPiece::ShogiSKW);
    p->Add(&ShogiPiece::ShogiSGW);
    p->Add(&ShogiPiece::ShogiSSW);
    p->Add(&ShogiPiece::ShogiSBW);
    p->Add(&ShogiPiece::ShogiSRW);
    p->Add(&ShogiPiece::ShogiSNW);
    p->Add(&ShogiPiece::ShogiSLW);
    p->Add(&ShogiPiece::ShogiSPW);

    p->Add(&ShogiPiece::ShogiPSW);
    p->Add(&ShogiPiece::ShogiPBW);
    p->Add(&ShogiPiece::ShogiPRW);
    p->Add(&ShogiPiece::ShogiPNW);
    p->Add(&ShogiPiece::ShogiPLW);
    p->Add(&ShogiPiece::ShogiPPW);

    p->Add(&ShogiPiece::ShogiSKB);
    p->Add(&ShogiPiece::ShogiSGB);
    p->Add(&ShogiPiece::ShogiSSB);
    p->Add(&ShogiPiece::ShogiSBB);
    p->Add(&ShogiPiece::ShogiSRB);
    p->Add(&ShogiPiece::ShogiSNB);
    p->Add(&ShogiPiece::ShogiSLB);
    p->Add(&ShogiPiece::ShogiSPB);

    p->Add(&ShogiPiece::ShogiPSB);
    p->Add(&ShogiPiece::ShogiPBB);
    p->Add(&ShogiPiece::ShogiPRB);
    p->Add(&ShogiPiece::ShogiPNB);
    p->Add(&ShogiPiece::ShogiPLB);
    p->Add(&ShogiPiece::ShogiPPB);
    return p;
  }

}
