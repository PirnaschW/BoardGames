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

      SetPiece({ BoardPart::Main,          0U,          0U }, ChessPiece::BK);
      SetPiece({ BoardPart::Main, sizeX_ - 1U, sizeY_ - 1U }, ChessPiece::WK);

      std::vector<Coordinate> c;
      for (Coordinate i = 1; i < sizeX_; i++) c.push_back(i);
      do Math::Shuffle(c);
      while (!((c[3] + c[4]) % 2));  // keep shuffling until the bishops are on different color fields

      SetPiece(Location{ BoardPart::Main, sizeX_ - 1U - c[0], sizeY_ - 1U }, ChessPiece::WQ);
      SetPiece(Location{ BoardPart::Main,               c[0], 0U          }, ChessPiece::BQ);
      SetPiece(Location{ BoardPart::Main, sizeX_ - 1U - c[1], sizeY_ - 1U }, ChessPiece::WR);
      SetPiece(Location{ BoardPart::Main,               c[1], 0U          }, ChessPiece::BR);
      SetPiece(Location{ BoardPart::Main, sizeX_ - 1U - c[2], sizeY_ - 1U }, ChessPiece::WR);
      SetPiece(Location{ BoardPart::Main,               c[2], 0U          }, ChessPiece::BR);
      SetPiece(Location{ BoardPart::Main, sizeX_ - 1U - c[3], sizeY_ - 1U }, ChessPiece::WB);
      SetPiece(Location{ BoardPart::Main,               c[3], 0U          }, ChessPiece::BB);
      SetPiece(Location{ BoardPart::Main, sizeX_ - 1U - c[4], sizeY_ - 1U }, ChessPiece::WB);
      SetPiece(Location{ BoardPart::Main,               c[4], 0U          }, ChessPiece::BB);
      SetPiece(Location{ BoardPart::Main, sizeX_ - 1U - c[5], sizeY_ - 1U }, ChessPiece::WN);
      SetPiece(Location{ BoardPart::Main,               c[5], 0U          }, ChessPiece::BN);
      SetPiece(Location{ BoardPart::Main, sizeX_ - 1U - c[6], sizeY_ - 1U }, ChessPiece::WN);
      SetPiece(Location{ BoardPart::Main,               c[6], 0U          }, ChessPiece::BN);
    }
  protected:
  };
  
  class FortressChessPosition : public ChessPosition
  {
  public:
    inline FortressChessPosition(const VariantChosen& v, const PieceMapP& p, const Dimensions& d) noexcept : ChessPosition(v, p, d) {}
    void SetStartingPosition() noexcept override;
  protected:
  };
  
  class HordeChessPosition : public ChessPosition
  {
  public:
    inline HordeChessPosition(const VariantChosen& v, const PieceMapP& p, const Dimensions& d) noexcept : ChessPosition(v, p, d) {}
    void SetStartingPosition() noexcept override;
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
    void SetStartingPosition() noexcept override;
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
    void SetStartingPosition() noexcept override;
  protected:
  };
  
  class EmbassyChessPosition : public ChessPosition
  {
  public:
    inline EmbassyChessPosition(const VariantChosen& v, const PieceMapP& p, const Dimensions& d) noexcept : ChessPosition(v, p, d) {}
    void SetStartingPosition() noexcept override;
  protected:
  };
  
  class ScreenChessPosition : public ChessPosition
  {
  public:
    inline ScreenChessPosition(const VariantChosen& v, const PieceMapP& p, const Dimensions& d) noexcept : ChessPosition(v, p, d) {}
    void SetStartingPosition() noexcept override;
  protected:
  };
  
  class CrazyScreenChessPosition : public ChessPosition
  {
  public:
    inline CrazyScreenChessPosition(const VariantChosen& v, const PieceMapP& p, const Dimensions& d) noexcept : ChessPosition(v, p, d) {}
    void SetStartingPosition() noexcept override;
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
    void SetStartingPosition() noexcept override;
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
    void SetStartingPosition() noexcept override;
  protected:
  };
  
  class LeganChessPosition : public ChessPosition
  {
  public:
    inline LeganChessPosition(const VariantChosen& v, const PieceMapP& p, const Dimensions& d) noexcept : ChessPosition(v, p, d) {}
    void SetStartingPosition() noexcept override;
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
    void SetStartingPosition() noexcept override;
  protected:
  };
  
  class CapablancaRandomChessPosition : public ChessPosition
  {
  public:
    inline CapablancaRandomChessPosition(const VariantChosen& v, const PieceMapP& p, const Dimensions& d) noexcept : ChessPosition(v, p, d) {}
    void SetStartingPosition() noexcept override;
  protected:
  };
  
  class LosAlamosChessPosition : public ChessPosition
  {
  public:
    inline LosAlamosChessPosition(const VariantChosen& v, const PieceMapP& p, const Dimensions& d) noexcept : ChessPosition(v, p, d) {}
    void SetStartingPosition() noexcept override;
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
    void SetStartingPosition() noexcept override;
  protected:
  };
  
  class BehemothChessPosition : public ChessPosition
  {
  public:
    inline BehemothChessPosition(const VariantChosen& v, const PieceMapP& p, const Dimensions& d) noexcept : ChessPosition(v, p, d) {}
    void SetStartingPosition() noexcept override;
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
    void SetStartingPosition() noexcept override;
  protected:
  };
  
  class Dice10x10ChessPosition : public ChessPosition
  {
  public:
    inline Dice10x10ChessPosition(const VariantChosen& v, const PieceMapP& p, const Dimensions& d) noexcept : ChessPosition(v, p, d) {}
    void SetStartingPosition() noexcept override;
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
  

  void FortressChessPosition::SetStartingPosition(void) noexcept {}          // TODO: FortressChessPosition::SetStartingPosition(void) {}        
  void HordeChessPosition::SetStartingPosition(void) noexcept {}             // TODO: HordeChessPosition::SetStartingPosition(void) {}           
  void MaharajahChessPosition::SetStartingPosition(void) noexcept {}         // TODO: MaharajahChessPosition::SetStartingPosition(void) {}       
  void JanusChessPosition::SetStartingPosition(void) noexcept {}             // TODO: JanusChessPosition::SetStartingPosition(void) {}           
  void EmbassyChessPosition::SetStartingPosition(void) noexcept {}           // TODO: EmbassyChessPosition::SetStartingPosition(void) {}         
  void ScreenChessPosition::SetStartingPosition(void) noexcept {}            // TODO: ScreenChessPosition::SetStartingPosition(void) {}          
  void CrazyScreenChessPosition::SetStartingPosition(void) noexcept {}       // TODO: CrazyScreenChessPosition::SetStartingPosition(void) {}     
  void AmazonsChessPosition::SetStartingPosition(void) noexcept {}           // TODO: AmazonsChessPosition::SetStartingPosition(void) {}         
  void FischerRandomChessPosition::SetStartingPosition(void) noexcept {}     // TODO: FischerRandomChessPosition::SetStartingPosition(void) {}   
  void LeganChessPosition::SetStartingPosition(void) noexcept {}             // TODO: LeganChessPosition::SetStartingPosition(void) {}           
  void GrandChessPosition::SetStartingPosition(void) noexcept {}             // TODO: GrandChessPosition::SetStartingPosition(void) {}           
  void CapablancaRandomChessPosition::SetStartingPosition(void) noexcept {}  // TODO: CapablancaRandomChessPosition::SetStartingPosition(void) {}
  void LosAlamosChessPosition::SetStartingPosition(void) noexcept {}         // TODO: LosAlamosChessPosition::SetStartingPosition(void) {}       
  void IceAgeChessPosition::SetStartingPosition(void) noexcept {}            // TODO: IceAgeChessPosition::SetStartingPosition(void) {}          
  void BehemothChessPosition::SetStartingPosition(void) noexcept {}          // TODO: BehemothChessPosition::SetStartingPosition(void) {}        
  void RacingKingsChessPosition::SetStartingPosition(void) noexcept {}       // TODO: RacingKingsChessPosition::SetStartingPosition(void) {}     
  void Dice10x10ChessPosition::SetStartingPosition(void) noexcept {}         // TODO: Dice10x10ChessPosition::SetStartingPosition(void) {}       





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
