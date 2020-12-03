#include "Games.h"

#include "EspionageGame.h"
#include "EspionageResource.h"

namespace Espionage
{
  const ETileColor ETileColor::Volcano{ 'V', IDB_ESPIONAGEVOLCANO };

  template <unsigned char z>
  const Soldier<z>  Soldier<z> ::TheSoldier    {};
  const Spy         Spy        ::TheSpy        {};
  const Sapper      Sapper     ::TheSapper     {};
  const Mine        Mine       ::TheMine       {};
  const Headquarter Headquarter::TheHeadquarter{};

  const EPiece EPiece::WSoldier1   { Soldier<'1'>::TheSoldier,     PieceColor::White, IDB_ESPIONAGEWSOLDIER1    };
  const EPiece EPiece::WSoldier2   { Soldier<'2'>::TheSoldier,     PieceColor::White, IDB_ESPIONAGEWSOLDIER2    };
  const EPiece EPiece::WSoldier3   { Soldier<'3'>::TheSoldier,     PieceColor::White, IDB_ESPIONAGEWSOLDIER3    };
  const EPiece EPiece::WSoldier4   { Soldier<'4'>::TheSoldier,     PieceColor::White, IDB_ESPIONAGEWSOLDIER4    };
  const EPiece EPiece::WSoldier5   { Soldier<'5'>::TheSoldier,     PieceColor::White, IDB_ESPIONAGEWSOLDIER5    };
  const EPiece EPiece::WSpy        { Spy         ::TheSpy,         PieceColor::White, IDB_ESPIONAGEWSPY         };
  const EPiece EPiece::WSapper     { Sapper      ::TheSapper,      PieceColor::White, IDB_ESPIONAGEWSAPPER      };
  const EPiece EPiece::WMine       { Mine        ::TheMine,        PieceColor::White, IDB_ESPIONAGEWMINE        };
  const EPiece EPiece::WHeadquarter{ Headquarter ::TheHeadquarter, PieceColor::White, IDB_ESPIONAGEWHEADQUARTER };

  const EPiece EPiece::BSoldier1   { Soldier<'1'>::TheSoldier,     PieceColor::Black, IDB_ESPIONAGEBSOLDIER1    };
  const EPiece EPiece::BSoldier2   { Soldier<'2'>::TheSoldier,     PieceColor::Black, IDB_ESPIONAGEBSOLDIER2    };
  const EPiece EPiece::BSoldier3   { Soldier<'3'>::TheSoldier,     PieceColor::Black, IDB_ESPIONAGEBSOLDIER3    };
  const EPiece EPiece::BSoldier4   { Soldier<'4'>::TheSoldier,     PieceColor::Black, IDB_ESPIONAGEBSOLDIER4    };
  const EPiece EPiece::BSoldier5   { Soldier<'5'>::TheSoldier,     PieceColor::Black, IDB_ESPIONAGEBSOLDIER5    };
  const EPiece EPiece::BSpy        { Spy         ::TheSpy,         PieceColor::Black, IDB_ESPIONAGEBSPY         };
  const EPiece EPiece::BSapper     { Sapper      ::TheSapper,      PieceColor::Black, IDB_ESPIONAGEBSAPPER      };
  const EPiece EPiece::BMine       { Mine        ::TheMine,        PieceColor::Black, IDB_ESPIONAGEBMINE        };
  const EPiece EPiece::BHeadquarter{ Headquarter ::TheHeadquarter, PieceColor::Black, IDB_ESPIONAGEBHEADQUARTER };

