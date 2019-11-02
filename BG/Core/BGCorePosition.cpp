#include "pch.h"

namespace BoardGamesCore
{

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

  void Position::SetPosition(std::vector<const Piece*> list)
  {
    unsigned int z{};
    for (const auto& p : list)
    {
      while (pieces_[z] == pMap_->GetIndex(Piece::NoTile)) z++; // skip non-existing fields
      pieces_[z++] = pMap_->GetIndex(*p);
    }
  }


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

  const Location MainPosition::GetNextTakenL(const Color& c) const noexcept
  {
    Coordinate y{ c == Color::White ? 0U : 1U };
    for (Coordinate x = 0; ; x++)
    {
      const Location l{ BoardPart::Taken, x, y };
      if (taken_.GetPiece(l).IsKind(noKind::NoKind)) return l;
    }
  }


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
          p.CollectMoves(*this, Location(BoardPart::Main, i, j), p.IsColor(Color::White) ? movesW_ : movesB_);
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
    if (onTurn_ == &Color::White) onTurn_ = &Color::Black;
    else onTurn_ = &Color::White;
  };

  void MainPosition::PreviousPlayer(void) noexcept
  {
    if (onTurn_ == &Color::White) onTurn_ = &Color::Black;
    else onTurn_ = &Color::White;
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