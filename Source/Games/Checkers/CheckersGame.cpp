#include "Games.h"

#include "CheckersGame.h"

namespace Checkers
{

  void CheckersBoard::SetStartingBoard() noexcept
  {
    PieceIndex pWC = PMap[CheckersPiece::CheckersPieceW];
    PieceIndex pBC = PMap[CheckersPiece::CheckersPieceB];

    for (Coordinate y = 0; y < stage_.GetSizeY() / 2 - 1; ++y)
    {
      for (Coordinate x = 0; x < stage_.GetSizeX(); ++x)
      {
        if ((x + y) % 2)
          SetPieceIndex(pBC, x, y);
        else
          SetPieceIndex(pWC, x, - 1 - y);
      }
    }
  }

  bool CheckersBoard::AddIfLegal(Moves& m, const Location& fr, const Location& to) const noexcept
  {
    assert(fr.b_ == BoardPartID::Stage); // should never come here
    assert(to.b_ == BoardPartID::Stage); // should never come here

    PieceIndex pfI = GetPieceIndex(fr.x_, fr.y_);
    assert(pfI != PMap[Piece::NoPiece]);
    assert(pfI != PMap[Piece::NoTile]);

    PieceIndex ptI = GetPieceIndex(to.x_, to.y_);
    if (ptI != PMap[Piece::NoPiece]) return false;

    const Piece& pf = PMap[pfI];
    const Piece& pf2 = CanPromote(to, pfI) ? pf.Promote(true) : pf;
    PieceIndex ptI2 = PMap[pf2];


    Actions a{};
    a.push_back(std::make_shared<ActionLift>(fr, pfI));     // pick piece up
    a.push_back(std::make_shared<ActionDrop>(to, ptI2));    // and place it on target
    m.push_back(std::make_shared<Move>(a));                 // add move to move list
    return true;
  }


  bool CheckersBoard::AddIfLegalJump(Moves& m, bool longjumps, const Actions& a, const Piece& p, const Location& fr) const noexcept
  {
    //const Location l0{ fr };
    //const Piece& p0 = p == Piece::NoTile ? GetPieceIndex(fr) : p;
    //assert(p0 != Piece::NoTile);

    bool any{ false };                                                    // were any more jumps possible?

    //for (const auto& d : Offset::BDirection)                              // jumps are allowed in any diagonal direction, including backwards
    //{
    //  for (int z1 = 1; longjumps || z1 == 1; z1++)                        // for longjumps, allow any z, otherwise only 1 step
    //  {
    //    // look for an opponent's piece to jump over
    //    const Location l1{ fr + d * z1 };                                 // location to jump over

    //                                                                      // check the jumped-over tile                                       
    //    const Piece& p1 = GetPieceIndex(l1);                                   // what is on the tile to jump over?
    //    if (p1 == Piece::NoTile) break;                                   // tile is not existing, can't jump over it - or any further
    //    if (p1.IsBlank()) continue;                                       // tile is not occupied, keep looking further (it's not a jump yet)

    //    const PieceColor& c1 = p1.GetColor();                                 // color of jumped-over piece
    //    if (p0.IsColor(c1)) break;                                        // can't jump own pieces, and not any further either

    //    // now we look for free spaces to land on
    //    for (int z2 = 1; longjumps || z2 == 1; z2++)                      // for longjumps, allow any z, otherwise only 1 step
    //    {
    //      const Location l2{ l1 + d * z2 };                               // location to land on

    //      // check the land-on tile                                       
    //      const Piece& p2 = GetPieceIndex(l2);                                 // what is on the jump-to tile
    //      if (p2 == Piece::NoTile) break;                                 // tile is not existing, can't jump there or any further
    //      if (!p2.IsBlank()) break;                                       // tile is occupied, can't jump there or any further

    //      // check if the same jump was done before (can't jump back and forth or in circles, and can't jump the same opponent piece twice)
    //      if (a.IsRepeat(l1, d)) break;                                    // don't allow the same piece to be jumped again

    //      // a legal jump was found !
    //      any = true;

    //      // add the jump to the StepSimple list
    //      Actions a0{ a };
    //      a0.push_back(std::make_shared<ActionLift>(fr, p0));             // pick piece up
    //      a0.push_back(std::make_shared<ActionJump>(l1, p1));             // jumped over piece
    //      a0.push_back(std::make_shared<ActionLift>(l1, p1));             // pick jumped piece up
    //      a0.push_back(std::make_shared<ActionDrop>(GetNextFreeTakenLocation(p1.GetColor()), p1)); // place it in taken
    //      a0.push_back(std::make_shared<ActionDrop>(l2, p0));             // place jumping piece it on target
    //      if (!AddIfLegalJump(m, longjumps, a0, p0, l2))                  // collect potential further jumps
    //      {
    //        if (CanPromote(l2, p0))                                       // could piece be promoted here?
    //        {
    //          a0.pop_back();                                              // remove unpromoted placement
    //          a0.push_back(std::make_shared<ActionDrop>(l2, p0.Promote(true)));  // place promoted piece on target
    //        }
    //        m.push_back(std::make_shared<Move>(a0));                      // add the action list as a move
    //      }
    //      // and keep trying for longer jumps
    //    }
    //    break;
    //  }
    //}
    //// if any longer jumps were found in deeper recursions, don't save (shorter) moves in the caller; longest jump is mandatory 
    return any;
  }

