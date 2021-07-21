#include "Games.h"

#include "AtaxxGame.h"

namespace Ataxx
{
  const Checker Checker::TheChecker{};
  const AtaxxPiece AtaxxPiece::AtaxxPieceW{ Checker::TheChecker, PieceColor::White, IDB_WCL };
  const AtaxxPiece AtaxxPiece::AtaxxPieceB{ Checker::TheChecker, PieceColor::Black, IDB_BCL };

  void Checker::CollectMoves(const MainPosition& p, const Location& l, Moves& m) const noexcept
  {
    const Location w{ BoardPart::Stock, 0, 0 };
    const Location b{ BoardPart::Stock, 0, 1 };

    const AtaxxPosition& pos = dynamic_cast<const AtaxxPosition&>(p);     // position must be an Ataxx position
    const Piece& p0 = pos.GetPiece(l);                                    // piece that is moving
    const Location ls{ p0.IsColor(PieceColor::White) ? w : b };

    for (const auto& d : Offset::QDirection)                              // try slides all eight directions
    {
      const Location to{ l + d };                                         // location to move to
      const Piece& p1 = pos.GetPiece(to);                                 // what is on the target location?
      if (p1 == Piece::NoTile) continue;                                  // out of board
      if (!p1.IsBlank()) continue;                                        // tile occupied
                                                                        
      Actions a{};                                                      
      a.push_back(std::make_shared<ActionLift>(l, p0));                   // pick piece up
      a.push_back(std::make_shared<ActionLift>(ls, p0));                  // get new piece from Stack
      a.push_back(std::make_shared<ActionDrop>(l, p0));                   // and refill source location
      for (const auto& df : Offset::QDirection)                           // try flips in all eight directions
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
    for (const auto& d : Offset::QDirection)                              // try jumps in all eight directions
    {                                                                   
      const Location to{ l + d * 2 };                                     // location to jump to
      const Piece& p1 = pos.GetPiece(to);                                 // what is on the target location?
      if (p1 == Piece::NoTile) continue;                                  // out of board
      if (!p1.IsBlank()) continue;                                        // tile occupied
                                                                        
      Actions a{};                                                      
      a.push_back(std::make_shared<ActionLift>(l, p0));                   // pick piece up (don't refill start position)
      for (const auto& df : Offset::QDirection)                           // try flips in all eight directions
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
    if (pos.v_.c & Ataxx::KnightJumps)
    {
      for (const auto& d : Offset::NJumps)                                // try knight jumps in all eight directions
      {
        const Location to{ l + d };                                       // location to jump to
        const Piece& p1 = pos.GetPiece(to);                               // what is on the target location?
        if (p1 == Piece::NoTile) continue;                                // out of board
        if (!p1.IsBlank()) continue;                                      // tile occupied

        Actions a{};
        a.push_back(std::make_shared<ActionLift>(l, p0));                 // pick piece up (don't refill start position)
        for (const auto& df : Offset::QDirection)                         // try flips in all eight directions
        {
          const Location lf{ to + df };                                   // location to check for flipping
          const Piece& pf = pos.GetPiece(lf);                             // what is on the flip location?
          if (pf == Piece::NoTile) continue;                              // out of board
          if (pf.IsBlank()) continue;                                     // tile empty
          if (pf.IsColor(p0.GetColor())) continue;                        // own piece

          // found piece to flip                                        
          a.push_back(std::make_shared<ActionLift>(lf, pf));              // pick up flipped piece
          a.push_back(std::make_shared<ActionLift>(ls, p0));              // get new piece from Stack
          a.push_back(std::make_shared<ActionDrop>(lf, p0));              // put on flip location
        }
        a.push_back(std::make_shared<ActionDrop>(to, p0));                // and place it on target
        m.push_back(std::make_shared<Move>(a));                           // add move to move list
      }
    }
  }


  AtaxxPosition::AtaxxPosition(const VariantChosen& v, const PieceMapP& p, const Dimensions& d) noexcept : MainPosition(v, p, d)
  {
    // put one piece in each corner:
    SetPiece(Location(BoardPart::Main, 0U,             d[0].yCount_-1), AtaxxPiece::AtaxxPieceW);
    SetPiece(Location(BoardPart::Main, d[0].xCount_-1, 0U            ), AtaxxPiece::AtaxxPieceW);
    SetPiece(Location(BoardPart::Main, 0U,             0U            ), AtaxxPiece::AtaxxPieceB);
    SetPiece(Location(BoardPart::Main, d[0].xCount_-1, d[0].yCount_-1), AtaxxPiece::AtaxxPieceB);
  }

  const VariantList& AtaxxGame::GetVariants(void) noexcept
  {
    static const VariantList v{
      { Variant{ "Ataxx",        Ataxx::Ataxx,        8, 8, 2, 20 } },
      { Variant{ "Assimilation", Ataxx::Assimilation, 8, 8, 2, 20 } },
    };
    return v;
  }

  const PieceMapP& AtaxxGame::GetPieces(const VariantChosen& v) noexcept
  {
    static const PieceMapP& p = std::make_shared<PieceMap>();
    p->Empty();  // the map is reused, but not its content - need to rebuild it every time
    p->Add(AtaxxPiece::AtaxxPieceW);
    p->Add(AtaxxPiece::AtaxxPieceB);
    return p;
  }

  const Dimensions AtaxxGame::GetDimensions(const VariantChosen& v) noexcept
  {
    Dimensions d{
       Dimension(v.x, v.y, BoardStartX, BoardStartY, FieldSizeX, FieldSizeY, 1, 1),
       Dimension(2, 2, BoardStartX + FieldSizeX * (v.x + 1), BoardStartY + v.y * FieldSizeY + FieldSizeY / 2, FieldSizeX, FieldSizeY),
       Dimension(0, 0, BoardStartX + FieldSizeX * (v.x + 1), BoardStartY + FieldSizeSY, FieldSizeSX, FieldSizeSY, 0, FieldSizeY * v.x - FieldSizeSY * 4),
    };
    return d;
  }
}
