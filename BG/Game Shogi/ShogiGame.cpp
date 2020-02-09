#include "pch.h"

#include "ShogiGame.h"

namespace Shogi
{
  const Pawn    Pawn   ::ThePawn   {};
  const Knight  Knight ::TheKnight {};
  const Bishop  Bishop ::TheBishop {};
  const Rook    Rook   ::TheRook   {};
  const Lance   Lance  ::TheLance  {};
  const Silver  Silver ::TheSilver {};
  const Gold    Gold   ::TheGold   {};
  const King    King   ::TheKing   {};
  const PPawn   PPawn  ::ThePPawn  {};
  const PKnight PKnight::ThePKnight{};
  const PBishop PBishop::ThePBishop{};
  const PRook   PRook  ::ThePRook  {};
  const PLance  PLance ::ThePLance {};
  const PSilver PSilver::ThePSilver{};
  // S = Standard pieces, White
  const ShogiPiece ShogiPiece::ShogiWK { King::TheKing,       PieceColor::White, ShogiPiece::ShogiWK,  ShogiPiece::ShogiBK,  IDB_SHOGI_WK_L };
  const ShogiPiece ShogiPiece::ShogiWG { Gold::TheGold,       PieceColor::White, ShogiPiece::ShogiWG,  ShogiPiece::ShogiBG,  IDB_SHOGI_WG_L };
  const ShogiPiece ShogiPiece::ShogiWS { Silver::TheSilver,   PieceColor::White, ShogiPiece::ShogiWSP, ShogiPiece::ShogiBS,  IDB_SHOGI_WS_L };
  const ShogiPiece ShogiPiece::ShogiWB { Bishop::TheBishop,   PieceColor::White, ShogiPiece::ShogiWBP, ShogiPiece::ShogiBB,  IDB_SHOGI_WB_L };
  const ShogiPiece ShogiPiece::ShogiWR { Rook::TheRook,       PieceColor::White, ShogiPiece::ShogiWRP, ShogiPiece::ShogiBR,  IDB_SHOGI_WR_L };
  const ShogiPiece ShogiPiece::ShogiWN { Knight::TheKnight,   PieceColor::White, ShogiPiece::ShogiWNP, ShogiPiece::ShogiBN,  IDB_SHOGI_WN_L };
  const ShogiPiece ShogiPiece::ShogiWL { Lance::TheLance,     PieceColor::White, ShogiPiece::ShogiWLP, ShogiPiece::ShogiBL,  IDB_SHOGI_WL_L };
  const ShogiPiece ShogiPiece::ShogiWP { Pawn::ThePawn,       PieceColor::White, ShogiPiece::ShogiWPP, ShogiPiece::ShogiBP,  IDB_SHOGI_WP_L };
  // P = Promoted Pieces, White                                                                   
  const ShogiPiece ShogiPiece::ShogiWSP{ PSilver::ThePSilver, PieceColor::White, ShogiPiece::ShogiWSP, ShogiPiece::ShogiBS,  IDB_SHOGI_WSP_L };
  const ShogiPiece ShogiPiece::ShogiWBP{ PBishop::ThePBishop, PieceColor::White, ShogiPiece::ShogiWBP, ShogiPiece::ShogiBB,  IDB_SHOGI_WBP_L };
  const ShogiPiece ShogiPiece::ShogiWRP{ PRook::ThePRook,     PieceColor::White, ShogiPiece::ShogiWRP, ShogiPiece::ShogiBR,  IDB_SHOGI_WRP_L };
  const ShogiPiece ShogiPiece::ShogiWNP{ PKnight::ThePKnight, PieceColor::White, ShogiPiece::ShogiWNP, ShogiPiece::ShogiBN,  IDB_SHOGI_WNP_L };
  const ShogiPiece ShogiPiece::ShogiWLP{ PLance::ThePLance,   PieceColor::White, ShogiPiece::ShogiWLP, ShogiPiece::ShogiBL,  IDB_SHOGI_WLP_L };
  const ShogiPiece ShogiPiece::ShogiWPP{ PPawn::ThePPawn,     PieceColor::White, ShogiPiece::ShogiWPP, ShogiPiece::ShogiBP,  IDB_SHOGI_WPP_L };
  // S = Standard pieces, Black                                                                   
  const ShogiPiece ShogiPiece::ShogiBK { King::TheKing,       PieceColor::Black, ShogiPiece::ShogiBK,  ShogiPiece::ShogiWK,  IDB_SHOGI_BK_L };
  const ShogiPiece ShogiPiece::ShogiBG { Gold::TheGold,       PieceColor::Black, ShogiPiece::ShogiBG,  ShogiPiece::ShogiWG,  IDB_SHOGI_BG_L };
  const ShogiPiece ShogiPiece::ShogiBS { Silver::TheSilver,   PieceColor::Black, ShogiPiece::ShogiBSP, ShogiPiece::ShogiWS,  IDB_SHOGI_BS_L };
  const ShogiPiece ShogiPiece::ShogiBB { Bishop::TheBishop,   PieceColor::Black, ShogiPiece::ShogiBBP, ShogiPiece::ShogiWB,  IDB_SHOGI_BB_L };
  const ShogiPiece ShogiPiece::ShogiBR { Rook::TheRook,       PieceColor::Black, ShogiPiece::ShogiBRP, ShogiPiece::ShogiWR,  IDB_SHOGI_BR_L };
  const ShogiPiece ShogiPiece::ShogiBN { Knight::TheKnight,   PieceColor::Black, ShogiPiece::ShogiBNP, ShogiPiece::ShogiWN,  IDB_SHOGI_BN_L };
  const ShogiPiece ShogiPiece::ShogiBL { Lance::TheLance,     PieceColor::Black, ShogiPiece::ShogiBLP, ShogiPiece::ShogiWL,  IDB_SHOGI_BL_L };
  const ShogiPiece ShogiPiece::ShogiBP { Pawn::ThePawn,       PieceColor::Black, ShogiPiece::ShogiBPP, ShogiPiece::ShogiWP,  IDB_SHOGI_BP_L };
  // P = Promoted pieces, Black                                                                   
  const ShogiPiece ShogiPiece::ShogiBSP{ PSilver::ThePSilver, PieceColor::Black, ShogiPiece::ShogiBSP, ShogiPiece::ShogiWS,  IDB_SHOGI_BSP_L };
  const ShogiPiece ShogiPiece::ShogiBBP{ PBishop::ThePBishop, PieceColor::Black, ShogiPiece::ShogiBBP, ShogiPiece::ShogiWB,  IDB_SHOGI_BBP_L };
  const ShogiPiece ShogiPiece::ShogiBRP{ PRook::ThePRook,     PieceColor::Black, ShogiPiece::ShogiBRP, ShogiPiece::ShogiWR,  IDB_SHOGI_BRP_L };
  const ShogiPiece ShogiPiece::ShogiBNP{ PKnight::ThePKnight, PieceColor::Black, ShogiPiece::ShogiBNP, ShogiPiece::ShogiWN,  IDB_SHOGI_BNP_L };
  const ShogiPiece ShogiPiece::ShogiBLP{ PLance::ThePLance,   PieceColor::Black, ShogiPiece::ShogiBLP, ShogiPiece::ShogiWL,  IDB_SHOGI_BLP_L };
  const ShogiPiece ShogiPiece::ShogiBPP{ PPawn::ThePPawn,     PieceColor::Black, ShogiPiece::ShogiBPP, ShogiPiece::ShogiWP,  IDB_SHOGI_BPP_L };

