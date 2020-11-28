
#include <string_view>
#include <vector>

namespace BoardGamesBK
{
  using GameID = int;
  using namespace BoardGamesCore;


  class BKGame
  {
  public:
    BKGame(const char* nr) noexcept : nr_(nr) {}
    GameID LoadGame(BoardGamesCore::VariantCode& c, BoardGamesCore::Coordinate& xSize, BoardGamesCore::Coordinate& ySize, std::vector<const Piece*>& list) const;

  private:

  private:
    const char* nr_;
    static const std::unordered_set<BoardGamesCore::VariantCode> map_;
  };

}
