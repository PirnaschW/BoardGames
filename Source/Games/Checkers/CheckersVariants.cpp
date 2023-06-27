#include "Games.h"

#include "CheckersGame.h"

namespace Checkers
{
  // templates to create a subclass for each variant
  template <CheckersVariant V> struct CheckersVariantData {};  // helper to allow to add data for one variant only
  template <> struct CheckersVariantData<CheckersVariant::Turkish> { mutable int i_; };  // EXAMPLE! only

  template <CheckersVariant V>
  class CheckersVariantBoard : public CheckersBoard, public CheckersVariantData<V>
  {
  protected:
    CheckersVariantBoard<V>(const CheckersVariantBoard<V>& m) noexcept : CheckersBoard(m), CheckersVariantData<V>(m) {}
  public:
    CheckersVariantBoard<V>(const VariantChosen& v, const BoardPartDimensions& d) noexcept : CheckersBoard(v, d) { SetDefaultStock(); }
    virtual Board* Clone() const noexcept override { return new CheckersVariantBoard<V>(*this); }
    virtual void SetStartingBoard() noexcept override { CheckersBoard::SetStartingBoard(); }
    virtual void GetAllMoves() const noexcept { CheckersBoard::GetAllMoves(); }
    virtual bool AddIfLegal(Moves& m, const Location& fr, const Location& to) const noexcept override { return CheckersBoard::AddIfLegal(m, fr, to); }
    constexpr virtual int GetMoveCountFactor() const noexcept override { return CheckersBoard::GetMoveCountFactor(); }
    virtual void EvaluateStatically() const noexcept override { return CheckersBoard::EvaluateStatically(); }

  private:
    Rule GetRule() const noexcept { return BackJump | MaxCapture | MaxPromotedCapture | PromotedJump | ContinueJumping | NoPromotedCapture | CapturePromotedFirst | PromoteToQueen; }
    static bool ValidBoard(const std::vector<Coordinate>& c) noexcept { return true; }
  };


  //#######################################################
  // specializations for CheckersVariant::Anti


  //#######################################################
  // specializations for CheckersVariant::Czech
  template <> inline CheckersVariantBoard<CheckersVariant::Czech>::CheckersVariantBoard(const VariantChosen& v, const BoardPartDimensions& d) noexcept : CheckersBoard(v, d) { SetQueensStock(); }


  //#######################################################
  // specializations for CheckersVariant::Corner
  template <> inline CheckersVariantBoard<CheckersVariant::Corner>::CheckersVariantBoard(const VariantChosen& v, const BoardPartDimensions& d) noexcept : CheckersBoard(v, d) { SetQueensStock(); }
  template <> inline void CheckersVariantBoard<CheckersVariant::Corner>::SetStartingBoard() noexcept
  {
    //for (Coordinate j = 0; j < sizeY_; j++)
    //{
    //  for (Coordinate i = 0; i < sizeX_; i++)
    //  {
    //    if ((i + j) % 2)
    //    {
    //      if ((i + j + 1) < sizeX_)
    //        SetPieceIndex(Location(BoardPartID::Stage, i, j), CheckersPiece::CheckersPieceB);
    //      else if ((i + j) > sizeX_)
    //        SetPieceIndex(Location(BoardPartID::Stage, i, j), CheckersPiece::CheckersPieceW);
    //    }
    //  }
    //}
  }


  //#######################################################
  // specializations for CheckersVariant::OneWay
  template <> inline CheckersVariantBoard<CheckersVariant::OneWay>::CheckersVariantBoard(const VariantChosen& v, const BoardPartDimensions& d) noexcept : CheckersBoard(v, d) { SetQueensStock(); }
  template <> inline void CheckersVariantBoard<CheckersVariant::OneWay>::SetStartingBoard() noexcept
  {
    //for (Coordinate j = 2; j < sizeY_; j++)
    //{
    //  for (Coordinate i = 0; i < sizeX_; i++)
    //  {
    //    if ((i + j) % 2)
    //    {
    //      if (i < sizeX_ / 2)
    //        SetPieceIndex(Location(BoardPartID::Stage, i, j), CheckersPiece::CheckersPieceB);
    //      else
    //        SetPieceIndex(Location(BoardPartID::Stage, i, j), CheckersPiece::CheckersPieceW);
    //    }
    //  }
    //}
  }


