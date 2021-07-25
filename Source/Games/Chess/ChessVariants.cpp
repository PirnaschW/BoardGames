#include "Games.h"

#include "ChessGame.h"

namespace Chess
{

  class CornerChessPosition : public ChessPosition
  {
  public:
    inline CornerChessPosition(const VariantChosen& v, const PieceMapP& p, const Dimensions& d) noexcept : ChessPosition(v, p, d) {}
    void SetStartingPosition() noexcept override
    {
      SetPawns(1U, PieceColor::Black);
      SetPawns(sizeY_ - 2U, PieceColor::White);

      SetPiecesPSymmetrical(0U, 0U, ChessPiece::BK, ChessPiece::WK);

      std::vector<Coordinate> c;
      for (Coordinate i = 1; i < sizeX_; i++) c.push_back(i);
      do Math::Shuffle(c);
      while (!ValidPosition(c));  // keep shuffling until the position is valid

      SetPiecesPSymmetrical(c[0], 0U, ChessPiece::BQ, ChessPiece::WQ);
      SetPiecesPSymmetrical(c[1], 0U, ChessPiece::BR, ChessPiece::WR);
      SetPiecesPSymmetrical(c[2], 0U, ChessPiece::BR, ChessPiece::WR);
      SetPiecesPSymmetrical(c[3], 0U, ChessPiece::BB, ChessPiece::WB);
      SetPiecesPSymmetrical(c[4], 0U, ChessPiece::BB, ChessPiece::WB);
      SetPiecesPSymmetrical(c[5], 0U, ChessPiece::BN, ChessPiece::WN);
      SetPiecesPSymmetrical(c[6], 0U, ChessPiece::BN, ChessPiece::WN);
    }
  private:
    static bool ValidPosition(const std::vector<Coordinate>& c) noexcept { return c[3] % 2 != c[4] % 2; } // check for bishops on differently colored fields
  };
  
  class FortressChessPosition : public CornerChessPosition
  {
  public:
    inline FortressChessPosition(const VariantChosen& v, const PieceMapP& p, const Dimensions& d) noexcept : CornerChessPosition(v, p, d) {}
    void SetStartingPosition() noexcept override
    {
      CornerChessPosition::SetStartingPosition();
      SetPiecesPSymmetrical(0U, 2U, ChessPiece::BP, ChessPiece::WP);
      SetPiecesPSymmetrical(1U, 2U, ChessPiece::BP, ChessPiece::WP);
      SetPiecesPSymmetrical(2U, 2U, ChessPiece::BP, ChessPiece::WP);
    }
  protected:
  };
  
  class HordeChessPosition : public ChessPosition
  {
  public:
    inline HordeChessPosition(const VariantChosen& v, const PieceMapP& p, const Dimensions& d) noexcept : ChessPosition(v, p, d) {}
    void SetStartingPosition() noexcept override
    {
      ChessPosition::SetStartingPosition();
      SetPawns(0U, PieceColor::Black);
      SetPawns(2U, PieceColor::Black);
      SetPawns(3U, PieceColor::Black);
      SetPiece({ BoardPart::Main, 3U, 4U }, ChessPiece::BP);
      SetPiece({ BoardPart::Main, 4U, 4U }, ChessPiece::BP);
      SetPiece({ BoardPart::Main, 3U, 0U }, Piece::NoPiece);
      SetPiece({ BoardPart::Main, 4U, 0U }, Piece::NoPiece);
    }
  protected:
  };
  
  class LoopChessPosition : public ChessPosition
  {
  public:
    inline LoopChessPosition(const VariantChosen& v, const PieceMapP& p, const Dimensions& d) noexcept : ChessPosition(v, p, d) {}
  protected:
  };
  
  class AntiChessPosition : public ChessPosition
  {
  public:
    inline AntiChessPosition(const VariantChosen& v, const PieceMapP& p, const Dimensions& d) noexcept : ChessPosition(v, p, d) {}
  protected:
  };
  
  class ExtinctionChessPosition : public ChessPosition
  {
  public:
    inline ExtinctionChessPosition(const VariantChosen& v, const PieceMapP& p, const Dimensions& d) noexcept : ChessPosition(v, p, d) {}
  protected:
  };
  
