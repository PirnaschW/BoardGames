#include "stdafx.h"

#include "Game Test/TestGame.h"

namespace BoardGamesCore
{

  Move&& Move::operator =(Move&& m) noexcept
  {
    std::swap(step, m.step);
    value = m.value;
    return std::move(*this);
  }

  bool Move::IsTake(void) const noexcept
  {
    for (auto& s : step) if (s.IsTake()) return true;
    return false;
  }

  const std::vector<Location> Move::GetJumped(void) const  // return list of jumped-over locations
  {
    std::vector<Location> jumped{};
    for (auto& s : step)
    {
      if (s.GetType() == Step::StepType::Jump) throw ("code missing!"); // TODO
    }
    return jumped;
  }

  const std::unordered_map<std::string, const Piece*>& Piece::GetHTMLPieceMap(void) noexcept
  {
    static const std::unordered_map<std::string, const Piece*> map{
      { R"(blank35)",     &Piece::NoPiece },
      { R"(chess/wk35)",&BoardGamesChessPieces::ChessPiece::WK },
      { R"(chess/wq35)",&BoardGamesChessPieces::ChessPiece::WQ },
      { R"(chess/wr35)",&BoardGamesChessPieces::ChessPiece::WR },
      { R"(chess/wb35)",&BoardGamesChessPieces::ChessPiece::WB },
      { R"(chess/wn35)",&BoardGamesChessPieces::ChessPiece::WN },
      { R"(chess/wp35)",&BoardGamesChessPieces::ChessPiece::WP },
      { R"(chess/bk35)",&BoardGamesChessPieces::ChessPiece::BK },
      { R"(chess/bq35)",&BoardGamesChessPieces::ChessPiece::BQ },
      { R"(chess/br35)",&BoardGamesChessPieces::ChessPiece::BR },
      { R"(chess/bb35)",&BoardGamesChessPieces::ChessPiece::BB },
      { R"(chess/bn35)",&BoardGamesChessPieces::ChessPiece::BN },
      { R"(chess/bp35)",&BoardGamesChessPieces::ChessPiece::BP },
    //{ R"(hshogi/w35)",&Hasami::HasamiPiece::HasamiPieceW },
    //{ R"(hshogi/b35)",&Hasami::HasamiPiece::HasamiPieceB },
    };
    return map;
  }


  std::vector<const Piece*> Piece::ListFromHTML(std::string s, const std::unordered_map<std::string, const Piece*>& map)
  {
    const static std::string s0{ R"(<div id="game-board-section">)" };
    const static std::string s1{ R"(<img src=https://i.brainking.com/)" };
    const static std::string s2{ R"(.gif)" };
    const static std::string s9{ R"(<div id="game-info-section">)" };

    std::vector<const Piece*> list{};

    std::string::size_type p1{ s.find(s0) };
    std::string::size_type p9{ s.find(s9,p1) };
    while ((p1 = s.find(s1, p1)) < p9)
    {
      p1 += s1.size();
      auto p2 = s.find(s2, p1);
      const auto& it{ map.find(s.substr(p1, p2 - p1)) };
      if (it != map.end()) list.push_back(it->second);
      else;// throw s.substr(p1, p2 - p1);
    }
    return list;
  }


  std::size_t Position::GetHash(void) const noexcept
  {
    if (hash) return hash;
    std::size_t z{};
    for (auto& p : pieces)
    {
      z ^= pm->GetPiece(p)->GetHash() + 0x9e3779b9 + (z << 6) + (z >> 2);
    }
    return hash = z;
  }

  void Position::SetPosition(std::vector<const Piece*> list)
  {
    unsigned int z{};
    for (const auto& p : list)
    {
      while (pieces[z] == pm->GetIndex(&Piece::NoTile)) z++; // skip non-existing fields
      pieces[z++] = pm->GetIndex(p);
    }
  }


  void MainPosition::GetAllMoves(void) // collect all moves for all pieces
  {
    assert(movelistW.empty());
    assert(movelistB.empty());

    movelistW.reserve(20);
    movelistB.reserve(20);
    for (Coordinate i = 0; i < sizeX; i++)
    {
      for (Coordinate j = 0; j < sizeY; j++)
      {
        const Piece* p = GetPiece(Location(i, j));
        assert(p != nullptr);
        if (!p->IsColor(&Color::NoColor))
        {
          p->CollectMoves(*this, Location(i, j), p->IsColor(&Color::White) ? movelistW : movelistB);
          assert(Test::Test::TestTaken(this));
        }
      }
    }
    assert(Test::Test::TestTaken(this));
    assert(Test::Test::TestMoveUndo(this));
  }

