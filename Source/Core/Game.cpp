#include "Core.h"

namespace BoardGamesCore
{

  Game::Game(const VariantChosen& v, const PieceMapP& m, MainPosition* p, MainLayout* l) noexcept : v_{ v }, pMap_ { m }, pos{ p }, lay{ l }
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
  
  void Game::SetPosition(std::vector<const Piece*>& list) noexcept
  {
    if (list.empty()) pos->SetStartingPosition();
    else pos->SetPosition(list);

    pos->SetValue(true, pos->EvaluateStatically());                       // evaluate position statically
    plist.insert(pos->Clone());                                           // and save a copy in the position list
  }

  void Game::Execute(const Move& m)
  {
    pos->Execute(m);                                                      // execute move (includes setting pos to next player)
    NextPlayer();                                                         // the game has also a pointer to the current player
  }

  void Game::Serialize(Archive& ar)
  {
    if (ar.IsStoring()) ar.Serialize(v_);  // when loading, game type was already read to create this object
    pos->Serialize(ar);
  }


}