  class MaharajahChessPosition : public ChessPosition
  {
  public:
    inline MaharajahChessPosition(const VariantChosen& v, const PieceMapP& p, const Dimensions& d) noexcept : ChessPosition(v, p, d) {}
    void SetStartingPosition() noexcept override
    {
      ChessPosition::SetStartingPosition();
      for (Coordinate i = 0; i < sizeX_; i++)
      {
        SetPiece({ BoardPart::Main, i, sizeY_ - 2U }, Piece::NoPiece);
        SetPiece({ BoardPart::Main, i, sizeY_ - 1U }, Piece::NoPiece);
      }
      SetPiece({ BoardPart::Main, 4U, sizeY_ - 1U }, ChessPiece::WA);
    }
  protected:
  };
  
  class ThreeChecksChessPosition : public ChessPosition
  {
  public:
    inline ThreeChecksChessPosition(const VariantChosen& v, const PieceMapP& p, const Dimensions& d) noexcept : ChessPosition(v, p, d) {}
  protected:
  };
  
  class DarkChessPosition : public ChessPosition
  {
  public:
    inline DarkChessPosition(const VariantChosen& v, const PieceMapP& p, const Dimensions& d) noexcept : ChessPosition(v, p, d) {}
  protected:
  };
  
  class AtomicChessPosition : public ChessPosition
  {
  public:
    inline AtomicChessPosition(const VariantChosen& v, const PieceMapP& p, const Dimensions& d) noexcept : ChessPosition(v, p, d) {}
  protected:
  };
  
  class JanusChessPosition : public ChessPosition
  {
  public:
    inline JanusChessPosition(const VariantChosen& v, const PieceMapP& p, const Dimensions& d) noexcept : ChessPosition(v, p, d) {}
    void SetStartingPosition() noexcept override
    {
      ChessPosition::SetStartingPosition();
      SetPiecesHSymmetrical(1U, 0U, ChessPiece::BC, ChessPiece::WC);
      SetPiecesHSymmetrical(2U, 0U, ChessPiece::BN, ChessPiece::WN);
      SetPiecesHSymmetrical(3U, 0U, ChessPiece::BB, ChessPiece::WB);
      SetPiecesHSymmetrical(6U, 0U, ChessPiece::BB, ChessPiece::WB);
      SetPiecesHSymmetrical(7U, 0U, ChessPiece::BN, ChessPiece::WN);
      SetPiecesHSymmetrical(8U, 0U, ChessPiece::BC, ChessPiece::WC);
    }
  protected:
  };
  
  class EmbassyChessPosition : public ChessPosition
  {
  public:
    inline EmbassyChessPosition(const VariantChosen& v, const PieceMapP& p, const Dimensions& d) noexcept : ChessPosition(v, p, d) {}
    void SetStartingPosition() noexcept override
    {
      ChessPosition::SetStartingPosition();
      SetPiecesHSymmetrical(3U, 0U, ChessPiece::BQ, ChessPiece::WQ);
      SetPiecesHSymmetrical(4U, 0U, ChessPiece::BK, ChessPiece::WK);
      SetPiecesHSymmetrical(5U, 0U, ChessPiece::BM, ChessPiece::WM);
      SetPiecesHSymmetrical(6U, 0U, ChessPiece::BC, ChessPiece::WC);
    }
  protected:
  };
  
  class ScreenChessPosition : public ChessPosition
  {
  public:
    inline ScreenChessPosition(const VariantChosen& v, const PieceMapP& p, const Dimensions& d) noexcept : ChessPosition(v, p, d) {}
    void SetStartingPosition() noexcept override {}  // start blank
  protected:
  };
  
  class CrazyScreenChessPosition : public ChessPosition
  {
  public:
    inline CrazyScreenChessPosition(const VariantChosen& v, const PieceMapP& p, const Dimensions& d) noexcept : ChessPosition(v, p, d) {}
    void SetStartingPosition() noexcept override {}  // start blank
  protected:
  };
  
