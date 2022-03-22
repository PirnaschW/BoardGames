#include "Games.h"

#include "CheckersGame.h"

namespace Checkers
{

  void CheckersPosition::SetStartingPosition() noexcept
  {
    for (Coordinate j = 0; j < sizeY_ / 2 - 1; j++)
    {
      for (Coordinate i = 0; i < sizeX_; i++)
      {
        if ((i + j) % 2)
          SetPiece(Location(BoardPart::Main, i, j), CheckersPiece::CheckersPieceB);
        else
          SetPiece(Location(BoardPart::Main, i, sizeY_ - 1 - j), CheckersPiece::CheckersPieceW);
      }
    }
  }

  bool CheckersPosition::AddIfLegal(Moves& m, const Location& fr, const Location& to) const noexcept
  {
    const Piece& pf = GetPiece(fr);
    assert(pf != Piece::NoTile);
    assert(!pf.IsBlank());
    const Piece& pt = GetPiece(to);
    if (pt == Piece::NoTile) return false;                                // out of board
    if (!pt.IsBlank()) return false;                                     // field is not empty

    const Piece& pf2 = CanPromote(to, pf) ? pf.Promote(true) : pf;

    Actions a{};
    a.push_back(std::make_shared<ActionLift>(fr, pf));                    // pick piece up
    a.push_back(std::make_shared<ActionDrop>(to, pf2));                  // and place it on target
    m.push_back(std::make_shared<Move>(a));                               // add move to move list
    return true;
  }


  bool CheckersPosition::AddIfLegalJump(Moves& m, bool longjumps, const Actions& a, const Piece& p, const Location& fr) const noexcept
  {
    const Location l0{ fr };
    const Piece& p0 = p == Piece::NoTile ? GetPiece(fr) : p;
    assert(p0 != Piece::NoTile);

    bool any{ false };                                                    // were any more jumps possible?

    for (const auto& d : Offset::BDirection)                              // jumps are allowed in any diagonal direction, including backwards
    {
      for (int z1 = 1; longjumps || z1 == 1; z1++)                        // for longjumps, allow any z, otherwise only 1 step
      {
        // look for an opponent's piece to jump over
        const Location l1{ fr + d * z1 };                                 // location to jump over

                                                                          // check the jumped-over tile                                       
        const Piece& p1 = GetPiece(l1);                                   // what is on the tile to jump over?
        if (p1 == Piece::NoTile) break;                                   // tile is not existing, can't jump over it - or any further
        if (p1.IsBlank()) continue;                                       // tile is not occupied, keep looking further (it's not a jump yet)

        const PieceColor& c1 = p1.GetColor();                                 // color of jumped-over piece
        if (p0.IsColor(c1)) break;                                        // can't jump own pieces, and not any further either

        // now we look for free spaces to land on
        for (int z2 = 1; longjumps || z2 == 1; z2++)                      // for longjumps, allow any z, otherwise only 1 step
        {
          const Location l2{ l1 + d * z2 };                               // location to land on

          // check the land-on tile                                       
          const Piece& p2 = GetPiece(l2);                                 // what is on the jump-to tile
          if (p2 == Piece::NoTile) break;                                 // tile is not existing, can't jump there or any further
          if (!p2.IsBlank()) break;                                       // tile is occupied, can't jump there or any further

          // check if the same jump was done before (can't jump back and forth or in circles, and can't jump the same opponent piece twice)
          if (a.IsRepeat(l1, d)) break;                                    // don't allow the same piece to be jumped again

          // a legal jump was found !
          any = true;

          // add the jump to the StepSimple list
          Actions a0{ a };
          a0.push_back(std::make_shared<ActionLift>(fr, p0));             // pick piece up
          a0.push_back(std::make_shared<ActionJump>(l1, p1));             // jumped over piece
          a0.push_back(std::make_shared<ActionLift>(l1, p1));             // pick jumped piece up
          a0.push_back(std::make_shared<ActionDrop>(GetNextTakenL(p1.GetColor()), p1)); // place it in taken
          a0.push_back(std::make_shared<ActionDrop>(l2, p0));             // place jumping piece it on target
          if (!AddIfLegalJump(m, longjumps, a0, p0, l2))                  // collect potential further jumps
          {
            if (CanPromote(l2, p0))                                       // could piece be promoted here?
            {
              a0.pop_back();                                              // remove unpromoted placement
              a0.push_back(std::make_shared<ActionDrop>(l2, p0.Promote(true)));  // place promoted piece on target
            }
            m.push_back(std::make_shared<Move>(a0));                      // add the action list as a move
          }
          // and keep trying for longer jumps
        }
        break;
      }
    }
    // if any longer jumps were found in deeper recursions, don't save (shorter) moves in the caller; longest jump is mandatory 
    return any;
  }

  PositionValue CheckersPosition::EvaluateStatically(void) const noexcept
  {
    return MainPosition::EvaluateStatically();
    // ...
  }

  void CheckersPosition::GetAllMoves(void) const noexcept                       // collect all moves for all pieces
  {
    MainPosition::GetAllMoves();
    JumpsOnly(movesW_);          // if there are any jumps, remove all non-jumps - jumping is mandatory
    JumpsOnly(movesB_);          // if there are any jumps, remove all non-jumps - jumping is mandatory
  }

  bool CheckersPosition::CanPromote(const Location& l, const Piece& p) const noexcept
  {
    return (p.IsColor(PieceColor::White) && l.y_ == 0) || (p.IsColor(PieceColor::Black) && l.y_ == sizeY_ - 1);
  }

  Rule CheckersPosition::GetRule() const noexcept
  {
    return PromotedJump;
  }


