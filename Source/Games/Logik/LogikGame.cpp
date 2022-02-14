#include "Games.h"

#include "LogikResource.h"
#include "LogikGame.h"
using namespace BoardGamesMFC;

namespace Logik
{

  const LogikPiece LogikPiece::LPieceB{ Peg<'B'>::ThePeg, PieceColor::NoColor, IDB_PEGB };
  const LogikPiece LogikPiece::LPieceW{ Peg<'W'>::ThePeg, PieceColor::NoColor, IDB_PEGW };
  const LogikPiece LogikPiece::LPiece1{ Peg<'1'>::ThePeg, PieceColor::NoColor, IDB_PEG1 };
  const LogikPiece LogikPiece::LPiece2{ Peg<'2'>::ThePeg, PieceColor::NoColor, IDB_PEG2 };
  const LogikPiece LogikPiece::LPiece3{ Peg<'3'>::ThePeg, PieceColor::NoColor, IDB_PEG3 };
  const LogikPiece LogikPiece::LPiece4{ Peg<'4'>::ThePeg, PieceColor::NoColor, IDB_PEG4 };
  const LogikPiece LogikPiece::LPiece5{ Peg<'5'>::ThePeg, PieceColor::NoColor, IDB_PEG5 };
  const LogikPiece LogikPiece::LPiece6{ Peg<'6'>::ThePeg, PieceColor::NoColor, IDB_PEG6 };
  const LogikPiece LogikPiece::LPiece7{ Peg<'7'>::ThePeg, PieceColor::NoColor, IDB_PEG7 };
  const LogikPiece LogikPiece::LPiece8{ Peg<'8'>::ThePeg, PieceColor::NoColor, IDB_PEG8 };

 
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

 
  bool LogikPosition::SetFirstFreePeg(const Piece& p) noexcept
  {
    for (unsigned int j = 0; j < MaxTries; ++j)
      for (unsigned int i = MaxPegs; i < 2 * MaxPegs; ++i)
        if (GetPiece(Location(BoardPart::Main, i, j)) == Piece::NoPiece)
        {
          SetPiece(Location(BoardPart::Main, i, j), p);
          return true;
        }
    return false;
  }

  bool LogikPosition::SetFirstFreeMarker(const Piece& p) noexcept
  {
    // define and execute a Lambda to find the Marker target row (= the last row containing any Pegs)
    unsigned char row = [this]() -> unsigned char
    {
      for (unsigned char j = 0, z = MaxTries - 1; j < MaxTries-1; ++j, --z)              // start with last row, check backwards
        for (unsigned char i = MaxPegs; i < 2 * MaxPegs; ++i)
          if (GetPiece(Location(BoardPart::Main, i, z)) != Piece::NoPiece) return z;     // if any Peg found, that's the target row
      return 0U;                                                                         // if no Pegs were found at all, the first row is the target
    } ();

    // define a Lambda to find a free Marker slot in a given row
    auto Find = [this, &p](unsigned char j) -> bool
    {
      for (unsigned char i = 0; i < MaxPegs; ++i)                         // check all Marker slots in the given row
      {
        const Location l{ BoardPart::Main, i, j };
        if (GetPiece(l) == Piece::NoPiece)                               // found a free slot, set the Marker
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
        const Piece& p = GetPiece(Location(BoardPart::Main, MaxPegs + i, k));
        if (p.IsKind(Peg<'1'>::ThePeg)) peg[i] = 0;
        else if (p.IsKind(Peg<'2'>::ThePeg)) peg[i] = 1;
        else if (p.IsKind(Peg<'3'>::ThePeg)) peg[i] = 2;
        else if (p.IsKind(Peg<'4'>::ThePeg)) peg[i] = 3;
        else if (p.IsKind(Peg<'5'>::ThePeg)) peg[i] = 4;
        else if (p.IsKind(Peg<'6'>::ThePeg)) peg[i] = 5;
        else if (p.IsKind(Peg<'7'>::ThePeg)) peg[i] = 6;
        else if (p.IsKind(Peg<'8'>::ThePeg)) peg[i] = 7;
        else return;                                                      // ignore incomplete lines and stop reading
      }

      // read Markers
      MarkerCount b{};
      MarkerCount w{};
      for (unsigned char i = 0; i < MaxPegs; ++i)
      {
        const Piece& p = GetPiece(Location(BoardPart::Main, i, k));
        if (p.IsKind(Peg<'B'>::ThePeg)) b++;
        if (p.IsKind(Peg<'W'>::ThePeg)) w++;
      }
      prevR_[k] = Result(b, w);
      previ_[k] = plays_[peg];
      prevc_++;                                                           // log this line as valid play
    }
  }