  void Pawn::CollectMoves(const MainPosition& p, const Location& l, Moves& moves) const noexcept
  {
    const int dy = p.OnTurn() == PieceColor::White ? -1 : 1;
    p.AddIfLegal(moves, l, l + Offset(0, dy));
  }
  bool Pawn::CanDrop(const Piece& p, const Location& l, const MainPosition* pos) const noexcept
  {
    if (p.IsColor(PieceColor::White) ? (l.y_ < 1) : (l.y_ > pos->GetSizeY() - 2)) return false;       // drop anywhere except last row
    for (Coordinate j = 0; j < pos->GetSizeY(); j++)
    {
      const Piece& pp = pos->GetPiece(Location{ BoardPart::Main, l.x_,j });
      if (pp == p) return false;                                                                   // no two pawns are allowed in the same row
    }
    return true;
  }

  void Lance::CollectMoves(const MainPosition& p, const Location& l, Moves& moves) const noexcept
  {
    const int dy = p.OnTurn() == PieceColor::White ? -1 : 1;
    for (int z = 1; p.AddIfLegal(moves, l, l + Offset(0, z*dy)); z++);
  }
  bool Lance::CanMove(const Piece& p, const Location& l, const MainPosition* pos) const noexcept
  {
    return p.IsColor(PieceColor::White) ? (l.y_ > 0) : (l.y_ < pos->GetSizeY() - 1);                  // move (or drop) anywhere except last row
  }

