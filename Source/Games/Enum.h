
#include <memory>
#include <vector>

namespace BoardGamesCore
{
  class Game;

  void GetGames(std::vector<GameID>& v);
  Game* CreateGame(const VariantChosen& v);
  VariantChosen LoadFromWWW(int no, std::vector<PieceIndex>& list);
}
