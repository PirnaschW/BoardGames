#include "stdafx.h"

#include "LogikResource.h"
#include "LogikGame.h"

namespace Logik
{

  const LogikPiece LogikPiece::LPieceB{ &Peg<'B'>::ThePeg, &Color::NoColor, IDB_PEGB, IDB_PEGBF };
  const LogikPiece LogikPiece::LPieceW{ &Peg<'W'>::ThePeg, &Color::NoColor, IDB_PEGW, IDB_PEGWF };
  const LogikPiece LogikPiece::LPiece1{ &Peg<'1'>::ThePeg, &Color::NoColor, IDB_PEG1, IDB_PEG1F };
  const LogikPiece LogikPiece::LPiece2{ &Peg<'2'>::ThePeg, &Color::NoColor, IDB_PEG2, IDB_PEG2F };
  const LogikPiece LogikPiece::LPiece3{ &Peg<'3'>::ThePeg, &Color::NoColor, IDB_PEG3, IDB_PEG3F };
  const LogikPiece LogikPiece::LPiece4{ &Peg<'4'>::ThePeg, &Color::NoColor, IDB_PEG4, IDB_PEG4F };
  const LogikPiece LogikPiece::LPiece5{ &Peg<'5'>::ThePeg, &Color::NoColor, IDB_PEG5, IDB_PEG5F };
  const LogikPiece LogikPiece::LPiece6{ &Peg<'6'>::ThePeg, &Color::NoColor, IDB_PEG6, IDB_PEG6F };
  const LogikPiece LogikPiece::LPiece7{ &Peg<'7'>::ThePeg, &Color::NoColor, IDB_PEG7, IDB_PEG7F };
  const LogikPiece LogikPiece::LPiece8{ &Peg<'8'>::ThePeg, &Color::NoColor, IDB_PEG8, IDB_PEG8F };

 
  Result::Result(const Play& p1, const Play& p2) noexcept                 // get the result code from comparing two plays_
  {
    bool u1[MaxPegs]{ false };
    bool u2[MaxPegs]{ false };
    MarkerCount b{};
    MarkerCount w{};
    for (unsigned int i = 0; i < MaxPegs; ++i)
    {
      if (p1[i] == p2[i])
      {
        b++;
        u1[i] = u2[i] = true;                                             // mark them as 'handled'
      }
    }

    for (unsigned int i = 0; i < MaxPegs; ++i)
    {
      if (!u1[i])
      {
        for (unsigned int j = 0; j < MaxPegs; ++j)
        {
          if ((i != j) && (!u2[j]))
          {
            if (p1[i] == p2[j])
            {
              w++;
              u1[i] = u2[j] = true;                                       // mark them as 'handled'
              break;                                                      // end the inner (j) loop, as the Peg at i is now 'used'
            }
          }
        }
      }
    }
    code_ = Convert(b, w);
  }

 
  bool LogikPosition::SetFirstFreePeg(const Piece* p) noexcept
  {
    for (unsigned int j = 0; j < MaxTries; ++j)
      for (unsigned int i = MaxPegs; i < 2 * MaxPegs; ++i)
        if (GetPiece(Location(BoardPart::Main, i, j)) == &Piece::NoPiece)
        {
          SetPiece(Location(BoardPart::Main, i, j), p);
          return true;
        }
    return false;
  }

  bool LogikPosition::SetFirstFreeMarker(const Piece* p) noexcept
  {
    // define and execute a Lambda to find the Marker target row (= the last row containing any Pegs)
    unsigned char row = [this]() -> unsigned char
    {
      for (unsigned char j = MaxTries - 1; j >= 0; --j)                   // start with last row, check backwards
        for (unsigned char i = MaxPegs; i < 2 * MaxPegs; ++i)
          if (GetPiece(Location(BoardPart::Main, i, j)) != &Piece::NoPiece) return j;  // if any Peg found, that's the target row
      return 0U;                                                          // if no Pegs were found at all, the first row is the target
    } ();

    // define a Lambda to find a free Marker slot in a given row
    auto Find = [this, p](unsigned char j) -> bool
    {
      for (unsigned char i = 0; i < MaxPegs; ++i)                         // check all Marker slots in the given row
      {
        const Location l{ BoardPart::Main, i, j };
        if (GetPiece(l) == &Piece::NoPiece)                               // found a free slot, set the Marker
        {
          SetPiece(l, p);
          return true;
        }
      }
      return false;
    };

    if (Find(row)) return true;                                           // if a slot was found, fine
    if (row < MaxTries - 1) return Find(++row);                           // else (if there are more rows) try the next row
    return false;                                                         // no good slot found
  }

