
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

  class CFourPosition : public MainPosition
  {
  public:
    inline CFourPosition(VariantCode c, const PieceMapP& p, const Dimensions& d) noexcept : MainPosition(c, p, d) {}
    virtual inline MainPosition* Clone(void) const noexcept override { return new CFourPosition(*this); }
    virtual void GetAllMoves(void) const noexcept override;
    virtual PositionValue EvaluateStatically(void) const noexcept override;
    virtual unsigned int GetChainValue(unsigned int z) const noexcept override;
  };


  class CFourGame : public Game
  {
  private:
    CFourGame(void) = delete;

  public:
    inline CFourGame(VariantCode c, const PieceMapP& m, const Dimensions& d) noexcept : Game(m, new CFourPosition(c, m, d), new MainLayout(d)) {}
    static const VariantList& GetVariants(void) noexcept;
    static const PieceMapP& GetPieces(const VariantChosen& v) noexcept;
    static const Dimensions GetDimensions(const VariantChosen& v) noexcept;
  };

}
