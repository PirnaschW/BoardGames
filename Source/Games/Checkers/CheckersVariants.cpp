#include "Games.h"

#include "CheckersGame.h"

namespace Checkers
{

  class InternationalCheckersPosition: public CheckersPosition
  {
  public:
    inline InternationalCheckersPosition(const VariantChosen& v, const PieceMapP& p, const Dimensions& d) noexcept: CheckersPosition(v, p, d) {}
  };

  class BrazilianCheckersPosition: public CheckersPosition
  {
  public:
    inline BrazilianCheckersPosition(const VariantChosen& v, const PieceMapP& p, const Dimensions& d) noexcept: CheckersPosition(v, p, d) {}
  };

  class CanadianCheckersPosition: public CheckersPosition
  {
  public:
    inline CanadianCheckersPosition(const VariantChosen& v, const PieceMapP& p, const Dimensions& d) noexcept: CheckersPosition(v, p, d) {}
  };

  class CzechCheckersPosition: public CheckersPosition
  {
  public:
    inline CzechCheckersPosition(const VariantChosen& v, const PieceMapP& p, const Dimensions& d) noexcept: CheckersPosition(v, p, d) {}
  };

  class ItalianCheckersPosition: public CheckersPosition
  {
  public:
    inline ItalianCheckersPosition(const VariantChosen& v, const PieceMapP& p, const Dimensions& d) noexcept: CheckersPosition(v, p, d) {}
    void SetStartingPosition() noexcept override;
  };

  class PortugueseCheckersPosition: public CheckersPosition
  {
  public:
    inline PortugueseCheckersPosition(const VariantChosen& v, const PieceMapP& p, const Dimensions& d) noexcept: CheckersPosition(v, p, d) {}
    void SetStartingPosition() noexcept override;
  protected:
    Rule GetRule() const noexcept override { return PromoteToQueen | JumpFurther | MaxCapture | MaxPromotedCapture; }
  };

  class RussianCheckersPosition: public CheckersPosition
  {
  public:
    inline RussianCheckersPosition(const VariantChosen& v, const PieceMapP& p, const Dimensions& d) noexcept: CheckersPosition(v, p, d) {}
  protected:
    Rule GetRule() const noexcept override { return PromoteToQueen | JumpFurther | MaxCapture | MaxPromotedCapture; }
  };

  class ThaiCheckersPosition: public CheckersPosition
  {
  public:
    inline ThaiCheckersPosition(const VariantChosen& v, const PieceMapP& p, const Dimensions& d) noexcept: CheckersPosition(v, p, d) {}
    void SetStartingPosition() noexcept override;
  protected:
    Rule GetRule() const noexcept override { return PromoteToQueen; }
  };

  class TurkishCheckersPosition: public CheckersPosition
  {
  public:
    inline TurkishCheckersPosition(const VariantChosen& v, const PieceMapP& p, const Dimensions& d) noexcept: CheckersPosition(v, p, d) {}
    void SetStartingPosition() noexcept override;
  protected:
    Rule GetRule() const noexcept override { return PromoteToQueen | JumpFurther; }
  };

  class AntiCheckersPosition: public CheckersPosition
  {
  public:
    inline AntiCheckersPosition(const VariantChosen& v, const PieceMapP& p, const Dimensions& d) noexcept: CheckersPosition(v, p, d) {}
  };

  class CornerCheckersPosition: public CheckersPosition
  {
  public:
    inline CornerCheckersPosition(const VariantChosen& v, const PieceMapP& p, const Dimensions& d) noexcept: CheckersPosition(v, p, d) {}
    void SetStartingPosition() noexcept override;
  protected:
    Rule GetRule() const noexcept override { return PromoteToQueen | JumpFurther | PromotedJump; }
    bool CanPromote(const Location& l, const Piece& p) const noexcept override
    {
      return
        (p.IsColor(PieceColor::White) && l.y_ == 0 && l.x_ == 1) ||
        (p.IsColor(PieceColor::White) && l.y_ == 1 && l.x_ == 0) ||
        (p.IsColor(PieceColor::Black) && l.y_ == sizeY_ - 1 && l.x_ == sizeX_ - 2) ||
        (p.IsColor(PieceColor::Black) && l.y_ == sizeY_ - 2 && l.x_ == sizeX_ - 1);
    }
  };

  class HawaiianCheckersPosition: public CheckersPosition
  {
  public:
    inline HawaiianCheckersPosition(const VariantChosen& v, const PieceMapP& p, const Dimensions& d) noexcept: CheckersPosition(v, p, d) {}
    void SetStartingPosition() noexcept override;
  protected:
    Rule GetRule() const noexcept override { return BackJump; }
  };