  bool MainPosition::JumpsOnly(std::vector<Move>& moves) const noexcept
  {
    // if there are any takes (catching opponent pieces) possible, remove all non-takes
    if (std::find_if(moves.begin(), moves.end(), [](const Move& m) {return m.GetSteps().front().IsTake(); }) == moves.end()) return false;
    moves.erase(std::remove_if(moves.begin(), moves.end(), [](const Move& m) {return !m.GetSteps().front().IsTake(); }), moves.end());
    return true;
  }

  void MainPosition::NextPlayer(void) noexcept
  {
    if (onTurn == &Color::White) onTurn = &Color::Black;
    else onTurn = &Color::White;
  };

  void MainPosition::PreviousPlayer(void) noexcept
  {
    if (onTurn == &Color::White) onTurn = &Color::Black;
    else onTurn = &Color::White;
  };

  const std::vector<const Piece*> MainPosition::Execute(const Move& m)
  {
    movelistW.clear();                                                    // after the move is executed, the movelists will be outdated
    movelistB.clear();                                                    // after the move is executed, the movelists will be outdated

    sequence.push_back(m);                                                // save the move in the sequence

    std::vector<const Piece*> taken{};
    const std::vector<Step> step{ m.GetSteps() };
    const Piece* p = m.GetFr().GetPiece();

    // collect taken pieces
    for (auto& s : step)
    {
      if (s.GetType() & Step::StepType::Promote) p = p->Promote(true);

      const std::vector<Field>& take = s.GetTake();
      for (auto& t : take)
      {
        assert(t.GetPiece() == GetPiece(t.GetLocation()));                // verify that the piece we expect is really there
        taken.push_back(t.GetPiece());                                    // collect potential pieces taken (target tile or jumped over)
        SetPiece(t.GetLocation(), &Piece::NoPiece);                       // remove the taken piece from the board
      }
    }

    SetPiece(m.GetFr().GetLocation(), &Piece::NoPiece);                   // empty the start tile
    SetPiece(m.GetTo().GetLocation(), m.GetTo().GetPiece());              // place the piece at the target field

    NextPlayer();                                                         // after the move, it's the next player's turn
    return taken;                                                         // return the collected taken pieces
  }

  void MainPosition::Undo(const Move& m)
  {
    const std::vector<Step> step = m.GetSteps();
    SetPiece(m.GetTo().GetLocation(), &Piece::NoPiece);                   // empty the target field
    for (auto& s : step)                                                  // for all steps
    {
      const std::vector<Field>& take = s.GetTake();
      for (auto& t : take) SetPiece(t.GetLocation(), t.GetPiece());       // Put the taken piece(s) back on the board
    }
    SetPiece(m.GetFr().GetLocation(), m.GetFr().GetPiece());              // put the piece back on the starting field
    PreviousPlayer();                                                     // after the undo, it's the previous player's turn
  }


  void TakenPosition::Push(unsigned int player, const std::vector<const Piece*>& p) noexcept
  {
    for (auto& pp : p)
    {
      if (pp == nullptr) continue;
      if (pp->IsBlank()) continue;
      for (Coordinate i = 0; ; i++)
      {
        assert(i < sizeX);
        const Location l{ i, player };
        if (GetPiece(l) == &Piece::NoTile)
        {
          SetPiece(l, pp);
          break;
        }
      }
    }
  }


  Game::Game(MainPosition* p, TakenPosition* t, StockPosition* s, Layout* l, TakenLayout* tl, StockLayout* sl, bool pl) noexcept
    : pos{ p }, tpos(t), spos(s), lay{ l }, tlay(tl), slay(sl), placing(pl)
  {
    AddPlayer(new Player(&PlayerType::Human, &Color::White));
    AddPlayer(new Player(&PlayerType::Computer, &Color::Black));
    // plist.reserve(16384);
  }

  Game::~Game(void)
  {
    for (auto& p : plist) delete p;
    delete pos;
    if (tpos != nullptr) delete tpos;
    if (spos != nullptr) delete spos;
    delete lay;
    if (tlay != nullptr) delete tlay;
    if (slay != nullptr) delete slay;
    for (auto& p : players) delete p;
  }

  void Game::ReadFromWWW(const std::string& gameno)
  {
    const std::string url = R"(http://brainking.com/en/ShowGame?g=)" + gameno;
    std::string html = URL::GetHTMLFromURL(url);
    pos->SetPosition(Piece::ListFromHTML(html, Piece::GetHTMLPieceMap()));
  }

  void Game::Execute(const Move& m)
  {
    const unsigned int z{ pos->OnTurn() == &Color::White ? 1U : 0U };     // need to buffer the index, as Execute changes who's on turn
    tpos->Push(z, pos->Execute(m));                                       // execute move (includes setting pos to next player), and display taken pieces
    NextPlayer();                                                         // the game has also a pointer to the current player
  }

}
