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

 
  LogikBoard::LogikBoard(const VariantChosen& v, const BoardPartDimensions& d) noexcept : Board(v, d),
    resultL_(d[3], (BoardPartID) LogikBoardPartID::ResultL, PMap[Piece::NoPiece]),
    resultR_(d[4], (BoardPartID) LogikBoardPartID::ResultR, PMap[Piece::NoPiece])
  {
    // fill Stock
    int i = 0;

    auto append = [this, &i] (const Piece & p) { stock_.SetPieceIndex(PMap[p], i / 2U, i % 2U); ++i; };

    append(LogikPiece::LPieceB);
    append(LogikPiece::LPieceW);
    append(LogikPiece::LPiece1);
    append(LogikPiece::LPiece2);
    append(LogikPiece::LPiece3);
    append(LogikPiece::LPiece4);
    switch (v.c)
    {
      case Logik::Logik4:
        break;

      case Logik::Logik6:
        append(LogikPiece::LPiece5);
        append(LogikPiece::LPiece6);
        break;
      case Logik::Logik8:
        append(LogikPiece::LPiece5);
        append(LogikPiece::LPiece6);
        append(LogikPiece::LPiece7);
        append(LogikPiece::LPiece8);
        break;

      default: assert(false);
    }
  }

  bool LogikBoard::SetFirstFreePeg(PieceIndex pI) noexcept
  {
    for (unsigned int j = 0; j < MaxTries; ++j)
      for (unsigned int i = 0; i < MaxPegs; ++i)
        if (GetPieceIndex(i, j, BoardPartID::Stage) == PMap[Piece::NoPiece])
        {
          SetPieceIndex(pI, i, j, BoardPartID::Stage);
          return true;
        }
    return false;
  }

  bool LogikBoard::SetFirstFreeMarker(PieceIndex pI) noexcept
  {
    // define and execute a Lambda to find the Marker target row (= the last row containing any Pegs)
    unsigned char row = [this]() -> unsigned char
    {
      for (unsigned char j = 0, z = MaxTries - 1; j < MaxTries-1; ++j, --z)              // start with last row, check backwards
        for (unsigned char i = 0; i < MaxPegs; ++i)
          if (GetPieceIndex(i, z, BoardPartID::Stage) != PMap[Piece::NoPiece]) return z;     // if any Peg found, that's the target row
      return 0U;                                                                         // if no Pegs were found at all, the first row is the target
    } ();

    // define a Lambda to find a free Marker slot in a given row
    auto Find = [this, &pI](unsigned char j) -> bool
    {
      for (unsigned char i = 0; i < MaxPegs; ++i)                         // check all Marker slots in the given row
      {
        if (resultL_.GetPieceIndex(i, j) == PMap[Piece::NoPiece])         // found a free slot, set the Marker
        {
          resultL_.SetPieceIndex(pI, i, j);
          return true;
        }
      }
      return false;
    };

    if (Find(row)) return true;                                           // if a slot was found, fine
    if (row < MaxTries - 1) return Find(++row);                           // else (if there are more rows) try the next row
    return false;                                                         // no good slot found
  }

  void LogikBoard::ReadBoard() noexcept
  {
    prevc_ = 0;                                                           // number of moves already made
    for (unsigned int k = 0; k < MaxTries; ++k)
    {
      PlayCfg peg{};
      for (unsigned int i = 0; i < MaxPegs; ++i)
      {
        const Piece& p = PMap[GetPieceIndex(i, k, BoardPartID::Stage)];
        if      (p.IsKind(Peg<'1'>::ThePeg)) peg[i] = 0;
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
        const Piece& p = PMap[resultL_.GetPieceIndex(i, k)];
        if (p.IsKind(Peg<'B'>::ThePeg)) b++;
        if (p.IsKind(Peg<'W'>::ThePeg)) w++;
      }
      prevR_[k] = Result(b, w);
      previ_[k] = plays_[peg];
      prevc_++;                                                           // log this line as valid play
    }
  }

  PlayCode LogikBoard::GetBestMove(unsigned int nThreads) const        // EvaluateAB best next move
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

  unsigned int LogikBoard::EvaluatePlay(const PlayCode& c) const noexcept
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

  void LogikBoard::CollectBestPlays(const PlayCode& c, unsigned int lmax) const noexcept
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

  void LogikBoard::Execute(const PlayCode& c)
  {
    previ_[prevc_] = c;
    for (unsigned int i = 0; i < MaxPegs; ++i)
    {
      SetPieceIndex(LogikPiece::GetPieceIndex(plays_[c][i]), i, prevc_, BoardPartID::Stage);
    }
    prevc_++;
  }

  void LogikBoard::Draw(DC* dc, bool showstock) const                    // draw the complete board
  {
    Board::Draw(dc, showstock);

    resultL_.DrawFrame(dc);
    resultL_.Draw(dc);
    resultR_.DrawFrame(dc);
    resultR_.Draw(dc);
  }


  // TODO: correct Logik Game drag&drop (AI always at move)

  bool LogikGame::React(unsigned int command)     // react to button/menu command
  {
    switch (command)
    {
      case ID_EDIT_MOVE: return AIMove();   // completely ignore Finite State Machine, simply have the computer make a move
      default: return Game::React(command);
    }
  }

  bool LogikGame::React(unsigned int nChar, unsigned int, unsigned int)  // react to keyboard input (not menu shortcuts, but typing)
  {
    LogikBoard* const lpos = static_cast<LogikBoard*>(board_);
    assert(lpos != nullptr);

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
          return lpos->SetFirstFreePeg(LogikPiece::GetPieceIndex(c));
        }
      case 'B':
      case 'b':
        return lpos->SetFirstFreeMarker(PMap[LogikPiece::LPieceB]);
      case 'W':
      case 'w':
        return lpos->SetFirstFreeMarker(PMap[LogikPiece::LPieceW]);
      default:
        return false;
    }
  }

  bool LogikGame::AIMove()
  {
    LogikBoard* lpos = static_cast<LogikBoard*>(board_);
    assert(lpos != nullptr);

    // read all user input from screen (this allows to set up or modify existing moves and responses)
    lpos->ReadBoard();

    PlayCode e{};
    try { e = lpos->GetBestMove(); }
    catch (int) // no possible move found
    {
      BoardGamesMFC::AfxMessageBox(L"No moves are possible - input is contradictory");
      return false;
    }
    lpos->Execute(e);
//    NextPlayer();
    return true;
  }

  void LogikGame::Register() noexcept
  {
    PMap.Register(LogikPiece::LPiece1);
    PMap.Register(LogikPiece::LPiece2);
    PMap.Register(LogikPiece::LPiece3);
    PMap.Register(LogikPiece::LPiece4);
    PMap.Register(LogikPiece::LPiece5);
    PMap.Register(LogikPiece::LPiece6);
    PMap.Register(LogikPiece::LPiece7);
    PMap.Register(LogikPiece::LPiece8);
    PMap.Register(LogikPiece::LPieceB);
    PMap.Register(LogikPiece::LPieceW);

    Variants.Register(Variant(0, IDR_GAMETYPE_LOGIK, "8 Colors", Logik::Logik8, 5, 7));
    Variants.Register(Variant(0, IDR_GAMETYPE_LOGIK, "6 Colors", Logik::Logik6, 5, 6));
    Variants.Register(Variant(0, IDR_GAMETYPE_LOGIK, "4 Colors", Logik::Logik4, 5, 5));
  }

  const BoardPartDimensions LogikGame::GetDimensions(const VariantChosen& v) noexcept
  {
    constexpr Pixel ed = 1;   // extra distance between tiles in x and y directions
    BoardPartDimensions d{
       BoardPartDimension(2 * v.x, v.y, LayoutType::Dark, BoardStartX + 2 * 4 + (ed + FieldSizeX) * v.x, BoardStartY, FieldSizeX, FieldSizeY, ed, ed),
       BoardPartDimension(6, 2, LayoutType::Light, BoardStartX + FieldSizeX * (v.x + 1), BoardStartY + v.y * FieldSizeY + FieldSizeY / 2, FieldSizeX, FieldSizeY),
       BoardPartDimension(v.x + 3, 1U, LayoutType::Light, BoardStartX + FieldSizeX * (4 + 3 + 5 * v.y - v.x) / 2 + BoardFrameX, BoardStartY + FieldSizeY * (2 * MaxTries + 1) / 2, FieldSizeX, FieldSizeY),
       BoardPartDimension(v.x, v.y, LayoutType::Light, BoardStartX, BoardStartY, FieldSizeX, FieldSizeY, ed, ed),
       BoardPartDimension(v.x, v.y, LayoutType::Light, BoardStartX + 4 * 4 + (ed + FieldSizeX) * v.x * 3, BoardStartY, FieldSizeX, FieldSizeY, ed, ed),
    };
    return d;
  }

}