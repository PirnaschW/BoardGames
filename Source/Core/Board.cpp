#include "Core.h"

namespace BoardGamesCore
{

  Board::Board(const Board& p) noexcept :
    v_(p.v_), sequence_(p.sequence_), stage_(p.stage_), stock_(p.stock_), taken_(p.taken_), whiteOnTurn_(p.whiteOnTurn_), depth_(p.depth_),
    value_(p.value_), movesW_(p.movesW_), movesB_(p.movesB_) {}

  Board::Board(const VariantChosen& v, const BoardPartDimensions& d) noexcept :
    v_(v),
    stage_(d[0], BoardPartID::Stage, PMap[Piece::NoPiece]),
    stock_(d[1], BoardPartID::Stock, PMap[Piece::NoPiece]),
    taken_(d[2], BoardPartID::Taken, PMap[Piece::NoTile])   // Taken starts with no tiles
  {
    // automatically put the content of the PieceMap in the Stock
    //for (PieceIndex i = 0; i < PMap.GetCount() - 1; i++) // skip the last piece, it is NoTile, and cannot be drawn
    //  SetPieceIndex(i, i / 2U, i % 2U, BoardPartID::Stock);  // expects respective Pieces with alternating colors
  }

  std::size_t Board::GetHash() const noexcept { return stage_.GetHash() + taken_.GetHash() + std::hash<bool>()(whiteOnTurn_); }

  void Board::Serialize(std::stringstream& s) const
  {
    //TODO!: serialize Variant
    // v_.Serialize(s);         
    stage_.Serialize(s);
    taken_.Serialize(s);
    //TODO!: serialize move sequence
    // sequence_.Serialize(s);
  }

  //void Board::Serialize(std::stringstream& s)
  //{
  //  bool save = true;
  //  auto f = save ?
  //    BoardPart::Activity{ [&s, this](PieceIndex& pI) { PMap.GetPiece(pI).Serialize(s); } } :
  //    BoardPart::Activity{ [&s, this](PieceIndex& pI) { pI = PMap.Find(s); } };
  //  //stage_.ForEach(f);
  //  //taken_.ForEach(f);
  //}

  void Board::SetupBoard(const std::vector<PieceIndex>& list)  noexcept
  {
    const auto noTile = PMap[Piece::NoTile];
    auto iList = list.cbegin();
    auto f = [&iList, &noTile, this](PieceIndex pI) -> PieceIndex { return (pI != noTile) ? *iList++ : pI; };
    stage_.ForEach(f);
    if (iList != list.cend()) taken_.ForEach(f);
    assert(iList == list.cend());  // should have exactly used all pieces in list
    value_ = PositionValue::PValueType::Undefined;
    depth_ = 0;
  }

  void Board::SetPieceIndex(PieceIndex pI, Coordinate x, Coordinate y, BoardPartID b) noexcept
  {
    switch (b)
    {
    case BoardPartID::Stock: stock_.SetPieceIndex(pI, x, y); break;
    case BoardPartID::Taken: taken_.SetPieceIndex(pI, x, y); break;
    default:                 stage_.SetPieceIndex(pI, x, y); break;
    }
  }

  //const Location Board::GetNextFreeTakenLocation(const PieceColor& c) const noexcept
  //{
  //  const auto noPiece = PMap[Piece::NoPiece];
  //  Coordinate y{ c == PieceColor::White ? 0 : 1 };
  //  for (Coordinate x = 0; ; x++)
  //  {
  //    if (taken_.GetPieceIndex(x, y) == noPiece) return { BoardPartID::Taken, x, y };
  //  }
  //}

  //bool Board::IsTaken(const Location& l) const noexcept { return l.b_ == BoardPartID::Taken; }

  void Board::GetAllMoves() const noexcept // collect all moves for all pieces
  {
    assert(movesW_.empty());
    assert(movesB_.empty());
    movesW_.reserve(20); // TODO: analyze Moves vector reserve for a better default
    movesB_.reserve(20);
  
    auto f = [this](const BoardPart& bp) -> void // lambda to loop over all pieces and collect their moves
    {
      for (Coordinate i = 0; i < bp.GetSizeX(); i++)
      {
        for (Coordinate j = 0; j < bp.GetSizeY(); j++)
        {
          const Piece& p = PMap[bp.GetPieceIndex(i, j)];
          p.CollectMoves(*this, Location(bp.GetID(), i, j), p.IsColor(PieceColor::White) ? movesW_ : movesB_);
        }
      }
    };
 
    f(stage_);   // collect all moves from the playing board
    f(stock_);   // collect all potential moves from the stock - some games allow insertion of new pieces
    f(taken_);   // collect all potential moves from the taken board - some games allow re-insertion of pieces
  }

  void Board::Execute(const Move& m) noexcept
  {
    sequence_.push_back(std::make_shared<Move>(m));                       // save the move in the sequence_
    m.GetActions().Log();
    for (const auto& aa : m.GetActions()) aa->Execute(this);              // execute all Actions
    whiteOnTurn_ = !whiteOnTurn_;                                         // after the move, it's the next player's turn

    movesW_.clear();                                                      // after the move is executed, the movelists will be outdated
    movesB_.clear();
    //depth_ = 0; " keep value for display
  }

  void Board::Draw(DC* dc, bool showstock) const
  { 
    stage_.DrawFrame(dc);
    stage_.Draw(dc);
    taken_.Draw(dc);
    if (showstock) stock_.Draw(dc);
  }

  void Board::DrawSelected(DC* dc, const Location& l) const
  {
    switch (l.b_)
    {
    case BoardPartID::Stock: stock_.DrawSelected(dc, l.x_, l.y_); break;
    case BoardPartID::Taken: taken_.DrawSelected(dc, l.x_, l.y_); break;
    default:                 stage_.DrawSelected(dc, l.x_, l.y_); break;
    }
  }