  const EPiece EPiece::USoldier1   { Soldier<'1'>::TheSoldier,     PieceColor::Black, IDB_ESPIONAGEUNKNOWN      };
  const EPiece EPiece::USoldier2   { Soldier<'2'>::TheSoldier,     PieceColor::Black, IDB_ESPIONAGEUNKNOWN      };
  const EPiece EPiece::USoldier3   { Soldier<'3'>::TheSoldier,     PieceColor::Black, IDB_ESPIONAGEUNKNOWN      };
  const EPiece EPiece::USoldier4   { Soldier<'4'>::TheSoldier,     PieceColor::Black, IDB_ESPIONAGEUNKNOWN      };
  const EPiece EPiece::USoldier5   { Soldier<'5'>::TheSoldier,     PieceColor::Black, IDB_ESPIONAGEUNKNOWN      };
  const EPiece EPiece::USpy        { Spy         ::TheSpy,         PieceColor::Black, IDB_ESPIONAGEUNKNOWN      };
  const EPiece EPiece::USapper     { Sapper      ::TheSapper,      PieceColor::Black, IDB_ESPIONAGEUNKNOWN      };
  const EPiece EPiece::UMine       { Mine        ::TheMine,        PieceColor::Black, IDB_ESPIONAGEUNKNOWN      };
  const EPiece EPiece::UHeadquarter{ Headquarter ::TheHeadquarter, PieceColor::Black, IDB_ESPIONAGEUNKNOWN      };


  void EKind::CollectMoves(const MainPosition& p, const Location& l, Moves& moves) const noexcept
  {
    for (auto& d : Offset::Rdirection)
      p.AddIfLegal(moves, l, l + d);
  }


  bool EKind::CanTake(const Kind& p) const noexcept
  {
    if ((p == Headquarter::TheHeadquarter) ||
        (p == Sapper::TheSapper)           ||
        (p == Spy::TheSpy)                 ) return true;                                          // anybody can take those
    if (p == Mine::TheMine) return *this == Sapper::TheSapper;

    if (p == Soldier<'5'>::TheSoldier) return *this == Soldier<'5'>::TheSoldier ||
                                              *this == Sapper::TheSapper;                          // Sapper and 5 takes 5
    if (p == Soldier<'4'>::TheSoldier) return *this == Soldier<'5'>::TheSoldier ||
                                              *this == Soldier<'4'>::TheSoldier;                   // 4 and 5 takes 4
    if (p == Soldier<'3'>::TheSoldier) return *this == Soldier<'5'>::TheSoldier ||
                                              *this == Soldier<'4'>::TheSoldier ||
                                              *this == Soldier<'3'>::TheSoldier;                   // 3 to 5 takes 3
    if (p == Soldier<'2'>::TheSoldier) return *this == Soldier<'5'>::TheSoldier ||
                                              *this == Soldier<'4'>::TheSoldier ||
                                              *this == Soldier<'3'>::TheSoldier ||
                                              *this == Soldier<'2'>::TheSoldier;                   // 2 to 5 takes 2
    if (p == Soldier<'1'>::TheSoldier) return *this == Soldier<'5'>::TheSoldier ||
                                              *this == Soldier<'4'>::TheSoldier ||
                                              *this == Soldier<'3'>::TheSoldier ||
                                              *this == Soldier<'2'>::TheSoldier ||
                                              *this == Soldier<'1'>::TheSoldier;                   // 1 to 5 takes 1
    return false;
  }

  EspionageLayout::EspionageLayout(const Dimensions& d) noexcept : MainLayout(d, LayoutType::None)
  {
    unsigned int z = 0;
    for (Coordinate i = 0; i < dim_.xCount_; i++)
      for (Coordinate j = 0; j < dim_.yCount_; j++, z++)
      {
        const Rect r{
          (int)(dim_.lEdge_ + dim_.xSkip_ * i + dim_.xDim_ * i),
          (int)(dim_.tEdge_ + dim_.ySkip_ * j + dim_.yDim_ * j),
          (int)(dim_.lEdge_ + dim_.xSkip_ * i + dim_.xDim_ * (i + 1U)),
          (int)(dim_.tEdge_ + dim_.ySkip_ * j + dim_.yDim_ * (j + 1U)) };

        tiles_[z] = new Tile(Location(BoardPart::Main, i, j), r, FC(i, j));
      }
  }

  const TileColor& EspionageLayout::FC(Coordinate i, Coordinate j) const noexcept
  {
    switch (EspionageGame::GetFieldType(dim_.xCount_, dim_.yCount_, i, j))
    {
      case EspionageGame::FieldType::Standard:     return ETileColor::Light;
      case EspionageGame::FieldType::Volcano:      return ETileColor::Volcano;
      default:                                     return ETileColor::Light;
    }
  }


