
#include <memory>
#include <vector>

namespace BoardGamesCore
{
  class Game;

  void GetGames(std::vector<GameID>& v);
  Game* CreateGame(const VariantChosen& v, const std::vector<PieceIndex>& list);
  VariantChosen LoadFromWWW(int no, std::vector<PieceIndex>& list);
}
