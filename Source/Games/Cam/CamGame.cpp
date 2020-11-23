#include "Games.h"

#include "CamGame.h"

namespace Cam
{
  constexpr int isqr(int x) noexcept { return x * x; }

  const Pawn   Pawn  ::ThePawn  {};
  const Knight Knight::TheKnight{};
  const CamPiece CamPiece::WP{ Pawn::ThePawn,     PieceColor::White, IDB_WPL };
  const CamPiece CamPiece::WN{ Knight::TheKnight, PieceColor::White, IDB_WNL };
  const CamPiece CamPiece::BP{ Pawn::ThePawn,     PieceColor::Black, IDB_BPL };
  const CamPiece CamPiece::BN{ Knight::TheKnight, PieceColor::Black, IDB_BNL };

  void Pawn::CollectMoves(const MainPosition& p, const Location& l, Moves& m) const noexcept
  {
    const CamPosition& pos = dynamic_cast<const CamPosition&>(p);         // position must be a Cam position
    const Piece& p0 = pos.GetPiece(l);                                    // piece that is moving
    CollectJumps(p, l, Actions{}, false, PieceColor::NoColor, m);                // collect all jumps

    for (const auto& d : Offset::Qdirection)                              // try slides all eight directions
    {
      const Location to{ l + d };                                         // location to move to
      const Piece& p1 = pos.GetPiece(to);                                 // what is on the target location?
      if (p1 == Piece::NoTile) continue;                                 // out of board
      if (!p1.IsBlank()) continue;                                       // tile occupied

      Actions a{};
      a.push_back(std::make_shared<ActionLift>(l, p0));                   // pick piece up
      a.push_back(std::make_shared<ActionDrop>(to, p0));                 // and place it on target
      m.push_back(std::make_shared<Move>(a));                             // add move to move list
    }
  }

  bool Pawn::CollectJumps(const MainPosition& p, const Location& fr, const Actions& a, bool charge, const PieceColor& c, Moves& m) const noexcept
  {
    const Piece& p0 = a.empty() ? p.GetPiece(fr) : a[0]->GetPiece();        // the piece that is moving
    assert(p0 != Piece::NoTile);
    assert(!p0.IsBlank());

    const PieceColor& c0 = p0.GetColor();                                     // color of the piece moving
    bool any{ false };                                                    // were any more jumps possible?

    for (auto& d : Offset::Qdirection)                                    // try all eight directions
    {
      Actions a1{ a };                                                    // copy the previous jump sequence, so we can extend it
      const Location l1{ fr + d };                                        // location to jump over
      const Location l2{ l1 + d };                                        // location to jump to

      // check the jumped-over tile                                         
      const Piece& p1 = p.GetPiece(l1);                                   // what is on the tile to jump over?
      if (p1 == Piece::NoTile) continue;                                  // tile is not existing, can't jump over it
      if (p1.IsBlank()) continue;                                         // tile is not occupied, can't jump over it
      const PieceColor& c1 = p1.GetColor();                                    // color of jumped-over piece

      // check the jump-to tile                                           
      const Piece& p2 = p.GetPiece(l2);                                   // what is on the jump-to tile
      if (p2 == Piece::NoTile) continue;                                  // tile is not existing, can't jump there
      if (!p2.IsBlank()) continue;                                        // tile is occupied, can't jump there

      // check the jump is allowed
      const PieceColor& cc{ c == PieceColor::NoColor ? c1 : c };                    // first jump - either one is allowed
      if (c != PieceColor::NoColor)                                            // trying to switch jumped color?
      {
        if (cc != c0) continue;                                           // can't switch back to jumping own pieces
        if (!charge) continue;                                            // can't switch to enemy pieces if not 'charging'
      }

      // check if the same jump was done before (can't jump back and forth or in circles, and can't jump the same opponent piece twice)
      if (a.IsRepeat(l1, d)) continue;                                    // don't allow the same move again

      // a legal jump was found
      any = true;
      a1.push_back(std::make_shared<ActionLift>(fr, p0));
      if (c1 != c0)                                                       // jump was over an enemy piece, take it
      {
        a1.push_back(std::make_shared<ActionLift>(l1, p1));
        a1.push_back(std::make_shared<ActionDrop>(Location(BoardPart::Taken, 0, p1.GetColor() == PieceColor::White ? 0U : 1U), p1));
      }
      a1.push_back(std::make_shared<ActionDrop>(l2, p0));

      if (!CollectJumps(p, l2, a1, charge, cc, m) || (c0 == c1))          // collect potential further jumps
        m.push_back(std::make_shared<Move>(a1));                          // if it could NOT be extended, or was a jump over an own piece, add as move
    }
    return any;
  }