  //#######################################################
  // specializations for CheckersVariant::Parachute
  template <> inline CheckersVariantBoard<CheckersVariant::Parachute>::CheckersVariantBoard(const VariantChosen& v, const BoardPartDimensions& d) noexcept : CheckersBoard(v, d)
  { 
    SetQueensStock(); 
    int i = 4;
    auto append = [this, &i](const Piece& p) { stock_.SetPieceIndex(PMap[p], i / 2U, i % 2U); ++i; };
    append(CheckersPiece::CheckersParaW);
    append(CheckersPiece::CheckersParaB);
  }
  template <> inline void CheckersVariantBoard<CheckersVariant::Parachute>::SetStartingBoard() noexcept
  {
    //for (Coordinate j = 0; j < sizeY_ / 2 - 1; j++)
    //{
    //  for (Coordinate i = 0; i < sizeX_; i++)
    //  {
    //    if ((i + j) % 2)
    //    {
    //      if (j == 0)
    //        SetPieceIndex(Location(BoardPartID::Stage, i, j), CheckersPiece::CheckersParaW);
    //      else
    //        SetPieceIndex(Location(BoardPartID::Stage, i, j), CheckersPiece::CheckersPieceB);
    //    }
    //    else
    //    {
    //      if (j == 0)
    //        SetPieceIndex(Location(BoardPartID::Stage, i, sizeY_ - 1 - j), CheckersPiece::CheckersParaB);
    //      else
    //        SetPieceIndex(Location(BoardPartID::Stage, i, sizeY_ - 1 - j), CheckersPiece::CheckersPieceW);
    //    }
    //  }
    //}
  }


  //#######################################################
  // specializations for CheckersVariant::Gothic
  template <> inline void CheckersVariantBoard<CheckersVariant::Gothic>::SetStartingBoard() noexcept
  {
    //for (Coordinate j = 0; j < sizeY_ / 4; j++)
    //{
    //  for (Coordinate i = 0; i < sizeX_; i++)
    //  {
    //    SetPieceIndex(Location(BoardPartID::Stage, i, j), CheckersPiece::CheckersPieceB);
    //    SetPieceIndex(Location(BoardPartID::Stage, i, sizeY_ - 1 - j), CheckersPiece::CheckersPieceW);
    //  }
    //}
  }


