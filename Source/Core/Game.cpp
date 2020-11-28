#include "Core.h"

namespace BoardGamesCore
{

  Game::Game(const PieceMapP& m, MainPosition* p, MainLayout* l) noexcept : pMap_{ m }, pos {p}, lay{ l }
  {
    // create the standard two players
    AddPlayer(new Player(&PlayerType::Human, PieceColor::White));
    AddPlayer(new Player(&PlayerType::Computer, PieceColor::Black));

    // automatically put the content of the PieceMap in the Stock
    for (PieceIndex i = 0; i < m->GetCount()-1; i++) // skip the last piece, it is NoTile, and cannot be drawn
      pos->SetPiece(Location(BoardPart::Stock, i / 2U, i % 2U), m->GetPiece(i));  // expects respective Pieces with alternating colors

    // for testing:
    _mode.Set(Mode::ShowStock);
  }

  Game::~Game(void)
  {
    delete pos;
    delete lay;
    for (auto& p : players) delete p;
  }

    void Game::Execute(const Move& m)
  {
    pos->Execute(m);                                                      // execute move (includes setting pos to next player)
    NextPlayer();                                                         // the game has also a pointer to the current player
  }

}
