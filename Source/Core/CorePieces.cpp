#include "Core.h"

namespace BoardGamesCore
{

  const Checker Checker::TheChecker{};
  const CorePiece CorePiece::WC{ Checker::TheChecker, PieceColor::White, IDB_W0L };
  const CorePiece CorePiece::BC{ Checker::TheChecker, PieceColor::Black, IDB_B0L };

}