  class CylinderChessPosition : public ChessPosition
  {
  public:
    inline CylinderChessPosition(const VariantChosen& v, const PieceMapP& p, const Dimensions& d) noexcept : ChessPosition(v, p, d) {}
  protected:
  };
  
  class AmazonsChessPosition : public ChessPosition
  {
  public:
    inline AmazonsChessPosition(const VariantChosen& v, const PieceMapP& p, const Dimensions& d) noexcept : ChessPosition(v, p, d) {}
    void SetStartingPosition() noexcept override
    {
      ChessPosition::SetStartingPosition();
      SetPiecesHSymmetrical(3U, 0U, ChessPiece::BA, ChessPiece::WA);
    }
  protected:
  };
  
  class BerolinaChessPosition : public ChessPosition
  {
  public:
    inline BerolinaChessPosition(const VariantChosen& v, const PieceMapP& p, const Dimensions& d) noexcept : ChessPosition(v, p, d) {}
  protected:
  };
  
  class FischerRandomChessPosition : public ChessPosition
  {
  public:
    inline FischerRandomChessPosition(const VariantChosen& v, const PieceMapP& p, const Dimensions& d) noexcept : ChessPosition(v, p, d) {}
    void SetStartingPosition() noexcept override
    {
      SetPawns(1U, PieceColor::Black);
      SetPawns(sizeY_ - 2U, PieceColor::White);

      std::vector<Coordinate> c;
      for (Coordinate i = 0; i < sizeX_; i++) c.push_back(i);
      
      do Math::Shuffle(c); while (!ValidPosition(c));   // keep shuffling until position is valid

      SetPiecesHSymmetrical(c[0], 0U, ChessPiece::BQ, ChessPiece::WQ);
      SetPiecesHSymmetrical(c[1], 0U, ChessPiece::BR, ChessPiece::WR);
      SetPiecesHSymmetrical(c[2], 0U, ChessPiece::BR, ChessPiece::WR);
      SetPiecesHSymmetrical(c[3], 0U, ChessPiece::BB, ChessPiece::WB);
      SetPiecesHSymmetrical(c[4], 0U, ChessPiece::BB, ChessPiece::WB);
      SetPiecesHSymmetrical(c[5], 0U, ChessPiece::BN, ChessPiece::WN);
      SetPiecesHSymmetrical(c[6], 0U, ChessPiece::BN, ChessPiece::WN);
      SetPiecesHSymmetrical(c[7], 0U, ChessPiece::BK, ChessPiece::WK);
    }
  private:
    static bool ValidPosition(const std::vector<Coordinate>& c) noexcept
    {
      if (c[3] % 2 == c[4] % 2) return false;       // bishops not on differently colored fields
      if (c[7] > c[1] && c[7] < c[2]) return true;  // king is between the rooks
      if (c[7] > c[2] && c[7] < c[1]) return true;  // king is between the rooks
      return false;
    }
  };
  
  class LeganChessPosition : public ChessPosition
  {
  public:
    inline LeganChessPosition(const VariantChosen& v, const PieceMapP& p, const Dimensions& d) noexcept : ChessPosition(v, p, d) {}
    void SetStartingPosition() noexcept override
    {
      SetPiecesPSymmetrical(0U, 0U, ChessPiece::BK, ChessPiece::WK);
      SetPiecesPSymmetrical(1U, 1U, ChessPiece::BQ, ChessPiece::WQ);
      SetPiecesPSymmetrical(0U, 3U, ChessPiece::BR, ChessPiece::WR);
      SetPiecesPSymmetrical(3U, 0U, ChessPiece::BR, ChessPiece::WR);
      SetPiecesPSymmetrical(0U, 1U, ChessPiece::BB, ChessPiece::WB);
      SetPiecesPSymmetrical(2U, 0U, ChessPiece::BB, ChessPiece::WB);
      SetPiecesPSymmetrical(0U, 2U, ChessPiece::BN, ChessPiece::WN);
      SetPiecesPSymmetrical(1U, 0U, ChessPiece::BN, ChessPiece::WN);

      SetPiecesPSymmetrical(1U, 2U, ChessPiece::BP, ChessPiece::WP);
      SetPiecesPSymmetrical(2U, 1U, ChessPiece::BP, ChessPiece::WP);
      SetPiecesPSymmetrical(4U, 0U, ChessPiece::BP, ChessPiece::WP);
      SetPiecesPSymmetrical(3U, 1U, ChessPiece::BP, ChessPiece::WP);
      SetPiecesPSymmetrical(2U, 2U, ChessPiece::BP, ChessPiece::WP);
      SetPiecesPSymmetrical(1U, 3U, ChessPiece::BP, ChessPiece::WP);
      SetPiecesPSymmetrical(0U, 4U, ChessPiece::BP, ChessPiece::WP);
      SetPiecesPSymmetrical(3U, 3U, ChessPiece::BP, ChessPiece::WP);
    }
  protected:
  };
  
