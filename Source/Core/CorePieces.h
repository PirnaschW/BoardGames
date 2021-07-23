
namespace BoardGamesCore
{

  // Core brings only two very basic piece - a black and a white 'Checker', which don't know how to move
  class Checker : public Kind
  {
  protected:
    constexpr inline Checker(void) noexcept : Kind('C') {}
  public:
    virtual inline unsigned int GetValue(const MainPosition& /*p*/, const Location& /*l*/) const noexcept override { return 100; }

  public:
    static const Checker TheChecker;
  };


  class CorePiece final : public Piece
  {
  private:
    CorePiece(const Kind& k, const PieceColor& c, unsigned int ID) : Piece(k, c, ID) {}
    constexpr CorePiece(const CorePiece&) = delete;
    constexpr CorePiece& operator=(const CorePiece&) = delete;

  public:
    static const CorePiece WC;  // Basic White Checker
    static const CorePiece BC;  // Basic Black Checker
  };

}
