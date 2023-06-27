
namespace Checkers
{
  using namespace BoardGamesCore;

  enum CheckersVariant : VariantCode   // recognized variants of Checkers
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

  enum CheckersRule : Rule // multi-use rule variants
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


  //class InternationalCheckersBoard: public CheckersBoard
  //{
  //public:
  //  inline InternationalCheckersBoard(const VariantChosen& v, const BoardPartDimensions& d) noexcept: CheckersBoard(v, d) {}
  //};

  //class BrazilianCheckersBoard: public CheckersBoard
  //{
  //public:
  //  inline BrazilianCheckersBoard(const VariantChosen& v, const BoardPartDimensions& d) noexcept: CheckersBoard(v, d) {}
  //};

  //class CanadianCheckersBoard: public CheckersBoard
  //{
  //public:
  //  inline CanadianCheckersBoard(const VariantChosen& v, const BoardPartDimensions& d) noexcept: CheckersBoard(v, d) {}
  //};

  //class CzechCheckersBoard: public CheckersBoard
  //{
  //public:
  //  inline CzechCheckersBoard(const VariantChosen& v, const BoardPartDimensions& d) noexcept: CheckersBoard(v, d) {}
  //};

  //class ItalianCheckersBoard: public CheckersBoard
  //{
  //public:
  //  inline ItalianCheckersBoard(const VariantChosen& v, const BoardPartDimensions& d) noexcept: CheckersBoard(v, d) {}
  //  void SetStartingBoard() noexcept override;
  //};

  //class PortugueseCheckersBoard: public CheckersBoard
  //{
  //public:
  //  inline PortugueseCheckersBoard(const VariantChosen& v, const BoardPartDimensions& d) noexcept: CheckersBoard(v, d) {}
  //  void SetStartingBoard() noexcept override;
  //protected:
  //  Rule GetRule() const noexcept override { return PromoteToQueen | JumpFurther | MaxCapture | MaxPromotedCapture; }
  //};

  //class RussianCheckersBoard: public CheckersBoard
  //{
  //public:
  //  inline RussianCheckersBoard(const VariantChosen& v, const BoardPartDimensions& d) noexcept: CheckersBoard(v, d) {}
  //protected:
  //  Rule GetRule() const noexcept override { return PromoteToQueen | JumpFurther | MaxCapture | MaxPromotedCapture; }
  //};

  //class ThaiCheckersBoard: public CheckersBoard
  //{
  //public:
  //  inline ThaiCheckersBoard(const VariantChosen& v, const BoardPartDimensions& d) noexcept: CheckersBoard(v, d) {}
  //  void SetStartingBoard() noexcept override;
  //protected:
  //  Rule GetRule() const noexcept override { return PromoteToQueen; }
  //};

  //class AntiCheckersBoard: public CheckersBoard
  //{
  //public:
  //  inline AntiCheckersBoard(const VariantChosen& v, const BoardPartDimensions& d) noexcept: CheckersBoard(v, d) {}
  //};

  //class CornerCheckersBoard: public CheckersBoard
  //{
  //public:
  //  inline CornerCheckersBoard(const VariantChosen& v, const BoardPartDimensions& d) noexcept: CheckersBoard(v, d) {}
  //  void SetStartingBoard() noexcept override;
  //protected:
  //  Rule GetRule() const noexcept override { return PromoteToQueen | JumpFurther | PromotedJump; }
  //  bool CanPromote(const Location& l, const Piece& p) const noexcept override
  //  {
  //    return
  //      (p.IsColor(PieceColor::White) && l.y_ == 0 && l.x_ == 1) ||
  //      (p.IsColor(PieceColor::White) && l.y_ == 1 && l.x_ == 0) /*||
  //      (p.IsColor(PieceColor::Black) && l.y_ == sizeY_ - 1 && l.x_ == sizeX_ - 2) ||
  //      (p.IsColor(PieceColor::Black) && l.y_ == sizeY_ - 2 && l.x_ == sizeX_ - 1)*/;
  //  }
  //};

  //class HawaiianCheckersBoard: public CheckersBoard
  //{
  //public:
  //  inline HawaiianCheckersBoard(const VariantChosen& v, const BoardPartDimensions& d) noexcept: CheckersBoard(v, d) {}
  //  void SetStartingBoard() noexcept override;
  //protected:
  //  Rule GetRule() const noexcept override { return BackJump; }
  //};

  //class OneWayCheckersBoard: public CheckersBoard
  //{
  //public:
  //  inline OneWayCheckersBoard(const VariantChosen& v, const BoardPartDimensions& d) noexcept: CheckersBoard(v, d) {}
  //  void SetStartingBoard() noexcept override;
  //protected:
  //  Rule GetRule() const noexcept override { return MaxCapture; }
  //};

  //class ParachuteCheckersBoard: public CheckersBoard
  //{
  //public:
  //  inline ParachuteCheckersBoard(const VariantChosen& v, const BoardPartDimensions& d) noexcept: CheckersBoard(v, d) {}
  //  void SetStartingBoard() noexcept override;
  //protected:
  //  Rule GetRule() const noexcept override { return BackJump; }
  //  bool CanPromote(const Location& l, const Piece& p) const noexcept override
  //  {
  //    if (p.IsKind(Checkers::Para::ThePara)) return true; // Para can always promotes (to normal piece) when jumping
  //    return CheckersBoard::CanPromote(l, p);
  //  }
  //};

  //class GothicCheckersBoard: public CheckersBoard
  //{
  //public:
  //  inline GothicCheckersBoard(const VariantChosen& v, const BoardPartDimensions& d) noexcept: CheckersBoard(v, d) {}
  //  void SetStartingBoard() noexcept override;
  //protected:
  //  Rule GetRule() const noexcept override { return PromotedJump; }
  //};

  //class DameoCheckersBoard: public CheckersBoard
  //{
  //public:
  //  inline DameoCheckersBoard(const VariantChosen& v, const BoardPartDimensions& d) noexcept: CheckersBoard(v, d) {}
  //  void SetStartingBoard() noexcept override;
  //protected:
  //  Rule GetRule() const noexcept override { return PromoteToQueen | JumpFurther | MaxCapture | ContinueJumping; }
  //};

}
