#include "stdafx.h"

namespace BoardGamesCore
{

  std::size_t Position::GetHash(void) const noexcept
  {
    if (hash) return hash;
    std::size_t z{};
    for (auto& p : pieces)
    {
      z ^= pMap->GetPiece(p)->GetHash() + 0x9e3779b9 + (z << 6) + (z >> 2);
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
    switch (l._b)
    {
      case BoardPart::Stock: return _stock.GetPiece(l);
      case BoardPart::Taken: return _taken.GetPiece(l);
      default:               return Position::GetPiece(l);
    }
  }

  const Piece* MainPosition::SetPiece(const Location& l, const Piece* p) noexcept
  {
    switch (l._b)
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


  void MainPosition::GetAllMoves(void) // collect all moves for all pieces
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
    if (std::find_if(moves.begin(), moves.end(), [](const MoveP m) {return m->GetSteps().front()->IsTake(); }) == moves.end()) return false;
    moves.erase(std::remove_if(moves.begin(), moves.end(), [](const MoveP m) {return !m->GetSteps().front()->IsTake(); }), moves.end());
    return true;
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

  const std::vector<const Piece*> MainPosition::Execute(MoveP m)
  {
    movelistW.clear();                                                    // after the move is executed, the movelists will be outdated
    movelistB.clear();                                                    // after the move is executed, the movelists will be outdated

    sequence.push_back(m);                                                // save the move in the sequence

    std::vector<const Piece*> taken{};
    const Steps& steps{ m->GetSteps() };
    const Piece* p = m->GetFr().GetPiece();

    // collect taken pieces
    for (auto& s : steps)
    {
      if (s->GetType() & Step::StepType::Promote) p = p->Promote(true);

      const Fields& take = s->GetTakes();
      for (auto& t : take)
      {
        const Piece* pt = t.GetPiece();
        if (pt->IsBlank()) continue;                                      // ignore blank fields - nothing was taken
        assert(pt != &Piece::NoTile);                                     // cannot be NoTile, as this means you moved out-of-bounds
        taken.push_back(t.GetPiece());                                    // collect potential pieces taken (target tile or jumped over)
        SetPiece(t.GetLocation(), &Piece::NoPiece);                       // remove the taken piece from the board
      }
    }

    SetPiece(m->GetFr().GetLocation(), &Piece::NoPiece);                  // empty the start tile
    SetPiece(m->GetTo().GetLocation(), m->GetTo().GetPiece());            // place the piece at the target field

    NextPlayer();                                                         // after the move, it's the next player's turn
    return taken;                                                         // return the collected taken pieces
  }


  void MainPosition::Execute(const Move& m) noexcept
  {
    movelistW.clear();                                                    // after the move is executed, the movelists will be outdated
    movelistB.clear();                                                    // after the move is executed, the movelists will be outdated
    for (const auto& aa : m.GetActions()) aa->Execute(this);                           // execute all Actions
    NextPlayer();                                                         // after the move, it's the next player's turn
  }

  void MainPosition::Undo(const MoveP m)
  {
    const Steps& steps = m->GetSteps();
    SetPiece(m->GetTo().GetLocation(), &Piece::NoPiece);                  // empty the target field
    for (auto& s : steps)                                                 // for all steps
    {
      const Fields& takes = s->GetTakes();
      for (auto& t : takes) SetPiece(t.GetLocation(), t.GetPiece());      // Put the taken piece(s) back on the board
    }
    SetPiece(m->GetFr().GetLocation(), m->GetFr().GetPiece());            // put the piece back on the starting field
    PreviousPlayer();                                                     // after the undo, it's the previous player's turn
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