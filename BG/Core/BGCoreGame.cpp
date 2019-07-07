#include "stdafx.h"

namespace BoardGamesCore
{

  Game::Game(const PieceMapP& m, MainPosition* p, Layout* l, bool pl) noexcept : pMap{ m }, pos {p}, lay{ l }, placing(pl), plist(std::make_shared<AIContext>())
  {
    AddPlayer(new Player(&PlayerType::Human, &Color::White));
    AddPlayer(new Player(&PlayerType::Computer, &Color::Black));
  }

  Game::~Game(void)
  {
    for (auto& p : *plist) delete p;
    delete pos;
    delete lay;
//    if (tlay != nullptr) delete tlay;
//    if (slay != nullptr) delete slay;
    for (auto& p : players) delete p;

  }

  void Game::ReadFromWWW(const std::string& gameno)
  {
    const std::string url = R"(http://brainking.com/en/ShowGame?g=)" + gameno;
    std::string html = URL::GetHTMLFromURL(url);
    pos->SetPosition(Piece::ListFromHTML(html, Piece::GetHTMLPieceMap()));
  }

  void Game::Execute(MoveP m)
  {
    const unsigned int z{ pos->OnTurn() == &Color::White ? 1U : 0U };     // need to buffer the index, as Execute changes who's on turn
    const std::vector<const Piece*> t = pos->Execute(m);                  // execute move (includes setting pos to next player)
    //if (t.size() > 0) pos->_taken.Push(z, t);                                   // append taken pieces
    NextPlayer();                                                         // the game has also a pointer to the current player
  }

  void Game::Execute(const Move& m)
  {
    pos->Execute(m);                                                      // execute move (includes setting pos to next player)
    NextPlayer();                                                         // the game has also a pointer to the current player
  }

}
