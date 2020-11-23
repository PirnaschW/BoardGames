#include "Core.h"

#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
#undef _CXX17_DEPRECATE_CODECVT_HEADER
#define _CXX17_DEPRECATE_CODECVT_HEADER
#include <xstring>
#include <locale>
#include <codecvt>

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

  void Game::ReadFromWWW(const std::string& gameno)
  {
    const std::wstring url = LR"(http://brainking.com/en/ShowGame?g=)" + std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(gameno);
    std::string html = URL::GetHTMLFromURL(url);
    pos->SetPosition(Piece::ListFromHTML(html, Piece::GetHTMLPieceMap()));
  }

  void Game::Execute(const Move& m)
  {
    pos->Execute(m);                                                      // execute move (includes setting pos to next player)
    NextPlayer();                                                         // the game has also a pointer to the current player
  }

}
