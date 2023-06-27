
namespace Checkers
{
  using namespace BoardGamesCore;

  enum CheckersVariant : VariantCode   // recognized variants of Checkers
  {
    Standard,         // Checkers
    International,    // International Checkers

    Brazilian,        // Brazilian Checkers
    Canadian,         // Canadian Checkers
    Czech,            // Czech Checkers
    Italian,          // Italian Checkers
    Portuguese,       // Portuguese Checkers
    Russian,          // Russian Checkers
    Thai,             // Thai Checkers
    Turkish,          // Turkish Checkers

    Anti,             // Anti Checkers
    Corner,           // Corner Checkers
    Hawaiian,         // Hawaiian Checkers
    OneWay,           // One Way Checkers
    Parachute,        // Parachute Checkers
    Gothic,           // Gothis Checkers
    Dameo,            // Dameo
  };

  enum CheckersRule : Rule // multi-use rule variants
  {
    None                  = 0x0000,     // none of these rules
    JumpFurther           = 0x0001,     // promoted pieces can jump further than behind opponent's piece
    BackJump              = 0x0002,     // checkers can jump backwards
    MaxCapture            = 0x0004,     // must capture the maximum possible number of pieces
    MaxPromotedCapture    = 0x0008,     // must capture the maximum possible number of promoted pieces
    PromotedJump          = 0x0010,     // must capture with promoted piece
    ContinueJumping       = 0x0020,     // checkers can continue jumping after being promoted
    NoPromotedCapture     = 0x0040,     // checkers cannot capture promoted pieces
    CapturePromotedFirst  = 0x0080,     // capture promoted pieces first
    PromoteToQueen        = 0x0100,     // pieces promote to Queens (not Kings)
  };

}