  //#######################################################
  // specializations for CheckersVariant::Turkish
  template <> inline CheckersVariantBoard<CheckersVariant::Turkish>::CheckersVariantBoard(const VariantChosen& v, const BoardPartDimensions& d) noexcept : CheckersBoard(v, d)
  {
    int i = 0;
    auto append = [this, &i](const Piece& p) { stock_.SetPieceIndex(PMap[p], i / 2U, i % 2U); ++i; };
    append(CheckersPiece::TurkCheckersPieceW);
    append(CheckersPiece::TurkCheckersPieceB);
    append(CheckersPiece::CheckersQueenW);
    append(CheckersPiece::CheckersQueenB);
  }
  template <> inline Rule CheckersVariantBoard<CheckersVariant::Turkish>::GetRule() const noexcept { return PromoteToQueen | JumpFurther; }
  template <> inline void CheckersVariantBoard<CheckersVariant::Turkish>::SetStartingBoard() noexcept
  {
    //for (Coordinate j = 1; j < sizeY_ / 4 + 1; j++)
    //{
    //  for (Coordinate i = 0; i < sizeX_; i++)
    //  {
    //    SetPiece(Location(BoardPartID::Main, i, j             ), TurkCheckersPiece::TurkCheckersPieceB);
    //    SetPiece(Location(BoardPartID::Main, i, sizeY_ - 1 - j), TurkCheckersPiece::TurkCheckersPieceW);
    //  }
    //}
  }
  //// p is the piece that is moving, but it doesn't necessarily come from fr - it could be a continuation jump
  //template <> inline bool CheckersVariantBoard<CheckersVariant::Turkish>::AddIfLegalJump(Moves& m, bool longjumps, const Actions& a, PieceIndex pI, const Location& fr) const noexcept
  //{
  //  bool any{ false };                                                    // were any more jumps possible?
  //
    //for (const auto& d : Offset::QDirection)                              // jumps are allowed in any direction, including backwards
    //{
    //  for (int z1 = 1; longjumps || z1 == 1; z1++)                        // for longjumps, allow any z, otherwise only 1 step
    //  {
    //    // look for an opponent's piece to jump over
    //    const Location l1{ fr + d * z1 };                                 // location to jump over
    //    if (!IsValid(l1)) break;                                          // tile is not existing, can't jump over it - or any further
    //    if (IsFree(l1)) continue;                                         // tile is not occupied, keep looking further (it's not a jump yet)
    //    if (IsMyPiece(l1, p.GetColor())) break;                           // can't jump own pieces, and not any further either
    //    
    //    // now we look for free spaces to land on
    //    for (int z2 = 1; longjumps || z2 == 1; z2++)                      // for longjumps, allow any z, otherwise only 1 step
    //    {
    //      const Location l2{ l1 + d * z2 };                               // location to land on
    //      if (!IsValid(l2)) break;                                        // tile is not existing, can't jump to it - or any further
    //      if (!IsFree(l2)) break;                                         // tile is occupied, can't jump there or any further

    //      // check if the same jump was done before (can't jump back and forth or in circles, and can't jump the same opponent piece twice)
    //      if (a.IsRepeat(l1, d)) break;                                   // don't allow the same piece to be jumped again
    //      if (a.IsRepeat(l1, d.Rotate(true))) break;                      // don't allow the same piece to be jumped again orthogonally either

    //      // a legal jump was found !
    //      any = true;

    //      // add the jump to the StepSimple list
    //      Actions a0{ a };
    //      const Piece& p1 = GetPiece(l1);
    //      assert(!p1.IsBlank());
    //      a0.push_back(std::make_shared<ActionLift>(fr, p));              // pick piece up
    //      a0.push_back(std::make_shared<ActionJump>(l1, p1));             // jumped over piece
    //      a0.push_back(std::make_shared<ActionLift>(l1, p1));             // pick jumped piece up
    //      a0.push_back(std::make_shared<ActionDrop>(GetNextFreeTakenLocation(p1.GetColor()), p1)); // place it in taken
    //      a0.push_back(std::make_shared<ActionDrop>(l2, p));              // place jumping piece it on target
    //      //a0.Log();
    //      if (!AddIfLegalJump(m, longjumps, a0, p, l2))                   // collect potential further jumps
    //      {
    //        if (CanPromote(l2, p))                                       // could piece be promoted here?
    //        {
    //          a0.pop_back();                                              // remove unpromoted placement
    //          a0.push_back(std::make_shared<ActionDrop>(l2, p.Promote(true)));  // place promoted piece on target
    //        }
    //        m.push_back(std::make_shared<Move>(a0));                      // add the action list as a move
    //      }
    //      // and keep trying for longer jumps
    //    }
    //    break;
    //  }
    //}
    //// if any longer jumps were found in deeper recursions, don't save (shorter) moves in the caller; longest jump is mandatory 
  //  return any;
  //}