  EspionagePosition::EspionagePosition(VariantCode c, const PieceMapP& p, const Dimensions& d) noexcept : MainPosition(c, p, d)
  {
    std::vector<const Piece*> pp;
    for (Coordinate z = 0U; z < 5U; ++z) pp.push_back(&EPiece::USoldier1);
    for (Coordinate z = 0U; z < 4U; ++z) pp.push_back(&EPiece::USoldier2);
    for (Coordinate z = 0U; z < 3U; ++z) pp.push_back(&EPiece::USoldier3);
    for (Coordinate z = 0U; z < 2U; ++z) pp.push_back(&EPiece::USoldier4);
    for (Coordinate z = 0U; z < 2U; ++z) pp.push_back(&EPiece::USoldier5);
    for (Coordinate z = 0U; z < 5U; ++z) pp.push_back(&EPiece::USpy);
    for (Coordinate z = 0U; z < 4U; ++z) pp.push_back(&EPiece::USapper);
    for (Coordinate z = 0U; z < 4U; ++z) pp.push_back(&EPiece::UMine);
    for (Coordinate z = 0U; z < 1U; ++z) pp.push_back(&EPiece::UHeadquarter);
    assert(pp.size() == 30);

    for (Coordinate i = 0; i < sizeX_; i++)
      for (Coordinate j = 0; j < 3; j++)
      {
        const unsigned int z = rand() % pp.size();
        SetPiece(Location{ BoardPart::Main,  i,j }, *(pp[z]));
        pp.erase(pp.begin() + z);
      }
    assert(pp.size() == 0);

    for (Coordinate z = 0U; z < 5U; ++z) pp.push_back(&EPiece::WSoldier1);
    for (Coordinate z = 0U; z < 4U; ++z) pp.push_back(&EPiece::WSoldier2);
    for (Coordinate z = 0U; z < 3U; ++z) pp.push_back(&EPiece::WSoldier3);
    for (Coordinate z = 0U; z < 2U; ++z) pp.push_back(&EPiece::WSoldier4);
    for (Coordinate z = 0U; z < 2U; ++z) pp.push_back(&EPiece::WSoldier5);
    for (Coordinate z = 0U; z < 5U; ++z) pp.push_back(&EPiece::WSpy);
    for (Coordinate z = 0U; z < 4U; ++z) pp.push_back(&EPiece::WSapper);
    for (Coordinate z = 0U; z < 4U; ++z) pp.push_back(&EPiece::WMine);
    for (Coordinate z = 0U; z < 1U; ++z) pp.push_back(&EPiece::WHeadquarter);
    assert(pp.size() == 30);

    for (Coordinate i = 0; i < sizeX_; i++)
      for (Coordinate j = 7; j < sizeY_; j++)
      {
        const unsigned int z = rand() % pp.size();
        SetPiece(Location{ BoardPart::Main,  i,j }, *(pp[z]));
        pp.erase(pp.begin() + z);
      }
    assert(pp.size() == 0);
  }

  bool EspionagePosition::AddIfLegal(Moves& m, const Location& fr, const Location& to) const noexcept
  {
    const Piece& pf = GetPiece(fr);                                       // piece to move
    if (pf == Piece::NoTile) return false;                                // out of board
    if (pf.IsBlank()) return false;                                       // tile not occupied

    const Piece& pt = GetPiece(to);                                       // piece on target field
    if (pt == Piece::NoTile) return false;                                // out of board
    if (pt.IsColor(pf.GetColor())) return false;                          // own piece
    
    switch (EspionageGame::GetFieldType(sizeX_, sizeY_, to.x_, to.y_))
    {
      case EspionageGame::FieldType::Volcano:      return false;          // Volcano
      default:;                                                           // fine
    }

    Actions a{};
    a.push_back(std::make_shared<ActionLift>(fr, pf));                    // pick own piece up
    if (pt.IsBlank())
    {
      a.push_back(std::make_shared<ActionDrop>(to, pf));                  // place it on target
    }
    else
    {
      a.push_back(std::make_shared<ActionLift>(to, pt));                  // pick opponent piece up
      if (pf.CanTake(pt))
      {
        a.push_back(std::make_shared<ActionDrop>(GetNextTakenL(pf.GetColor()), pt));                   // place it in Taken
        a.push_back(std::make_shared<ActionDrop>(to, pf));                  // place own piece on target
      }
      else
      {
        a.push_back(std::make_shared<ActionDrop>(GetNextTakenL(pt.GetColor()), pf));                   // place own piece in Taken
        a.push_back(std::make_shared<ActionDrop>(to, pt));                  // place opponent piece (back) on target
      }
    }

    m.push_back(std::make_shared<Move>(a));                               // add move to move list
    return false;
  };

