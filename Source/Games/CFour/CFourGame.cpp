#include "Games.h"

#include "CFourGame.h"

namespace CFour
{

  CFourBoard::CFourBoard(const VariantChosen& v, const BoardPartDimensions& d) noexcept : Board(v, d)
  {
    // fill Stock
    int i = 0;

    auto append = [this, &i](const Piece& p) { stock_.SetPieceIndex(PMap[p], i / 2U, i % 2U); ++i; };

    switch (v.c)
    {
      case CFour::Classic:
      case CFour::Linetris:
      case CFour::Spider:
      case CFour::SpiderLinetris:
      case CFour::Anti:
        append(CorePiece::WC);
        append(CorePiece::BC);
        break;
      default: assert(false);
    }
  }

  // TODO: refactor ConnectFour completely so it works again
  // TODO: handle LineTris functionality (disappearing complete lines)
  void CFourBoard::GetAllMoves() const noexcept // collect all moves
  {
    //assert(movesW_.empty());
    //assert(movesB_.empty());

    //// collect all potential moves in an unordered set, to discard duplicates ('spider' variants might produce duplicates)
    //const auto hash = [](const Location& l) { return (l.x_<<8)+l.y_; };  // simple hash to differentiate the locations
    //std::unordered_set<Location,decltype(hash)> m{};

    //for (Coordinate i = 0; i < sizeX_; ++i)                               // vertical ('for all columns')
    //{
    //  for (Coordinate z = sizeY_ - 1, j = 0; j < sizeY_; ++j,--z)         //   bottom -> top
    //  {
    //    const Location l{ BoardPartID::Stage, i, z };
    //    const Piece& p = GetPieceIndex(l);
    //    if (p.IsBlank())  // skip filled fields as well as nonexisting tiles
    //    {
    //      m.insert(l);
    //      break;
    //    }
    //  }
    //  if (v_.c && Spider) // non-spider versions don't allow placement on other sides
    //  {
    //    for (Coordinate j = 0; j < sizeY_; ++j)                           //   top -> bottom
    //    {
    //      const Location l{ BoardPartID::Stage, i, j };
    //      const Piece& p = GetPieceIndex(l);
    //      if (p.IsBlank())  // skip filled fields as well as nonexisting tiles
    //      {
    //        m.insert(l);
    //        break;
    //      }
    //    }
    //  }
    //}
    //if (v_.c && Spider) // non-spider versions don't allow placement on other sides
    //{
    //  for (Coordinate j = 0; j < sizeY_; ++j)                             // horizontal ('for all rows')
    //  {
    //    for (Coordinate i = 0; i < sizeX_; ++i)                           //   left -> right
    //    {
    //      const Location l{ BoardPartID::Stage, i, j };
    //      const Piece& p = GetPieceIndex(l);
    //      if (p.IsBlank())  // skip filled fields as well as nonexisting tiles
    //      {
    //        m.insert(l);
    //        break;
    //      }
    //    }
    //    Coordinate z = sizeX_ - 1;
    //    for (Coordinate i = 0; i < sizeX_; ++i, --z)                      //   right -> left
    //    {
    //      const Location l{ BoardPartID::Stage, z, j };
    //      const Piece& p = GetPieceIndex(l);
    //      if (p.IsBlank())  // skip filled fields as well as nonexisting tiles
    //      {
    //        m.insert(l);
    //        break;
    //      }
    //    }
    //  }
    //}

    //movesW_.reserve(m.size());
    //movesB_.reserve(m.size());

    //const Location w{ BoardPartID::Stock, 0, 0 };
    //const Location b{ BoardPartID::Stock, 0, 1 };

    //for (const Location& l : m)
    //{
    //  Actions aw{};
    //  aw.push_back(std::make_shared<ActionLift>(w, CorePiece::WC));       // pick new piece up
    //  aw.push_back(std::make_shared<ActionDrop>(l, CorePiece::WC));       // and place it on target
    //  movesW_.push_back(std::make_shared<Move>(aw));                      // add move to move list
    //                                                                               
    //  Actions ab{};                                                                
    //  ab.push_back(std::make_shared<ActionLift>(b, CorePiece::BC));       // pick new piece up
    //  ab.push_back(std::make_shared<ActionDrop>(l, CorePiece::BC));       // and place it on target
    //  movesB_.push_back(std::make_shared<Move>(ab));                      // add move to move list
    //}
  }

  //PositionValue CFourBoard::EvaluateStatically() const noexcept    // as seen from White
  //{
  //  assert(movesW_.empty());
  //  assert(movesB_.empty());

  //  GetAllMoves();                                                        // fill the move lists
  //  depth_ = 1;

  //  if (movesW_.empty()) return PositionValue::PValueType::Tie;           // if no more moves, game over.
  //                                                                        // Note that B has the same moves as W; enough to check W
  //  return v_.c & Anti ? -EvaluateChainLengths(4) : +EvaluateChainLengths(4);
  //}

  void CFourGame::Register() noexcept
  {
    PMap.Register(CorePiece::WC);
    PMap.Register(CorePiece::BC);

    Variants.Register(Variant(0, IDR_GAMETYPE_CFOUR, "Classic Connect Four", CFour::Classic,        7, 6, 4, 20 ));
    Variants.Register(Variant(0, IDR_GAMETYPE_CFOUR, "Linetris",             CFour::Linetris,       8, 8, 4, 20 ));
    Variants.Register(Variant(0, IDR_GAMETYPE_CFOUR, "Spider Line Four",     CFour::Spider,         8, 8, 4, 20 ));
    Variants.Register(Variant(0, IDR_GAMETYPE_CFOUR, "Spider LineTris",      CFour::SpiderLinetris, 8, 8, 4, 20 ));
    Variants.Register(Variant(0, IDR_GAMETYPE_CFOUR, "Anti Line Four",       CFour::Anti,           8, 8, 4, 20 ));
  }

  const BoardPartDimensions CFourGame::GetDimensions(const VariantChosen& v) noexcept
  {
    BoardPartDimensions d{
       BoardPartDimension(v.x, v.y, LayoutType::Light, BoardStartX, BoardStartY, FieldSizeX, FieldSizeY, 1, 1),
       BoardPartDimension(2, 2, LayoutType::Light, BoardStartX + FieldSizeX * (v.x + 1), BoardStartY + v.y * FieldSizeY + FieldSizeY / 2, FieldSizeX, FieldSizeY),
       BoardPartDimension(1, 1, LayoutType::Small, BoardStartX + FieldSizeX * (v.x + 1), BoardStartY + FieldSizeSY, FieldSizeSX, FieldSizeSY, 0, FieldSizeY * v.x - FieldSizeSY * 4),
    };
    return d;
  }
}