  PlayCode LogikPosition::GetBestMove(unsigned int nThreads) const        // Evaluate best next move
  {
    auto f = [this](PlayCode min, PlayCode max_)
    {
      for (PlayCode c = min; c < max_; ++c)
      {
        const unsigned int lmax = EvaluatePlay(c);
        if (lmax > 0U) CollectBestPlays(c, lmax);
      }
    };

    std::vector<std::unique_ptr<std::thread>> tt{};
    mx = new std::mutex;
    for (unsigned int t = 0U; t < nThreads; ++t)
    {
      PlayCode min{ Plays::Max / nThreads * t };
      PlayCode max_{ Plays::Max / nThreads * (t + 1) };
      tt.push_back(std::make_unique<std::thread>(f, min, max_));
    }
    for (unsigned int t = 0U; t < nThreads; ++t)
    {
      tt[t]->join();
    }
    delete mx;

    if (bestI.size() == 0) throw 0;                                       // no moves are possible, there is a contradiction in the data
    return bestI[rand() % bestI.size()];                                  // randomly pick one of the equally good tries
  }

  unsigned int LogikPosition::EvaluatePlay(const PlayCode& c) const noexcept
  {
    for (unsigned int k = 0; k < prevc_; ++k)                             // compare play to all previous plays_
    {                                                                     
      if (c == previ_[k]) return 0U;                                      // if we tried this before, don't try it again
      if (prevR_[k] != Result{ plays_[c], plays_[previ_[k]] }) return 0U; // if the result doesn't match, that can't be the opponent's solutions, so don't even try it
    }                                                                     
    unsigned int RCCount[Result::RMax()]{};                               // counter for how often each of the results happened
    for (PlayCode j = 0; j < Plays::Max; ++j)                             // for all possible opponent's solutions
    {                                                                     
      bool match{ true };                                                 
      for (unsigned int k = 0; match && (k < prevc_); ++k)                // compare play to all previous plays_
      {                                                                   
        match = (prevR_[k] == Result{ plays_[j], plays_[previ_[k]] });    // if the result doesn't match, that can't be the opponent's solutions
      }                                                                   
                                                                          
      if (match)                                                          
      {                                                                   
        const Result r(plays_[c], plays_[j]);                             // create result for the current play
        RCCount[r]++;                                                     // increment counter for this result
      }
    }

    unsigned int lmax = 0U;
    for (unsigned int k = 0; k < Result::RMax(); ++k)
    {
      if (RCCount[k] > lmax)
      {
        lmax = RCCount[k];
      }
    }

    return lmax;
  }

  void LogikPosition::CollectBestPlays(const PlayCode& c, unsigned int lmax) const noexcept
  {
    mx->lock();                                                            // lock the mutex
    if (lmax < gmin)                                                      // did we find a better move?
    {                                                                     
      gmin = lmax;                                                        // save best 'worst count' and its index
      bestI.clear();                                                      
      bestI.push_back(c);                                                 
    }                                                                     
    else if (lmax == gmin)                                                // found an equally good move, collect it
    {                                                                     
      bestI.push_back(c);                                                 // save the index
    }                                                                     
    mx->unlock();                                                          // unlock the mutex
  }


  void LogikPosition::Execute(const PlayCode& c)
  {
    previ_[prevc_] = c;
    for (unsigned int i = 0; i < MaxPegs; ++i)
    {
      SetPiece(Location(BoardPart::Main, MaxPegs + i, prevc_), LogikPiece::GetPiece(plays_[c][i]));
    }
    prevc_++;
  }


  LogikLayout::LogikLayout(const Dimensions& d) noexcept : MainLayout(d, LayoutType::None)
  {
    unsigned int z = 0;
    for (Coordinate i = 0; i < d[0].xCount_; i++)
    {
      const int dx = (i < d[0].xCount_ / 4 ? 0 : BoardFrameX) + (i < 2 * d[0].xCount_ / 4 ? 0 : 1) + (i < 3 * d[0].xCount_ / 4 ? 0 : BoardFrameX);
      for (Coordinate j = 0; j < d[0].yCount_; j++, z++)
      {
        const Rect r{
          (int)(dim_.lEdge_ + dim_.xSkip_ * i + dim_.xDim_ * (i + 0U) + dx),
          (int)(dim_.tEdge_ + dim_.ySkip_ * j + dim_.yDim_ * (j + 0U)),
          (int)(dim_.lEdge_ + dim_.xSkip_ * i + dim_.xDim_ * (i + 1U) + dx),
          (int)(dim_.tEdge_ + dim_.ySkip_ * j + dim_.yDim_ * (j + 1U))
        };
        tiles_[z] = new Tile(Location(BoardPart::Main, i, j), r, FC(i, j));
      }
    }
  }