  void Knight::CollectMoves(const MainPosition& p, const Location& l, Moves& moves) const noexcept
  {
    const int dy = p.OnTurn() == PieceColor::White ? -1 : 1;
    p.AddIfLegal(moves, l, l + Offset(+1, 2 * dy));
    p.AddIfLegal(moves, l, l + Offset(-1, 2 * dy));
  }
  bool Knight::CanMove(const Piece& p, const Location& l, const MainPosition* pos) const noexcept
  {
    return p.IsColor(PieceColor::White) ? (l.y_ > 1) : (l.y_ < pos->GetSizeY() - 2);                  // move (or drop) anywhere except last two rows
  }

  void Bishop::CollectMoves(const MainPosition& p, const Location& l, Moves& moves) const noexcept
  {
    for (auto& d : Offset::Bdirection)
      for (int z = 1; p.AddIfLegal(moves, l, l + d * z); z++);
  }

  void Rook::CollectMoves(const MainPosition& p, const Location& l, Moves& moves) const noexcept
  {
    for (auto& d : Offset::Rdirection)
      for (int z = 1; p.AddIfLegal(moves, l, l + d * z); z++);
  }

  void Silver::CollectMoves(const MainPosition& p, const Location& l, Moves& moves) const noexcept
  {
    const int dy = p.OnTurn() == PieceColor::White ? -1 : 1;
    p.AddIfLegal(moves, l, l + Offset(+1, dy));
    p.AddIfLegal(moves, l, l + Offset(+0, dy));
    p.AddIfLegal(moves, l, l + Offset(-1, dy));
    p.AddIfLegal(moves, l, l + Offset(-1, -dy));
    p.AddIfLegal(moves, l, l + Offset(+1, -dy));
  }

  void Gold::CollectMoves(const MainPosition& p, const Location& l, Moves& moves) const noexcept
  {
    Gold::CollectGoldMoves(p, l, moves);
  }
  void Gold::CollectGoldMoves(const MainPosition& p, const Location& l, Moves& moves) noexcept
  {
    const int dy = p.OnTurn() == PieceColor::White ? -1 : 1;
    p.AddIfLegal(moves, l, l + Offset(+1, dy));
    p.AddIfLegal(moves, l, l + Offset(+0, dy));
    p.AddIfLegal(moves, l, l + Offset(-1, dy));
    p.AddIfLegal(moves, l, l + Offset(+1, +0));
    p.AddIfLegal(moves, l, l + Offset(-1, +0));
    p.AddIfLegal(moves, l, l + Offset(+0, -dy));
  }

  void King::CollectMoves(const MainPosition& p, const Location& l, Moves& moves) const noexcept
  {
    for (auto& d : Offset::Qdirection)
      p.AddIfLegal(moves, l, l + d);
  }

  void PPawn::CollectMoves(const MainPosition& p, const Location& l, Moves& moves) const noexcept
  {
    Gold::CollectGoldMoves(p, l, moves);
  }

  void PLance::CollectMoves(const MainPosition& p, const Location& l, Moves& moves) const noexcept
  {
    Gold::CollectGoldMoves(p, l, moves);
  }

  void PKnight::CollectMoves(const MainPosition& p, const Location& l, Moves& moves) const noexcept
  {
    Gold::CollectGoldMoves(p, l, moves);
  }

  void PBishop::CollectMoves(const MainPosition& p, const Location& l, Moves& moves) const noexcept
  {
    for (auto& d : Offset::Bdirection)
      for (int z = 1; p.AddIfLegal(moves, l, l + d * z); z++);
    for (auto& d : Offset::Rdirection)
      p.AddIfLegal(moves, l, l + d);
  }

  void PRook::CollectMoves(const MainPosition& p, const Location& l, Moves& moves) const noexcept
  {
    for (auto& d : Offset::Rdirection)
      for (int z = 1; p.AddIfLegal(moves, l, l + d * z); z++);
    for (auto& d : Offset::Bdirection)
      p.AddIfLegal(moves, l, l + d);
  }