  bool Board::GetLocationFromPoint(const Point& p, Location& l) const noexcept
  {
    if (stage_.GetLocationFromPoint(p, l.x_, l.y_)) { l.b_ = BoardPartID::Stage; return true; }
    if (taken_.GetLocationFromPoint(p, l.x_, l.y_)) { l.b_ = BoardPartID::Taken; return true; }
    if (stock_.GetLocationFromPoint(p, l.x_, l.y_)) { l.b_ = BoardPartID::Stock; return true; }
    return false;
  }

  void Board::EvaluateStatically() const noexcept   // as seen from White
  {
    // default evaluation: count all material, and add difference of move count. Overwrite for each game as needed
    depth_ = 1;
    if (WhiteOnTurn() && movesW_.empty())
    {
      value_ = PositionValue::PValueType::Lost; // if no more moves, game over
      return;
    }
    if (!WhiteOnTurn() && movesB_.empty())
    {
      value_ = PositionValue::PValueType::Won; // if no more moves, game over
      return;
    }
    PositionValue v{ GetMoveCountFactor() * (static_cast<PValueBaseType>(movesW_.size()) - static_cast<PValueBaseType>(movesB_.size())) };
    for (Coordinate j = 0; j < stage_.GetSizeY(); j++)
    {
      for (Coordinate i = 0; i < stage_.GetSizeX(); i++)                                // loop through all locations
      {
        const Location l{ BoardPartID::Stage, i,j };
        const PieceIndex pI = GetPieceIndex(i, j);
        const Piece& p = PMap[pI];
        if ((p == Piece::NoTile) || (p == Piece::NoPiece)) continue;         // nothing here
        v += (p.IsColor(PieceColor::White) ? 1 : -1) * p.GetValue(*this, l);
      }
    }
    value_ = v;
  }

  [[ nodiscard ]] PositionValue Board::EvaluateChainLengths(unsigned int towin) const noexcept           // as seen from White
  {
    PositionValue v{ 0 };
    for (Coordinate j = 0; j < stage_.GetSizeY(); j++)
    {
      for (Coordinate i = 0; i < stage_.GetSizeX(); i++)                                                      // loop through all locations
      {
        PositionValue vv = EvaluateChainLength(Location(BoardPartID::Stage, i, j), towin);
        if (vv != PositionValue::PValueType::Normal) return vv;
        v += vv;
      }
    }

    return v;
  }

  [[ nodiscard ]] PositionValue Board::EvaluateChainLength(Location l, unsigned int towin) const noexcept    // as seen from White
  {
    PValueBaseType v1{ 0 };
    PValueBaseType v2{ 0 };

    const PieceIndex pI = GetPieceIndex(l.x_, l.y_, l.b_);
    const Piece& p = PMap[pI];
    if (p.IsColor(PieceColor::NoColor)) return 0;                                       // nothing here, so no chain can start
    const bool w = p.IsColor(PieceColor::White);
    const int n = sizeof(Offset::QDirection) / sizeof(Offset::QDirection[0]) / 2;
    for (int i = 0; i < n; ++i)
    {
      PValueBaseType bonus = 0;
      const Offset& d = Offset::QDirection[i];
      const Location l0 = l + d * -1;
      const Piece& pp{ PMap[GetPieceIndex(l0.x_,l0.y_,l0.b_)] };
      if (pp != Piece::NoTile && pp.IsColor(p)) continue;         // if same color is that direction, we counted it already, so move on
      if (pp != Piece::NoTile && pp.IsBlank()) ++bonus;            // free field, give extra points - much better than opponent's piece
      Location ll{ l };
//      Location ll{ BoardPartID::Stage,i,j };
      unsigned int z{ 1 };
      while (true)
      {
        ll += d;
        const Piece& ppp = PMap[GetPieceIndex(ll.x_, ll.y_, ll.b_)];
        if (ppp == Piece::NoTile) break;
        if (ppp.IsColor(p)) z++;
        else
        {
          if (ppp.IsColor(PieceColor::NoColor)) ++bonus;                                 // if line ends with free field, give an extra point - much better than opponent's piece
          break;
        }
      }
      if (z >= towin) return w ? PositionValue::PValueType::Won : PositionValue::PValueType::Lost;
      (w ? v1 : v2) += GetChainValue(z) + bonus * GetChainValue(z - 1);
    }
    return v1 - v2;
  }

  constexpr PValueBaseType Board::GetChainValue(unsigned int z) const noexcept
  {
    switch (z)
    {
      case 0:  return             1;
      case 1:  return            10;
      case 2:  return           100;
      case 3:  return         1'000;
      case 4:  return        10'000;
      case 5:  return       100'000;
      case 6:  return     1'000'000;
      case 7:  return    10'000'000;
      case 8:  return   100'000'000;
      case 9:  return 1'000'000'000;
      default: return          0;
    }
  }

  //void Board::JumpsOnly(Moves& moves) const noexcept
  //{
  //  // if there are any Jumps (catching opponent pieces) possible, remove all non-takes
  //  auto MhasJump = [](const MoveP& m) -> bool { return m->GetActions().HasJump(); };
  //  if (std::find_if(moves.cbegin(), moves.cend(), MhasJump) != moves.cend())
  //  {
  //    auto MhasNoJump = [](const MoveP& m) -> bool { return !m->GetActions().HasJump(); };
  //    moves.erase(std::remove_if(moves.begin(), moves.end(), MhasNoJump), moves.end());
  //  }
  //}

}