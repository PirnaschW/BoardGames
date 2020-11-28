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
  Game* CreateGame(GameID id, VariantCode c, Coordinate xSize, Coordinate ySize)
  {
    switch (id)
    {
    case IDR_GAMETYPE_ATAXX:      return new Ataxx        ::AtaxxGame     (c, Variants<Ataxx        ::AtaxxGame    >::GetPieces(c), Ataxx        ::AtaxxGame    ::GetDimensions(c, xSize, ySize));
    case IDR_GAMETYPE_CAMELOT:    return new Cam          ::CamGame       (c, Variants<Cam          ::CamGame      >::GetPieces(c), Cam          ::CamGame      ::GetDimensions(c, xSize, ySize));
    case IDR_GAMETYPE_CFOUR:      return new CFour        ::CFourGame     (c, Variants<CFour        ::CFourGame    >::GetPieces(c), CFour        ::CFourGame    ::GetDimensions(c, xSize, ySize));
    case IDR_GAMETYPE_CHECKERS:   return new Checkers     ::CheckersGame  (c, Variants<Checkers     ::CheckersGame >::GetPieces(c), Checkers     ::CheckersGame ::GetDimensions(c, xSize, ySize));
    case IDR_GAMETYPE_CHEVERSI:   return new Cheversi     ::CheversiGame  (c, Variants<Cheversi     ::CheversiGame >::GetPieces(c), Cheversi     ::CheversiGame ::GetDimensions(c, xSize, ySize));
    case IDR_GAMETYPE_ESPIONAGE:  return new Espionage    ::EspionageGame (c, Variants<Espionage    ::EspionageGame>::GetPieces(c), Espionage    ::EspionageGame::GetDimensions(c, xSize, ySize));
    case IDR_GAMETYPE_HASAMI:     return new Hasami       ::HasamiGame    (c, Variants<Hasami       ::HasamiGame   >::GetPieces(c), Hasami       ::HasamiGame   ::GetDimensions(c, xSize, ySize));
    case IDR_GAMETYPE_KNIGHTF:    return new KnightF      ::KnightFGame   (c, Variants<KnightF      ::KnightFGame  >::GetPieces(c), KnightF      ::KnightFGame  ::GetDimensions(c, xSize, ySize));
    case IDR_GAMETYPE_LOA:        return new LoA          ::LoAGame       (c, Variants<LoA          ::LoAGame      >::GetPieces(c), LoA          ::LoAGame      ::GetDimensions(c, xSize, ySize));
    case IDR_GAMETYPE_LOGIK:      return new Logik        ::LogikGame     (c, Variants<Logik        ::LogikGame    >::GetPieces(c), Logik        ::LogikGame    ::GetDimensions(c, xSize, ySize));
    case IDR_GAMETYPE_MCHESS:     return new MassacreChess::MCGame        (c, Variants<MassacreChess::MCGame       >::GetPieces(c), MassacreChess::MCGame       ::GetDimensions(c, xSize, ySize));
    case IDR_GAMETYPE_SHOGI:      return new Shogi        ::ShogiGame     (c, Variants<Shogi        ::ShogiGame    >::GetPieces(c), Shogi        ::ShogiGame    ::GetDimensions(c, xSize, ySize));
    case IDR_GAMETYPE_TICTACTOE:  return new TicTacToe    ::TicTacToeGame (c, Variants<TicTacToe    ::TicTacToeGame>::GetPieces(c), TicTacToe    ::TicTacToeGame::GetDimensions(c, xSize, ySize));
    case IDR_GAMETYPE_TANKB:      return new TankB        ::TankBGame     (c, Variants<TankB        ::TankBGame    >::GetPieces(c), TankB        ::TankBGame    ::GetDimensions(c, xSize, ySize));
    default:                      return nullptr;
    }
  }

  GameID LoadFromWWW(VariantCode& c, Coordinate& xSize, Coordinate& ySize, std::vector<const Piece*>& list)
  {
    char no[10] = "8569856";
    int z = 8569856;
    sprintf_s(no, "%7d", z);
    BoardGamesBK::BKGame g(no);
    GameID id = g.LoadGame(c, xSize, ySize, list);
    return g.LoadGame(c, xSize, ySize, list);
  }

}