  void PSilver::CollectMoves(const MainPosition& p, const Location& l, Moves& moves) const noexcept
  {
    Gold::CollectGoldMoves(p, l, moves);
  }


  ShogiPosition::ShogiPosition(const PieceMapP& p, const Dimensions& d) noexcept : MainPosition(p, d)
  {
    assert((d[0].xCount_ == 9 && d[0].yCount_ == 9) || (d[0].xCount_ == 5 && d[0].yCount_ == 5));
    if (ShogiGame::IsFull(d[0].xCount_, d[0].yCount_))
    {
      SetPiece(Location( BoardPart::Main, 0U, 0U), ShogiPiece::ShogiBL);
      SetPiece(Location( BoardPart::Main, 1U, 0U), ShogiPiece::ShogiBN);
      SetPiece(Location( BoardPart::Main, 2U, 0U), ShogiPiece::ShogiBS);
      SetPiece(Location( BoardPart::Main, 3U, 0U), ShogiPiece::ShogiBG);
      SetPiece(Location( BoardPart::Main, 4U, 0U), ShogiPiece::ShogiBK);
      SetPiece(Location( BoardPart::Main, 5U, 0U), ShogiPiece::ShogiBG);
      SetPiece(Location( BoardPart::Main, 6U, 0U), ShogiPiece::ShogiBS);
      SetPiece(Location( BoardPart::Main, 7U, 0U), ShogiPiece::ShogiBN);
      SetPiece(Location( BoardPart::Main, 8U, 0U), ShogiPiece::ShogiBL);
                         
      SetPiece(Location( BoardPart::Main, 1U, 1U), ShogiPiece::ShogiBR);
      SetPiece(Location( BoardPart::Main, 7U, 1U), ShogiPiece::ShogiBB);
                        
      SetPiece(Location( BoardPart::Main, 0U, 2U), ShogiPiece::ShogiBP);
      SetPiece(Location( BoardPart::Main, 1U, 2U), ShogiPiece::ShogiBP);
      SetPiece(Location( BoardPart::Main, 2U, 2U), ShogiPiece::ShogiBP);
      SetPiece(Location( BoardPart::Main, 3U, 2U), ShogiPiece::ShogiBP);
      SetPiece(Location( BoardPart::Main, 4U, 2U), ShogiPiece::ShogiBP);
      SetPiece(Location( BoardPart::Main, 5U, 2U), ShogiPiece::ShogiBP);
      SetPiece(Location( BoardPart::Main, 6U, 2U), ShogiPiece::ShogiBP);
      SetPiece(Location( BoardPart::Main, 7U, 2U), ShogiPiece::ShogiBP);
      SetPiece(Location( BoardPart::Main, 8U, 2U), ShogiPiece::ShogiBP);
                         
                         
      SetPiece(Location( BoardPart::Main, 0U, 6U), ShogiPiece::ShogiWP);
      SetPiece(Location( BoardPart::Main, 1U, 6U), ShogiPiece::ShogiWP);
      SetPiece(Location( BoardPart::Main, 2U, 6U), ShogiPiece::ShogiWP);
      SetPiece(Location( BoardPart::Main, 3U, 6U), ShogiPiece::ShogiWP);
      SetPiece(Location( BoardPart::Main, 4U, 6U), ShogiPiece::ShogiWP);
      SetPiece(Location( BoardPart::Main, 5U, 6U), ShogiPiece::ShogiWP);
      SetPiece(Location( BoardPart::Main, 6U, 6U), ShogiPiece::ShogiWP);
      SetPiece(Location( BoardPart::Main, 7U, 6U), ShogiPiece::ShogiWP);
      SetPiece(Location( BoardPart::Main, 8U, 6U), ShogiPiece::ShogiWP);
                         
      SetPiece(Location( BoardPart::Main, 1U, 7U), ShogiPiece::ShogiWB);
      SetPiece(Location( BoardPart::Main, 7U, 7U), ShogiPiece::ShogiWR);
                         
      SetPiece(Location( BoardPart::Main, 0U, 8U), ShogiPiece::ShogiWL);
      SetPiece(Location( BoardPart::Main, 1U, 8U), ShogiPiece::ShogiWN);
      SetPiece(Location( BoardPart::Main, 2U, 8U), ShogiPiece::ShogiWS);
      SetPiece(Location( BoardPart::Main, 3U, 8U), ShogiPiece::ShogiWG);
      SetPiece(Location( BoardPart::Main, 4U, 8U), ShogiPiece::ShogiWK);
      SetPiece(Location( BoardPart::Main, 5U, 8U), ShogiPiece::ShogiWG);
      SetPiece(Location( BoardPart::Main, 6U, 8U), ShogiPiece::ShogiWS);
      SetPiece(Location( BoardPart::Main, 7U, 8U), ShogiPiece::ShogiWN);
      SetPiece(Location( BoardPart::Main, 8U, 8U), ShogiPiece::ShogiWL);
    }
    else
    {
      SetPiece(Location( BoardPart::Main, 0U, 0U), ShogiPiece::ShogiBR);
      SetPiece(Location( BoardPart::Main, 1U, 0U), ShogiPiece::ShogiBB);
      SetPiece(Location( BoardPart::Main, 2U, 0U), ShogiPiece::ShogiBS);
      SetPiece(Location( BoardPart::Main, 3U, 0U), ShogiPiece::ShogiBG);
      SetPiece(Location( BoardPart::Main, 4U, 0U), ShogiPiece::ShogiBK);
      SetPiece(Location( BoardPart::Main, 4U, 1U), ShogiPiece::ShogiBP);
                         
      SetPiece(Location( BoardPart::Main, 4U, 4U), ShogiPiece::ShogiWR);
      SetPiece(Location( BoardPart::Main, 3U, 4U), ShogiPiece::ShogiWB);
      SetPiece(Location( BoardPart::Main, 2U, 4U), ShogiPiece::ShogiWS);
      SetPiece(Location( BoardPart::Main, 1U, 4U), ShogiPiece::ShogiWG);
      SetPiece(Location( BoardPart::Main, 0U, 4U), ShogiPiece::ShogiWK);
      SetPiece(Location( BoardPart::Main, 0U, 3U), ShogiPiece::ShogiWP);
    }
  }

