

namespace Chess
{

  enum class ChessVariant : VariantCode   // recognized variants of Chess
  {
    Standard,                               // Standard Chess            
    Corner,                                 // Corner Chess            
    Fortress,                               // Fortress Chess          
    Horde,                                  // Horde Chess             
    Loop,                                   // Loop Chess              
    Anti,                                   // Anti Chess              
    Extinction,                             // Extinction Chess        
    Maharajah,                              // Maharajah Chess         
    ThreeChecks,                            // Three Checks Chess      
    Dark,                                   // Dark Chess              
    Atomic,                                 // Atomic Chess            
    Janus,                                  // Janus Chess             
    Embassy,                                // Embassy Chess           
    Screen,                                 // Screen Chess            
    CrazyScreen,                            // Crazy Screen Chess      
    Cylinder,                               // Cylinder Chess          
    Amazons,                                // Amazon Chess            
    Berolina,                               // Berolina Chess          
    FischerRandom,                          // Fischer Random Chess    
    Legan,                                  // Legan Chess             
    KnightRelay,                            // Knight Relay Chess      
    Grand,                                  // Grand Chess             
    CapablancaRandom,                       // Capablanca Random Chess 
    LosAlamos,                              // Los Alamos Chess        
    Ambiguous,                              // Ambiguous Chess         
    Cheversi,                               // Cheversi
    Dice,                                   // Dice Chess              
    Recycle,                                // Recycle Chess           
    IceAge,                                 // Ice Age Chess           
    Behemoth,                               // Behemoth Chess          
    CheshireCat,                            // Cheshire Cat Chess      
    Knightmate,                             // Knightmate Chess        
    RacingKings,                            // Racing Kings            
    Dice10x10,                              // Dice Chess 10x10         
    Massacre,                               // Massacre Chess
  };
  inline VariantCode VC(ChessVariant v) { return static_cast<VariantCode>(v); }

  enum ChessRule : Rule // multi-use rule variants
  {
    None                 = 0x0000,                         // none of these rules
    CastlingShort        = 0x0001,                         // King side (short) castling is allowed
    CastlingLong         = 0x0002,                         // Queen side (long) castling is allowed
    Castling             = CastlingShort | CastlingLong,   // any castling is allowed
    AllowMoves           = 0x0004,                         // normal moves are allowed
    AllowTakes           = 0x0008,                         // take moves are allowed
    MustTake             = 0x0010,                         // must take if possible
    TakeKing             = 0x0020,                         // King can be taken
    TakeOwn              = 0x0040,                         // own pieces can be taken
    PawnsPromote         = 0x0080,                         // Pawns can promote
    PawnsDoubleStep      = 0x0100,                         // Pawns can make a double-step
    DropTakenPieces      = 0x0200,                         // taken pieces can be dropped as a move
    LeganPawns           = 0x0400,                         // Pawns move diagonal left and hit left and up
    BerolinaPawns        = 0x0800,                         // Pawns move diagonal and take straight
    EnPassant            = 0x1000,                         // taking en passant allowed
  };

}