  class KnightRelayChessPosition : public ChessPosition
  {
  public:
    inline KnightRelayChessPosition(const VariantChosen& v, const PieceMapP& p, const Dimensions& d) noexcept : ChessPosition(v, p, d) {}
  protected:
  };
  
  class GrandChessPosition : public ChessPosition
  {
  public:
    inline GrandChessPosition(const VariantChosen& v, const PieceMapP& p, const Dimensions& d) noexcept : ChessPosition(v, p, d) {}
    void SetStartingPosition() noexcept override
    {
      SetPawns(2U, PieceColor::Black);
      SetPawns(sizeY_ - 3U, PieceColor::White);

      SetPiecesHSymmetrical(0U, 0U, ChessPiece::BR, ChessPiece::WR);
      SetPiecesHSymmetrical(1U, 1U, ChessPiece::BN, ChessPiece::WN);
      SetPiecesHSymmetrical(2U, 1U, ChessPiece::BB, ChessPiece::WB);
      SetPiecesHSymmetrical(3U, 1U, ChessPiece::BQ, ChessPiece::WQ);
      SetPiecesHSymmetrical(4U, 1U, ChessPiece::BK, ChessPiece::WK);
      SetPiecesHSymmetrical(5U, 1U, ChessPiece::BM, ChessPiece::WM);
      SetPiecesHSymmetrical(6U, 1U, ChessPiece::BC, ChessPiece::WC);
      SetPiecesHSymmetrical(7U, 1U, ChessPiece::BB, ChessPiece::WB);
      SetPiecesHSymmetrical(8U, 1U, ChessPiece::BN, ChessPiece::WN);
      SetPiecesHSymmetrical(9U, 0U, ChessPiece::BR, ChessPiece::WR);
    }
  protected:
  };
  
  class CapablancaRandomChessPosition : public ChessPosition
  {
  public:
    inline CapablancaRandomChessPosition(const VariantChosen& v, const PieceMapP& p, const Dimensions& d) noexcept : ChessPosition(v, p, d) {}
    void SetStartingPosition() noexcept override
    {
      SetPawns(1U, PieceColor::Black);
      SetPawns(sizeY_ - 2U, PieceColor::White);

      std::vector<Coordinate> c;
      for (Coordinate i = 0; i < sizeX_; i++) c.push_back(i);

      do Math::Shuffle(c); while (!ValidPosition(c));   // keep shuffling until position is valid

      SetPiecesHSymmetrical(c[0], 0U, ChessPiece::BQ, ChessPiece::WQ);
      SetPiecesHSymmetrical(c[1], 0U, ChessPiece::BR, ChessPiece::WR);
      SetPiecesHSymmetrical(c[2], 0U, ChessPiece::BR, ChessPiece::WR);
      SetPiecesHSymmetrical(c[3], 0U, ChessPiece::BB, ChessPiece::WB);
      SetPiecesHSymmetrical(c[4], 0U, ChessPiece::BB, ChessPiece::WB);
      SetPiecesHSymmetrical(c[5], 0U, ChessPiece::BN, ChessPiece::WN);
      SetPiecesHSymmetrical(c[6], 0U, ChessPiece::BN, ChessPiece::WN);
      SetPiecesHSymmetrical(c[7], 0U, ChessPiece::BK, ChessPiece::WK);
      SetPiecesHSymmetrical(c[8], 0U, ChessPiece::BC, ChessPiece::WC);
      SetPiecesHSymmetrical(c[9], 0U, ChessPiece::BM, ChessPiece::WM);
    }
  private:
    static bool ValidPosition(const std::vector<Coordinate>& c) noexcept
    {
      if (c[3] % 2 == c[4] % 2) return false;       // bishops not on differently colored fields
      if (c[7] > c[1] && c[7] < c[2]) return true;  // king is between the rooks
      if (c[7] > c[2] && c[7] < c[1]) return true;  // king is between the rooks
      return false;
    }
  protected:
  };
  