  void LogikLayout::Draw(DC* dc, const MainPosition* pos, _Mode mode) const
  {
    MainLayout::Draw(dc, pos, mode);       // let the (generic) parent draw the board itself

    // frame around the complete board (corrected, as Layout:: wouldn't know about extra pixels)
    for (unsigned int z = 4; z > 0; z--)
    {
      if (z != 2)
        dc->Rectangle(Rect{ (int) (dim_.lEdge_ - z), (int) (dim_.tEdge_ - z), (int) (dim_.lEdge_ + dim_.xCount_ * (dim_.xDim_ + dim_.xSkip_) + 2 * BoardFrameX + 1 + z), (int) (dim_.tEdge_ + dim_.yCount_ * (dim_.yDim_ + dim_.ySkip_) + z) });
    }

    // frame around the inner board
    for (unsigned int z = BoardFrameX; z > 0; z--)
    {
      if (z != 2)
        dc->Rectangle(Rect{
          static_cast<int>(dim_.lEdge_ + (1 * MaxPegs) * (dim_.xDim_ + dim_.xSkip_) + 0 + BoardFrameX - z),
          static_cast<int>(dim_.tEdge_ - z),
          static_cast<int>(dim_.lEdge_ + (3 * MaxPegs) * (dim_.xDim_ + dim_.xSkip_) + 1 + BoardFrameX + z),
          static_cast<int>(dim_.tEdge_ + (MaxTries * (dim_.yDim_ + dim_.ySkip_)) + z) });
    }
  }

  const TileColor& LogikLayout::FC(Coordinate i, Coordinate j) const noexcept
  {
    if ((i < dim_.xCount_ / 4) || (i >= 3 * dim_.xCount_ / 4)) return TileColor::Light;
    if ((j < dim_.yCount_ - 1) || (i < 2 * dim_.xCount_ / 4)) return TileColor::Dark;
    return TileColor::Light;
  }


  bool LogikGame::React(unsigned int nChar, unsigned int, unsigned int)  // react to keyboard input (not menu shortcuts, but typing)
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
          return lpos->SetFirstFreePeg(LogikPiece::GetPiece(c));
        }
      case 'B':
      case 'b':
        return lpos->SetFirstFreeMarker(LogikPiece::LPieceB);
      case 'W':
      case 'w':
        return lpos->SetFirstFreeMarker(LogikPiece::LPieceW);
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
      BoardGamesMFC::AfxMessageBox(L"No moves are possible - input is contradictory");
      return false;
    }
    lpos->Execute(e);
    NextPlayer();
    return true;
  }

  const VariantList& LogikGame::GetVariants(void) noexcept
  {
    static VariantList v{
      { Variant{ "8 Colors", '8', 5, 7, 2, 8, 2, 16 } },
      { Variant{ "6 Colors", '6', 5, 6, 2, 8, 2, 16 } },
      { Variant{ "4 Colors", '4', 5, 5, 2, 8, 2, 16 } },
    };
    return v;
  }

  const Dimensions LogikGame::GetDimensions(const VariantChosen& v) noexcept
  {
    Dimensions d{
       Dimension(4 * v.x, v.y, BoardStartX, BoardStartY, FieldSizeX, FieldSizeY, 1, 1),
       Dimension(6, 2, BoardStartX + FieldSizeX * (v.x + 1), BoardStartY + v.y * FieldSizeY + FieldSizeY / 2, FieldSizeX, FieldSizeY),
       Dimension(v.x + 3, 1U, BoardStartX + FieldSizeX * (4 + 3 + 5 * v.y - v.x) / 2 + BoardFrameX, BoardStartY + FieldSizeY * (2 * MaxTries + 1) / 2, FieldSizeX, FieldSizeY),
    };
    return d;
  }

  const PieceMapP& LogikGame::GetPieces(const VariantChosen& v) noexcept
  {
    static const PieceMapP p = std::make_shared<PieceMap>();
    p->Empty();
    p->Add(LogikPiece::LPieceB);
    p->Add(LogikPiece::LPieceW);
    if constexpr (MaxColors > 0) p->Add(LogikPiece::LPiece1);
    if constexpr (MaxColors > 1) p->Add(LogikPiece::LPiece2);
    if constexpr (MaxColors > 2) p->Add(LogikPiece::LPiece3);
    if constexpr (MaxColors > 3) p->Add(LogikPiece::LPiece4);
    if constexpr (MaxColors > 4) p->Add(LogikPiece::LPiece5);
    if constexpr (MaxColors > 5) p->Add(LogikPiece::LPiece6);
    if constexpr (MaxColors > 6) p->Add(LogikPiece::LPiece7);
    if constexpr (MaxColors > 7) p->Add(LogikPiece::LPiece8);
    return p;
  }

}