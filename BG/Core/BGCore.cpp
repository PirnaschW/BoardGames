#include "stdafx.h"

#include "Game Test/TestGame.h"

namespace BoardGamesCore
{
  //Step& Step::operator =(const Step& s)
  //{
  //  from = s.from; 
  //  to = s.to; 
  //  assert(to == s.to);
  //  type = s.type; 
  //  take = s.take; 
  //  return *this;
  //}


//  Move& Move::operator =(const Move& m)
//  { 
//    step = m.step;
//    assert(step == m.step);
//    value = m.value; 
////    assert(m == *this);
//    return *this;
//  }

  Move&& Move::operator =(Move&& m) noexcept
  {
    std::swap(step, m.step);
    value = m.value;
    return std::move(*this);
  }

  bool Move::IsTake(void) const noexcept
  {
    for (auto& s : step) if (s.IsTake()) return true;
    return false;
  }

  const std::vector<Location> Move::GetJumped(void) const  // return list of jumped-over locations
  {
    std::vector<Location> jumped{};
    for (auto& s : step)
    {
      if (s.GetType() == Step::StepType::Jump) throw ("code missing!"); // TODO
    }
    return jumped;
  }


  std::size_t Position::GetHash(void) const noexcept
  {
    if (hash) return hash;
    std::size_t z{};
    for (auto& p : pieces)
    {
      z ^= p->GetHash() + 0x9e3779b9 + (z << 6) + (z >> 2);
    }
    return hash = z;
  }


  void MainPosition::GetAllMoves(void) // collect all moves for all pieces
  {
    assert(movelistW.size() == 0);
    assert(movelistB.size() == 0);

    movelistW.clear();
    movelistB.clear();
    for (unsigned int i = 0; i < sizeX; i++)
    {
      for (unsigned int j = 0; j < sizeY; j++)
      {
        const Piece* p = GetPiece(Location(i, j));
        if (!p->IsColor(&Color::NoColor))
        {
          p->CollectMoves(*this, Location(i, j), p->IsColor(&Color::White) ? movelistW : movelistB);
          assert(Test::Test::TestTaken(this));
        }
      }
    }
    assert(Test::Test::TestTaken(this));
    assert(Test::Test::TestMoveUndo(this));
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

  const std::vector<const Piece*> MainPosition::Execute(const Move& m)
  {
    movelistW.clear();                                                    // after the move is executed, the movelists will be outdated
    movelistB.clear();                                                    // after the move is executed, the movelists will be outdated

    std::vector<const Piece*> taken{};
    const std::vector<Step> step{ m.GetSteps() };
    const Piece* p = GetPiece(m.GetFr());

    // collect taken pieces
    for (auto& s : step)
    {
      if (s.GetType() & Step::StepType::Promote) p = p->Promote(true);

      const std::vector<Field>& take = s.GetTake();
      for (auto& t : take)
      {
        assert(t.GetPiece() == GetPiece(t.GetLocation()));                // verify that the piece we expect is really there
        taken.push_back(t.GetPiece());                                    // collect potential pieces taken (target tile or jumped over)
        SetPiece(t.GetLocation(), &Piece::NoPiece);                       // remove the taken piece from the board
      }
    }

    SetPiece(m.GetTo(), p);                                               // move the piece
    SetPiece(m.GetFr(), &Piece::NoPiece);                                 // empty the start tile

    NextPlayer();                                                         // after the move, it's the next player's turn
    return taken;                                                         // return the collected taken pieces
  }

  void MainPosition::Undo(const Move& m)
  {
    const std::vector<Step> step = m.GetSteps();
    assert(GetPiece(m.GetFr()) == &Piece::NoPiece);                       // verify that the target is really empty
    SetPiece(m.GetFr(), GetPiece(m.GetTo()));                             // put the piece back on the starting field
    SetPiece(m.GetTo(), &Piece::NoPiece);                                 // replace it with no piece
    for (auto& s : step)                                                  // for all steps
    {
      const std::vector<Field>& take = s.GetTake();
      for (auto& t : take)
      {
        assert(GetPiece(t.GetLocation()) == &Piece::NoPiece);             // verify that the target is really empty
        SetPiece(t.GetLocation(), t.GetPiece());                          // Put the taken piece(s) back on the board
      }
    }
    PreviousPlayer();                                                     // after the undo, it's the previous player's turn
  }


  void TakenPosition::Push(unsigned int player, const std::vector<const Piece*>& p)
  {
    for (auto& pp : p)
    {
      if (pp == nullptr) continue;
      if (pp->IsBlank()) continue;
      for (unsigned int i = 0; ; i++)
      {
        Location l{ i, player };
        if (GetPiece(l) == &Piece::NoTile)
        {
          SetPiece(l, pp);
          break;
        }
      }
    }
  }


  Game::Game(MainPosition* p, TakenPosition* t, StockPosition* s, Layout* l, TakenLayout* tl, StockLayout* sl) noexcept
    : pos{ p }, tpos(t), spos(s), lay{ l }, tlay(tl), slay(sl)
  {
    AddPlayer(new Player(&PlayerType::Human, &Color::White));
    AddPlayer(new Player(&PlayerType::Computer, &Color::Black));
    plist.reserve(10000);
  }

  Game::~Game(void)
  {
    for (auto& p : plist) delete p;
    delete pos;
    if (tpos != nullptr) delete tpos;
    if (spos != nullptr) delete spos;
    delete lay;
    if (tlay != nullptr) delete tlay;
    if (slay != nullptr) delete slay;
    for (auto& p : players) delete p;
  }

  void Game::Execute(const Move& m)
  {
    unsigned int z{ pos->OnTurn() == &Color::White ? 1U : 0U }; // need to buffer the index, as Execute changes who's on turn
    tpos->Push(z, pos->Execute(m));  // execute move (includes setting pos to next player), and display taken pieces
    NextPlayer();                    // the game has also a pointer to the current player
  }

}