  //#######################################################
  // specializations for CheckersVariant::Hawaiian
  template <> inline void CheckersVariantBoard<CheckersVariant::Hawaiian>::SetStartingBoard() noexcept
  {
    //for (Coordinate j = 0; j < sizeY_; j++)
    //{
    //  for (Coordinate i = 0; i < sizeX_; i++)
    //  {
    //    if ((i + j) % 2)
    //      SetPieceIndex(Location(BoardPartID::Stage, i, j), CheckersPiece::CheckersPieceW);
    //    else
    //      SetPieceIndex(Location(BoardPartID::Stage, i, j), CheckersPiece::CheckersPieceB);
    //  }
    //}
  }


  //#######################################################
  // specializations for CheckersVariant::International
  template <> inline CheckersVariantBoard<CheckersVariant::International>::CheckersVariantBoard(const VariantChosen& v, const BoardPartDimensions& d) noexcept : CheckersBoard(v, d) { SetQueensStock(); }


  //#######################################################
  // specializations for CheckersVariant::Russian
  template <> inline CheckersVariantBoard<CheckersVariant::Russian>::CheckersVariantBoard(const VariantChosen& v, const BoardPartDimensions& d) noexcept : CheckersBoard(v, d) { SetQueensStock(); }


  //#######################################################
  // specializations for CheckersVariant::Canadian
  template <> inline CheckersVariantBoard<CheckersVariant::Canadian>::CheckersVariantBoard(const VariantChosen& v, const BoardPartDimensions& d) noexcept : CheckersBoard(v, d) { SetQueensStock(); }


  //#######################################################
  // specializations for CheckersVariant::Thai
  template <> inline CheckersVariantBoard<CheckersVariant::Thai>::CheckersVariantBoard(const VariantChosen& v, const BoardPartDimensions& d) noexcept : CheckersBoard(v, d) { SetQueensStock(); }
  template <> inline void CheckersVariantBoard<CheckersVariant::Thai>::SetStartingBoard() noexcept
  {
    //for (Coordinate j = 0; j < sizeY_ / 4; j++)
    //{
    //  for (Coordinate i = 0; i < sizeX_; i++)
    //  {
    //    if ((i + j) % 2)
    //      SetPieceIndex(Location(BoardPartID::Stage, i, j), CheckersPiece::CheckersPieceB);
    //    else
    //      SetPieceIndex(Location(BoardPartID::Stage, i, sizeY_ - 1 - j), CheckersPiece::CheckersPieceW);
    //  }
    //}
  }


  //#######################################################
  // specializations for CheckersVariant::Brazilian
  template <> inline CheckersVariantBoard<CheckersVariant::Brazilian>::CheckersVariantBoard(const VariantChosen& v, const BoardPartDimensions& d) noexcept : CheckersBoard(v, d) { SetQueensStock(); }


  //#######################################################
  // specializations for CheckersVariant::Portuguese
  template <> inline CheckersVariantBoard<CheckersVariant::Portuguese>::CheckersVariantBoard(const VariantChosen& v, const BoardPartDimensions& d) noexcept : CheckersBoard(v, d) { SetQueensStock(); }
  template <> inline void CheckersVariantBoard<CheckersVariant::Portuguese>::SetStartingBoard() noexcept
  {
    //for (Coordinate j = 0; j < sizeY_ / 2 - 1; j++)
    //{
    //  for (Coordinate i = 0; i < sizeX_; i++)
    //  {
    //    if ((i + j) % 2)
    //      SetPieceIndex(Location(BoardPartID::Stage, i, sizeY_ - 1 - j), CheckersPiece::CheckersPieceW);
    //    else
    //      SetPieceIndex(Location(BoardPartID::Stage, i, j), CheckersPiece::CheckersPieceB);
    //  }
    //}
  }


  //#######################################################
  // specializations for CheckersVariant::Italian
  template <> inline void CheckersVariantBoard<CheckersVariant::Italian>::SetStartingBoard() noexcept
  {
    for (Coordinate j = 0; j < stage_.GetSizeY() / 2 - 1; j++)
    {
      for (Coordinate i = 0; i < stage_.GetSizeX(); i++)
      {
        if ((i + j) % 2)
          SetPieceIndex(PMap[CheckersPiece::CheckersPieceW], i, stage_.GetSizeY() - 1 - j);
        else
          SetPieceIndex(PMap[CheckersPiece::CheckersPieceB], i, j);
      }
    }
  }


