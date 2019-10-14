#include "stdafx.h"

namespace BoardGamesCore
{

  std::size_t Position::GetHash(void) const noexcept
  {
    if (hash) return hash;
    std::size_t z{};
    for (auto& p : pieces)
    {
      z ^= static_cast<size_t>(p) + 0x9e3779b9 + (z << 6) + (z >> 2);
//      z ^= pMap->GetPiece(p)->GetHash() + 0x9e3779b9 + (z << 6) + (z >> 2);
    }
    return hash = z;
  }

  void Position::SetPosition(std::vector<const Piece*> list)
  {
    unsigned int z{};
    for (const auto& p : list)
    {
      while (pieces[z] == pMap->GetIndex(&Piece::NoTile)) z++; // skip non-existing fields
      pieces[z++] = pMap->GetIndex(p);
    }
  }


  const Piece* MainPosition::GetPiece(const Location& l) const noexcept
  {
    switch (l.b_)
    {
      case BoardPart::Stock: return _stock.GetPiece(l);
      case BoardPart::Taken: return _taken.GetPiece(l);
      default:               return Position::GetPiece(l);
    }
  }

  const Piece* MainPosition::SetPiece(const Location& l, const Piece* p) noexcept
  {
    switch (l.b_)
    {
      case BoardPart::Stock: return _stock.SetPiece(l, p);
      case BoardPart::Taken: return _taken.SetPiece(l, p);
      default:               return Position::SetPiece(l, p);
    }
  }

  const Location MainPosition::GetNextTakenL(const Color* c) const noexcept
  {
    Coordinate y{ c == &Color::White ? 0U : 1U };
    for (Coordinate x = 0; ; x++)
    {
      const Location l{ BoardPart::Taken, x, y };
      if (_taken.GetPiece(l)->IsKind(noKind::NoKind)) return l;
    }
  }


  void MainPosition::GetAllMoves(void) noexcept // collect all moves for all pieces
  {
    assert(movelistW.empty());
    assert(movelistB.empty());

    movelistW.reserve(20);
    movelistB.reserve(20);
    for (Coordinate i = 0; i < sizeX; i++)
    {
      for (Coordinate j = 0; j < sizeY; j++)
      {
        const Piece* p = GetPiece(Location(BoardPart::Main, i, j));
        assert(p != nullptr);
        if (!p->IsKind(noKind::NoKind))  // skip blank fields as well as nonexisting tiles
        {
          p->CollectMoves(*this, Location(BoardPart::Main, i, j), p->IsColor(&Color::White) ? movelistW : movelistB);
        }
      }
    }
  }

  bool MainPosition::JumpsOnly(Moves& moves) const noexcept
  {
    // if there are any takes (catching opponent pieces) possible, remove all non-takes
    //if (std::find_if(moves.begin(), moves.end(), [](const MoveP m) {return m->GetSteps().front()->IsTake(); }) == moves.end()) return false;
    //moves.erase(std::remove_if(moves.begin(), moves.end(), [](const MoveP m) {return !m->GetSteps().front()->IsTake(); }), moves.end());
    return false;
//    return true;
  }

  void MainPosition::NextPlayer(void) noexcept
  {
    if (onTurn == &Color::White) onTurn = &Color::Black;
    else onTurn = &Color::White;
  };

  void MainPosition::PreviousPlayer(void) noexcept
  {
    if (onTurn == &Color::White) onTurn = &Color::Black;
    else onTurn = &Color::White;
  };


  void MainPosition::Execute(const Move& m) noexcept
  {
    movelistW.clear();                                                    // after the move is executed, the movelists will be outdated
    movelistB.clear();                                                    // after the move is executed, the movelists will be outdated
    depth = 0;

    sequence.push_back(std::make_shared<Move>(m));                                                // save the move in the sequence
    for (const auto& aa : m.GetActions()) aa->Execute(this);              // execute all Actions
    NextPlayer();                                                         // after the move, it's the next player's turn
  }

  void MainPosition::Undo(const Move& m) noexcept
  {
    assert(1 == 0);
  }

  //void TakenPosition::Push(unsigned int player, const std::vector<const Piece*>& p) noexcept
  //{
  //  for (auto& pp : p)
  //  {
  //    if (pp == nullptr) continue;
  //    if (pp->IsBlank()) continue;
  //    for (Coordinate i = 0; ; i++)
  //    {
  //      assert(i < sizeX);
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