  class LosAlamosChessPosition : public ChessPosition
  {
  public:
    inline LosAlamosChessPosition(const VariantChosen& v, const PieceMapP& p, const Dimensions& d) noexcept : ChessPosition(v, p, d) {}
  protected:
  };
  
  class AmbiguousChessPosition : public ChessPosition
  {
  public:
    inline AmbiguousChessPosition(const VariantChosen& v, const PieceMapP& p, const Dimensions& d) noexcept : ChessPosition(v, p, d) {}
  protected:
  };
  
  class CheversiChessPosition : public ChessPosition
  {
  public:
    inline CheversiChessPosition(const VariantChosen& v, const PieceMapP& p, const Dimensions& d) noexcept : ChessPosition(v, p, d) {}
    void SetStartingPosition() noexcept override {};
  protected:
  };
  
  class DiceChessPosition : public ChessPosition
  {
  public:
    inline DiceChessPosition(const VariantChosen& v, const PieceMapP& p, const Dimensions& d) noexcept : ChessPosition(v, p, d) {}
  protected:
  };
  
  class RecycleChessPosition : public ChessPosition
  {
  public:
    inline RecycleChessPosition(const VariantChosen& v, const PieceMapP& p, const Dimensions& d) noexcept : ChessPosition(v, p, d) {}
  protected:
  };
  
  class IceAgeChessPosition : public ChessPosition
  {
  public:
    inline IceAgeChessPosition(const VariantChosen& v, const PieceMapP& p, const Dimensions& d) noexcept : ChessPosition(v, p, d) {}
    void SetStartingPosition() noexcept override
    {
      // TODO: IceAgeChessPosition::SetStartingPosition
    }
  protected:
  };
  
  class BehemothChessPosition : public ChessPosition
  {
  public:
    inline BehemothChessPosition(const VariantChosen& v, const PieceMapP& p, const Dimensions& d) noexcept : ChessPosition(v, p, d) {}
    void SetStartingPosition() noexcept override
    {
      ChessPosition::SetStartingPosition();
      SetPiece({ BoardPart::Main,3U, 4U }, ChessPiece::WA);
    }
  protected:
  };
  
  class CheshireCatChessPosition : public ChessPosition
  {
  public:
    inline CheshireCatChessPosition(const VariantChosen& v, const PieceMapP& p, const Dimensions& d) noexcept : ChessPosition(v, p, d) {}
  protected:
  };
  
  class KnightmateChessPosition : public ChessPosition
  {
  public:
    inline KnightmateChessPosition(const VariantChosen& v, const PieceMapP& p, const Dimensions& d) noexcept : ChessPosition(v, p, d) {}
  protected:
  };
  
