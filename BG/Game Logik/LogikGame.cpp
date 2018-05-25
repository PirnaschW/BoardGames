#include "stdafx.h"

#include "LogikResource.h"
#include "LogikGame.h"

namespace Logik
{

  LogikPiece const LogikPiece::LPieceB{&Peg<'B'>::ThePeg, &Color::NoColor, IDB_PEGB, IDB_PEGBF};
  LogikPiece const LogikPiece::LPieceW{&Peg<'W'>::ThePeg, &Color::NoColor, IDB_PEGW, IDB_PEGWF};
  LogikPiece const LogikPiece::LPiece1{&Peg<'1'>::ThePeg, &Color::NoColor, IDB_PEG1, IDB_PEG1F};
  LogikPiece const LogikPiece::LPiece2{&Peg<'2'>::ThePeg, &Color::NoColor, IDB_PEG2, IDB_PEG2F};
  LogikPiece const LogikPiece::LPiece3{&Peg<'3'>::ThePeg, &Color::NoColor, IDB_PEG3, IDB_PEG3F};
  LogikPiece const LogikPiece::LPiece4{&Peg<'4'>::ThePeg, &Color::NoColor, IDB_PEG4, IDB_PEG4F};
  LogikPiece const LogikPiece::LPiece5{&Peg<'5'>::ThePeg, &Color::NoColor, IDB_PEG5, IDB_PEG5F};
  LogikPiece const LogikPiece::LPiece6{&Peg<'6'>::ThePeg, &Color::NoColor, IDB_PEG6, IDB_PEG6F};
  LogikPiece const LogikPiece::LPiece7{&Peg<'7'>::ThePeg, &Color::NoColor, IDB_PEG7, IDB_PEG7F};
  LogikPiece const LogikPiece::LPiece8{&Peg<'8'>::ThePeg, &Color::NoColor, IDB_PEG8, IDB_PEG8F};


  LLayout::LLayout(unsigned int x, unsigned int y) : MainLayout(Dimension(x, y, BoardStartX, BoardStartY, FieldSizeX, FieldSizeY))
  {
    unsigned int z = 0;
    for (unsigned int i = 0; i < x; i++)
      for (unsigned int j = 0; j < y; j++, z++)
      {
        CRect r{(int)(BoardStartX + FieldSizeX * (3 + 1 + i + 0) + (i < y ? 0 : BoardFrameX) + (i < 2 * y ? 0 : 1) + (i < 3 * y ? 0 : BoardFrameX)),
                 (int)(BoardStartY + FieldSizeY * (j + 0)),
                 (int)(BoardStartX + FieldSizeX * (3 + 1 + i + 1) + (i < y ? 0 : BoardFrameX) + (i < 2 * y ? 0 : 1) + (i < 3 * y ? 0 : BoardFrameX)),
                 (int)(BoardStartY + FieldSizeY * (j + 1))};
        tiles[z] = new Tile(Location(i, j), r, FC(i, j));
      }
  }


  LStockLayout::LStockLayout(unsigned int x, unsigned int y, unsigned int z) :
    StockLayout(Dimension(x + 3, 1U, BoardStartX + FieldSizeX * (4 + 3 + 5 * y - x) / 2 + BoardFrameX, BoardStartY + FieldSizeY * (2 * z + 1) / 2, FieldSizeX, FieldSizeY)) {}

}