  PositionValue EspionagePosition::EvaluateStatically(void) const noexcept
  {
    return MainPosition::EvaluateStatically();
    // ...
  }

  EspionageGame::FieldType EspionageGame::GetFieldType(const Coordinate& sizeX, const Coordinate& sizeY, const Coordinate& x, const Coordinate& y) noexcept
  {
    const static Coordinate volcano1 = rand() % 3 + 2;
    const static Coordinate volcano2 = rand() % 5;

    if (y == 4 && x == volcano1 + 0) return EspionageGame::FieldType::Volcano;
    if (y == 4 && x == 9 - volcano2) return EspionageGame::FieldType::Volcano;
    if (y == 5 && x == volcano2 + 0) return EspionageGame::FieldType::Volcano;
    if (y == 5 && x == 9 - volcano1) return EspionageGame::FieldType::Volcano;
    return                                  EspionageGame::FieldType::Standard;
  }


  const VariantList& EspionageGame::GetVariants(void) noexcept
  {
    static VariantList v{
      { Variant{ "Espionage",            'E', 10, 10, 2, 20 } },
      { Variant{ "Small Espionage",      'S',  8,  8, 2, 20 } },
      { Variant{ "Fast Espionage",       'F', 10, 10, 2, 20 } },
      { Variant{ "Open Fast Espionage",  'O', 10, 10, 2, 20 } },
      { Variant{ "Small Fast Espionage", 'X',  8,  8, 2, 20 } },
    };
    return v;
  }

  const PieceMapP& EspionageGame::GetPieces(VariantCode c) noexcept
  {
    static const PieceMapP& p = std::make_shared<PieceMap>();
    p->Empty();
    p->Add(EPiece::WSoldier1);
    p->Add(EPiece::BSoldier1);
    p->Add(EPiece::WSoldier2);
    p->Add(EPiece::BSoldier2);
    p->Add(EPiece::WSoldier3);
    p->Add(EPiece::BSoldier3);
    p->Add(EPiece::WSoldier4);
    p->Add(EPiece::BSoldier4);
    p->Add(EPiece::WSoldier5);
    p->Add(EPiece::BSoldier5);
    p->Add(EPiece::WSpy);
    p->Add(EPiece::BSpy);
    p->Add(EPiece::WSapper);
    p->Add(EPiece::BSapper);
    p->Add(EPiece::WMine);
    p->Add(EPiece::BMine);
    p->Add(EPiece::WHeadquarter);
    p->Add(EPiece::BHeadquarter);
    p->Add(EPiece::USoldier1);
    p->Add(EPiece::USoldier2);
    p->Add(EPiece::USoldier3);
    p->Add(EPiece::USoldier4);
    p->Add(EPiece::USoldier5);
    p->Add(EPiece::USpy);
    p->Add(EPiece::USapper);
    p->Add(EPiece::UMine);
    p->Add(EPiece::UHeadquarter);
    return p;
  }

  const Dimensions& EspionageGame::GetDimensions(const VariantChosen& v) noexcept
  {
    static Dimensions d{
       Dimension(v.x, v.y, BoardStartX, BoardStartY, FieldSizeX, FieldSizeY, 1, 1),
       Dimension(15, 2, BoardStartX + FieldSizeX * (v.x + 1), BoardStartY + v.y * FieldSizeY + FieldSizeY / 2, FieldSizeX, FieldSizeY),
       Dimension(3 * v.x, 2, BoardStartX + FieldSizeX * (v.x + 1), BoardStartY + FieldSizeSY, FieldSizeSX, FieldSizeSY, 0, FieldSizeY * v.x - FieldSizeSY * 4),
    };
    return d;
  }
}
