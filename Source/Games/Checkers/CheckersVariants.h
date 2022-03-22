
namespace Checkers
{
  using namespace BoardGamesCore;

  enum CheckerVariant : VariantCode   // recognized variants of Checkers
  {
    Standard      = '\0',          // Checkers
    International = '*',           // International Checkers
                                        
    Brazilian     = 'B',           // Brazilian Checkers
    Canadian      = 'C',           // Canadian Checkers
    Czech         = 'Z',           // Czech Checkers
    Italian       = 'I',           // Italian Checkers
    Portuguese    = 'P',           // Portuguese Checkers
    Russian       = 'R',           // Russian Checkers
    Thai          = 'H',           // Thai Checkers
    Turkish       = 'T',           // Turkish Checkers

    Anti          = 'a',           // Anti Checkers
    Corner        = 'c',           // Corner Checkers
    Hawaiian      = 'h',           // Hawaiian Checkers
    OneWay        = 'o',           // One Way Checkers
    Parachute     = 'p',           // Parachute Checkers
    Gothic        = 'g',           // Gothis Checkers
    Dameo         = 'd',           // Dameo
  };

  enum CheckerRule : Rule // multi-use rule variants
  {
    None                  = 0x0000,                // none of these rules
    JumpFurther           = 0x0001,                // promoted pieces can jump further than behind opponent's piece
    BackJump              = 0x0002,                // checkers can jump backwards
    MaxCapture            = 0x0004,                // must capture the maximum possible number of pieces
    MaxPromotedCapture    = 0x0008,                // must capture the maximum possible number of promoted pieces
    PromotedJump          = 0x0010,                // must capture with promoted piece
    ContinueJumping       = 0x0020,                // checkers can continue jumping after being promoted
    NoPromotedCapture     = 0x0040,                // checkers cannot capture promoted pieces
    CapturePromotedFirst  = 0x0080,                // capture promoted pieces first
    PromoteToQueen        = 0x0100,                // pieces promote to Queens (not Kings)
  };


  class InternationalCheckersPosition: public CheckersPosition
  {
  public:
    inline InternationalCheckersPosition(const VariantChosen& v, const PieceMapP& p, const Dimensions& d) noexcept: CheckersPosition(v, p, d) {}
  };

  class BrazilianCheckersPosition: public CheckersPosition
  {
  public:
    inline BrazilianCheckersPosition(const VariantChosen& v, const PieceMapP& p, const Dimensions& d) noexcept: CheckersPosition(v, p, d) {}
  };

  class CanadianCheckersPosition: public CheckersPosition
  {
  public:
    inline CanadianCheckersPosition(const VariantChosen& v, const PieceMapP& p, const Dimensions& d) noexcept: CheckersPosition(v, p, d) {}
  };

  class CzechCheckersPosition: public CheckersPosition
  {
  public:
    inline CzechCheckersPosition(const VariantChosen& v, const PieceMapP& p, const Dimensions& d) noexcept: CheckersPosition(v, p, d) {}
  };

  class ItalianCheckersPosition: public CheckersPosition
  {
  public:
    inline ItalianCheckersPosition(const VariantChosen& v, const PieceMapP& p, const Dimensions& d) noexcept: CheckersPosition(v, p, d) {}
    void SetStartingPosition() noexcept override;
  };

  class PortugueseCheckersPosition: public CheckersPosition
  {
  public:
    inline PortugueseCheckersPosition(const VariantChosen& v, const PieceMapP& p, const Dimensions& d) noexcept: CheckersPosition(v, p, d) {}
    void SetStartingPosition() noexcept override;
  protected:
    Rule GetRule() const noexcept override { return PromoteToQueen | JumpFurther | MaxCapture | MaxPromotedCapture; }
  };

  class RussianCheckersPosition: public CheckersPosition
  {
  public:
    inline RussianCheckersPosition(const VariantChosen& v, const PieceMapP& p, const Dimensions& d) noexcept: CheckersPosition(v, p, d) {}
  protected:
    Rule GetRule() const noexcept override { return PromoteToQueen | JumpFurther | MaxCapture | MaxPromotedCapture; }
  };

