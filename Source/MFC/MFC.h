#include <unordered_map>
#include <sstream>
#include <string>
#include <string_view>
#include <xstring>

using Pixel = short;            // screen coordinates and distances
static_assert((std::numeric_limits<Pixel     >::max)() >= 4096, "Pixel too small for screen coordinates");

#include "MFC-Constants.h"
#include "MFC-Point.h"
#include "MFC-Rect.h"
#include "MFC-DC.h"
#include "MFC-Archive.h"
#include "MFC-Bitmap.h"
#include "MFC-CmdUI.h"
#include "MFC-Message.h"
