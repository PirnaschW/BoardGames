
#include <memory>
#include <vector>

namespace BoardGamesCore
{
  class Game;
  class Piece;

  void GetGames(std::vector<GameID>& v);
  const VariantList* GetVariants(GameID id);
  Game* CreateGame(const VariantChosen& v);
  VariantChosen LoadFromWWW(int no, std::vector<const Piece*>& list);
}
