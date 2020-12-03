#include "Games.h"

#include "Games\Ataxx\AtaxxGame.h"
#include "Games\Cam\CamGame.h"
#include "Games\CFour\CFourGame.h"
#include "Games\Checkers\CheckersGame.h"
#include "Games\Cheversi\CheversiGame.h"
#include "Games\Espionage\EspionageGame.h"
#include "Games\Hasami\HasamiGame.h"
#include "Games\Knight Fight\KnightFGame.h"
#include "Games\LoA\LoAGame.h"
#include "Games\Logik\LogikGame.h"
#include "Games\Massacre Chess\MCGame.h"
#include "Games\Shogi\ShogiGame.h"
#include "Games\Tank Battle\TankBGame.h"
#include "Games\TicTacToe\TicTacToeGame.h"

#include "Enum.h"
#include "BrainKing.h"

namespace BoardGamesCore
{

  void GetGames(std::vector<GameID>& v)
  {
    v.push_back(IDR_GAMETYPE_ATAXX);
    v.push_back(IDR_GAMETYPE_CAMELOT);
    v.push_back(IDR_GAMETYPE_CFOUR);
    v.push_back(IDR_GAMETYPE_CHECKERS);
    v.push_back(IDR_GAMETYPE_CHEVERSI);
    v.push_back(IDR_GAMETYPE_ESPIONAGE);
    v.push_back(IDR_GAMETYPE_HASAMI);
    v.push_back(IDR_GAMETYPE_KNIGHTF);
    v.push_back(IDR_GAMETYPE_LOA);
    v.push_back(IDR_GAMETYPE_LOGIK);
    v.push_back(IDR_GAMETYPE_MCHESS);
    v.push_back(IDR_GAMETYPE_SHOGI);
    v.push_back(IDR_GAMETYPE_TANKB);
    v.push_back(IDR_GAMETYPE_TICTACTOE);
  }

  const VariantList* GetVariants(GameID id)
  {
    switch (id)
    {
    case IDR_GAMETYPE_ATAXX:      return &Variants<Ataxx::AtaxxGame    >::GetVariants();
    case IDR_GAMETYPE_CAMELOT:    return &Variants<Cam::CamGame      >::GetVariants();
    case IDR_GAMETYPE_CFOUR:      return &Variants<CFour::CFourGame    >::GetVariants();
    case IDR_GAMETYPE_CHECKERS:   return &Variants<Checkers::CheckersGame >::GetVariants();
    case IDR_GAMETYPE_CHEVERSI:   return &Variants<Cheversi::CheversiGame >::GetVariants();
    case IDR_GAMETYPE_ESPIONAGE:  return &Variants<Espionage::EspionageGame>::GetVariants();
    case IDR_GAMETYPE_HASAMI:     return &Variants<Hasami::HasamiGame   >::GetVariants();
    case IDR_GAMETYPE_KNIGHTF:    return &Variants<KnightF::KnightFGame  >::GetVariants();
    case IDR_GAMETYPE_LOA:        return &Variants<LoA::LoAGame      >::GetVariants();
    case IDR_GAMETYPE_LOGIK:      return &Variants<Logik::LogikGame    >::GetVariants();
    case IDR_GAMETYPE_MCHESS:     return &Variants<MassacreChess::MCGame       >::GetVariants();
    case IDR_GAMETYPE_SHOGI:      return &Variants<Shogi::ShogiGame    >::GetVariants();
    case IDR_GAMETYPE_TICTACTOE:  return &Variants<TicTacToe::TicTacToeGame>::GetVariants();
    case IDR_GAMETYPE_TANKB:      return &Variants<TankB::TankBGame    >::GetVariants();
    default:                      return &Variants<TicTacToe::TicTacToeGame>::GetVariants();
    }
  }