  class ThaiCheckersPosition: public CheckersPosition
  {
  public:
    inline ThaiCheckersPosition(const VariantChosen& v, const PieceMapP& p, const Dimensions& d) noexcept: CheckersPosition(v, p, d) {}
    void SetStartingPosition() noexcept override;
  protected:
    Rule GetRule() const noexcept override { return PromoteToQueen; }
  };

  class AntiCheckersPosition: public CheckersPosition
  {
  public:
    inline AntiCheckersPosition(const VariantChosen& v, const PieceMapP& p, const Dimensions& d) noexcept: CheckersPosition(v, p, d) {}
  };

  class CornerCheckersPosition: public CheckersPosition
  {
  public:
    inline CornerCheckersPosition(const VariantChosen& v, const PieceMapP& p, const Dimensions& d) noexcept: CheckersPosition(v, p, d) {}
    void SetStartingPosition() noexcept override;
  protected:
    Rule GetRule() const noexcept override { return PromoteToQueen | JumpFurther | PromotedJump; }
    bool CanPromote(const Location& l, const Piece& p) const noexcept override
    {
      return
        (p.IsColor(PieceColor::White) && l.y_ == 0 && l.x_ == 1) ||
        (p.IsColor(PieceColor::White) && l.y_ == 1 && l.x_ == 0) ||
        (p.IsColor(PieceColor::Black) && l.y_ == sizeY_ - 1 && l.x_ == sizeX_ - 2) ||
        (p.IsColor(PieceColor::Black) && l.y_ == sizeY_ - 2 && l.x_ == sizeX_ - 1);
    }
  };

  class HawaiianCheckersPosition: public CheckersPosition
  {
  public:
    inline HawaiianCheckersPosition(const VariantChosen& v, const PieceMapP& p, const Dimensions& d) noexcept: CheckersPosition(v, p, d) {}
    void SetStartingPosition() noexcept override;
  protected:
    Rule GetRule() const noexcept override { return BackJump; }
  };

  class OneWayCheckersPosition: public CheckersPosition
  {
  public:
    inline OneWayCheckersPosition(const VariantChosen& v, const PieceMapP& p, const Dimensions& d) noexcept: CheckersPosition(v, p, d) {}
    void SetStartingPosition() noexcept override;
  protected:
    Rule GetRule() const noexcept override { return MaxCapture; }
  };

  class ParachuteCheckersPosition: public CheckersPosition
  {
  public:
    inline ParachuteCheckersPosition(const VariantChosen& v, const PieceMapP& p, const Dimensions& d) noexcept: CheckersPosition(v, p, d) {}
    void SetStartingPosition() noexcept override;
  protected:
    Rule GetRule() const noexcept override { return BackJump; }
    bool CanPromote(const Location& l, const Piece& p) const noexcept override
    {
      if (p.IsKind(Checkers::Para::ThePara)) return true; // Para can always promotes (to normal piece) when jumping
      return CheckersPosition::CanPromote(l, p);
    }
  };

  class GothicCheckersPosition: public CheckersPosition
  {
  public:
    inline GothicCheckersPosition(const VariantChosen& v, const PieceMapP& p, const Dimensions& d) noexcept: CheckersPosition(v, p, d) {}
    void SetStartingPosition() noexcept override;
  protected:
    Rule GetRule() const noexcept override { return PromotedJump; }
  };

  class DameoCheckersPosition: public CheckersPosition
  {
  public:
    inline DameoCheckersPosition(const VariantChosen& v, const PieceMapP& p, const Dimensions& d) noexcept: CheckersPosition(v, p, d) {}
    void SetStartingPosition() noexcept override;
  protected:
    Rule GetRule() const noexcept override { return PromoteToQueen | JumpFurther | MaxCapture | ContinueJumping; }
  };

}