  class RacingKingsChessPosition : public ChessPosition
  {
  public:
    inline RacingKingsChessPosition(const VariantChosen& v, const PieceMapP& p, const Dimensions& d) noexcept : ChessPosition(v, p, d) {}
    void SetStartingPosition() noexcept override
    {
      SetPiece({ BoardPart::Main,              0U,sizeY_ - 1U }, ChessPiece::BQ);
      SetPiece({ BoardPart::Main,              0U,sizeY_ - 2U }, ChessPiece::BK);
      SetPiece({ BoardPart::Main,              1U,sizeY_ - 1U }, ChessPiece::BR);
      SetPiece({ BoardPart::Main,              1U,sizeY_ - 2U }, ChessPiece::BR);
      SetPiece({ BoardPart::Main,              2U,sizeY_ - 1U }, ChessPiece::BB);
      SetPiece({ BoardPart::Main,              2U,sizeY_ - 2U }, ChessPiece::BB);
      SetPiece({ BoardPart::Main,              3U,sizeY_ - 1U }, ChessPiece::BN);
      SetPiece({ BoardPart::Main,              3U,sizeY_ - 2U }, ChessPiece::BN);

      SetPiece({ BoardPart::Main,sizeX_ - 1U - 0U,sizeY_ - 1U }, ChessPiece::WQ);
      SetPiece({ BoardPart::Main,sizeX_ - 1U - 0U,sizeY_ - 2U }, ChessPiece::WK);
      SetPiece({ BoardPart::Main,sizeX_ - 1U - 1U,sizeY_ - 1U }, ChessPiece::WR);
      SetPiece({ BoardPart::Main,sizeX_ - 1U - 1U,sizeY_ - 2U }, ChessPiece::WR);
      SetPiece({ BoardPart::Main,sizeX_ - 1U - 2U,sizeY_ - 1U }, ChessPiece::WB);
      SetPiece({ BoardPart::Main,sizeX_ - 1U - 2U,sizeY_ - 2U }, ChessPiece::WB);
      SetPiece({ BoardPart::Main,sizeX_ - 1U - 3U,sizeY_ - 1U }, ChessPiece::WN);
      SetPiece({ BoardPart::Main,sizeX_ - 1U - 3U,sizeY_ - 2U }, ChessPiece::WN);

    }
  protected:
  };
  
  class Dice10x10ChessPosition : public ChessPosition
  {
  public:
    inline Dice10x10ChessPosition(const VariantChosen& v, const PieceMapP& p, const Dimensions& d) noexcept : ChessPosition(v, p, d) {}
    void SetStartingPosition() noexcept override
    {
      ChessPosition::SetStartingPosition();
      SetPiecesHSymmetrical(              3U, 0U, ChessPiece::BQ, ChessPiece::WQ);
      SetPiecesHSymmetrical(              4U, 0U, ChessPiece::BK, ChessPiece::WK);
      SetPiecesHSymmetrical(sizeX_ - 1U - 3U, 0U, ChessPiece::BK, ChessPiece::WK);
      SetPiecesHSymmetrical(sizeX_ - 1U - 4U, 0U, ChessPiece::BK, ChessPiece::WK);
    }
  protected:
  };
  
  class MassacreChessPosition : public ChessPosition
  {
  public:
    inline MassacreChessPosition(const VariantChosen& v, const PieceMapP& p, const Dimensions& d) noexcept : ChessPosition(v, p, d) {}
    void SetStartingPosition() noexcept override
    {
      std::vector<Location> ll;
      for (Coordinate i = 0; i < sizeX_; i++)
        for (Coordinate j = 0; j < sizeY_; j++)
        {
          const Location l{ BoardPart::Main,  i,j };
          assert(GetPiece(l) == Piece::NoPiece);
          ll.push_back(l);
        }
      assert(ll.size() == sizeX_ * sizeY_);
      Math::Shuffle(ll);
      auto it = ll.begin();
      for (Coordinate z = 0; z < sizeX_ * sizeY_ / 8; z++)
      {
        SetPiece(*it++, ChessPiece::WQ);
        SetPiece(*it++, ChessPiece::BQ);
        SetPiece(*it++, ChessPiece::WR);
        SetPiece(*it++, ChessPiece::BR);
        SetPiece(*it++, ChessPiece::WB);
        SetPiece(*it++, ChessPiece::BB);
        SetPiece(*it++, ChessPiece::WN);
        SetPiece(*it++, ChessPiece::BN);
      }
      assert(it == ll.end());
    }

  protected:
  };
  

