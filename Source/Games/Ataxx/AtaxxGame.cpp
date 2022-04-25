#include "Games.h"

#include "AtaxxGame.h"

namespace Ataxx
{
  const Checker Checker::TheChecker{};
  const AtaxxPiece AtaxxPiece::AtaxxPieceW{ Checker::TheChecker, PieceColor::White, IDB_W0L };
  const AtaxxPiece AtaxxPiece::AtaxxPieceB{ Checker::TheChecker, PieceColor::Black, IDB_B0L };

  void Checker::CollectMoves(const Board& p, const Location& l, Moves& m) const noexcept
  {
    const Location w{ BoardPartID::Stock, 0, 0 };
    const Location b{ BoardPartID::Stock, 0, 1 };

    const AtaxxBoard& board_ = down_cast<const AtaxxBoard&>(p);           // position must be an Ataxx position
    PieceIndex pI0 = board_.GetPieceIndex(l.x_,l.y_);                       // piece that is moving
    //const Piece& p0 = board_.pMap_.GetPiece(pI0);
    //const Location ls{ p0.IsColor(PieceColor::White) ? w : b };

    //for (const auto& d : Offset::QDirection)                              // try slides all eight directions
    //{
    //  const Location to{ l + d };                                         // location to move to
    //  const Piece& p1 = board_.GetPieceIndex(to.x_,to.y_);                     // what is on the target location?
    //  if (p1 == Piece::NoTile) continue;                                  // out of board
    //  if (!p1.IsBlank()) continue;                                        // tile occupied
    //                                                                    
    //  Actions a{};                                                      
    //  a.push_back(std::make_shared<ActionLift>(l, p0));                   // pick piece up
    //  a.push_back(std::make_shared<ActionLift>(ls, p0));                  // get new piece from Stack
    //  a.push_back(std::make_shared<ActionDrop>(l, p0));                   // and refill source location
    //  for (const auto& df : Offset::QDirection)                           // try flips in all eight directions
    //  {                                                                 
    //    const Location lf{ to + df };                                     // location to check for flipping
    //    const Piece& pf = board_.GetPieceIndex(lf.x_, lf.y_);                  // what is on the flip location?
    //    if (pf == Piece::NoTile) continue;                                // out of board
    //    if (pf.IsBlank()) continue;                                       // tile empty
    //    if (pf.IsColor(p0)) continue;                                     // own piece
    //                                                                    
    //    // found piece to flip                                          
    //    a.push_back(std::make_shared<ActionLift>(lf, pf));                // pick up flipped piece
    //    a.push_back(std::make_shared<ActionLift>(ls, p0));                // get new piece from Stack
    //    a.push_back(std::make_shared<ActionDrop>(lf, p0));                // put on flip location
    //  }                                                                 
    //  a.push_back(std::make_shared<ActionDrop>(to, p0));                  // and place it on target
    //  m.push_back(std::make_shared<Move>(a));                             // add move to move list
    //}                                                                   
    //for (const auto& d : Offset::QDirection)                              // try jumps in all eight directions
    //{                                                                   
    //  const Location to{ l + d * 2 };                                     // location to jump to
    //  const Piece& p1 = board_.GetPieceIndex(to.x_, to.y_);                    // what is on the target location?
    //  if (p1 == Piece::NoTile) continue;                                  // out of board
    //  if (!p1.IsBlank()) continue;                                        // tile occupied
    //                                                                    
    //  Actions a{};                                                      
    //  a.push_back(std::make_shared<ActionLift>(l, p0));                   // pick piece up (don't refill start position)
    //  for (const auto& df : Offset::QDirection)                           // try flips in all eight directions
    //  {                                                                 
    //    const Location lf{ to + df };                                     // location to check for flipping
    //    const Piece& pf = board_.GetPieceIndex(lf.x_, lf.y_);                  // what is on the flip location?
    //    if (pf == Piece::NoTile) continue;                                // out of board
    //    if (pf.IsBlank()) continue;                                       // tile empty
    //    if (pf.IsColor(p0)) continue;                                     // own piece
    //                                                                    
    //    // found piece to flip                                          
    //    a.push_back(std::make_shared<ActionLift>(lf, pf));                // pick up flipped piece
    //    a.push_back(std::make_shared<ActionLift>(ls, p0));                // get new piece from Stack
    //    a.push_back(std::make_shared<ActionDrop>(lf, p0));                // put on flip location
    //  }                                                                 
    //  a.push_back(std::make_shared<ActionDrop>(to, p0));                  // and place it on target
    //  m.push_back(std::make_shared<Move>(a));                             // add move to move list
    //}
    //if (board_.v_.c & Ataxx::KnightJumps)
    //{
    //  for (const auto& d : Offset::NJumps)                                // try knight jumps in all eight directions
    //  {
    //    const Location to{ l + d };                                       // location to jump to
    //    const Piece& p1 = board_.GetPieceIndex(to.x_,to.y_);                   // what is on the target location?
    //    if (p1 == Piece::NoTile) continue;                                // out of board
    //    if (!p1.IsBlank()) continue;                                      // tile occupied

    //    Actions a{};
    //    a.push_back(std::make_shared<ActionLift>(l, p0));                 // pick piece up (don't refill start position)
    //    for (const auto& df : Offset::QDirection)                         // try flips in all eight directions
    //    {
    //      const Location lf{ to + df };                                   // location to check for flipping
    //      const Piece& pf = board_.GetPieceIndex(lf.x_,lf.y_);                 // what is on the flip location?
    //      if (pf == Piece::NoTile) continue;                              // out of board
    //      if (pf.IsBlank()) continue;                                     // tile empty
    //      if (pf.IsColor(p0)) continue;                                   // own piece

    //      // found piece to flip                                        
    //      a.push_back(std::make_shared<ActionLift>(lf, pf));              // pick up flipped piece
    //      a.push_back(std::make_shared<ActionLift>(ls, p0));              // get new piece from Stack
    //      a.push_back(std::make_shared<ActionDrop>(lf, p0));              // put on flip location
    //    }
    //    a.push_back(std::make_shared<ActionDrop>(to, p0));                // and place it on target
    //    m.push_back(std::make_shared<Move>(a));                           // add move to move list
    //  }
    //}
  }