  //void CheckersBoard::EvaluateStatically() const noexcept
  //{
  //  Board::EvaluateStatically();
  //  // ...
  //}

  //void CheckersBoard::GetAllMoves() const noexcept                       // collect all moves for all pieces
  //{
  //  Board::GetAllMoves();
  //  JumpsOnly(movesW_);          // if there are any jumps, remove all non-jumps - jumping is mandatory
  //  JumpsOnly(movesB_);          // if there are any jumps, remove all non-jumps - jumping is mandatory
  //}

  bool CheckersBoard::CanPromote(const Location& l, const Piece& p) const noexcept
  {
    return (p.IsColor(PieceColor::White) && l.y_ == 0) || (p.IsColor(PieceColor::Black) && l.y_ == stage_.GetSizeY() - 1);
  }

  bool CheckersBoard::CanPromote(const Location& l, PieceIndex p) const noexcept
  {
    return (PMap.IsColor(p,PieceColor::White) && l.y_ == 0) || (PMap.IsColor(p,PieceColor::Black) && l.y_ == stage_.GetSizeY() -1);
  }

  Rule CheckersBoard::GetRule() const noexcept
  {
    return PromotedJump;
  }


  Board* CheckersGame::GetNewBoard(const VariantChosen& v, const BoardPartDimensions& d) noexcept
  {
    switch (static_cast<CheckerVariant>(v.c))
    {
      case CheckerVariant::Standard:      return new CheckersBoard(v, d);     // Checkers
      case CheckerVariant::International: return new InternationalCheckersBoard(v, d);     // International Checkers
      case CheckerVariant::Brazilian:     return new BrazilianCheckersBoard(v, d);     // Brazilian Checkers
      case CheckerVariant::Canadian:      return new CanadianCheckersBoard(v, d);     // Canadian Checkers
      case CheckerVariant::Czech:         return new CzechCheckersBoard(v, d);     // Czech Checkers
      case CheckerVariant::Italian:       return new ItalianCheckersBoard(v, d);     // Italian Checkers
      case CheckerVariant::Portuguese:    return new PortugueseCheckersBoard(v, d);     // Portuguese Checkers
      case CheckerVariant::Russian:       return new RussianCheckersBoard(v, d);     // Russian Checkers
      case CheckerVariant::Thai:          return new ThaiCheckersBoard(v, d);     // Thai Checkers
      case CheckerVariant::Turkish:       return new TurkCheckersBoard(v, d);     // Turkish Checkers
      case CheckerVariant::Anti:          return new AntiCheckersBoard(v, d);     // Anti Checkers
      case CheckerVariant::Corner:        return new CornerCheckersBoard(v, d);     // Corner Checkers
      case CheckerVariant::Hawaiian:      return new HawaiianCheckersBoard(v, d);     // Hawaiian Checkers
      case CheckerVariant::OneWay:        return new OneWayCheckersBoard(v, d);     // One Way Checkers
      case CheckerVariant::Parachute:     return new ParachuteCheckersBoard(v, d);     // Parachute Checkers
      case CheckerVariant::Gothic:        return new GothicCheckersBoard(v, d);     // Gothis Checkers
      case CheckerVariant::Dameo:         return new DameoCheckersBoard(v, d);     // Dameo
      default: return nullptr; // must not happen
    }
  }