  MainPosition* ChessGame::GetNewPosition(const VariantChosen& v, const PieceMapP& m, const Dimensions& d) noexcept
  {
    switch (static_cast<ChessVariant>(v.c))
    {
      case ChessVariant::Standard:          return new ChessPosition                (v, m, d);     // Standard Chess          
      case ChessVariant::Corner:            return new CornerChessPosition          (v, m, d);     // Corner Chess            
      case ChessVariant::Fortress:          return new FortressChessPosition        (v, m, d);     // Fortress Chess          
      case ChessVariant::Horde:             return new HordeChessPosition           (v, m, d);     // Horde Chess             
      case ChessVariant::Loop:              return new LoopChessPosition            (v, m, d);     // Loop Chess              
      case ChessVariant::Anti:              return new AntiChessPosition            (v, m, d);     // Anti Chess              
      case ChessVariant::Extinction:        return new ExtinctionChessPosition      (v, m, d);     // Extinction Chess        
      case ChessVariant::Maharajah:         return new MaharajahChessPosition       (v, m, d);     // Maharajah Chess         
      case ChessVariant::ThreeChecks:       return new ThreeChecksChessPosition     (v, m, d);     // Three Checks Chess      
      case ChessVariant::Dark:              return new DarkChessPosition            (v, m, d);     // Dark Chess              
      case ChessVariant::Atomic:            return new AtomicChessPosition          (v, m, d);     // Atomic Chess            
      case ChessVariant::Janus:             return new JanusChessPosition           (v, m, d);     // Janus Chess             
      case ChessVariant::Embassy:           return new EmbassyChessPosition         (v, m, d);     // Embassy Chess           
      case ChessVariant::Screen:            return new ScreenChessPosition          (v, m, d);     // Screen Chess            
      case ChessVariant::CrazyScreen:       return new CrazyScreenChessPosition     (v, m, d);     // Crazy Screen Chess      
      case ChessVariant::Cylinder:          return new CylinderChessPosition        (v, m, d);     // Cylinder Chess          
      case ChessVariant::Amazons:           return new AmazonsChessPosition         (v, m, d);     // Amazon Chess            
      case ChessVariant::Berolina:          return new BerolinaChessPosition        (v, m, d);     // Berolina Chess          
      case ChessVariant::FischerRandom:     return new FischerRandomChessPosition   (v, m, d);     // Fischer Random Chess    
      case ChessVariant::Legan:             return new LeganChessPosition           (v, m, d);     // Legan Chess             
      case ChessVariant::KnightRelay:       return new KnightRelayChessPosition     (v, m, d);     // Knight Relay Chess      
      case ChessVariant::Grand:             return new GrandChessPosition           (v, m, d);     // Grand Chess             
      case ChessVariant::CapablancaRandom:  return new CapablancaRandomChessPosition(v, m, d);     // Capablanca Random Chess 
      case ChessVariant::LosAlamos:         return new LosAlamosChessPosition       (v, m, d);     // Los Alamos Chess        
      case ChessVariant::Ambiguous:         return new AmbiguousChessPosition       (v, m, d);     // Ambiguous Chess         
      case ChessVariant::Cheversi:          return new CheversiChessPosition        (v, m, d);     // Cheversi
      case ChessVariant::Dice:              return new DiceChessPosition            (v, m, d);     // Dice Chess              
      case ChessVariant::Recycle:           return new RecycleChessPosition         (v, m, d);     // Recycle Chess           
      case ChessVariant::IceAge:            return new IceAgeChessPosition          (v, m, d);     // Ice Age Chess           
      case ChessVariant::Behemoth:          return new BehemothChessPosition        (v, m, d);     // Behemoth Chess          
      case ChessVariant::CheshireCat:       return new CheshireCatChessPosition     (v, m, d);     // Cheshire Cat Chess      
      case ChessVariant::Knightmate:        return new KnightmateChessPosition      (v, m, d);     // Knightmate Chess        
      case ChessVariant::RacingKings:       return new RacingKingsChessPosition     (v, m, d);     // Racing Kings            
      case ChessVariant::Dice10x10:         return new Dice10x10ChessPosition       (v, m, d);     // Dice Chess 10x10        
      case ChessVariant::Massacre:          return new MassacreChessPosition        (v, m, d);     // Massacre Chess
      default: return nullptr; // must not happen
    }
  }


}
