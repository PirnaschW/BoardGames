#include "Core.h"

namespace BoardGamesCore
{
  
  Position::Position(const PieceMapP& p, Coordinate x, Coordinate y, const Piece& init) noexcept
    : pMap_{ p }, sizeX_(x), sizeY_(y), pieces_(1ULL * x * y, pMap_->GetIndex(init)) {}

  Position::Position(const Position& p) noexcept : pMap_{ p.pMap_ }, sizeX_(p.sizeX_), sizeY_(p.sizeY_), pieces_{ p.pieces_ } {}
  
  const Piece& Position::GetPiece(const Location& l) const noexcept { return l.Valid(sizeX_, sizeY_) ? pMap_->GetPiece(pieces_[l.Index(sizeX_, sizeY_)]) : Piece::NoTile; }
  
  bool Position::HasPiece(const Piece& p) const noexcept { const PieceIndex z = pMap_->GetIndex(p); for (const auto& pp : pieces_) if (pp == z) return true; return false; }
  
  bool Position::operator ==(const Position* p) const noexcept { return p->sizeX_ == sizeX_ && p->sizeY_ == sizeY_ && p->pieces_ == pieces_; }
  bool Position::operator !=(const Position* p) const noexcept { return !(*this == p); }


  std::size_t Position::GetHash(void) const noexcept
  {
    if (hash_) return hash_;
    std::size_t z{};
    for (auto& p : pieces_)
    {
      z ^= static_cast<size_t>(p) + 0x9e3779b9 + (z << 6) + (z >> 2);
//      z ^= pMap_->GetPiece(p)->GetHash() + 0x9e3779b9 + (z << 6) + (z >> 2);
    }
    return hash_ = z;
  }

  void Position::Serialize(Archive& ar) const { for (auto& p : pieces_) pMap_->GetPiece(p).Serialize(ar); }
  
  const Piece& Position::SetPiece(const Location& l, const Piece& p) noexcept { hash_ = 0; pieces_[l.Index(sizeX_, sizeY_)] = pMap_->GetIndex(p); return p; }

  void Position::SetPosition(std::vector<const Piece*>& list)
  {
    unsigned int z{};
    for (const auto& p : list)
    {
      while (pieces_[z] == pMap_->GetIndex(Piece::NoTile)) z++; // skip non-existing fields
      pieces_[z++] = pMap_->GetIndex(*p);
    }
  }

  
  MainPosition::MainPosition(const MainPosition& m) noexcept : Position(m),
    vCode_(m.vCode_), sequence_(m.sequence_), stock_(m.stock_), taken_(m.taken_), onTurn_(m.onTurn_), depth_(m.depth_),
    value_(m.value_), movesW_(m.movesW_), movesB_(m.movesB_) {}

  MainPosition::MainPosition(VariantCode c, const PieceMapP& p, const Dimensions& d) noexcept :
    Position(p, d[0].xCount_, d[0].yCount_), vCode_(c), stock_(p, d[1].xCount_, d[1].yCount_), taken_(p, d[2].xCount_, d[2].yCount_) {}

  bool MainPosition::operator ==(const MainPosition& p) const noexcept { return OnTurn() == p.OnTurn() && Position::operator==(&p); }

  std::size_t MainPosition::GetHash(void) const noexcept { return Position::GetHash() + taken_.GetHash() + std::hash<const PieceColor*>()(onTurn_); }

  void  MainPosition::SetOnTurn(const PieceColor& c) noexcept { onTurn_ = &c; }

  const PieceColor& MainPosition::OnTurn(void) const noexcept { return *onTurn_; }

  PositionValue MainPosition::GetValue(bool w) const noexcept { return value_.Relative(w); }

  PositionValue MainPosition::SetValue(bool w, PositionValue v) const noexcept { return value_ = v.Relative(w); }

  const Piece& MainPosition::GetPiece(const Location& l) const noexcept
  {
    switch (l.b_)
    {
      case BoardPart::Stock: return stock_.GetPiece(l);
      case BoardPart::Taken: return taken_.GetPiece(l);
      default:               return Position::GetPiece(l);
    }
  }

