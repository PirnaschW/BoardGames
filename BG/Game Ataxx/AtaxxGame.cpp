#include "pch.h"

#include "AtaxxGame.h"

namespace Ataxx
{
  inline const Checker Checker::TheChecker{};
  inline const AtaxxPiece AtaxxPiece::AtaxxPieceW{ Checker::TheChecker, Color::White, IDB_WCL, IDB_WCD, IDB_WCS };
  inline const AtaxxPiece AtaxxPiece::AtaxxPieceB{ Checker::TheChecker, Color::Black, IDB_BCL, IDB_BCD, IDB_BCS };


  void Checker::CollectMoves(const MainPosition& p, const Location& l, Moves& m) const noexcept
  {
    const Location w{ BoardPart::Stock, 0, 0 };
    const Location b{ BoardPart::Stock, 0, 1 };

    const AtaxxPosition& pos = dynamic_cast<const AtaxxPosition&>(p);     // position must be a Cam position
    const Piece& p0 = pos.GetPiece(l);                                    // piece that is moving
    const Location ls{ p0.IsColor(Color::White) ? w : b };

    for (const auto& d : Offset::Qdirection)                              // try slides all eight directions
    {
      const Location to{ l + d };                                         // location to move to
      const Piece& p1 = pos.GetPiece(to);                                 // what is on the target location?
      if (p1 == Piece::NoTile) continue;                                  // out of board
      if (!p1.IsBlank()) continue;                                        // tile occupied
                                                                        
      Actions a{};                                                      
      a.push_back(std::make_shared<ActionLift>(l, p0));                   // pick piece up
      a.push_back(std::make_shared<ActionLift>(ls, p0));                  // get new piece from Stack
      a.push_back(std::make_shared<ActionDrop>(l, p0));                   // and refill source location
      for (const auto& df : Offset::Qdirection)                           // try flips in all eight directions
      {                                                                 
        const Location lf{ to + df };                                     // location to check for flipping
        const Piece& pf = pos.GetPiece(lf);                               // what is on the flip location?
        if (pf == Piece::NoTile) continue;                                // out of board
        if (pf.IsBlank()) continue;                                       // tile empty
        if (pf.IsColor(p0.GetColor())) continue;                          // own piece
                                                                        
        // found piece to flip                                          
        a.push_back(std::make_shared<ActionLift>(lf, pf));                // pick up flipped piece
        a.push_back(std::make_shared<ActionLift>(ls, p0));                // get new piece from Stack
        a.push_back(std::make_shared<ActionDrop>(lf, p0));                // put on flip location
      }                                                                 
      a.push_back(std::make_shared<ActionDrop>(to, p0));                  // and place it on target
      m.push_back(std::make_shared<Move>(a));                             // add move to move list
    }                                                                   
    for (const auto& d : Offset::Qdirection)                              // try jumps in all eight directions
    {                                                                   
      const Location to{ l + d * 2 };                                     // location to jump to
      const Piece& p1 = pos.GetPiece(to);                                 // what is on the target location?
      if (p1 == Piece::NoTile) continue;                                  // out of board
      if (!p1.IsBlank()) continue;                                        // tile occupied
                                                                        
      Actions a{};                                                      
      a.push_back(std::make_shared<ActionLift>(l, p0));                   // get new piece from Stack
      for (const auto& df : Offset::Qdirection)                           // try flips in all eight directions
      {                                                                 
        const Location lf{ to + df };                                     // location to check for flipping
        const Piece& pf = pos.GetPiece(lf);                               // what is on the flip location?
        if (pf == Piece::NoTile) continue;                                // out of board
        if (pf.IsBlank()) continue;                                       // tile empty
        if (pf.IsColor(p0.GetColor())) continue;                          // own piece
                                                                        
        // found piece to flip                                          
        a.push_back(std::make_shared<ActionLift>(lf, pf));                // pick up flipped piece
        a.push_back(std::make_shared<ActionLift>(ls, p0));                // get new piece from Stack
        a.push_back(std::make_shared<ActionDrop>(lf, p0));                // put on flip location
      }                                                                 
      a.push_back(std::make_shared<ActionDrop>(to, p0));                  // and place it on target
      m.push_back(std::make_shared<Move>(a));                             // add move to move list

    }
  }


  AtaxxPosition::AtaxxPosition(const PieceMapP& p, const Dimensions& d) noexcept : MainPosition(p, d)
  {
    SetPiece(Location(BoardPart::Main, 0U, 7U), AtaxxPiece::AtaxxPieceW);
    SetPiece(Location(BoardPart::Main, 7U, 0U), AtaxxPiece::AtaxxPieceW);
    SetPiece(Location(BoardPart::Main, 0U, 0U), AtaxxPiece::AtaxxPieceB);
    SetPiece(Location(BoardPart::Main, 7U, 7U), AtaxxPiece::AtaxxPieceB);
  }

  const VariantList& AtaxxGame::GetVariants(void) noexcept
  {
    static VariantList v{ { Variant{ 8, 8, "Ataxx",        2, 20 } },
                          { Variant{ 8, 8, "Assimilation", 2, 20 } } };
    return v;
  }

  const PieceMapP& AtaxxGame::GetPieces(void) noexcept
  {
    static const PieceMapP& p = std::make_shared<PieceMap>();
    p->Add(AtaxxPiece::AtaxxPieceW);
    p->Add(AtaxxPiece::AtaxxPieceB);
    return p;
  }

  const Dimensions& AtaxxGame::GetDimensions(Coordinate x, Coordinate y) noexcept
  {
    static Dimensions d{
       Dimension(x, y, BoardStartX, BoardStartY, FieldSizeX, FieldSizeY, 1, 1),
       Dimension(2, 2, BoardStartX + FieldSizeX * (x + 1), BoardStartY + y * FieldSizeY + FieldSizeY / 2, FieldSizeX, FieldSizeY),
       Dimension(0, 0, BoardStartX + FieldSizeX * (x + 1), BoardStartY + FieldSizeSY, FieldSizeSX, FieldSizeSY, 0, FieldSizeY * x - FieldSizeSY * 4),
    };
    return d;
  }
}
