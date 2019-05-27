#include "stdafx.h"

namespace BoardGamesCore
{

  bool Position::operator ==(const Position* p) const noexcept
  {
    if (p->sizeX != sizeX || p->sizeY != sizeY) return false;
    for (Coordinate i = 0; i < sizeX; i++)
    {
      for (Coordinate j = 0; j < sizeY; j++)
      {
        if (*(p->GetPiece(Location(i, j))) == *GetPiece(Location(i, j))) return false;
      }
    }
    return true;
  }

  std::size_t Position::GetHash(void) const noexcept
  {
    if (hash) return hash;
    std::size_t z{};
    for (auto& p : pieces)
    {
      z ^= pm->GetPiece(p)->GetHash() + 0x9e3779b9 + (z << 6) + (z >> 2);
    }
    return hash = z;
  }

  void Position::SetPosition(std::vector<const Piece*> list)
  {
    unsigned int z{};
    for (const auto& p : list)
    {
      while (pieces[z] == pm->GetIndex(&Piece::NoTile)) z++; // skip non-existing fields
      pieces[z++] = pm->GetIndex(p);
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
        const Piece* p = GetPiece(Location(i, j));
        assert(p != nullptr);
        if (!p->IsColor(&Color::NoColor))
        {
          p->CollectMoves(*this, Location(i, j), p->IsColor(&Color::White) ? movelistW : movelistB);
          //assert(Test::Test::TestTaken(this));
        }
      }
    }
    //assert(Test::Test::TestTaken(this));
    //assert(Test::Test::TestMoveUndo(this));
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
        assert(t.GetPiece() == GetPiece(t.GetLocation()));                // verify that the piece we expect is really there
        taken.push_back(t.GetPiece());                                    // collect potential pieces taken (target tile or jumped over)
        SetPiece(t.GetLocation(), &Piece::NoPiece);                       // remove the taken piece from the board
      }
    }

    SetPiece(m->GetFr().GetLocation(), &Piece::NoPiece);                   // empty the start tile
    SetPiece(m->GetTo().GetLocation(), m->GetTo().GetPiece());              // place the piece at the target field

    NextPlayer();                                                         // after the move, it's the next player's turn
    return taken;                                                         // return the collected taken pieces
  }

  void MainPosition::Undo(const MoveP m)
  {
    const Steps& steps = m->GetSteps();
    SetPiece(m->GetTo().GetLocation(), &Piece::NoPiece);                   // empty the target field
    for (auto& s : steps)                                                  // for all steps
    {
      const Fields& takes = s->GetTakes();
      for (auto& t : takes) SetPiece(t.GetLocation(), t.GetPiece());       // Put the taken piece(s) back on the board
    }
    SetPiece(m->GetFr().GetLocation(), m->GetFr().GetPiece());              // put the piece back on the starting field
    PreviousPlayer();                                                     // after the undo, it's the previous player's turn
  }


  void TakenPosition::Push(unsigned int player, const std::vector<const Piece*>& p) noexcept
  {
    for (auto& pp : p)
    {
      if (pp == nullptr) continue;
      if (pp->IsBlank()) continue;
      for (Coordinate i = 0; ; i++)
      {
        assert(i < sizeX);
        const Location l{ i, player };
        if (GetPiece(l) == &Piece::NoTile)
        {
          SetPiece(l, pp);
          break;
        }
      }
    }
  }

}