  const Piece& MainPosition::SetPiece(const Location& l, const Piece& p) noexcept
  {
    switch (l.b_)
    {
      case BoardPart::Stock: return stock_.SetPiece(l, p);
      case BoardPart::Taken: return taken_.SetPiece(l, p);
      default:               return Position::SetPiece(l, p);
    }
  }

  const Location MainPosition::GetNextTakenL(const PieceColor& c) const noexcept
  {
    Coordinate y{ c == PieceColor::White ? 0U : 1U };
    for (Coordinate x = 0; ; x++)
    {
      const Location l{ BoardPart::Taken, x, y };
      if (taken_.GetPiece(l).IsKind(noKind::NoKind)) return l;
    }
  }

  bool MainPosition::IsTaken(const Location& l) const noexcept { return l.b_ == BoardPart::Taken; }

  MoveP MainPosition::GetBestMove(bool w) const noexcept { return (w ? movesW_[0] : movesB_[0]); }

  void MainPosition::GetAllMoves(void) const noexcept // collect all moves for all pieces
  {
    assert(movesW_.empty());
    assert(movesB_.empty());

    movesW_.reserve(20);
    movesB_.reserve(20);
    for (Coordinate i = 0; i < sizeX_; i++)
    {
      for (Coordinate j = 0; j < sizeY_; j++)
      {
        const Piece& p = GetPiece(Location(BoardPart::Main, i, j));
        if (!p.IsKind(noKind::NoKind))  // skip blank fields as well as nonexisting tiles
        {
          p.CollectMoves(*this, Location(BoardPart::Main, i, j), p.IsColor(PieceColor::White) ? movesW_ : movesB_);
        }
      }
    }
  } 

  void MainPosition::JumpsOnly(Moves& moves) const noexcept
  {
    // if there are any Jumps (catching opponent pieces) possible, remove all non-takes
    auto MhasJump = [](const MoveP& m) -> bool { return m->GetActions().HasJump(); };
    if (std::find_if(moves.cbegin(), moves.cend(), MhasJump) != moves.cend())
    {
      auto MhasNoJump = [](const MoveP& m) -> bool { return !m->GetActions().HasJump(); };
      moves.erase(std::remove_if(moves.begin(), moves.end(), MhasNoJump), moves.end());
    }
  }

  void MainPosition::NextPlayer(void) noexcept
  {
    if (onTurn_ == &PieceColor::White) onTurn_ = &PieceColor::Black;
    else onTurn_ = &PieceColor::White;
  };

  void MainPosition::PreviousPlayer(void) noexcept
  {
    if (onTurn_ == &PieceColor::White) onTurn_ = &PieceColor::Black;
    else onTurn_ = &PieceColor::White;
  };


  void MainPosition::Execute(const Move& m) noexcept
  {
    movesW_.clear();                                                      // after the move is executed, the movelists will be outdated
    movesB_.clear();
    //depth_ = 0; " keep value for display

    sequence_.push_back(std::make_shared<Move>(m));                       // save the move in the sequence_
    for (const auto& aa : m.GetActions()) aa->Execute(this);              // execute all Actions
    NextPlayer();                                                         // after the move, it's the next player's turn
  }

  void MainPosition::Undo(const Move& m) noexcept
  {
    assert(1 == 0);
  }

  //void TakenPosition::Push(unsigned int player, const std::vector<const Piece&>& p) noexcept
  //{
  //  for (auto& pp : p)
  //  {
  //    if (pp == nullptr) continue;
  //    if (pp->IsBlank()) continue;
  //    for (Coordinate i = 0; ; i++)
  //    {
  //      assert(i < sizeX_);
  //      const Location l{ BoardPart::Taken, i, static_cast<unsigned char>(player) };
  //      if (GetPiece(l) == &Piece::NoTile)
  //      {
  //        SetPiece(l, pp);
  //        break;
  //      }
  //    }
  //  }
  //}

}