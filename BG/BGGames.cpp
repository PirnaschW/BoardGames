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
    case IDR_GAMETYPE_LOGIK:      return Variants<Logik::LogikGame>::GetVariants();
    case IDR_GAMETYPE_MCHESS:     return Variants<MassacreChess::MCGame>::GetVariants();
    case IDR_GAMETYPE_SHOGI:      return Variants<Shogi::ShogiGame>::GetVariants();
    case IDR_GAMETYPE_TICTACTOE:  return Variants<TicTacToe::TicTacToeGame>::GetVariants();
    case IDR_GAMETYPE_TEMPLATE:   
//    case IDR_GAMETYPE_TEST:       return Variants<Test::TestGame>::GetVariants();
    default:                      return Variants<Template::TemplateGame>::GetVariants();
  }
}


// Creating the right game
BoardGamesCore::Game* BGSelect::CreateGame(void)
{
  if (DoModal() == IDOK)
  {
    switch (m_game_id)
    {
      case IDR_GAMETYPE_CAMELOT:    return new Cam::          CamGame      (Variants<Cam::          CamGame      >::GetPieces(), Cam::          CamGame::      GetDimensions(m_size_x, m_size_y));
      case IDR_GAMETYPE_CHECKERS:   return new Checkers::     CheckersGame (Variants<Checkers::     CheckersGame >::GetPieces(), Checkers::     CheckersGame:: GetDimensions(m_size_x, m_size_y));
      case IDR_GAMETYPE_HASAMI:     return new Hasami::       HasamiGame   (Variants<Hasami::       HasamiGame   >::GetPieces(), Hasami::       HasamiGame::   GetDimensions(m_size_x, m_size_y));
      case IDR_GAMETYPE_LOA:        return new LoA::          LoAGame      (Variants<LoA::          LoAGame      >::GetPieces(), LoA::          LoAGame::      GetDimensions(m_size_x, m_size_y));
      case IDR_GAMETYPE_LOGIK:      return new Logik::        LogikGame    (Variants<Logik::        LogikGame    >::GetPieces(), Logik::        LogikGame::    GetDimensions(m_size_x, m_size_y));
      case IDR_GAMETYPE_MCHESS:     return new MassacreChess::MCGame       (Variants<MassacreChess::MCGame       >::GetPieces(), MassacreChess::MCGame::       GetDimensions(m_size_x, m_size_y));
      case IDR_GAMETYPE_SHOGI:      return new Shogi::        ShogiGame    (Variants<Shogi::        ShogiGame    >::GetPieces(), Shogi::        ShogiGame::    GetDimensions(m_size_x, m_size_y));
      case IDR_GAMETYPE_TICTACTOE:  return new TicTacToe::    TicTacToeGame(Variants<TicTacToe::    TicTacToeGame>::GetPieces(), TicTacToe::    TicTacToeGame::GetDimensions(m_size_x, m_size_y));
      case IDR_GAMETYPE_TEMPLATE:   return new Template::     TemplateGame (Variants<Template::     TemplateGame >::GetPieces(), Template::     TemplateGame:: GetDimensions(m_size_x, m_size_y));
      default:                      return new Template::     TemplateGame (Variants<Template::     TemplateGame >::GetPieces(), Template::     TemplateGame:: GetDimensions(m_size_x, m_size_y));
        //      case IDR_GAMETYPE_TEST:
//        Test::Test::TestAll();      return new Test::         TestGame     (Variants<Test::         TestGame     >::GetPieces(), m_size_x, m_size_y);
    }
  }
  return nullptr;
}