  // Creating the right game
  Game* CreateGame(const VariantChosen& v)
  {
    switch (v.id)
    {
    case IDR_GAMETYPE_ATAXX:      return new Ataxx        ::AtaxxGame     (v.c, Variants<Ataxx        ::AtaxxGame    >::GetPieces(v.c), Ataxx        ::AtaxxGame    ::GetDimensions(v.c, v.x, v.y));
    case IDR_GAMETYPE_CAMELOT:    return new Cam          ::CamGame       (v.c, Variants<Cam          ::CamGame      >::GetPieces(v.c), Cam          ::CamGame      ::GetDimensions(v.c, v.x, v.y));
    case IDR_GAMETYPE_CFOUR:      return new CFour        ::CFourGame     (v.c, Variants<CFour        ::CFourGame    >::GetPieces(v.c), CFour        ::CFourGame    ::GetDimensions(v.c, v.x, v.y));
    case IDR_GAMETYPE_CHECKERS:   return new Checkers     ::CheckersGame  (v.c, Variants<Checkers     ::CheckersGame >::GetPieces(v.c), Checkers     ::CheckersGame ::GetDimensions(v.c, v.x, v.y));
    case IDR_GAMETYPE_CHEVERSI:   return new Cheversi     ::CheversiGame  (v.c, Variants<Cheversi     ::CheversiGame >::GetPieces(v.c), Cheversi     ::CheversiGame ::GetDimensions(v.c, v.x, v.y));
    case IDR_GAMETYPE_ESPIONAGE:  return new Espionage    ::EspionageGame (v.c, Variants<Espionage    ::EspionageGame>::GetPieces(v.c), Espionage    ::EspionageGame::GetDimensions(v.c, v.x, v.y));
    case IDR_GAMETYPE_HASAMI:     return new Hasami       ::HasamiGame    (v.c, Variants<Hasami       ::HasamiGame   >::GetPieces(v.c), Hasami       ::HasamiGame   ::GetDimensions(v.c, v.x, v.y));
    case IDR_GAMETYPE_KNIGHTF:    return new KnightF      ::KnightFGame   (v.c, Variants<KnightF      ::KnightFGame  >::GetPieces(v.c), KnightF      ::KnightFGame  ::GetDimensions(v.c, v.x, v.y));
    case IDR_GAMETYPE_LOA:        return new LoA          ::LoAGame       (v.c, Variants<LoA          ::LoAGame      >::GetPieces(v.c), LoA          ::LoAGame      ::GetDimensions(v.c, v.x, v.y));
    case IDR_GAMETYPE_LOGIK:      return new Logik        ::LogikGame     (v.c, Variants<Logik        ::LogikGame    >::GetPieces(v.c), Logik        ::LogikGame    ::GetDimensions(v.c, v.x, v.y));
    case IDR_GAMETYPE_MCHESS:     return new MassacreChess::MCGame        (v.c, Variants<MassacreChess::MCGame       >::GetPieces(v.c), MassacreChess::MCGame       ::GetDimensions(v.c, v.x, v.y));
    case IDR_GAMETYPE_SHOGI:      return new Shogi        ::ShogiGame     (v.c, Variants<Shogi        ::ShogiGame    >::GetPieces(v.c), Shogi        ::ShogiGame    ::GetDimensions(v.c, v.x, v.y));
    case IDR_GAMETYPE_TICTACTOE:  return new TicTacToe    ::TicTacToeGame (v.c, Variants<TicTacToe    ::TicTacToeGame>::GetPieces(v.c), TicTacToe    ::TicTacToeGame::GetDimensions(v.c, v.x, v.y));
    case IDR_GAMETYPE_TANKB:      return new TankB        ::TankBGame     (v.c, Variants<TankB        ::TankBGame    >::GetPieces(v.c), TankB        ::TankBGame    ::GetDimensions(v.c, v.x, v.y));
    default:                      return nullptr;
    }
  }

  VariantChosen LoadFromWWW(int no, std::vector<const BoardGamesCore::Piece*>& list)
  {
    BoardGamesBK::BKGame g(no);
    return g.LoadGame(list);
  }

}