  void Knight::CollectMoves(const MainPosition& p, const Location& l, Moves& m) const noexcept
  {
    Pawn::CollectMoves(p, l, m);
    const CamPosition& pos = dynamic_cast<const CamPosition&>(p);         // position must be a Cam position
    CollectJumps(p, l, Actions{}, true, PieceColor::NoColor, m);             // collect all jumps
  }

  unsigned int CamPiece::GetValue(const MainPosition& p, const Location& l) const noexcept
  {
    return kind_.GetValue(p, l) + isqr(IsColor(PieceColor::White) ? p.GetSizeY() - 1 - l.y_ : l.y_);
  }


  CamPosition::CamPosition(VariantCode c, const PieceMapP& p, const Dimensions& d) noexcept : MainPosition(c, p, d)
  {
    if (d[0].xCount_ == 12) // full Camelot
    {
      // Black Knights
      SetPiece(Location( BoardPart::Main,  2U,  5U), CamPiece::BN);
      SetPiece(Location( BoardPart::Main,  3U,  6U), CamPiece::BN);
      SetPiece(Location( BoardPart::Main,  8U,  6U), CamPiece::BN);
      SetPiece(Location( BoardPart::Main,  9U,  5U), CamPiece::BN);
                                
      // Black Pawns          
      SetPiece(Location( BoardPart::Main,  3U,  5U), CamPiece::BP);
      SetPiece(Location( BoardPart::Main,  4U,  5U), CamPiece::BP);
      SetPiece(Location( BoardPart::Main,  5U,  5U), CamPiece::BP);
      SetPiece(Location( BoardPart::Main,  6U,  5U), CamPiece::BP);
      SetPiece(Location( BoardPart::Main,  7U,  5U), CamPiece::BP);
      SetPiece(Location( BoardPart::Main,  8U,  5U), CamPiece::BP);
                            
      SetPiece(Location( BoardPart::Main,  4U,  6U), CamPiece::BP);
      SetPiece(Location( BoardPart::Main,  5U,  6U), CamPiece::BP);
      SetPiece(Location( BoardPart::Main,  6U,  6U), CamPiece::BP);
      SetPiece(Location( BoardPart::Main,  7U,  6U), CamPiece::BP);
                          
      // White Knights    
      SetPiece(Location( BoardPart::Main,  2U, 10U), CamPiece::WN);
      SetPiece(Location( BoardPart::Main,  3U,  9U), CamPiece::WN);
      SetPiece(Location( BoardPart::Main,  8U,  9U), CamPiece::WN);
      SetPiece(Location( BoardPart::Main,  9U, 10U), CamPiece::WN);
                           
      // White Pawns        
      SetPiece(Location( BoardPart::Main,  3U, 10U), CamPiece::WP);
      SetPiece(Location( BoardPart::Main,  4U, 10U), CamPiece::WP);
      SetPiece(Location( BoardPart::Main,  5U, 10U), CamPiece::WP);
      SetPiece(Location( BoardPart::Main,  6U, 10U), CamPiece::WP);
      SetPiece(Location( BoardPart::Main,  7U, 10U), CamPiece::WP);
      SetPiece(Location( BoardPart::Main,  8U, 10U), CamPiece::WP);
                          
      SetPiece(Location( BoardPart::Main,  4U,  9U), CamPiece::WP);
      SetPiece(Location( BoardPart::Main,  5U,  9U), CamPiece::WP);
      SetPiece(Location( BoardPart::Main,  6U,  9U), CamPiece::WP);
      SetPiece(Location( BoardPart::Main,  7U,  9U), CamPiece::WP);
                          
      // Illegal tiles     
      SetPiece(Location( BoardPart::Main,  0U,  0U), Piece::NoTile);
      SetPiece(Location( BoardPart::Main,  1U,  0U), Piece::NoTile);
      SetPiece(Location( BoardPart::Main,  2U,  0U), Piece::NoTile);
      SetPiece(Location( BoardPart::Main,  3U,  0U), Piece::NoTile);
      SetPiece(Location( BoardPart::Main,  4U,  0U), Piece::NoTile);
      SetPiece(Location( BoardPart::Main,  7U,  0U), Piece::NoTile);
      SetPiece(Location( BoardPart::Main,  8U,  0U), Piece::NoTile);
      SetPiece(Location( BoardPart::Main,  9U,  0U), Piece::NoTile);
      SetPiece(Location( BoardPart::Main, 10U,  0U), Piece::NoTile);
      SetPiece(Location( BoardPart::Main, 11U,  0U), Piece::NoTile);
                          
      SetPiece(Location( BoardPart::Main,  0U,  1U), Piece::NoTile);
      SetPiece(Location( BoardPart::Main,  1U,  1U), Piece::NoTile);
      SetPiece(Location( BoardPart::Main, 10U,  1U), Piece::NoTile);
      SetPiece(Location( BoardPart::Main, 11U,  1U), Piece::NoTile);
                          
      SetPiece(Location( BoardPart::Main,  0U,  2U), Piece::NoTile);
      SetPiece(Location( BoardPart::Main, 11U,  2U), Piece::NoTile);

      SetPiece(Location( BoardPart::Main,  0U, 13U), Piece::NoTile);
      SetPiece(Location( BoardPart::Main, 11U, 13U), Piece::NoTile);

      SetPiece(Location( BoardPart::Main,  0U, 14U), Piece::NoTile);
      SetPiece(Location( BoardPart::Main,  1U, 14U), Piece::NoTile);
      SetPiece(Location( BoardPart::Main, 10U, 14U), Piece::NoTile);
      SetPiece(Location( BoardPart::Main, 11U, 14U), Piece::NoTile);

      SetPiece(Location( BoardPart::Main,  0U, 15U), Piece::NoTile);
      SetPiece(Location( BoardPart::Main,  1U, 15U), Piece::NoTile);
      SetPiece(Location( BoardPart::Main,  2U, 15U), Piece::NoTile);
      SetPiece(Location( BoardPart::Main,  3U, 15U), Piece::NoTile);
      SetPiece(Location( BoardPart::Main,  4U, 15U), Piece::NoTile);
      SetPiece(Location( BoardPart::Main,  7U, 15U), Piece::NoTile);
      SetPiece(Location( BoardPart::Main,  8U, 15U), Piece::NoTile);
      SetPiece(Location( BoardPart::Main,  9U, 15U), Piece::NoTile);
      SetPiece(Location( BoardPart::Main, 10U, 15U), Piece::NoTile);
      SetPiece(Location( BoardPart::Main, 11U, 15U), Piece::NoTile);
    }

    else  // Mini Cam position
    {
      // Black Knights
      SetPiece(Location( BoardPart::Main,  2U,  3U), CamPiece::BN);
      SetPiece(Location( BoardPart::Main,  4U,  3U), CamPiece::BN);
                         
      // Black Pawns      
      SetPiece(Location( BoardPart::Main,  1U,  4U), CamPiece::BP);
      SetPiece(Location( BoardPart::Main,  2U,  4U), CamPiece::BP);
      SetPiece(Location( BoardPart::Main,  3U,  4U), CamPiece::BP);
      SetPiece(Location( BoardPart::Main,  4U,  4U), CamPiece::BP);
      SetPiece(Location( BoardPart::Main,  5U,  4U), CamPiece::BP);
                         
      // White Knights   
      SetPiece(Location( BoardPart::Main,  2U,  9U), CamPiece::WN);
      SetPiece(Location( BoardPart::Main,  4U,  9U), CamPiece::WN);
                          
      // White Pawns     
      SetPiece(Location( BoardPart::Main,  1U,  8U), CamPiece::WP);
      SetPiece(Location( BoardPart::Main,  2U,  8U), CamPiece::WP);
      SetPiece(Location( BoardPart::Main,  3U,  8U), CamPiece::WP);
      SetPiece(Location( BoardPart::Main,  4U,  8U), CamPiece::WP);
      SetPiece(Location( BoardPart::Main,  5U,  8U), CamPiece::WP);
                         
      // Illegal tiles    
      SetPiece(Location( BoardPart::Main,  0U,  0U), Piece::NoTile);
      SetPiece(Location( BoardPart::Main,  1U,  0U), Piece::NoTile);
      SetPiece(Location( BoardPart::Main,  2U,  0U), Piece::NoTile);
      SetPiece(Location( BoardPart::Main,  4U,  0U), Piece::NoTile);
      SetPiece(Location( BoardPart::Main,  5U,  0U), Piece::NoTile);
      SetPiece(Location( BoardPart::Main,  6U,  0U), Piece::NoTile);
                              
      SetPiece(Location( BoardPart::Main,  0U,  1U), Piece::NoTile);
      SetPiece(Location( BoardPart::Main,  1U,  1U), Piece::NoTile);
      SetPiece(Location( BoardPart::Main,  5U,  1U), Piece::NoTile);
      SetPiece(Location( BoardPart::Main,  6U,  1U), Piece::NoTile);
                               
      SetPiece(Location( BoardPart::Main,  0U,  2U), Piece::NoTile);
      SetPiece(Location( BoardPart::Main,  6U,  2U), Piece::NoTile);
                          
      SetPiece(Location( BoardPart::Main,  0U, 10U), Piece::NoTile);
      SetPiece(Location( BoardPart::Main,  6U, 10U), Piece::NoTile);
                         
      SetPiece(Location( BoardPart::Main,  0U, 11U), Piece::NoTile);
      SetPiece(Location( BoardPart::Main,  1U, 11U), Piece::NoTile);
      SetPiece(Location( BoardPart::Main,  5U, 11U), Piece::NoTile);
      SetPiece(Location( BoardPart::Main,  6U, 11U), Piece::NoTile);
                           
      SetPiece(Location( BoardPart::Main,  0U, 12U), Piece::NoTile);
      SetPiece(Location( BoardPart::Main,  1U, 12U), Piece::NoTile);
      SetPiece(Location( BoardPart::Main,  2U, 12U), Piece::NoTile);
      SetPiece(Location( BoardPart::Main,  4U, 12U), Piece::NoTile);
      SetPiece(Location( BoardPart::Main,  5U, 12U), Piece::NoTile);
      SetPiece(Location( BoardPart::Main,  6U, 12U), Piece::NoTile);
    }
  }