  void CheckersGame::Register() noexcept
  {
    const auto pIWC = PMap.Register(CheckersPiece::CheckersPieceW);
    const auto pIBC = PMap.Register(CheckersPiece::CheckersPieceB);
    const auto pIWT = PMap.Register(TurkCheckersPiece::TurkCheckersPieceW);
    const auto pIBT = PMap.Register(TurkCheckersPiece::TurkCheckersPieceB);
    const auto pIWK = PMap.Register(CheckersPiece::CheckersKingW);    
    const auto pIBK = PMap.Register(CheckersPiece::CheckersKingB);
    const auto pIWQ = PMap.Register(CheckersPiece::CheckersQueenW);    
    const auto pIBQ = PMap.Register(CheckersPiece::CheckersQueenB);
    const auto pIWP = PMap.Register(CheckersPiece::CheckersParaW);     
    const auto pIBP = PMap.Register(CheckersPiece::CheckersParaB);

    Variants.Register(Variant(IDR_GAMETYPE_CHECKERS, "Checkers",               Checkers::Standard     ,   8,  8,  2, 20 ));
    Variants.Register(Variant(IDR_GAMETYPE_CHECKERS, "International Checkers", Checkers::International,  10, 10         ));
    Variants.Register(Variant(IDR_GAMETYPE_CHECKERS, "Brazilian Checkers",     Checkers::Brazilian    ,   8,  8         ));
    Variants.Register(Variant(IDR_GAMETYPE_CHECKERS, "Canadian Checkers",      Checkers::Canadian     ,  12, 12         ));
    Variants.Register(Variant(IDR_GAMETYPE_CHECKERS, "Czech Checkers",         Checkers::Czech        ,   8,  8         ));
    Variants.Register(Variant(IDR_GAMETYPE_CHECKERS, "Italian Checkers",       Checkers::Italian      ,   8,  8         ));
    Variants.Register(Variant(IDR_GAMETYPE_CHECKERS, "Portuguese Checkers",    Checkers::Portuguese   ,   8,  8         ));
    Variants.Register(Variant(IDR_GAMETYPE_CHECKERS, "Russian Checkers",       Checkers::Russian      ,   8,  8         ));
    Variants.Register(Variant(IDR_GAMETYPE_CHECKERS, "Thai Checkers",          Checkers::Thai         ,   8,  8         ));
    Variants.Register(Variant(IDR_GAMETYPE_CHECKERS, "Turkish Checkers",       Checkers::Turkish      ,   8,  8         ));

    Variants.Register(Variant(IDR_GAMETYPE_CHECKERS, "Anti Checkers",          Checkers::Anti         ,   8,  8         ));
    Variants.Register(Variant(IDR_GAMETYPE_CHECKERS, "Dameo",                  Checkers::Dameo        ,   8,  8         ));
    Variants.Register(Variant(IDR_GAMETYPE_CHECKERS, "Corner Checkers",        Checkers::Corner       ,   8,  8         ));
    Variants.Register(Variant(IDR_GAMETYPE_CHECKERS, "Gothic Checkers",        Checkers::Gothic       ,   8,  8         ));
    Variants.Register(Variant(IDR_GAMETYPE_CHECKERS, "Hawaiian Checkers",      Checkers::Hawaiian     ,   8,  8         ));
    Variants.Register(Variant(IDR_GAMETYPE_CHECKERS, "One Way Checkers",       Checkers::OneWay       ,   8,  8         ));
    Variants.Register(Variant(IDR_GAMETYPE_CHECKERS, "Parachute Checkers",     Checkers::Parachute    ,   8,  8         ));
  }

  const BoardPartDimensions CheckersGame::GetDimensions(const VariantChosen& v) noexcept
  {
    BoardPartDimensions d{
       BoardPartDimension(v.x, v.y, LayoutType::Alternating, BoardStartX, BoardStartY, FieldSizeX, FieldSizeY, 1, 1),
       BoardPartDimension(20, 2, LayoutType::Light, BoardStartX + FieldSizeX * (v.x + 1), BoardStartY + FieldSizeY / 2 + FieldSizeY * (v.y - 2), FieldSizeX, FieldSizeY),
       BoardPartDimension(2 * v.x, 2, LayoutType::Small, FieldSizeX * (v.x + 1), BoardStartY + FieldSizeSY, FieldSizeSX, FieldSizeSY, 0, FieldSizeY * v.y - FieldSizeSY * 4),
    };
    return d;
  }

}
