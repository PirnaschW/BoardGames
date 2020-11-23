
#include <memory>
#include <vector>

namespace BoardGamesCore
{
  using GameID = int;
  class Game;

  void GetGames(std::vector<GameID>& v);
  const VariantList* GetVariants(GameID id);
  Game* CreateGame(GameID id, VariantCode c, Coordinate xSize, Coordinate ySize);
}