  void CamPosition::GetAllMoves(void) const noexcept                            // collect all moves for all pieces
  {
    MainPosition::GetAllMoves();
    JumpsOnly(movesW_);
    JumpsOnly(movesB_);
  }

  //PositionValue CamPosition::EvaluateWin(void) const
  //{
  //  bool wfree{false};
  //  bool bfree{false};

  //  for (Coordinate i = 0; i < sizeX_; i++)
  //  {
  //    const Piece& pw = GetPiece(Location(i, 0));
  //    const Piece& pb = GetPiece(Location(i, sizeY_ - 1));
  //    if (pw != &Piece::NoTile && !pw->IsColor(&PieceColor::White)) wfree = true;
  //    if (pb != &Piece::NoTile && !pb->IsColor(&PieceColor::Black)) bfree = true;
  //  }

  //  int dy = OnTurn() == &PieceColor::White ? 1 : -1;
  //  if (!bfree) return PositionValue::PValueType::Lost * dy;
  //  if (!wfree) return PositionValue::PValueType::Won * dy;
  //  return 0;
  //}


  const VariantList& CamGame::GetVariants(void) noexcept
  {
    static VariantList v{
      { Variant{ "Camelot", 'A', 12, 16 } },
      { Variant{ "Cam",     'B',  7, 13 } },
    };
    return v;
  }

  const PieceMapP& CamGame::GetPieces(VariantCode c) noexcept
  {
    static const PieceMapP& p = std::make_shared<PieceMap>();
    p->Empty();
    p->Add(CamPiece::WN);
    p->Add(CamPiece::BN);
    p->Add(CamPiece::WP);
    p->Add(CamPiece::BP);
    return p;
  }


  const Dimensions& CamGame::GetDimensions(VariantCode c, Coordinate x, Coordinate y) noexcept
  {
    static Dimensions d{
       Dimension(x, y, BoardStartX, BoardStartY, FieldSizeX, FieldSizeY, 1, 1),
       Dimension(3, 2, BoardStartX + FieldSizeX * (x + 1), BoardStartY + FieldSizeY / 2 + FieldSizeY * (y - 2), FieldSizeX, FieldSizeY),
       Dimension(2 * x, 2, FieldSizeX * (x + 1), BoardStartY + FieldSizeSY, FieldSizeSX, FieldSizeSY, 0, FieldSizeY * y - FieldSizeSY * 4),
    };
    return d;
  }

}
