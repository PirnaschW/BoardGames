#include "Games.h"

#include "Games\Ataxx\AtaxxGame.h"
#include "Games\Cam\CamGame.h"
//#include "Games\CFour\CFourGame.h"
#include "Games\Checkers\CheckersGame.h"
//#include "Games\Chess\ChessGame.h"
//#include "Games\Espionage\EspionageGame.h"
//#include "Games\Hasami\HasamiGame.h"
//#include "Games\Knight Fight\KnightFGame.h"
//#include "Games\LoA\LoAGame.h"
//#include "Games\Logik\LogikGame.h"
//#include "Games\Shogi\ShogiGame.h"
//#include "Games\Tank Battle\TankBGame.h"
//#include "Games\TicTacToe\TicTacToeGame.h"

#include "Enum.h"
#include "BrainKing.h"

namespace BoardGamesCore
{

  void GetGames(std::vector<GameID>& v)
  {
    PMap.Register(Piece::NoPiece); // not a good solution... but where?
    PMap.Register(Piece::NoTile);
    Registration<Ataxx        ::AtaxxGame    >::Register();
    Registration<Cam          ::CamGame      >::Register();
  //Registration<CFour        ::CFourGame    >::Register();
    Registration<Checkers     ::CheckersGame >::Register();
  //Registration<Chess        ::ChessGame    >::Register();
  //Registration<Espionage    ::EspionageGame>::Register();
  //Registration<Hasami       ::HasamiGame   >::Register();
  //Registration<KnightF      ::KnightFGame  >::Register();
  //Registration<LoA          ::LoAGame      >::Register();
  //Registration<Logik        ::LogikGame    >::Register();
  //Registration<Shogi        ::ShogiGame    >::Register();
  //Registration<TicTacToe    ::TicTacToeGame>::Register();
  //Registration<TankB        ::TankBGame    >::Register();

  //v.push_back(IDR_GAMETYPE_ATAXX);
  //v.push_back(IDR_GAMETYPE_CAMELOT);
  //v.push_back(IDR_GAMETYPE_CFOUR);
  //v.push_back(IDR_GAMETYPE_CHECKERS);
  //v.push_back(IDR_GAMETYPE_CHESS);
  //v.push_back(IDR_GAMETYPE_ESPIONAGE);
  //v.push_back(IDR_GAMETYPE_HASAMI);
  //v.push_back(IDR_GAMETYPE_KNIGHTF);
  //v.push_back(IDR_GAMETYPE_LOA);
  //v.push_back(IDR_GAMETYPE_LOGIK);
  //v.push_back(IDR_GAMETYPE_SHOGI);
  //v.push_back(IDR_GAMETYPE_TANKB);
  //v.push_back(IDR_GAMETYPE_TICTACTOE);

    v = Variants.GetGameIDList();
  }

  // Creating the right game
  Game* CreateGame(const VariantChosen& v, const std::vector<PieceIndex>& list)
  {
    switch (v.id)
    {
    case IDR_GAMETYPE_ATAXX:      return new Ataxx        ::AtaxxGame     (v, list, Ataxx        ::Registration<class Ataxx        ::AtaxxGame    >::GetDimensions(v));
    case IDR_GAMETYPE_CAMELOT:    return new Cam          ::CamGame       (v, list, Cam          ::Registration<class Cam          ::CamGame      >::GetDimensions(v));
  //case IDR_GAMETYPE_CFOUR:      return new CFour        ::CFourGame     (v, list, CFour        ::Registration<class CFour        ::CFourGame    >::GetDimensions(v));
    case IDR_GAMETYPE_CHECKERS:   return new Checkers     ::CheckersGame  (v, list, Checkers     ::Registration<class Checkers     ::CheckersGame >::GetDimensions(v));
  //case IDR_GAMETYPE_CHESS:      return new Chess        ::ChessGame     (v, list, Chess        ::Registration<class Chess        ::ChessGame    >::GetDimensions(v));
  //case IDR_GAMETYPE_ESPIONAGE:  return new Espionage    ::EspionageGame (v, list, Espionage    ::Registration<class Espionage    ::EspionageGame>::GetDimensions(v));
  //case IDR_GAMETYPE_HASAMI:     return new Hasami       ::HasamiGame    (v, list, Hasami       ::Registration<class Hasami       ::HasamiGame   >::GetDimensions(v));
  //case IDR_GAMETYPE_KNIGHTF:    return new KnightF      ::KnightFGame   (v, list, KnightF      ::Registration<class KnightF      ::KnightFGame  >::GetDimensions(v));
  //case IDR_GAMETYPE_LOA:        return new LoA          ::LoAGame       (v, list, LoA          ::Registration<class LoA          ::LoAGame      >::GetDimensions(v));
  //case IDR_GAMETYPE_LOGIK:      return new Logik        ::LogikGame     (v, list, Logik        ::Registration<class Logik        ::LogikGame    >::GetDimensions(v));
  //case IDR_GAMETYPE_SHOGI:      return new Shogi        ::ShogiGame     (v, list, Shogi        ::Registration<class Shogi        ::ShogiGame    >::GetDimensions(v));
  //case IDR_GAMETYPE_TICTACTOE:  return new TicTacToe    ::TicTacToeGame (v, list, TicTacToe    ::Registration<class TicTacToe    ::TicTacToeGame>::GetDimensions(v));
  //case IDR_GAMETYPE_TANKB:      return new TankB        ::TankBGame     (v, list, TankB        ::Registration<class TankB        ::TankBGame    >::GetDimensions(v));
    default:                      return nullptr;
    }
  }

  //VariantChosen LoadFromWWW(int no, std::vector<const BoardGamesCore::Piece*>& list)
  //{
  //  BoardGamesBK::BKGame g(no);
  //  return g.LoadGame(list);
  //}

}
