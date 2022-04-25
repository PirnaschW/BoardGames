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

  void Pawn::CollectMoves(const Board& p, const Location& l, Moves& m) const noexcept
  {
    //const CamBoard& board_ = down_cast<const CamBoard&>(p);         // position must be a Cam position
    //const Piece& p0 = board_.GetPieceIndex(l);                                    // piece that is moving
    //CollectJumps(p, l, Actions{}, false, PieceColor::NoColor, m);                // collect all jumps

    //for (const auto& d : Offset::QDirection)                              // try slides all eight directions
    //{
    //  const Location to{ l + d };                                         // location to move to
    //  const Piece& p1 = board_.GetPieceIndex(to);                                 // what is on the target location?
    //  if (p1 == Piece::NoTile) continue;                                 // out of board
    //  if (!p1.IsBlank()) continue;                                       // tile occupied

    //  Actions a{};
    //  a.push_back(std::make_shared<ActionLift>(l, p0));                   // pick piece up
    //  a.push_back(std::make_shared<ActionDrop>(to, p0));                 // and place it on target
    //  m.push_back(std::make_shared<Move>(a));                             // add move to move list
    //}
  }

  bool Pawn::CollectJumps(const Board& p, const Location& fr, const Actions& a, bool charge, const PieceColor& c, Moves& m) const noexcept
  {
    //const Piece& p0 = a.empty() ? p.GetPieceIndex(fr) : a[0]->GetPieceIndex();        // the piece that is moving
    //assert(p0 != Piece::NoTile);
    //assert(!p0.IsBlank());

    //const PieceColor& c0 = p0.GetColor();                                     // color of the piece moving
    bool any{ false };                                                    // were any more jumps possible?

    //for (auto& d : Offset::QDirection)                                    // try all eight directions
    //{
    //  Actions a1{ a };                                                    // copy the previous jump sequence, so we can extend it
    //  const Location l1{ fr + d };                                        // location to jump over
    //  const Location l2{ l1 + d };                                        // location to jump to

    //  // check the jumped-over tile                                         
    //  const Piece& p1 = p.GetPieceIndex(l1);                                   // what is on the tile to jump over?
    //  if (p1 == Piece::NoTile) continue;                                  // tile is not existing, can't jump over it
    //  if (p1.IsBlank()) continue;                                         // tile is not occupied, can't jump over it
    //  const PieceColor& c1 = p1.GetColor();                                    // color of jumped-over piece

    //  // check the jump-to tile                                           
    //  const Piece& p2 = p.GetPieceIndex(l2);                                   // what is on the jump-to tile
    //  if (p2 == Piece::NoTile) continue;                                  // tile is not existing, can't jump there
    //  if (!p2.IsBlank()) continue;                                        // tile is occupied, can't jump there

    //  // check the jump is allowed
    //  const PieceColor& cc{ c == PieceColor::NoColor ? c1 : c };                    // first jump - either one is allowed
    //  if (c != PieceColor::NoColor)                                            // trying to switch jumped color?
    //  {
    //    if (cc != c0) continue;                                           // can't switch back to jumping own pieces
    //    if (!charge) continue;                                            // can't switch to enemy pieces if not 'charging'
    //  }

    //  // check if the same jump was done before (can't jump back and forth or in circles, and can't jump the same opponent piece twice)
    //  if (a.IsRepeat(l1, d)) continue;                                    // don't allow the same move again

    //  // a legal jump was found
    //  any = true;
    //  a1.push_back(std::make_shared<ActionLift>(fr, p0));
    //  if (c1 != c0)                                                       // jump was over an enemy piece, take it
    //  {
    //    a1.push_back(std::make_shared<ActionLift>(l1, p1));
    //    a1.push_back(std::make_shared<ActionDrop>(Location(BoardPartID::Taken, 0, p1.GetColor() == PieceColor::White ? 0U : 1U), p1));
    //  }
    //  a1.push_back(std::make_shared<ActionDrop>(l2, p0));

    //  if (!CollectJumps(p, l2, a1, charge, cc, m) || (c0 == c1))          // collect potential further jumps
    //    m.push_back(std::make_shared<Move>(a1));                          // if it could NOT be extended, or was a jump over an own piece, add as move
    //}
    return any;
  }


  void Knight::CollectMoves(const Board& p, const Location& l, Moves& m) const noexcept
  {
    Pawn::CollectMoves(p, l, m);
    const CamBoard& board_ = down_cast<const CamBoard&>(p);         // position must be a Cam position
    CollectJumps(p, l, Actions{}, true, PieceColor::NoColor, m);             // collect all jumps
  }

  PositionValue CamPiece::GetValue(const Board& p, const Location& l) const noexcept
  {
    //return kind_.GetValue(p, l) + isqr(IsColor(PieceColor::White) ? p.GetSizeY() - 1 - l.y_ : l.y_);
    return 0;
  }


  void CamBoard::SetStartingBoard() noexcept
  {
    //switch (v_.c)
    //{
    //  case 'F': // full Camelot
    //    // Black Knights
    //    SetPieceIndex(CamPiece::BN,  2U,  5U, BoardPartID::Stage);
    //    SetPieceIndex(CamPiece::BN,  3U,  6U, BoardPartID::Stage);
    //    SetPieceIndex(CamPiece::BN,  8U,  6U, BoardPartID::Stage);
    //    SetPieceIndex(CamPiece::BN,  9U,  5U, BoardPartID::Stage);
    //                                                     
    //    // Black Pawns                                   
    //    SetPieceIndex(CamPiece::BP,  3U,  5U, BoardPartID::Stage);
    //    SetPieceIndex(CamPiece::BP,  4U,  5U, BoardPartID::Stage);
    //    SetPieceIndex(CamPiece::BP,  5U,  5U, BoardPartID::Stage);
    //    SetPieceIndex(CamPiece::BP,  6U,  5U, BoardPartID::Stage);
    //    SetPieceIndex(CamPiece::BP,  7U,  5U, BoardPartID::Stage);
    //    SetPieceIndex(CamPiece::BP,  8U,  5U, BoardPartID::Stage);
    //                                                             
    //    SetPieceIndex(CamPiece::BP,  4U,  6U, BoardPartID::Stage);
    //    SetPieceIndex(CamPiece::BP,  5U,  6U, BoardPartID::Stage);
    //    SetPieceIndex(CamPiece::BP,  6U,  6U, BoardPartID::Stage);
    //    SetPieceIndex(CamPiece::BP,  7U,  6U, BoardPartID::Stage);

    //    // White Knights
    //    SetPieceIndex(Location(BoardPartID::Stage,  2U, 10U), CamPiece::WN);
    //    SetPieceIndex(Location(BoardPartID::Stage,  3U,  9U), CamPiece::WN);
    //    SetPieceIndex(Location(BoardPartID::Stage,  8U,  9U), CamPiece::WN);
    //    SetPieceIndex(Location(BoardPartID::Stage,  9U, 10U), CamPiece::WN);

    //    // White Pawns
    //    SetPieceIndex(Location(BoardPartID::Stage,  3U, 10U), CamPiece::WP);
    //    SetPieceIndex(Location(BoardPartID::Stage,  4U, 10U), CamPiece::WP);
    //    SetPieceIndex(Location(BoardPartID::Stage,  5U, 10U), CamPiece::WP);
    //    SetPieceIndex(Location(BoardPartID::Stage,  6U, 10U), CamPiece::WP);
    //    SetPieceIndex(Location(BoardPartID::Stage,  7U, 10U), CamPiece::WP);
    //    SetPieceIndex(Location(BoardPartID::Stage,  8U, 10U), CamPiece::WP);

    //    SetPieceIndex(Location(BoardPartID::Stage,  4U,  9U), CamPiece::WP);
    //    SetPieceIndex(Location(BoardPartID::Stage,  5U,  9U), CamPiece::WP);
    //    SetPieceIndex(Location(BoardPartID::Stage,  6U,  9U), CamPiece::WP);
    //    SetPieceIndex(Location(BoardPartID::Stage,  7U,  9U), CamPiece::WP);
    //                                               
    //    // Illegal tiles                           
    //    SetPieceIndex(Location(BoardPartID::Stage,  0U,  0U), Piece::NoTile);
    //    SetPieceIndex(Location(BoardPartID::Stage,  1U,  0U), Piece::NoTile);
    //    SetPieceIndex(Location(BoardPartID::Stage,  2U,  0U), Piece::NoTile);
    //    SetPieceIndex(Location(BoardPartID::Stage,  3U,  0U), Piece::NoTile);
    //    SetPieceIndex(Location(BoardPartID::Stage,  4U,  0U), Piece::NoTile);
    //    SetPieceIndex(Location(BoardPartID::Stage,  7U,  0U), Piece::NoTile);
    //    SetPieceIndex(Location(BoardPartID::Stage,  8U,  0U), Piece::NoTile);
    //    SetPieceIndex(Location(BoardPartID::Stage,  9U,  0U), Piece::NoTile);
    //    SetPieceIndex(Location(BoardPartID::Stage, 10U,  0U), Piece::NoTile);
    //    SetPieceIndex(Location(BoardPartID::Stage, 11U,  0U), Piece::NoTile);

    //    SetPieceIndex(Location(BoardPartID::Stage,  0U,  1U), Piece::NoTile);
    //    SetPieceIndex(Location(BoardPartID::Stage,  1U,  1U), Piece::NoTile);
    //    SetPieceIndex(Location(BoardPartID::Stage, 10U,  1U), Piece::NoTile);
    //    SetPieceIndex(Location(BoardPartID::Stage, 11U,  1U), Piece::NoTile);

    //    SetPieceIndex(Location(BoardPartID::Stage,  0U,  2U), Piece::NoTile);
    //    SetPieceIndex(Location(BoardPartID::Stage, 11U,  2U), Piece::NoTile);

    //    SetPieceIndex(Location(BoardPartID::Stage,  0U, 13U), Piece::NoTile);
    //    SetPieceIndex(Location(BoardPartID::Stage, 11U, 13U), Piece::NoTile);

    //    SetPieceIndex(Location(BoardPartID::Stage,  0U, 14U), Piece::NoTile);
    //    SetPieceIndex(Location(BoardPartID::Stage,  1U, 14U), Piece::NoTile);
    //    SetPieceIndex(Location(BoardPartID::Stage, 10U, 14U), Piece::NoTile);
    //    SetPieceIndex(Location(BoardPartID::Stage, 11U, 14U), Piece::NoTile);

    //    SetPieceIndex(Location(BoardPartID::Stage,  0U, 15U), Piece::NoTile);
    //    SetPieceIndex(Location(BoardPartID::Stage,  1U, 15U), Piece::NoTile);
    //    SetPieceIndex(Location(BoardPartID::Stage,  2U, 15U), Piece::NoTile);
    //    SetPieceIndex(Location(BoardPartID::Stage,  3U, 15U), Piece::NoTile);
    //    SetPieceIndex(Location(BoardPartID::Stage,  4U, 15U), Piece::NoTile);
    //    SetPieceIndex(Location(BoardPartID::Stage,  7U, 15U), Piece::NoTile);
    //    SetPieceIndex(Location(BoardPartID::Stage,  8U, 15U), Piece::NoTile);
    //    SetPieceIndex(Location(BoardPartID::Stage,  9U, 15U), Piece::NoTile);
    //    SetPieceIndex(Location(BoardPartID::Stage, 10U, 15U), Piece::NoTile);
    //    SetPieceIndex(Location(BoardPartID::Stage, 11U, 15U), Piece::NoTile);
    //    break;
    //  case 'M': // Mini Cam position
    //    // Black Knights
    //    SetPieceIndex(Location(BoardPartID::Stage, 2U, 3U), CamPiece::BN);
    //    SetPieceIndex(Location(BoardPartID::Stage, 4U, 3U), CamPiece::BN);

    //    // Black Pawns      
    //    SetPieceIndex(Location(BoardPartID::Stage, 1U, 4U), CamPiece::BP);
    //    SetPieceIndex(Location(BoardPartID::Stage, 2U, 4U), CamPiece::BP);
    //    SetPieceIndex(Location(BoardPartID::Stage, 3U, 4U), CamPiece::BP);
    //    SetPieceIndex(Location(BoardPartID::Stage, 4U, 4U), CamPiece::BP);
    //    SetPieceIndex(Location(BoardPartID::Stage, 5U, 4U), CamPiece::BP);

    //    // White Knights   
    //    SetPieceIndex(Location(BoardPartID::Stage, 2U, 9U), CamPiece::WN);
    //    SetPieceIndex(Location(BoardPartID::Stage, 4U, 9U), CamPiece::WN);

    //    // White Pawns     
    //    SetPieceIndex(Location(BoardPartID::Stage, 1U, 8U), CamPiece::WP);
    //    SetPieceIndex(Location(BoardPartID::Stage, 2U, 8U), CamPiece::WP);
    //    SetPieceIndex(Location(BoardPartID::Stage, 3U, 8U), CamPiece::WP);
    //    SetPieceIndex(Location(BoardPartID::Stage, 4U, 8U), CamPiece::WP);
    //    SetPieceIndex(Location(BoardPartID::Stage, 5U, 8U), CamPiece::WP);

    //    // Illegal tiles    
    //    SetPieceIndex(Location(BoardPartID::Stage, 0U, 0U), Piece::NoTile);
    //    SetPieceIndex(Location(BoardPartID::Stage, 1U, 0U), Piece::NoTile);
    //    SetPieceIndex(Location(BoardPartID::Stage, 2U, 0U), Piece::NoTile);
    //    SetPieceIndex(Location(BoardPartID::Stage, 4U, 0U), Piece::NoTile);
    //    SetPieceIndex(Location(BoardPartID::Stage, 5U, 0U), Piece::NoTile);
    //    SetPieceIndex(Location(BoardPartID::Stage, 6U, 0U), Piece::NoTile);

    //    SetPieceIndex(Location(BoardPartID::Stage, 0U, 1U), Piece::NoTile);
    //    SetPieceIndex(Location(BoardPartID::Stage, 1U, 1U), Piece::NoTile);
    //    SetPieceIndex(Location(BoardPartID::Stage, 5U, 1U), Piece::NoTile);
    //    SetPieceIndex(Location(BoardPartID::Stage, 6U, 1U), Piece::NoTile);

    //    SetPieceIndex(Location(BoardPartID::Stage, 0U, 2U), Piece::NoTile);
    //    SetPieceIndex(Location(BoardPartID::Stage, 6U, 2U), Piece::NoTile);

    //    SetPieceIndex(Location(BoardPartID::Stage, 0U, 10U), Piece::NoTile);
    //    SetPieceIndex(Location(BoardPartID::Stage, 6U, 10U), Piece::NoTile);

    //    SetPieceIndex(Location(BoardPartID::Stage, 0U, 11U), Piece::NoTile);
    //    SetPieceIndex(Location(BoardPartID::Stage, 1U, 11U), Piece::NoTile);
    //    SetPieceIndex(Location(BoardPartID::Stage, 5U, 11U), Piece::NoTile);
    //    SetPieceIndex(Location(BoardPartID::Stage, 6U, 11U), Piece::NoTile);

    //    SetPieceIndex(Location(BoardPartID::Stage, 0U, 12U), Piece::NoTile);
    //    SetPieceIndex(Location(BoardPartID::Stage, 1U, 12U), Piece::NoTile);
    //    SetPieceIndex(Location(BoardPartID::Stage, 2U, 12U), Piece::NoTile);
    //    SetPieceIndex(Location(BoardPartID::Stage, 4U, 12U), Piece::NoTile);
    //    SetPieceIndex(Location(BoardPartID::Stage, 5U, 12U), Piece::NoTile);
    //    SetPieceIndex(Location(BoardPartID::Stage, 6U, 12U), Piece::NoTile);
    //    break;

    //  default:
    //    assert(false); // illegal variant
    //}
  }


  //void CamBoard::GetAllMoves() const noexcept                            // collect all moves for all pieces
  //{
  //  //Board::GetAllMoves();
  //  JumpsOnly(movesW_);
  //  JumpsOnly(movesB_);
  //}

  //PositionValue CamBoard::EvaluateWin() const
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

  void CamGame::Register() noexcept
  {
    PMap.Register(CamPiece::WN);
    PMap.Register(CamPiece::BN);
    PMap.Register(CamPiece::WP);
    PMap.Register(CamPiece::BP);

    Variants.Register(Variant(0, IDR_GAMETYPE_CAMELOT, "Camelot", 'F', 12, 16 ));
    Variants.Register(Variant(0, IDR_GAMETYPE_CAMELOT, "Cam",     'M',  7, 13 ));
  }

  const BoardPartDimensions CamGame::GetDimensions(const VariantChosen& v) noexcept
  {
    BoardPartDimensions dF{  // Full Game ("Camelot")
       BoardPartDimension(12, 16, LayoutType::Alternating, BoardStartX, BoardStartY, FieldSizeX, FieldSizeY, 1, 1),
       BoardPartDimension(3, 2, LayoutType::Light, BoardStartX + FieldSizeX * (12 + 1), BoardStartY + FieldSizeY / 2 + FieldSizeY * (16 - 2), FieldSizeX, FieldSizeY),
       BoardPartDimension(24, 2, LayoutType::Small, FieldSizeX * (12 + 1), BoardStartY + FieldSizeSY, FieldSizeSX, FieldSizeSY, 0, FieldSizeY * 16 - FieldSizeSY * 4),
    };
    BoardPartDimensions dM{  // Mini ("Cam")
       BoardPartDimension(7, 13, LayoutType::Alternating, BoardStartX, BoardStartY, FieldSizeX, FieldSizeY, 1, 1),
       BoardPartDimension(3, 2, LayoutType::Light, BoardStartX + FieldSizeX * (7 + 1), BoardStartY + FieldSizeY / 2 + FieldSizeY * (13 - 2), FieldSizeX, FieldSizeY),
       BoardPartDimension(2 * 7, 2, LayoutType::Small, FieldSizeX * (7 + 1), BoardStartY + FieldSizeSY, FieldSizeSX, FieldSizeSY, 0, FieldSizeY * 13 - FieldSizeSY * 4),
    };
    return v.c == 'F' ? dF : dM;
  }

}
