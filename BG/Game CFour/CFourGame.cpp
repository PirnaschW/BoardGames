#include "pch.h"

#include "CFourGame.h"

namespace CFour
{
  inline const Checker Checker::TheChecker{};
  inline const CFourPiece CFourPiece::CFourPieceW{ Checker::TheChecker, Color::White, IDB_WCL, IDB_WCD, IDB_WCS };
  inline const CFourPiece CFourPiece::CFourPieceB{ Checker::TheChecker, Color::Black, IDB_BCL, IDB_BCD, IDB_BCS };

  bool CFourPosition::AddIfLegal(Moves& m, const Location& fr, const Location& to) const noexcept
  {
    const Piece& pf = GetPiece(fr);                                       // piece to move
    if (pf == Piece::NoTile) return false;                               // out of board
    if (pf.IsBlank()) return false;                                      // tile not occupied

    const Piece& pt = GetPiece(to);                                       // piece on target field
    if (pt == Piece::NoTile) return false;                               // out of board

    Actions a{};
    a.push_back(std::make_shared<ActionLift>(fr, pf));                    // pick piece up
    if (!pt.IsBlank())
    {
      a.push_back(std::make_shared<ActionLift>(to, pt));                    // pick opponent piece up
      a.push_back(std::make_shared<ActionDrop>(GetNextTakenL(pf.GetColor()), pt));                   // place it in Taken
    }
    a.push_back(std::make_shared<ActionDrop>(to, pf));                   // and place it on target
    m.push_back(std::make_shared<Move>(a));                               // add move to move list
    return false;
  };


  void CFourPosition::GetAllMoves(void) const noexcept // collect all moves
  {
    assert(movesW_.empty());
    assert(movesB_.empty());

    movesW_.reserve(7);
    movesB_.reserve(7);

    const Location w{ BoardPart::Stock, 0, 0 };
    const Location b{ BoardPart::Stock, 0, 1 };

    for (Coordinate i = 0; i < sizeX_; ++i)
    {
      for (Coordinate j = 0, z = sizeY_; j < sizeY_; ++j)
      {
        --z;
        const Location l{ BoardPart::Main, i, z };
        const Piece& p = GetPiece(l);
        if (p.IsBlank())  // skip filled fields as well as nonexisting tiles
        {
          Actions aw{};
          aw.push_back(std::make_shared<ActionLift>(w, CFourPiece::CFourPieceW));                  // pick new piece up
          aw.push_back(std::make_shared<ActionDrop>(l, CFourPiece::CFourPieceW));                  // and place it on target
          movesW_.push_back(std::make_shared<Move>(aw));                                           // add move to move list

          Actions ab{};
          ab.push_back(std::make_shared<ActionLift>(b, CFourPiece::CFourPieceB));                  // pick new piece up
          ab.push_back(std::make_shared<ActionDrop>(l, CFourPiece::CFourPieceB));                  // and place it on target
          movesB_.push_back(std::make_shared<Move>(ab));                                           // add move to move list
          break;                                                                                   // done with this row
        }
      }
    }
  }

  PositionValue CFourPosition::EvaluateStatically(void) const noexcept   // as seen from White
  {
    assert(movesW_.empty());
    assert(movesB_.empty());

    GetAllMoves();                                                                                 // fill the move lists
    depth_ = 1;

    if (onTurn_ == &Color::White && movesW_.empty()) return PositionValue::PValueType::Lost;     // if no more moves, game over
    if (onTurn_ == &Color::Black && movesB_.empty()) return PositionValue::PValueType::Won;
    return EvaluateChainLengths(4);
  }

  unsigned int CFourPosition::GetChainValue(unsigned int z) const noexcept
  {
    switch (z)
    {
      case 1:  return   100;
      case 2:  return   800;
      case 3:  return  9000;
      default: return  0;
    }
  }



  const VariantList& CFourGame::GetVariants(void) noexcept
  {
    static VariantList v{ { Variant{ 7, 6, "Connect Four", 4, 20 } } };
    return v;
  }

  const PieceMapP& CFourGame::GetPieces(void) noexcept
  {
    static const PieceMapP& p = std::make_shared<PieceMap>();
    p->Empty();
    p->Add(CFourPiece::CFourPieceW);
    p->Add(CFourPiece::CFourPieceB);
    return p;
  }

  const Dimensions& CFourGame::GetDimensions(Coordinate x, Coordinate y) noexcept
  {
    static Dimensions d{
       Dimension(x, y, BoardStartX, BoardStartY, FieldSizeX, FieldSizeY, 1, 1),
       Dimension(2, 2, BoardStartX + FieldSizeX * (x + 1), BoardStartY + y * FieldSizeY + FieldSizeY / 2, FieldSizeX, FieldSizeY),
       Dimension(0, 0, BoardStartX + FieldSizeX * (x + 1), BoardStartY + FieldSizeSY, FieldSizeSX, FieldSizeSY, 0, FieldSizeY * x - FieldSizeSY * 4),
    };
    return d;
  }
}
