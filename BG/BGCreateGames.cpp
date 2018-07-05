#include "stdafx.h"

#include "Game LoA\LoAGame.h"
#include "Game Logik\LogikGame.h"
#include "Game Massacre Chess\MCGame.h"
#include "Game Cam\CamGame.h"
#include "Game Shogi\ShogiGame.h"
#include "Game Hasami\HasamiGame.h"
#include "Game TicTacToe\TicTacToeGame.h"
#include "Game Template\TemplateGame.h"
#include "Game Test\TestGame.h"

// Creating the right game
BoardGamesCore::Game* GetGame(UINT nID)
{
  switch (nID)
  {
    case IDR_GAMETYPE_LOA:        return new LoA::LoAGame(8, 8);
    case IDR_GAMETYPE_LOGIK:      return new Logik::LGame<Logik::PegColors, Logik::PegCount, Logik::MaxTries>();
    case IDR_GAMETYPE_MCHESS4:    return new MassacreChess::MCGame(4, 4);
    case IDR_GAMETYPE_MCHESS6:    return new MassacreChess::MCGame(6, 6);
    case IDR_GAMETYPE_MCHESS8:    return new MassacreChess::MCGame(8, 8);
    case IDR_GAMETYPE_CAM:        return new Cam::MiniCamGame();
    case IDR_GAMETYPE_CAMELOT:    return new Cam::FullCamGame();
    case IDR_GAMETYPE_FULLSHOGI:  return new Shogi::FullShogiGame();
    case IDR_GAMETYPE_MINISHOGI:  return new Shogi::MiniShogiGame();
    case IDR_GAMETYPE_HASAMI:     return new Hasami::HasamiGame(9, 9);
    case IDR_GAMETYPE_TICTACTOE:  return new TicTacToe::TicTacToeGame(3, 3);
    case IDR_GAMETYPE_TEMPLATE:   return new Template::TemplateGame(8, 8);
    case IDR_GAMETYPE_TEST:       Test::Test::TestAll();  return new Test::TestGame(4, 4);
  }
  return nullptr;
}