  void LogikPosition::ReadPosition(void) noexcept
  {
    prevc_ = 0;                                                           // number of moves already made
    for (unsigned int k = 0; k < MaxTries; ++k)
    {
      PlayCfg peg{};
      for (unsigned int i = 0; i < MaxPegs; ++i)
      {
        const Piece* p = GetPiece(Location(BoardPart::Main, MaxPegs + i, k));
        if (p->IsKind(Peg<'1'>::ThePeg)) peg[i] = 0;
        else if (p->IsKind(Peg<'2'>::ThePeg)) peg[i] = 1;
        else if (p->IsKind(Peg<'3'>::ThePeg)) peg[i] = 2;
        else if (p->IsKind(Peg<'4'>::ThePeg)) peg[i] = 3;
        else if (p->IsKind(Peg<'5'>::ThePeg)) peg[i] = 4;
        else if (p->IsKind(Peg<'6'>::ThePeg)) peg[i] = 5;
        else if (p->IsKind(Peg<'7'>::ThePeg)) peg[i] = 6;
        else if (p->IsKind(Peg<'8'>::ThePeg)) peg[i] = 7;
        else return;                                                      // ignore incomplete lines and stop reading
      }

      // read Markers
      MarkerCount b{};
      MarkerCount w{};
      for (unsigned char i = 0; i < MaxPegs; ++i)
      {
        const Piece* p = GetPiece(Location(BoardPart::Main, i, k));
        if (p->IsKind(Peg<'B'>::ThePeg)) b++;
        if (p->IsKind(Peg<'W'>::ThePeg)) w++;
      }
      prevR_[k] = Result(b, w);
      previ_[k] = plays_[peg];
      prevc_++;                                                           // log this line as valid play
    }
  }

  PlayCode LogikPosition::GetBestMove(void) const                         // Evaluate best next move
  {
    unsigned int gmin{ Plays::Max };
    std::vector<PlayCode> bestI{};

    for (PlayCode i = 0; i < Plays::Max; ++i)                             // for all possible plays_ we could try
    {
      [this, i, &gmin, &bestI]
      {
        for (unsigned int k = 0; k < prevc_; ++k)                         // compare play to all previous plays_
        {
          if (i == previ_[k]) return;                                     // if we tried this before, don't try it again
          if (prevR_[k] != Result{ plays_[i], plays_[previ_[k]] }) return;// if the result doesn't match, that can't be the opponent's solutions, so don't even try it
        }
        unsigned int RCCount[Result::RMax()]{};                           // counter for how often each of the results happened
        for (PlayCode j = 0; j < Plays::Max; ++j)                         // for all possible opponent's solutions
        {
          bool match{ true };
          for (unsigned int k = 0; match && (k < prevc_); ++k)            // compare play to all previous plays_
          {
            match = (prevR_[k] == Result{ plays_[j], plays_[previ_[k]] });// if the result doesn't match, that can't be the opponent's solutions
          }

          if (match)
          {
            const Result r(plays_[i], plays_[j]);                         // create result for the current play
            RCCount[r]++;                                                 // increment counter for this result
          }
        }

        unsigned int lmax = 0;                                            // worst count found
        for (unsigned int k = 0; k < Result::RMax(); ++k)
        {
          if (RCCount[k] > lmax)
          {
            lmax = RCCount[k];
          }
        }

        if (lmax == 0);                                                   // ignore useless moves
        else if (lmax < gmin)                                             // did we find a better move?
        {
          gmin = lmax;                                                    // save best 'worst count' and its index
          bestI.clear();
          bestI.push_back(i);
        }
        else if (lmax == gmin)                                            // found an equally good move, collect it
        {
          bestI.push_back(i);                                             // save the index
        }
      }();
    }

    if (bestI.size() == 0) throw 0;                                       // no moves are possible, there is a contradiction in the data
    return bestI[rand() % bestI.size()];                                  // randomly pick one of the equally good tries
  }

  void LogikPosition::Execute(const PlayCode& c)
  {
    previ_[prevc_] = c;
    for (unsigned int i = 0; i < MaxPegs; ++i)
    {
      SetPiece(Location(BoardPart::Main, MaxPegs + i, prevc_), &LogikPiece::GetPiece(plays_[c][i]));
    }
    prevc_++;
  }


  LogikLayout::LogikLayout(const Dimensions& d) noexcept : MainLayout(d, LayoutType::None)
  {
    unsigned int z = 0;
    for (Coordinate i = 0; i < d[0].xCount; i++)
    {
      const int dx = (i < d[0].xCount / 4 ? 0 : BoardFrameX) + (i < 2 * d[0].xCount / 4 ? 0 : 1) + (i < 3 * d[0].xCount / 4 ? 0 : BoardFrameX);
      for (Coordinate j = 0; j < d[0].yCount; j++, z++)
      {
        const CRect r{
          (int)(dim.lEdge + dim.xSkip * i + dim.xDim * (i + 0U) + dx),
          (int)(dim.tEdge + dim.ySkip * j + dim.yDim * (j + 0U)),
          (int)(dim.lEdge + dim.xSkip * i + dim.xDim * (i + 1U) + dx),
          (int)(dim.tEdge + dim.ySkip * j + dim.yDim * (j + 1U))
        };
        tiles[z] = new Tile(Location(BoardPart::Main, i, j), r, FC(i, j));
      }
    }
  }

