//#define WINDOWS_EXPORT_ALL_SYMBOLS

#include <array>
#include <functional>
#include <limits>
#include <memory>
#include <sstream>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <assert.h>

#include "Util.h"
#include "MFC.h"

#include "CoreResource.h"
#include "Log.h"
#include "Using.h"
#include "Die.h"
#include "PieceColor.h"
#include "TileColor.h"
#include "Offset.h"
#include "Location.h"
#include "PlayerType.h"
#include "Player.h"
#include "PValue.h"
#include "Kind.h"
#include "Piece.h"
#include "PieceMap.h"
#include "Action.h"
#include "Move.h"
#include "Field.h"
#include "BoardPart.h"
#include "Variant.h"
#include "Board.h"
#include "UI.h"
#include "PositionMemory.h"
#include "AI.h"
#include "Game.h"

#include "CorePieces.h"
