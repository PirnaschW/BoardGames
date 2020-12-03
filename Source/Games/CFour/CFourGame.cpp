#include "Games.h"

#include "CFourGame.h"

namespace CFour
{

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

    // collect all potential moves in an unordered set, to remove duplicates (spider might produce multiples)
    auto hash = [](const Location& l) { return (l.x_<<8)+l.y_; };
    std::unordered_set<Location,decltype(hash)> m{};
    for (Coordinate i = 0; i < sizeX_; ++i)                                                        // vertical
    {
      Coordinate z = sizeY_ - 1;
      for (Coordinate j = 0; j < sizeY_; ++j,--z)                                                  //   bottom -> top
      {
        const Location l{ BoardPart::Main, i, z };
        const Piece& p = GetPiece(l);
        if (p.IsBlank())  // skip filled fields as well as nonexisting tiles
        {
          m.insert(l);
          break;
        }
      }
      switch (vCode_)
      {
        case 'S': // fall through intended
        case 'X':
          for (Coordinate j = 0; j < sizeY_; ++j)                                                      //   top -> bottom
          {
            const Location l{ BoardPart::Main, i, j };
            const Piece& p = GetPiece(l);
            if (p.IsBlank())  // skip filled fields as well as nonexisting tiles
            {
              m.insert(l);
              break;
            }
          }
        default: break;
      }
    }
    switch (vCode_)
    {
      case 'S': // fall through intended
      case 'X':
        for (Coordinate j = 0; j < sizeY_; ++j)                                                        // horizontal
        {
          for (Coordinate i = 0; i < sizeX_; ++i)                                                      //   left -> right
          {
            const Location l{ BoardPart::Main, i, j };
            const Piece& p = GetPiece(l);
            if (p.IsBlank())  // skip filled fields as well as nonexisting tiles
            {
              m.insert(l);
              break;
            }
          }
          Coordinate z = sizeX_ - 1;
          for (Coordinate i = 0; i < sizeX_; ++i, --z)                                                  //   right -> left
          {
            const Location l{ BoardPart::Main, z, j };
            const Piece& p = GetPiece(l);
            if (p.IsBlank())  // skip filled fields as well as nonexisting tiles
            {
              m.insert(l);
              break;
            }
          }
        }
      default: break;
    }

    movesW_.reserve(m.size());
    movesB_.reserve(m.size());

    const Location w{ BoardPart::Stock, 0, 0 };
    const Location b{ BoardPart::Stock, 0, 1 };

    for (const Location& l : m)
    {
      Actions aw{};
      aw.push_back(std::make_shared<ActionLift>(w, CorePiece::WC));                                // pick new piece up
      aw.push_back(std::make_shared<ActionDrop>(l, CorePiece::WC));                                // and place it on target
      movesW_.push_back(std::make_shared<Move>(aw));                                               // add move to move list
                                                                                                            
      Actions ab{};                                                                                         
      ab.push_back(std::make_shared<ActionLift>(b, CorePiece::BC));                                // pick new piece up
      ab.push_back(std::make_shared<ActionDrop>(l, CorePiece::BC));                                // and place it on target
      movesB_.push_back(std::make_shared<Move>(ab));                                               // add move to move list
    }
  }

  PositionValue CFourPosition::EvaluateStatically(void) const noexcept   // as seen from White
  {
    assert(movesW_.empty());
    assert(movesB_.empty());

    GetAllMoves();                                                                                 // fill the move lists
    depth_ = 1;

    if (onTurn_ == &PieceColor::White && movesW_.empty()) return PositionValue::PValueType::Lost;     // if no more moves, game over
    if (onTurn_ == &PieceColor::Black && movesB_.empty()) return PositionValue::PValueType::Won;
    return EvaluateChainLengths(4);
  }

  unsigned int CFourPosition::GetChainValue(unsigned int z) const noexcept
  {
    switch (z)
    {
      case 0:  return   100;
      case 1:  return   100;
      case 2:  return   800;
      case 3:  return  9000;
      default: return  0;
    }
  }


  const VariantList& CFourGame::GetVariants(void) noexcept
  {
    static VariantList v{
      { Variant{ "Classic Connect Four", 'C', 7, 6, 4, 20 } },
      { Variant{ "Linetris",             'L', 8, 8, 4, 20 } },
      { Variant{ "Spider Line Four",     'S', 8, 8, 4, 20 } },
      { Variant{ "Spider LineTris",      'X', 8, 8, 4, 20 } },
    };
    return v;
  }

  const PieceMapP& CFourGame::GetPieces(VariantCode c) noexcept
  {
    static const PieceMapP& p = std::make_shared<PieceMap>();
    p->Empty();
    p->Add(CorePiece::WC);
    p->Add(CorePiece::BC);
    return p;
  }

  const Dimensions& CFourGame::GetDimensions(const VariantChosen& v) noexcept
  {
    static Dimensions d{
       Dimension(v.x, v.y, BoardStartX, BoardStartY, FieldSizeX, FieldSizeY, 1, 1),
       Dimension(2, 2, BoardStartX + FieldSizeX * (v.x + 1), BoardStartY + v.y * FieldSizeY + FieldSizeY / 2, FieldSizeX, FieldSizeY),
       Dimension(0, 0, BoardStartX + FieldSizeX * (v.x + 1), BoardStartY + FieldSizeSY, FieldSizeSX, FieldSizeSY, 0, FieldSizeY * v.x - FieldSizeSY * 4),
    };
    return d;
  }
}
