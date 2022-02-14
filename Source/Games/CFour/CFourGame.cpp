#include "Games.h"

#include "CFourGame.h"

namespace CFour
{

  // TODO: handle LineTris functionality (disappearing complete lines)
  void CFourPosition::GetAllMoves(void) const noexcept // collect all moves
  {
    assert(movesW_.empty());
    assert(movesB_.empty());

    // collect all potential moves in an unordered set, to discard duplicates ('spider' variants might produce duplicates)
    const auto hash = [](const Location& l) { return (l.x_<<8)+l.y_; };  // simple hash to differentiate the locations
    std::unordered_set<Location,decltype(hash)> m{};

    for (Coordinate i = 0; i < sizeX_; ++i)                               // vertical ('for all columns')
    {
      for (Coordinate z = sizeY_ - 1, j = 0; j < sizeY_; ++j,--z)         //   bottom -> top
      {
        const Location l{ BoardPart::Main, i, z };
        const Piece& p = GetPiece(l);
        if (p.IsBlank())  // skip filled fields as well as nonexisting tiles
        {
          m.insert(l);
          break;
        }
      }
      if (v_.c && Spider) // non-spider versions don't allow placement on other sides
      {
        for (Coordinate j = 0; j < sizeY_; ++j)                           //   top -> bottom
        {
          const Location l{ BoardPart::Main, i, j };
          const Piece& p = GetPiece(l);
          if (p.IsBlank())  // skip filled fields as well as nonexisting tiles
          {
            m.insert(l);
            break;
          }
        }
      }
    }
    if (v_.c && Spider) // non-spider versions don't allow placement on other sides
    {
      for (Coordinate j = 0; j < sizeY_; ++j)                             // horizontal ('for all rows')
      {
        for (Coordinate i = 0; i < sizeX_; ++i)                           //   left -> right
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
        for (Coordinate i = 0; i < sizeX_; ++i, --z)                      //   right -> left
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
    }

    movesW_.reserve(m.size());
    movesB_.reserve(m.size());

    const Location w{ BoardPart::Stock, 0, 0 };
    const Location b{ BoardPart::Stock, 0, 1 };

    for (const Location& l : m)
    {
      Actions aw{};
      aw.push_back(std::make_shared<ActionLift>(w, CorePiece::WC));       // pick new piece up
      aw.push_back(std::make_shared<ActionDrop>(l, CorePiece::WC));       // and place it on target
      movesW_.push_back(std::make_shared<Move>(aw));                      // add move to move list
                                                                                   
      Actions ab{};                                                                
      ab.push_back(std::make_shared<ActionLift>(b, CorePiece::BC));       // pick new piece up
      ab.push_back(std::make_shared<ActionDrop>(l, CorePiece::BC));       // and place it on target
      movesB_.push_back(std::make_shared<Move>(ab));                      // add move to move list
    }
  }

  PositionValue CFourPosition::EvaluateStatically(void) const noexcept    // as seen from White
  {
    assert(movesW_.empty());
    assert(movesB_.empty());

    GetAllMoves();                                                        // fill the move lists
    depth_ = 1;

    if (movesW_.empty()) return PositionValue::PValueType::Tie;           // if no more moves, game over.
                                                                          // Note that B has the same moves as W; enough to check W
    return v_.c & Anti ? -EvaluateChainLengths(4) : +EvaluateChainLengths(4);
  }

  const VariantList& CFourGame::GetVariants(void) noexcept
  {
    static const VariantList v{
      { Variant{ "Classic Connect Four", CFour::Classic,        7, 6, 4, 20 } },
      { Variant{ "Linetris",             CFour::Linetris,       8, 8, 4, 20 } },
      { Variant{ "Spider Line Four",     CFour::Spider,         8, 8, 4, 20 } },
      { Variant{ "Spider LineTris",      CFour::SpiderLinetris, 8, 8, 4, 20 } },
      { Variant{ "Anti Line Four",       CFour::Anti,           8, 8, 4, 20 } },
    };
    return v;
  }

  const PieceMapP& CFourGame::GetPieces(const VariantChosen& v) noexcept
  {
    static const PieceMapP& p = std::make_shared<PieceMap>();
    p->Empty();   // the map is reused, but not its content - need to rebuild it every time
    p->Add(CorePiece::WC);
    p->Add(CorePiece::BC);
    return p;
  }

  const Dimensions CFourGame::GetDimensions(const VariantChosen& v) noexcept
  {
    Dimensions d{
       Dimension(v.x, v.y, BoardStartX, BoardStartY, FieldSizeX, FieldSizeY, 1, 1),
       Dimension(2, 2, BoardStartX + FieldSizeX * (v.x + 1), BoardStartY + v.y * FieldSizeY + FieldSizeY / 2, FieldSizeX, FieldSizeY),
       Dimension(0, 0, BoardStartX + FieldSizeX * (v.x + 1), BoardStartY + FieldSizeSY, FieldSizeSX, FieldSizeSY, 0, FieldSizeY * v.x - FieldSizeSY * 4),
    };
    return d;
  }
}