  void ShogiPosition::GetAllMoves(void) const noexcept                          // generate all moves and save list
  {
    MainPosition::GetAllMoves();                                          // standard: get moves for all pieces on the board

    // add all potential drop moves
    for (Coordinate y = 0U; y < 2; y++)
      for (Coordinate x = 0U; true; x++)
      {
        const Location l(BoardPart::Taken, x, y);
        const Piece& p = taken_.GetPiece(l);
        if (p == Piece::NoTile) break;                                          // end of Taken reached
        if (p.IsBlank()) continue;                                       // no piece here to drop, try next Location
        const ShogiPiece& pp = dynamic_cast<const ShogiPiece&>(p);        // must be a shogipiece,
        assert(pp != Piece::NoTile);                                            // verify it
        Moves& m = pp.IsColor(PieceColor::White) ? movesW_ : movesB_;

        Actions a{};
        a.push_back(std::make_shared<ActionLift>(l, pp));
        for (Coordinate i = 0; i < sizeX_; i++)
          for (Coordinate j = 0; j < sizeY_; j++)
          {
            const Location ll{ BoardPart::Main, i, j };
            if (GetPiece(ll).IsBlank() && pp.CanDrop(this, ll))
            {
              Actions aa{ a };
              aa.push_back(std::make_shared<ActionDrop>(ll, pp));
              m.push_back(std::make_shared<Move>(aa));
            }
          }
      }
  }
  
