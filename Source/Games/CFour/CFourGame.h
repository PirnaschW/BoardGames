
namespace CFour
{

  // Display Dimensions
  constexpr unsigned int FieldSizeX = 50;   // pixels per tile
  constexpr unsigned int FieldSizeY = 50;   // pixels per tile

  constexpr unsigned int FieldSizeSX = 20;  // pixels per small tile
  constexpr unsigned int FieldSizeSY = 18;  // pixels per small tile

  constexpr unsigned int BoardStartX = 30;  // offset for drawing the board
  constexpr unsigned int BoardStartY = 30;  // offset for drawing the board

  using namespace BoardGamesCore;

  enum CFourVariant : VariantCode   // recognized variants of CFour
  {
    Classic = 0x00,
    Linetris = 0x01,
    Spider = 0x02,
    Anti = 0x04,
    SpiderLinetris = Linetris | Spider,
  };

  class CFourBoard : public Board
  {
  public:
    CFourBoard(const VariantChosen& v, const BoardPartDimensions& d) noexcept;
    virtual inline Board* Clone() const noexcept override { return new CFourBoard(*this); }
    virtual void SetStartingBoard() noexcept override {};
    virtual void GetAllMoves() const noexcept override;
    //virtual PositionValue EvaluateStatically() const noexcept override;
  };


  class CFourGame : public Game
  {
  private:
    CFourGame() = delete;

  public:
    inline CFourGame(const VariantChosen& v, const std::vector<PieceIndex>& list, const BoardPartDimensions& d) noexcept : Game(v, list, new CFourBoard(v, d)) {}
    static void Register() noexcept;
    //static const VariantList& GetVariants() noexcept;
    static const BoardPartDimensions GetDimensions(const VariantChosen& v) noexcept;
  };

}