  //#######################################################
  // specializations for CheckersVariant::Dameo
  template <> inline CheckersVariantBoard<CheckersVariant::Dameo>::CheckersVariantBoard(const VariantChosen& v, const BoardPartDimensions& d) noexcept : CheckersBoard(v, d) { SetQueensStock(); }
  template <> inline void CheckersVariantBoard<CheckersVariant::Dameo>::SetStartingBoard() noexcept
  {
    //for (Coordinate j = 0; j < sizeY_ / 2 - 1; j++)
    //{
    //  for (Coordinate i = j; i < sizeX_ - j; i++)
    //  {
    //    SetPieceIndex(Location(BoardPartID::Stage, i, j), CheckersPiece::CheckersPieceB);
    //    SetPieceIndex(Location(BoardPartID::Stage, i, sizeY_ - 1 - j), CheckersPiece::CheckersPieceW);
    //  }
    //}
  }



  Board* CheckersGame::GetNewBoard(const VariantChosen& v, const BoardPartDimensions& d) noexcept
  {
    // this switch looks silly, but cannot be avoided - the compiler has no idea which variants would be possible, 
    // but needs to generate the code for each one. An explicit list is needed in some form!
    switch (static_cast<CheckersVariant>(v.c))
    {
      case CheckersVariant::Standard:      return new CheckersVariantBoard<CheckersVariant::Standard     >(v, d);     // Checkers
      case CheckersVariant::International: return new CheckersVariantBoard<CheckersVariant::International>(v, d);     // International Checkers
      case CheckersVariant::Brazilian:     return new CheckersVariantBoard<CheckersVariant::Brazilian    >(v, d);     // Brazilian Checkers
      case CheckersVariant::Canadian:      return new CheckersVariantBoard<CheckersVariant::Canadian     >(v, d);     // Canadian Checkers
      case CheckersVariant::Czech:         return new CheckersVariantBoard<CheckersVariant::Czech        >(v, d);     // Czech Checkers
      case CheckersVariant::Italian:       return new CheckersVariantBoard<CheckersVariant::Italian      >(v, d);     // Italian Checkers
      case CheckersVariant::Portuguese:    return new CheckersVariantBoard<CheckersVariant::Portuguese   >(v, d);     // Portuguese Checkers
      case CheckersVariant::Russian:       return new CheckersVariantBoard<CheckersVariant::Russian      >(v, d);     // Russian Checkers
      case CheckersVariant::Thai:          return new CheckersVariantBoard<CheckersVariant::Thai         >(v, d);     // Thai Checkers
      case CheckersVariant::Turkish:       return new CheckersVariantBoard<CheckersVariant::Turkish      >(v, d);     // Turkish Checkers
      case CheckersVariant::Anti:          return new CheckersVariantBoard<CheckersVariant::Anti         >(v, d);     // Anti Checkers
      case CheckersVariant::Corner:        return new CheckersVariantBoard<CheckersVariant::Corner       >(v, d);     // Corner Checkers
      case CheckersVariant::Hawaiian:      return new CheckersVariantBoard<CheckersVariant::Hawaiian     >(v, d);     // Hawaiian Checkers
      case CheckersVariant::OneWay:        return new CheckersVariantBoard<CheckersVariant::OneWay       >(v, d);     // One Way Checkers
      case CheckersVariant::Parachute:     return new CheckersVariantBoard<CheckersVariant::Parachute    >(v, d);     // Parachute Checkers
      case CheckersVariant::Gothic:        return new CheckersVariantBoard<CheckersVariant::Gothic       >(v, d);     // Gothis Checkers
      case CheckersVariant::Dameo:         return new CheckersVariantBoard<CheckersVariant::Dameo        >(v, d);     // Dameo
      default: return nullptr; // must not happen
    }
  }

}