  MainPosition* CheckersGame::GetNewPosition(const VariantChosen& v, const PieceMapP& m, const Dimensions& d) noexcept
  {
    switch (static_cast<CheckerVariant>(v.c))
    {
      case CheckerVariant::Standard:      return new CheckersPosition(v, m, d);     // Checkers
      case CheckerVariant::International: return new InternationalCheckersPosition(v, m, d);     // International Checkers
      case CheckerVariant::Brazilian:     return new BrazilianCheckersPosition(v, m, d);     // Brazilian Checkers
      case CheckerVariant::Canadian:      return new CanadianCheckersPosition(v, m, d);     // Canadian Checkers
      case CheckerVariant::Czech:         return new CzechCheckersPosition(v, m, d);     // Czech Checkers
      case CheckerVariant::Italian:       return new ItalianCheckersPosition(v, m, d);     // Italian Checkers
      case CheckerVariant::Portuguese:    return new PortugueseCheckersPosition(v, m, d);     // Portuguese Checkers
      case CheckerVariant::Russian:       return new RussianCheckersPosition(v, m, d);     // Russian Checkers
      case CheckerVariant::Thai:          return new ThaiCheckersPosition(v, m, d);     // Thai Checkers
      case CheckerVariant::Turkish:       return new TurkCheckersPosition(v, m, d);     // Turkish Checkers
      case CheckerVariant::Anti:          return new AntiCheckersPosition(v, m, d);     // Anti Checkers
      case CheckerVariant::Corner:        return new CornerCheckersPosition(v, m, d);     // Corner Checkers
      case CheckerVariant::Hawaiian:      return new HawaiianCheckersPosition(v, m, d);     // Hawaiian Checkers
      case CheckerVariant::OneWay:        return new OneWayCheckersPosition(v, m, d);     // One Way Checkers
      case CheckerVariant::Parachute:     return new ParachuteCheckersPosition(v, m, d);     // Parachute Checkers
      case CheckerVariant::Gothic:        return new GothicCheckersPosition(v, m, d);     // Gothis Checkers
      case CheckerVariant::Dameo:         return new DameoCheckersPosition(v, m, d);     // Dameo
      default: return nullptr; // must not happen
    }
  }

  const VariantList& CheckersGame::GetVariants(void) noexcept
  {
    static VariantList v{
      { Variant{ "Checkers",               Checkers::Standard     ,   8,  8,  2, 20 } },
      { Variant{ "International Checkers", Checkers::International,  10, 10         } },
      { Variant{ "Brazilian Checkers",     Checkers::Brazilian    ,   8,  8         } },
      { Variant{ "Canadian Checkers",      Checkers::Canadian     ,  12, 12         } },
      { Variant{ "Czech Checkers",         Checkers::Czech        ,   8,  8         } },
      { Variant{ "Italian Checkers",       Checkers::Italian      ,   8,  8         } },
      { Variant{ "Portuguese Checkers",    Checkers::Portuguese   ,   8,  8         } },
      { Variant{ "Russian Checkers",       Checkers::Russian      ,   8,  8         } },
      { Variant{ "Thai Checkers",          Checkers::Thai         ,   8,  8         } },
      { Variant{ "Turkish Checkers",       Checkers::Turkish      ,   8,  8         } },

      { Variant{ "Anti Checkers",          Checkers::Anti         ,   8,  8         } },
      { Variant{ "Dameo",                  Checkers::Dameo        ,   8,  8         } },
      { Variant{ "Corner Checkers",        Checkers::Corner       ,   8,  8         } },
      { Variant{ "Gothic Checkers",        Checkers::Gothic       ,   8,  8         } },
      { Variant{ "Hawaiian Checkers",      Checkers::Hawaiian     ,   8,  8         } },
      { Variant{ "One Way Checkers",       Checkers::OneWay       ,   8,  8         } },
      { Variant{ "Parachute Checkers",     Checkers::Parachute    ,   8,  8         } },
    };
    return v;
  }

  const PieceMapP& CheckersGame::GetPieces(const VariantChosen& v) noexcept
  {
    static const PieceMapP& p = std::make_shared<PieceMap>();
    p->Empty();
    if (v.c == Checkers::Turkish)
    {
      p->Add(TurkCheckersPiece::TurkCheckersPieceW); p->Add(TurkCheckersPiece::TurkCheckersPieceB);
    }
    else
    {
      p->Add(CheckersPiece::CheckersPieceW);     p->Add(CheckersPiece::CheckersPieceB);
    }
    p->Add(CheckersPiece::CheckersKingW);      p->Add(CheckersPiece::CheckersKingB);
    p->Add(CheckersPiece::CheckersQueenW);     p->Add(CheckersPiece::CheckersQueenB);
    p->Add(CheckersPiece::CheckersParaW);      p->Add(CheckersPiece::CheckersParaB);
    return p;
  }

  const Dimensions CheckersGame::GetDimensions(const VariantChosen& v) noexcept
  {
    Dimensions d{
       Dimension(v.x, v.y, BoardStartX, BoardStartY, FieldSizeX, FieldSizeY, 1, 1),
       Dimension(5, 2, BoardStartX + FieldSizeX * (v.x + 1), BoardStartY + FieldSizeY / 2 + FieldSizeY * (v.y - 2), FieldSizeX, FieldSizeY),
       Dimension(2 * v.x, 2, FieldSizeX * (v.x + 1), BoardStartY + FieldSizeSY, FieldSizeSX, FieldSizeSY, 0, FieldSizeY * v.y - FieldSizeSY * 4),
    };
    return d;
  }

}