  class OneWayCheckersPosition: public CheckersPosition
  {
  public:
    inline OneWayCheckersPosition(const VariantChosen& v, const PieceMapP& p, const Dimensions& d) noexcept: CheckersPosition(v, p, d) {}
    void SetStartingPosition() noexcept override;
  protected:
    Rule GetRule() const noexcept override { return MaxCapture; }
  };

  class ParachuteCheckersPosition: public CheckersPosition
  {
  public:
    inline ParachuteCheckersPosition(const VariantChosen& v, const PieceMapP& p, const Dimensions& d) noexcept: CheckersPosition(v, p, d) {}
    void SetStartingPosition() noexcept override;
  protected:
    Rule GetRule() const noexcept override { return BackJump; }
    bool CanPromote(const Location& l, const Piece& p) const noexcept override
    {
      if (p.IsKind(Checkers::Para::ThePara)) return true; // Para can always promotes (to normal piece) when jumping
      return CheckersPosition::CanPromote(l, p);
    }
  };

  class GothicCheckersPosition: public CheckersPosition
  {
  public:
    inline GothicCheckersPosition(const VariantChosen& v, const PieceMapP& p, const Dimensions& d) noexcept: CheckersPosition(v, p, d) {}
    void SetStartingPosition() noexcept override;
  protected:
    Rule GetRule() const noexcept override { return PromotedJump; }
  };

  class DameoCheckersPosition: public CheckersPosition
  {
  public:
    inline DameoCheckersPosition(const VariantChosen& v, const PieceMapP& p, const Dimensions& d) noexcept: CheckersPosition(v, p, d) {}
    void SetStartingPosition() noexcept override;
  protected:
    Rule GetRule() const noexcept override { return PromoteToQueen | JumpFurther | MaxCapture | ContinueJumping; }
  };


  void ItalianCheckersPosition::SetStartingPosition() noexcept
  {
    for (Coordinate j = 0; j < sizeY_ / 2 - 1; j++)
    {
      for (Coordinate i = 0; i < sizeX_; i++)
      {
        if ((i + j) % 2)
          SetPiece(Location(BoardPart::Main, i, sizeY_ - 1 - j), CheckersPiece::CheckersPieceW);
        else
          SetPiece(Location(BoardPart::Main, i, j), CheckersPiece::CheckersPieceB);
      }
    }
  }


  void PortugueseCheckersPosition::SetStartingPosition() noexcept
  {
    for (Coordinate j = 0; j < sizeY_ / 2 - 1; j++)
    {
      for (Coordinate i = 0; i < sizeX_; i++)
      {
        if ((i + j) % 2)
          SetPiece(Location(BoardPart::Main, i, sizeY_ - 1 - j), CheckersPiece::CheckersPieceW);
        else
          SetPiece(Location(BoardPart::Main, i, j), CheckersPiece::CheckersPieceB);
      }
    }
  }


  void CornerCheckersPosition::SetStartingPosition() noexcept
  {
    for (Coordinate j = 0; j < sizeY_; j++)
    {
      for (Coordinate i = 0; i < sizeX_; i++)
      {
        if ((i + j) % 2)
        {
          if ((i + j + 1) < sizeX_)
            SetPiece(Location(BoardPart::Main, i, j), CheckersPiece::CheckersPieceB);
          else if ((i + j) > sizeX_)
            SetPiece(Location(BoardPart::Main, i, j), CheckersPiece::CheckersPieceW);
        }
      }
    }
  }

  void OneWayCheckersPosition::SetStartingPosition() noexcept
  {
    for (Coordinate j = 2; j < sizeY_; j++)
    {
      for (Coordinate i = 0; i < sizeX_; i++)
      {
        if ((i + j) % 2)
        {
          if (i < sizeX_ / 2)
            SetPiece(Location(BoardPart::Main, i, j), CheckersPiece::CheckersPieceB);
          else
            SetPiece(Location(BoardPart::Main, i, j), CheckersPiece::CheckersPieceW);
        }
      }
    }
  }

  void GothicCheckersPosition::SetStartingPosition() noexcept
  {
    for (Coordinate j = 0; j < sizeY_ / 4; j++)
    {
      for (Coordinate i = 0; i < sizeX_; i++)
      {
        SetPiece(Location(BoardPart::Main, i, j), CheckersPiece::CheckersPieceB);
        SetPiece(Location(BoardPart::Main, i, sizeY_ - 1 - j), CheckersPiece::CheckersPieceW);
      }
    }
  }

