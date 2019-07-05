#include "stdafx.h"

namespace BoardGamesCore
{

  Game::Game(const PieceMapP& m, MainPosition* p, TakenPosition* t, StockPosition* s, Layout* l, TakenLayout* tl, StockLayout* sl, bool pl) noexcept
    : pMap{ m }, pos {p}, tpos(t), spos(s), lay{ l }, tlay(tl), slay(sl), placing(pl)
  {
    AddPlayer(new Player(&PlayerType::Human, &Color::White));
    AddPlayer(new Player(&PlayerType::Computer, &Color::Black));
    plist = new AIContext();
    // plist.reserve(16384);
  }

  Game::~Game(void)
  {
    for (auto& p : *plist) delete p;
    delete plist;

    delete pos;
    if (tpos != nullptr) delete tpos;
    if (spos != nullptr) delete spos;
    delete lay;
    if (tlay != nullptr) delete tlay;
    if (slay != nullptr) delete slay;
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
    tpos->Push(z, pos->Execute(m));                                       // execute move (includes setting pos to next player), and display taken pieces
    NextPlayer();                                                         // the game has also a pointer to the current player
  }

}