  void AtaxxBoard::SetStartingBoard() noexcept
  {
    // put one piece in each corner:
    //SetPieceIndex(AtaxxPiece::AtaxxPieceW,  0, -1);
    //SetPieceIndex(AtaxxPiece::AtaxxPieceW, -1,  0);
    //SetPieceIndex(AtaxxPiece::AtaxxPieceB,  0,  0);
    //SetPieceIndex(AtaxxPiece::AtaxxPieceB, -1, -1);
  }

  void AtaxxGame::Register() noexcept
  { 
    PMap.Register(AtaxxPiece::AtaxxPieceW);
    PMap.Register(AtaxxPiece::AtaxxPieceB);

    Variants.Register(Variant(0, IDR_GAMETYPE_ATAXX, "Ataxx",        Ataxx::Ataxx,        8, 8, 2, 20));
    Variants.Register(Variant(0, IDR_GAMETYPE_ATAXX, "Assimilation", Ataxx::Assimilation, 8, 8, 2, 20));
  }

  const BoardPartDimensions AtaxxGame::GetDimensions(const VariantChosen& v) noexcept
  {
    BoardPartDimensions d{
       BoardPartDimension(v.x, v.y, LayoutType::Alternating, BoardStartX, BoardStartY, FieldSizeX, FieldSizeY, 1, 1),
       BoardPartDimension(2, 2, LayoutType::Light, BoardStartX + FieldSizeX * (v.x + 1), BoardStartY + v.y * FieldSizeY + FieldSizeY / 2, FieldSizeX, FieldSizeY),
       BoardPartDimension(0, 0, LayoutType::Small, BoardStartX + FieldSizeX * (v.x + 1), BoardStartY + FieldSizeSY, FieldSizeSX, FieldSizeSY, 0, FieldSizeY * v.x - FieldSizeSY * 4),
    };
    return d;
  }
}
