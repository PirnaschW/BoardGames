#include "pch.h"
#include "BGSelect.h"

// this code is in a separate file as it needs to include all games' includes, and will therefore be compiled often

#include "Game Ataxx\AtaxxGame.h"
#include "Game Cam\CamGame.h"
#include "Game CFour\CFourGame.h"
#include "Game Checkers\CheckersGame.h"
#include "Game Cheversi\CheversiGame.h"
#include "Game Hasami\HasamiGame.h"
#include "Game Knight Fight\KnightFGame.h"
#include "Game LoA\LoAGame.h"
#include "Game Logik\LogikGame.h"
#include "Game Massacre Chess\MCGame.h"
#include "Game Shogi\ShogiGame.h"
#include "Game Tank Battle\TankBGame.h"
#include "Game TicTacToe\TicTacToeGame.h"

using namespace BoardGamesCore;

void BGSelect::AddGames(void)
{
  AddGame(IDR_GAMETYPE_ATAXX);
  AddGame(IDR_GAMETYPE_CAMELOT);
  AddGame(IDR_GAMETYPE_CFOUR);
  AddGame(IDR_GAMETYPE_CHECKERS);
  AddGame(IDR_GAMETYPE_CHEVERSI);
  AddGame(IDR_GAMETYPE_HASAMI);
  AddGame(IDR_GAMETYPE_KNIGHTF);
  AddGame(IDR_GAMETYPE_LOA);
  AddGame(IDR_GAMETYPE_LOGIK);
  AddGame(IDR_GAMETYPE_MCHESS);
  AddGame(IDR_GAMETYPE_SHOGI);
  AddGame(IDR_GAMETYPE_TANKB);
  AddGame(IDR_GAMETYPE_TICTACTOE);
}

const VariantList& BGSelect::GetVariants(int id) const
{
  switch (id)
  {
    case IDR_GAMETYPE_ATAXX:      return Variants<Ataxx::        AtaxxGame    >::GetVariants();
    case IDR_GAMETYPE_CAMELOT:    return Variants<Cam::          CamGame      >::GetVariants();
    case IDR_GAMETYPE_CFOUR:      return Variants<CFour::        CFourGame    >::GetVariants();
    case IDR_GAMETYPE_CHECKERS:   return Variants<Checkers::     CheckersGame >::GetVariants();
    case IDR_GAMETYPE_CHEVERSI:   return Variants<Cheversi::     CheversiGame >::GetVariants();
    case IDR_GAMETYPE_HASAMI:     return Variants<Hasami::       HasamiGame   >::GetVariants();
    case IDR_GAMETYPE_KNIGHTF:    return Variants<KnightF::      KnightFGame  >::GetVariants();
    case IDR_GAMETYPE_LOA:        return Variants<LoA::          LoAGame      >::GetVariants();
    case IDR_GAMETYPE_LOGIK:      return Variants<Logik::        LogikGame    >::GetVariants();
    case IDR_GAMETYPE_MCHESS:     return Variants<MassacreChess::MCGame       >::GetVariants();
    case IDR_GAMETYPE_SHOGI:      return Variants<Shogi::        ShogiGame    >::GetVariants();
    case IDR_GAMETYPE_TICTACTOE:  return Variants<TicTacToe::    TicTacToeGame>::GetVariants();
    case IDR_GAMETYPE_TANKB:      return Variants<TankB::        TankBGame    >::GetVariants();
    default:                      return Variants<TicTacToe::    TicTacToeGame>::GetVariants();
  }
}

// Creating the right game
Game* BGSelect::CreateGame(void)
{
  if (DoModal() == IDOK)
  {
    switch (m_game_id)
    {
      case IDR_GAMETYPE_ATAXX:      return new Ataxx::        AtaxxGame    (Variants<Ataxx::        AtaxxGame    >::GetPieces(), Ataxx::        AtaxxGame::    GetDimensions(m_size_x, m_size_y));
      case IDR_GAMETYPE_CAMELOT:    return new Cam::          CamGame      (Variants<Cam::          CamGame      >::GetPieces(), Cam::          CamGame::      GetDimensions(m_size_x, m_size_y));
      case IDR_GAMETYPE_CFOUR:      return new CFour::        CFourGame    (Variants<CFour::        CFourGame    >::GetPieces(), CFour::        CFourGame::    GetDimensions(m_size_x, m_size_y));
      case IDR_GAMETYPE_CHECKERS:   return new Checkers::     CheckersGame (Variants<Checkers::     CheckersGame >::GetPieces(), Checkers::     CheckersGame:: GetDimensions(m_size_x, m_size_y));
      case IDR_GAMETYPE_CHEVERSI:   return new Cheversi::     CheversiGame (Variants<Cheversi::     CheversiGame >::GetPieces(), Cheversi::     CheversiGame:: GetDimensions(m_size_x, m_size_y));
      case IDR_GAMETYPE_HASAMI:     return new Hasami::       HasamiGame   (Variants<Hasami::       HasamiGame   >::GetPieces(), Hasami::       HasamiGame::   GetDimensions(m_size_x, m_size_y));
      case IDR_GAMETYPE_KNIGHTF:    return new KnightF::      KnightFGame  (Variants<KnightF::      KnightFGame  >::GetPieces(), KnightF::      KnightFGame::  GetDimensions(m_size_x, m_size_y));
      case IDR_GAMETYPE_LOA:        return new LoA::          LoAGame      (Variants<LoA::          LoAGame      >::GetPieces(), LoA::          LoAGame::      GetDimensions(m_size_x, m_size_y));
      case IDR_GAMETYPE_LOGIK:      return new Logik::        LogikGame    (Variants<Logik::        LogikGame    >::GetPieces(), Logik::        LogikGame::    GetDimensions(m_size_x, m_size_y));
      case IDR_GAMETYPE_MCHESS:     return new MassacreChess::MCGame       (Variants<MassacreChess::MCGame       >::GetPieces(), MassacreChess::MCGame::       GetDimensions(m_size_x, m_size_y));
      case IDR_GAMETYPE_SHOGI:      return new Shogi::        ShogiGame    (Variants<Shogi::        ShogiGame    >::GetPieces(), Shogi::        ShogiGame::    GetDimensions(m_size_x, m_size_y));
      case IDR_GAMETYPE_TICTACTOE:  return new TicTacToe::    TicTacToeGame(Variants<TicTacToe::    TicTacToeGame>::GetPieces(), TicTacToe::    TicTacToeGame::GetDimensions(m_size_x, m_size_y));
      case IDR_GAMETYPE_TANKB:      return new TankB::        TankBGame    (Variants<TankB::        TankBGame    >::GetPieces(), TankB::        TankBGame::    GetDimensions(m_size_x, m_size_y));
    }
  }
  return nullptr;
}