  bool ShogiPosition::AddIfLegal(Moves& m, const Location& fr, const Location& to) const noexcept // returns true if further tries in this direction are allowed
  {
    const Piece& pt = GetPiece(to);                                       // what is on the target field
    if (pt == Piece::NoTile) return false;                                      // out of board
    if (IsTaken(fr) && !pt.IsBlank()) return false;                      // can't drop onto existing piece

    const ShogiPiece& pf = dynamic_cast<const ShogiPiece&>(GetPiece(fr));
    if (pt.IsColor(pf.GetColor())) return false;                        // own piece

    if (IsTaken(fr) && !pf.CanDrop(this, to)) return false;

    Actions a{};
    a.push_back(std::make_shared<ActionLift>(fr, pf));
    if (!pt.IsBlank()) // if something is there, take it and place in Taken
    {
      a.push_back(std::make_shared<ActionLift>(to, pt));
      const Piece& pt0 = pt.Promote(false);  // demote and change color/owner
      a.push_back(std::make_shared<ActionDrop>(GetNextTakenL(pt.GetColor()), pt0));
    }
    // if this is not a drop, check if promotion is possible
    if (!IsTaken(fr) && (CanPromote(pf.GetColor(), fr) || CanPromote(pf.GetColor(), to)) && pf.IsPromotable())
    {
      Actions aa{ a }; // promote is optional, so we make a copy and add as second move
      aa.push_back(std::make_shared<ActionDrop>(to, pf.Promote(true)));
      m.push_back(std::make_shared<Move>(aa));
    }
    if (pf.CanMove(this, to)) // if no promotion, verify if this piece can ever move/drop there
    {
      a.push_back(std::make_shared<ActionDrop>(to, pf));  // unpromoted placement
      m.push_back(std::make_shared<Move>(a));
    }
    return pt.IsBlank();   // if free tile, keep trying this direction
  }

  PositionValue ShogiPosition::EvaluateStatically(void) const noexcept
  {
    //bool kw{false};
    //bool kb{false};

    //for (Coordinate i = 0; i < sizeX_; i++)
    //{
    //  for (Coordinate j = 0; j < sizeY_; j++)
    //  {
    //    const Piece& p = GetPiece(Location{ BoardPart::Main,i,j });
    //    if ((p == &ShogiPiece::ShogiWK)) kw = true;
    //    if ((p == &ShogiPiece::ShogiBK)) kb = true;
    //  }
    //}

    //if (!kw) { value_ = PositionValue::PValueType::Lost; return; }
    //if (!kb) { value_ = PositionValue::PValueType::Won;  return; }

    if (!HasPiece(ShogiPiece::ShogiWK)) return PositionValue::PValueType::Lost;
    if (!HasPiece(ShogiPiece::ShogiBK)) return PositionValue::PValueType::Won;
    return MainPosition::EvaluateStatically();
  }

  inline bool ShogiPosition::CanPromote(const PieceColor& c, const Location& l) const noexcept
  {
    const bool f = ShogiGame::IsFull(sizeX_, sizeY_);
    return c == PieceColor::White ? (l.y_ < (f ? 3U : 1U)) : (l.y_ > (f ? 8U : 3U));
  }
    
  const VariantList& ShogiGame::GetVariants(void) noexcept
  {
    static VariantList v{ { Variant{ 9, 9, "Shogi" },{ Variant{ 5, 5, "Mini Shogi" } } } };
    return v;
  }

  const PieceMapP& ShogiGame::GetPieces(void) noexcept
  {
    static const PieceMapP& p = std::make_shared<PieceMap>();
    p->Empty();
    p->Add(ShogiPiece::ShogiWK);
    p->Add(ShogiPiece::ShogiBK);
    p->Add(ShogiPiece::ShogiWG);
    p->Add(ShogiPiece::ShogiBG);
    p->Add(ShogiPiece::ShogiWS);
    p->Add(ShogiPiece::ShogiBS);
    p->Add(ShogiPiece::ShogiWB);
    p->Add(ShogiPiece::ShogiBB);
    p->Add(ShogiPiece::ShogiWR);
    p->Add(ShogiPiece::ShogiBR);
    p->Add(ShogiPiece::ShogiWN);
    p->Add(ShogiPiece::ShogiBN);
    p->Add(ShogiPiece::ShogiWL);
    p->Add(ShogiPiece::ShogiBL);
    p->Add(ShogiPiece::ShogiWP);
    p->Add(ShogiPiece::ShogiBP);

    p->Add(ShogiPiece::ShogiWSP);
    p->Add(ShogiPiece::ShogiBSP);
    p->Add(ShogiPiece::ShogiWBP);
    p->Add(ShogiPiece::ShogiBBP);
    p->Add(ShogiPiece::ShogiWRP);
    p->Add(ShogiPiece::ShogiBRP);
    p->Add(ShogiPiece::ShogiWNP);
    p->Add(ShogiPiece::ShogiBNP);
    p->Add(ShogiPiece::ShogiWLP);
    p->Add(ShogiPiece::ShogiBLP);
    p->Add(ShogiPiece::ShogiWPP);
    p->Add(ShogiPiece::ShogiBPP);

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