  void TurkishCheckersPosition::SetStartingPosition() noexcept
  {
    for (Coordinate j = 1; j < sizeY_ / 4 + 1; j++)
    {
      for (Coordinate i = 0; i < sizeX_; i++)
      {
        SetPiece(Location(BoardPart::Main, i, j), CheckersPiece::CheckersPieceB);
        SetPiece(Location(BoardPart::Main, i, sizeY_ - 1 - j), CheckersPiece::CheckersPieceW);
      }
    }
  }

  void ThaiCheckersPosition::SetStartingPosition() noexcept
  {
    for (Coordinate j = 0; j < sizeY_ / 4; j++)
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

  void DameoCheckersPosition::SetStartingPosition() noexcept
  {
    for (Coordinate j = 0; j < sizeY_ / 2 - 1; j++)
    {
      for (Coordinate i = j; i < sizeX_ - j; i++)
      {
        SetPiece(Location(BoardPart::Main, i, j), CheckersPiece::CheckersPieceB);
        SetPiece(Location(BoardPart::Main, i, sizeY_ - 1 - j), CheckersPiece::CheckersPieceW);
      }
    }
  }

  void HawaiianCheckersPosition::SetStartingPosition() noexcept
  {
    for (Coordinate j = 0; j < sizeY_; j++)
    {
      for (Coordinate i = 0; i < sizeX_; i++)
      {
        if ((i + j) % 2)
          SetPiece(Location(BoardPart::Main, i, j), CheckersPiece::CheckersPieceW);
        else
          SetPiece(Location(BoardPart::Main, i, j), CheckersPiece::CheckersPieceB);
      }
    }
  }

  void ParachuteCheckersPosition::SetStartingPosition() noexcept
  {
    for (Coordinate j = 0; j < sizeY_ / 2 - 1; j++)
    {
      for (Coordinate i = 0; i < sizeX_; i++)
      {
        if ((i + j) % 2)
        {
          if (j == 0)
            SetPiece(Location(BoardPart::Main, i, j), CheckersPiece::CheckersParaW);
          else
            SetPiece(Location(BoardPart::Main, i, j), CheckersPiece::CheckersPieceB);
        }
        else
        {
          if (j == 0)
            SetPiece(Location(BoardPart::Main, i, sizeY_ - 1 - j), CheckersPiece::CheckersParaB);
          else
            SetPiece(Location(BoardPart::Main, i, sizeY_ - 1 - j), CheckersPiece::CheckersPieceW);
        }
      }
    }
  }


  MainPosition* CheckersGame::GetNewPosition(const VariantChosen& v, const PieceMapP& m, const Dimensions& d) noexcept
  {
    switch (static_cast<CheckerVariant>(v.c))
    {
      case CheckerVariant::Standard:      return new CheckersPosition              (v, m, d);     // Checkers
      case CheckerVariant::International: return new InternationalCheckersPosition (v, m, d);     // International Checkers
      case CheckerVariant::Brazilian:     return new BrazilianCheckersPosition     (v, m, d);     // Brazilian Checkers
      case CheckerVariant::Canadian:      return new CanadianCheckersPosition      (v, m, d);     // Canadian Checkers
      case CheckerVariant::Czech:         return new CzechCheckersPosition         (v, m, d);     // Czech Checkers
      case CheckerVariant::Italian:       return new ItalianCheckersPosition       (v, m, d);     // Italian Checkers
      case CheckerVariant::Portuguese:    return new PortugueseCheckersPosition    (v, m, d);     // Portuguese Checkers
      case CheckerVariant::Russian:       return new RussianCheckersPosition       (v, m, d);     // Russian Checkers
      case CheckerVariant::Thai:          return new ThaiCheckersPosition          (v, m, d);     // Thai Checkers
      case CheckerVariant::Turkish:       return new TurkishCheckersPosition       (v, m, d);     // Turkish Checkers
      case CheckerVariant::Anti:          return new AntiCheckersPosition          (v, m, d);     // Anti Checkers
      case CheckerVariant::Corner:        return new CornerCheckersPosition        (v, m, d);     // Corner Checkers
      case CheckerVariant::Hawaiian:      return new HawaiianCheckersPosition      (v, m, d);     // Hawaiian Checkers
      case CheckerVariant::OneWay:        return new OneWayCheckersPosition        (v, m, d);     // One Way Checkers
      case CheckerVariant::Parachute:     return new ParachuteCheckersPosition     (v, m, d);     // Parachute Checkers
      case CheckerVariant::Gothic:        return new GothicCheckersPosition        (v, m, d);     // Gothis Checkers
      case CheckerVariant::Dameo:         return new DameoCheckersPosition         (v, m, d);     // Dameo
      default: return nullptr; // must not happen
    }
  }

}