  void LogikLayout::Draw(CDC* pDC, const MainPosition* pos, _Mode mode) const
  {
    MainLayout::Draw(pDC, pos, mode);       // let the (generic) parent draw the board itself


    CGdiObject* pOld = pDC->SelectStockObject(NULL_BRUSH);
    // frame around the complete board (corrected, as Layout:: wouldn't know about extra pixels)
    for (unsigned int z = 4; z > 0; z--)
    {
      if (z != 2)
        pDC->Rectangle((int)(dim.lEdge - z), (int)(dim.tEdge - z), (int)(dim.lEdge + dim.xCount * (dim.xDim + dim.xSkip) + 2 * BoardFrameX + 1 + z), (int)(dim.tEdge + dim.yCount * (dim.yDim + dim.ySkip) + z));
    }

    // frame around the inner board
    for (unsigned int z = BoardFrameX; z > 0; z--)
    {
      if (z != 2)
        pDC->Rectangle(
          static_cast<int>(dim.lEdge + (1 * MaxPegs) * (dim.xDim + dim.xSkip) + 0 + BoardFrameX - z),
          static_cast<int>(dim.tEdge - z),
          static_cast<int>(dim.lEdge + (3 * MaxPegs) * (dim.xDim + dim.xSkip) + 1 + BoardFrameX + z),
          static_cast<int>(dim.tEdge + (MaxTries * (dim.yDim + dim.ySkip)) + z));
    }
    pDC->SelectObject(pOld);
  }

  const TileColor* LogikLayout::FC(Coordinate i, Coordinate j) const noexcept
  {
    if ((i < dim.xCount / 4) || (i >= 3 * dim.xCount / 4)) return &TileColor::Light;
    if ((j < dim.yCount - 1) || (i < 2 * dim.xCount / 4)) return &TileColor::Dark;
    return &TileColor::Light;
  }


  bool LogikGame::React(UINT nChar, UINT, UINT)  // react to keyboard input (not menu shortcuts, but typing)
  {
    LogikPosition* const lpos = dynamic_cast<LogikPosition*>(pos);
    assert(lpos != nullptr);

    SetCurrentPlayer(1);
    switch (nChar)
    {
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
        if (nChar > '0' + MaxColors) return false;
        {
          const unsigned int c = nChar - '1';
          return lpos->SetFirstFreePeg(&LogikPiece::GetPiece(c));
        }
      case 'B':
      case 'b':
        return lpos->SetFirstFreeMarker(&LogikPiece::LPieceB);
      case 'W':
      case 'w':
        return lpos->SetFirstFreeMarker(&LogikPiece::LPieceW);
      case '\r':
        AIMove();
        return true;
    }
    return false;
  }

  bool LogikGame::AIMove(void)
  {
    LogikPosition* lpos = dynamic_cast<LogikPosition*>(pos);
    assert(lpos != nullptr);

    // read all user input from screen (this allows to set up or modify existing moves and responses)
    lpos->ReadPosition();

    PlayCode e{};
    try { e = lpos->GetBestMove(); }
    catch (int) // no possible move found
    {
      ::AfxMessageBox(L"No moves are possible - input is contradictory");
      return false;
    }
    lpos->Execute(e);
    NextPlayer();
    return true;
  }

  const VariantList& LogikGame::GetVariants(void) noexcept
  {
    static VariantList v{
      { Variant{ 5, 7, "8 Colors", 2, 8, 2, 16 } },
      { Variant{ 5, 6, "6 Colors", 2, 8, 2, 16 } },
      { Variant{ 5, 5, "4 Colors", 2, 8, 2, 16 } },
    };
    return v;
  }

  const Dimensions& LogikGame::GetDimensions(Coordinate x, Coordinate y) noexcept
  {
    static Dimensions d{
       Dimension(4 * x, y, BoardStartX, BoardStartY, FieldSizeX, FieldSizeY, 1, 1),
       Dimension(15, 2, BoardStartX + FieldSizeX * (x + 1), BoardStartY + y * FieldSizeY + FieldSizeY / 2, FieldSizeX, FieldSizeY),
       Dimension(x + 3, 1U, BoardStartX + FieldSizeX * (4 + 3 + 5 * y - x) / 2 + BoardFrameX, BoardStartY + FieldSizeY * (2 * MaxTries + 1) / 2, FieldSizeX, FieldSizeY),
    };
    return d;
  }

  const PieceMapP& LogikGame::GetPieces(void) noexcept
  {
    static const PieceMapP p = std::make_shared<PieceMap>();
    p->Empty();
    p->Add(&LogikPiece::LPieceB);
    p->Add(&LogikPiece::LPieceW);
    if (MaxColors > 0) p->Add(&LogikPiece::LPiece1);
    if (MaxColors > 1) p->Add(&LogikPiece::LPiece2);
    if (MaxColors > 2) p->Add(&LogikPiece::LPiece3);
    if (MaxColors > 3) p->Add(&LogikPiece::LPiece4);
    if (MaxColors > 4) p->Add(&LogikPiece::LPiece5);
    if (MaxColors > 5) p->Add(&LogikPiece::LPiece6);
    if (MaxColors > 6) p->Add(&LogikPiece::LPiece7);
    if (MaxColors > 7) p->Add(&LogikPiece::LPiece8);
    return p;
  }

}