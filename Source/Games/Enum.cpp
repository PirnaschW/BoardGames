#include "Games.h"

#include "Games\Ataxx\AtaxxGame.h"
#include "Games\Cam\CamGame.h"
#include "Games\CFour\CFourGame.h"
#include "Games\Checkers\CheckersGame.h"
#include "Games\Chess\ChessGame.h"
#include "Games\Cheversi\CheversiGame.h"
#include "Games\Espionage\EspionageGame.h"
#include "Games\Hasami\HasamiGame.h"
#include "Games\Knight Fight\KnightFGame.h"
#include "Games\LoA\LoAGame.h"
#include "Games\Logik\LogikGame.h"
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
    v.push_back(IDR_GAMETYPE_CHESS);
    v.push_back(IDR_GAMETYPE_CHEVERSI);
    v.push_back(IDR_GAMETYPE_ESPIONAGE);
    v.push_back(IDR_GAMETYPE_HASAMI);
    v.push_back(IDR_GAMETYPE_KNIGHTF);
    v.push_back(IDR_GAMETYPE_LOA);
    v.push_back(IDR_GAMETYPE_LOGIK);
    v.push_back(IDR_GAMETYPE_SHOGI);
    v.push_back(IDR_GAMETYPE_TANKB);
    v.push_back(IDR_GAMETYPE_TICTACTOE);
  }

  const VariantList* GetVariants(GameID id)
  {
    switch (id)
    {
    case IDR_GAMETYPE_ATAXX:      return &Variants<Ataxx        ::AtaxxGame    >::GetVariants();
    case IDR_GAMETYPE_CAMELOT:    return &Variants<Cam          ::CamGame      >::GetVariants();
    case IDR_GAMETYPE_CFOUR:      return &Variants<CFour        ::CFourGame    >::GetVariants();
    case IDR_GAMETYPE_CHECKERS:   return &Variants<Checkers     ::CheckersGame >::GetVariants();
    case IDR_GAMETYPE_CHESS:      return &Variants<Chess        ::ChessGame    >::GetVariants();
    case IDR_GAMETYPE_CHEVERSI:   return &Variants<Cheversi     ::CheversiGame >::GetVariants();
    case IDR_GAMETYPE_ESPIONAGE:  return &Variants<Espionage    ::EspionageGame>::GetVariants();
    case IDR_GAMETYPE_HASAMI:     return &Variants<Hasami       ::HasamiGame   >::GetVariants();
    case IDR_GAMETYPE_KNIGHTF:    return &Variants<KnightF      ::KnightFGame  >::GetVariants();
    case IDR_GAMETYPE_LOA:        return &Variants<LoA          ::LoAGame      >::GetVariants();
    case IDR_GAMETYPE_LOGIK:      return &Variants<Logik        ::LogikGame    >::GetVariants();
    case IDR_GAMETYPE_SHOGI:      return &Variants<Shogi        ::ShogiGame    >::GetVariants();
    case IDR_GAMETYPE_TICTACTOE:  return &Variants<TicTacToe    ::TicTacToeGame>::GetVariants();
    case IDR_GAMETYPE_TANKB:      return &Variants<TankB        ::TankBGame    >::GetVariants();
    default:                      return &Variants<TicTacToe    ::TicTacToeGame>::GetVariants();
    }
  }

  // Creating the right game
  Game* CreateGame(const VariantChosen& v)
  {
    switch (v.id)
    {
    case IDR_GAMETYPE_ATAXX:      return new Ataxx        ::AtaxxGame     (v, Variants<class Ataxx        ::AtaxxGame    >::GetPieces(v), Ataxx        ::Variants<class Ataxx        ::AtaxxGame    >::GetDimensions(v));
    case IDR_GAMETYPE_CAMELOT:    return new Cam          ::CamGame       (v, Variants<class Cam          ::CamGame      >::GetPieces(v), Cam          ::Variants<class Cam          ::CamGame      >::GetDimensions(v));
    case IDR_GAMETYPE_CFOUR:      return new CFour        ::CFourGame     (v, Variants<class CFour        ::CFourGame    >::GetPieces(v), CFour        ::Variants<class CFour        ::CFourGame    >::GetDimensions(v));
    case IDR_GAMETYPE_CHECKERS:   return new Checkers     ::CheckersGame  (v, Variants<class Checkers     ::CheckersGame >::GetPieces(v), Checkers     ::Variants<class Checkers     ::CheckersGame >::GetDimensions(v));
    case IDR_GAMETYPE_CHESS:      return new Chess        ::ChessGame     (v, Variants<class Chess        ::ChessGame    >::GetPieces(v), Chess        ::Variants<class Chess        ::ChessGame    >::GetDimensions(v));
    case IDR_GAMETYPE_CHEVERSI:   return new Cheversi     ::CheversiGame  (v, Variants<class Cheversi     ::CheversiGame >::GetPieces(v), Cheversi     ::Variants<class Cheversi     ::CheversiGame >::GetDimensions(v));
    case IDR_GAMETYPE_ESPIONAGE:  return new Espionage    ::EspionageGame (v, Variants<class Espionage    ::EspionageGame>::GetPieces(v), Espionage    ::Variants<class Espionage    ::EspionageGame>::GetDimensions(v));
    case IDR_GAMETYPE_HASAMI:     return new Hasami       ::HasamiGame    (v, Variants<class Hasami       ::HasamiGame   >::GetPieces(v), Hasami       ::Variants<class Hasami       ::HasamiGame   >::GetDimensions(v));
    case IDR_GAMETYPE_KNIGHTF:    return new KnightF      ::KnightFGame   (v, Variants<class KnightF      ::KnightFGame  >::GetPieces(v), KnightF      ::Variants<class KnightF      ::KnightFGame  >::GetDimensions(v));
    case IDR_GAMETYPE_LOA:        return new LoA          ::LoAGame       (v, Variants<class LoA          ::LoAGame      >::GetPieces(v), LoA          ::Variants<class LoA          ::LoAGame      >::GetDimensions(v));
    case IDR_GAMETYPE_LOGIK:      return new Logik        ::LogikGame     (v, Variants<class Logik        ::LogikGame    >::GetPieces(v), Logik        ::Variants<class Logik        ::LogikGame    >::GetDimensions(v));
    case IDR_GAMETYPE_SHOGI:      return new Shogi        ::ShogiGame     (v, Variants<class Shogi        ::ShogiGame    >::GetPieces(v), Shogi        ::Variants<class Shogi        ::ShogiGame    >::GetDimensions(v));
    case IDR_GAMETYPE_TICTACTOE:  return new TicTacToe    ::TicTacToeGame (v, Variants<class TicTacToe    ::TicTacToeGame>::GetPieces(v), TicTacToe    ::Variants<class TicTacToe    ::TicTacToeGame>::GetDimensions(v));
    case IDR_GAMETYPE_TANKB:      return new TankB        ::TankBGame     (v, Variants<class TankB        ::TankBGame    >::GetPieces(v), TankB        ::Variants<class TankB        ::TankBGame    >::GetDimensions(v));
    default:                      return nullptr;
    }
  }

  VariantChosen LoadFromWWW(int no, std::vector<const BoardGamesCore::Piece*>& list)
  {
    BoardGamesBK::BKGame g(no);
    return g.LoadGame(list);
  }

}
