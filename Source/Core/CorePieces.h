
namespace BoardGamesCore
{

  // Core brings only two very basic piece - a black and a white 'Checker', which DO NOT KNOW how to move
  class Checker : public Kind
  {
  protected:
    constexpr inline Checker() noexcept : Kind('0') {}
  public:
    void CollectMoves(const Board&, const Location&, Moves&) const noexcept override {};
    constexpr inline PositionValue GetValue(const Board& /*p*/, const Location& /*l*/) const noexcept override { return 100; }

  public:
    static const Checker TheChecker;
  };
  constexpr inline const Checker Checker::TheChecker{};

  static_assert(&Checker::TheChecker, "TheChecker is not constexpr");


  class CorePiece final : public Piece
  {
  private:
    constexpr CorePiece(const Kind& k, const PieceColor& c, unsigned int ID) : Piece(k, c, ID) {}
    constexpr ~CorePiece() = default;
    constexpr CorePiece(const CorePiece&) = delete;
    constexpr CorePiece& operator=(const CorePiece&) = delete;

  public:
    static const CorePiece WC;  // Basic White Checker
    static const CorePiece BC;  // Basic Black Checker
  };

  constexpr inline const CorePiece CorePiece::WC{ Checker::TheChecker, PieceColor::White, IDB_W0L };
  constexpr inline const CorePiece CorePiece::BC{ Checker::TheChecker, PieceColor::Black, IDB_B0L };

  static_assert(CorePiece::WC != CorePiece::BC, "CorePieces are not constexpr");

}
