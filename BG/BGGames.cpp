#include "stdafx.h"

// this code is in a separate file as it needs to include all games' includes, and will therefore be compiled often

#include "Game LoA\LoAGame.h"
#include "Game Logik\LogikGame.h"
#include "Game Massacre Chess\MCGame.h"
#include "Game Cam\CamGame.h"
#include "Game Shogi\ShogiGame.h"
#include "Game Hasami\HasamiGame.h"
#include "Game TicTacToe\TicTacToeGame.h"
#include "Game Template\TemplateGame.h"
#include "Game Test\TestGame.h"


void BGSelect::AddGames(void)
{
  AddGame(IDR_GAMETYPE_LOA);
  AddGame(IDR_GAMETYPE_LOGIK);
  AddGame(IDR_GAMETYPE_MCHESS);
  AddGame(IDR_GAMETYPE_CAMELOT);
  AddGame(IDR_GAMETYPE_SHOGI);
  AddGame(IDR_GAMETYPE_HASAMI);
  AddGame(IDR_GAMETYPE_TICTACTOE);
  AddGame(IDR_GAMETYPE_TEMPLATE);
  AddGame(IDR_GAMETYPE_TEST);

  m_ctrl_game_variant.AddString(LR"(4x4)");
  m_ctrl_game_variant.AddString(LR"(6x6)");
  m_ctrl_game_variant.AddString(LR"(8x8)");

}


void BGSelect::SetControlsForGame(void)
{
  GetDlgItem(IDC_GAME_SELECT_SIZE_Y)->ShowWindow(SW_SHOW);
  GetDlgItem(IDC_GAME_SELECT_SIZE_LOCK)->ShowWindow(SW_SHOW);

  switch (m_game_id)
  {
    case IDR_GAMETYPE_LOA:
      m_size_x = 8;
      m_size_y = 8;
      m_ctrl_size_x.SetRange(2, 20, TRUE);
      m_ctrl_size_y.SetRange(2, 20, TRUE);
      m_size_lock = TRUE;
      break;

    case IDR_GAMETYPE_LOGIK:     
      m_size_x = 5; 
      m_size_y = 8; 
      m_ctrl_size_x.SetRange(5,  5, TRUE); 
      m_ctrl_size_y.SetRange(8,  8, TRUE); 
      m_size_lock = TRUE;   
      GetDlgItem(IDC_GAME_SELECT_SIZE_Y)->ShowWindow(SW_HIDE);
      GetDlgItem(IDC_GAME_SELECT_SIZE_LOCK)->ShowWindow(SW_HIDE);
      break;

    case IDR_GAMETYPE_MCHESS:
      m_size_x = 8;
      m_size_y = 8;
      m_ctrl_size_x.SetRange(2, 20, TRUE);
      m_ctrl_size_y.SetRange(2, 20, TRUE);
      m_size_lock = TRUE;
      break;

    case IDR_GAMETYPE_CAMELOT:
      //m_ctrl_size_x.SetPos(12);
      //m_ctrl_size_y.SetPos(16);
      m_size_x = 12;
      m_size_y = 16;
      m_ctrl_size_x.SetRange(7, 12, TRUE);
      GetDlgItem(IDC_GAME_SELECT_SIZE_Y)->ShowWindow(SW_HIDE);
      GetDlgItem(IDC_GAME_SELECT_SIZE_LOCK)->ShowWindow(SW_HIDE);
      break;

    case IDR_GAMETYPE_SHOGI:
      m_size_x = 9; 
      m_size_y = 9;
      m_ctrl_size_x.SetRange(5, 9, TRUE);
      m_ctrl_size_y.SetRange(5, 9, TRUE);
      m_size_lock = TRUE;
      GetDlgItem(IDC_GAME_SELECT_SIZE_Y)->ShowWindow(SW_HIDE);
      GetDlgItem(IDC_GAME_SELECT_SIZE_LOCK)->ShowWindow(SW_HIDE);
      break;

    case IDR_GAMETYPE_HASAMI:    
      m_size_x = 9; 
      m_size_y = 9;
      m_ctrl_size_x.SetRange(2, 20, TRUE); 
      m_ctrl_size_y.SetRange(5, 20, TRUE);
      m_size_lock = TRUE;
      break;

    case IDR_GAMETYPE_TICTACTOE: 
      m_size_x = 3; 
      m_size_y = 3; 
      m_ctrl_size_x.SetRange(3, 3, TRUE); 
      m_ctrl_size_y.SetRange(3, 3, TRUE);
      GetDlgItem(IDC_GAME_SELECT_SIZE_Y)->ShowWindow(SW_HIDE);
      GetDlgItem(IDC_GAME_SELECT_SIZE_LOCK)->ShowWindow(SW_HIDE);
      m_size_lock = TRUE;
      break;

    case IDR_GAMETYPE_TEMPLATE: 
      m_size_x = 8;
      m_size_y = 8;
      m_ctrl_size_x.SetRange(2, 20, TRUE);
      m_ctrl_size_y.SetRange(2, 20, TRUE);
      m_size_lock = TRUE;
      break;

    case IDR_GAMETYPE_TEST:
      m_size_x = 2;
      m_size_y = 4;
      m_ctrl_size_x.SetRange(2, 8, TRUE); 
      m_ctrl_size_y.SetRange(2, 8, TRUE);
      m_size_lock = FALSE;   
      break;
  }
}


// Creating the right game
BoardGamesCore::Game* BGSelect::CreateGame(void)
{
  BoardGamesCore::Game* g{ nullptr };
  if (DoModal() == IDOK) switch (m_game_id)
  {
    case IDR_GAMETYPE_LOA:        g = new LoA::LoAGame(m_size_x, m_size_y);                                                 break;
    case IDR_GAMETYPE_LOGIK:      g = new Logik::LGame<Logik::PegColors, Logik::PegCount, Logik::MaxTries>();               break;
    case IDR_GAMETYPE_MCHESS:     g = new MassacreChess::MCGame(m_size_x, m_size_y);                                        break;
    case IDR_GAMETYPE_CAMELOT:    g = new Cam::CamGame(m_size_x, m_size_y);                                                 break;
    case IDR_GAMETYPE_SHOGI:      g = new Shogi::ShogiGame(m_size_x, m_size_y);                                             break;
    case IDR_GAMETYPE_HASAMI:     g = new Hasami::HasamiGame(m_size_x, m_size_y);                                           break;
    case IDR_GAMETYPE_TICTACTOE:  g = new TicTacToe::TicTacToeGame(m_size_x, m_size_y);                                     break;
    case IDR_GAMETYPE_TEMPLATE:   g = new Template::TemplateGame(m_size_x, m_size_y);                                       break;
    case IDR_GAMETYPE_TEST:       Test::Test::TestAll();  g = new Test::TestGame(m_size_x, m_size_y);                       break;
  }
  return g;
}
