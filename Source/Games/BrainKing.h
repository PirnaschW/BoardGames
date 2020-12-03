
#include <string_view>
#include <vector>

namespace BoardGamesBK
{
  using namespace BoardGamesCore;


  class BKGame
  {
  public:
    BKGame(int no) noexcept : no_(no) {}
    VariantChosen LoadGame(std::vector<const Piece*>& list) const;

  private:

  private:
    int no_;
    static const std::unordered_set<BoardGamesCore::VariantCode> map_;
  };

}
