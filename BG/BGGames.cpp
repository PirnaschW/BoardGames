#include "stdafx.h"
#include "BGSelect.h"

// this code is in a separate file as it needs to include all games' includes, and will therefore be compiled often

#include "Game Cam\CamGame.h"
#include "Game Checkers\CheckersGame.h"
#include "Game Hasami\HasamiGame.h"
#include "Game LoA\LoAGame.h"
#include "Game Logik\LogikGame.h"
#include "Game Massacre Chess\MCGame.h"
#include "Game Shogi\ShogiGame.h"
#include "Game TicTacToe\TicTacToeGame.h"
#include "Game Template\TemplateGame.h"
#include "Game Test\TestGame.h"

void BGSelect::AddGames(void)
{
  AddGame(IDR_GAMETYPE_CAMELOT);
  AddGame(IDR_GAMETYPE_CHECKERS);
  AddGame(IDR_GAMETYPE_HASAMI);
  AddGame(IDR_GAMETYPE_LOA);
  AddGame(IDR_GAMETYPE_LOGIK);
  AddGame(IDR_GAMETYPE_MCHESS);
  AddGame(IDR_GAMETYPE_SHOGI);
  AddGame(IDR_GAMETYPE_TICTACTOE);
  AddGame(IDR_GAMETYPE_TEMPLATE);
  AddGame(IDR_GAMETYPE_TEST);
}

const BoardGamesCore::VariantList& BGSelect::GetVariants(int id) const
{
  switch (id)
  {
    case IDR_GAMETYPE_CAMELOT:    return Variants<Cam::CamGame>::GetVariants();
    case IDR_GAMETYPE_CHECKERS:   return Variants<Checkers::CheckersGame>::GetVariants();
    case IDR_GAMETYPE_HASAMI:     return Variants<Hasami::HasamiGame>::GetVariants();
    case IDR_GAMETYPE_LOA:        return Variants<LoA::LoAGame>::GetVariants();
    case IDR_GAMETYPE_LOGIK:      return Variants<Logik::LGame<8, 5, 7>>::GetVariants();
    case IDR_GAMETYPE_MCHESS:     return Variants<MassacreChess::MCGame>::GetVariants();
    case IDR_GAMETYPE_SHOGI:      return Variants<Shogi::ShogiGame>::GetVariants();
    case IDR_GAMETYPE_TICTACTOE:  return Variants<TicTacToe::TicTacToeGame>::GetVariants();
    case IDR_GAMETYPE_TEMPLATE:   return Variants<Template::TemplateGame>::GetVariants();
    case IDR_GAMETYPE_TEST:       return Variants<Test::TestGame>::GetVariants();
    default:                      return Variants<Test::TestGame>::GetVariants();
  }
}


// Creating the right game
BoardGamesCore::Game* BGSelect::CreateGame(void)
{
  if (DoModal() == IDOK)
  {
    switch (m_game_id)
    {
      case IDR_GAMETYPE_CAMELOT:    return new Cam::CamGame(m_size_x, m_size_y);
      case IDR_GAMETYPE_CHECKERS:   return new Checkers::CheckersGame(m_size_x, m_size_y);
      case IDR_GAMETYPE_HASAMI:     return new Hasami::HasamiGame(m_size_x, m_size_y);
      case IDR_GAMETYPE_LOA:        return new LoA::LoAGame(m_size_x, m_size_y);
      case IDR_GAMETYPE_LOGIK:      return new Logik::LGame<Logik::PegColors, Logik::PegCount, Logik::MaxTries>();
      case IDR_GAMETYPE_MCHESS:     return new MassacreChess::MCGame(m_size_x, m_size_y);
      case IDR_GAMETYPE_SHOGI:      return new Shogi::ShogiGame(m_size_x, m_size_y);
      case IDR_GAMETYPE_TICTACTOE:  return new TicTacToe::TicTacToeGame(m_size_x, m_size_y);
      case IDR_GAMETYPE_TEMPLATE:   return new Template::TemplateGame(m_size_x, m_size_y);
      case IDR_GAMETYPE_TEST:       Test::Test::TestAll();  return new Test::TestGame(m_size_x, m_size_y);
    }
  }
  return nullptr;
}
