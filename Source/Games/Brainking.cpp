#include "Games.h"

#include <filesystem>
#include <fstream>
#include <iostream>

#include "BrainKing.h"

// needed for the game-specific Pieces / Variants:
#include "Ataxx/AtaxxGame.h"
#include "CFour/CFourGame.h"
#include "Checkers/CheckersGame.h"
#include "Chess/ChessGame.h"
#include "Espionage/EspionageGame.h"
#include "Hasami/HasamiGame.h"      
#include "Shogi/ShogiGame.h"        

namespace BoardGamesBK
{

  using BKGameType = unsigned char;


  // local functions and data (not defined in .h / not usable outside this file):
  namespace
  {

    static const std::unordered_map<BKGameType, VariantChosen> BKGameMap{
      {   0, { IDR_GAMETYPE_XXXXX,     '\0',                      0,  0 } },  // dummy for "unknown game type"
      // variants for Ataxx
      {  89, { IDR_GAMETYPE_ATAXX,     Ataxx::Assimilation,       8,  8 } },  // Assimilation
      {  90, { IDR_GAMETYPE_ATAXX,     Ataxx::Ataxx,              8,  8 } },  // Ataxx
      // variants for Cam                                               }
      { 124, { IDR_GAMETYPE_CAMELOT,   'F',                      12, 16 } },  // Camelot
      { 126, { IDR_GAMETYPE_CAMELOT,   'M',                       7, 13 } },  // Cam
      // variants for Connect Four                                      }
      {  13, { IDR_GAMETYPE_CFOUR,     CFour::Classic,            8,  8 } },  // Line4
      {  14, { IDR_GAMETYPE_CFOUR,     CFour::Anti,               8,  8 } },  // Anti Line4
      {  15, { IDR_GAMETYPE_CFOUR,     CFour::Linetris,           8,  8 } },  // Linetris
      {  16, { IDR_GAMETYPE_CFOUR,     CFour::Spider,             8,  8 } },  // Spider Line4
      {  61, { IDR_GAMETYPE_CFOUR,     CFour::SpiderLinetris,     8,  8 } },  // Spider Linetris
      // variants for Checkers                                          }
      {   7, { IDR_GAMETYPE_CHECKERS,  Checkers::Standard,        8,  8 } },  // Checkers                         
      {   8, { IDR_GAMETYPE_CHECKERS,  Checkers::Anti,            8,  8 } },  // Anti Checkers
      {  29, { IDR_GAMETYPE_CHECKERS,  Checkers::Czech,           8,  8 } },  // Czech Checkers
      {  30, { IDR_GAMETYPE_CHECKERS,  Checkers::Corner,          8,  8 } },  // Corner Checkers
      {  31, { IDR_GAMETYPE_CHECKERS,  Checkers::OneWay,          8,  8 } },  // One Way Checkers
      {  32, { IDR_GAMETYPE_CHECKERS,  Checkers::Parachute,       8,  8 } },  // Parachute Checkers
      {  35, { IDR_GAMETYPE_CHECKERS,  Checkers::Gothic,          8,  8 } },  // Gothic Checkers
      {  36, { IDR_GAMETYPE_CHECKERS,  Checkers::Turkish,         8,  8 } },  // Turkish Checkers
      {  94, { IDR_GAMETYPE_CHECKERS,  Checkers::Hawaiian,        8,  8 } },  // Hawaiian Checkers
      { 115, { IDR_GAMETYPE_CHECKERS,  Checkers::International,  10, 10 } },  // International Checkers
      { 117, { IDR_GAMETYPE_CHECKERS,  Checkers::Russian,         8,  8 } },  // Russian Checkers
      { 118, { IDR_GAMETYPE_CHECKERS,  Checkers::Canadian,       12, 12 } },  // Canadian Checkers
      { 119, { IDR_GAMETYPE_CHECKERS,  Checkers::Thai,            8,  8 } },  // Thai Checkers
      { 120, { IDR_GAMETYPE_CHECKERS,  Checkers::Brazilian,       8,  8 } },  // Brazilian Checkers
      { 121, { IDR_GAMETYPE_CHECKERS,  Checkers::Portuguese,      8,  8 } },  // Portuguese Checkers
      { 122, { IDR_GAMETYPE_CHECKERS,  Checkers::Italian,         8,  8 } },  // Italian Checkers
      { 123, { IDR_GAMETYPE_CHECKERS,  Checkers::Dameo,           8,  8 } },  // Dameo
      // variants for Chess                                             }
      {   1, { IDR_GAMETYPE_CHESS,     VC(Chess::ChessVariant::Standard),           8,  8 } },  // Chess
      {   2, { IDR_GAMETYPE_CHESS,     VC(Chess::ChessVariant::Corner),             8,  8 } },  // Corner Chess
      {   3, { IDR_GAMETYPE_CHESS,     VC(Chess::ChessVariant::Fortress),           8,  8 } },  // Fortress Chess
      {   4, { IDR_GAMETYPE_CHESS,     VC(Chess::ChessVariant::Horde),              8,  8 } },  // Horde Chess
      {   6, { IDR_GAMETYPE_CHESS,     VC(Chess::ChessVariant::Loop),               8,  8 } },  // Loop Chess
      {  17, { IDR_GAMETYPE_CHESS,     VC(Chess::ChessVariant::Anti),               8,  8 } },  // Anti Chess
      {  18, { IDR_GAMETYPE_CHESS,     VC(Chess::ChessVariant::Extinction),         8,  8 } },  // Extinction Chess
      {  21, { IDR_GAMETYPE_CHESS,     VC(Chess::ChessVariant::Maharajah),          8,  8 } },  // Maharajah Chess
      {  22, { IDR_GAMETYPE_CHESS,     VC(Chess::ChessVariant::ThreeChecks),        8,  8 } },  // Three Checks Chess
      {  27, { IDR_GAMETYPE_CHESS,     VC(Chess::ChessVariant::Dark),               8,  8 } },  // Dark Chess
      {  28, { IDR_GAMETYPE_CHESS,     VC(Chess::ChessVariant::Atomic),             8,  8 } },  // Atomic Chess
      {  40, { IDR_GAMETYPE_CHESS,     VC(Chess::ChessVariant::Janus),             10,  8 } },  // Janus Chess
      {  41, { IDR_GAMETYPE_CHESS,     VC(Chess::ChessVariant::Embassy),           10,  8 } },  // Embassy Chess
      {  47, { IDR_GAMETYPE_CHESS,     VC(Chess::ChessVariant::Screen),             8,  8 } },  // Screen Chess
      {  48, { IDR_GAMETYPE_CHESS,     VC(Chess::ChessVariant::CrazyScreen),        8,  8 } },  // Crazy Screen Chess
      {  57, { IDR_GAMETYPE_CHESS,     VC(Chess::ChessVariant::Cylinder),           8,  8 } },  // Cylinder Chess
      {  58, { IDR_GAMETYPE_CHESS,     VC(Chess::ChessVariant::Amazons),            8,  8 } },  // Amazon Chess
      {  59, { IDR_GAMETYPE_CHESS,     VC(Chess::ChessVariant::Berolina),           8,  8 } },  // Berolina Chess
      {  67, { IDR_GAMETYPE_CHESS,     VC(Chess::ChessVariant::FischerRandom),      8,  8 } },  // Fischer Random Chess
      {  70, { IDR_GAMETYPE_CHESS,     VC(Chess::ChessVariant::Legan),              8,  8 } },  // Legan Chess
      {  71, { IDR_GAMETYPE_CHESS,     VC(Chess::ChessVariant::KnightRelay),        8,  8 } },  // Knight Relay Chess
      {  74, { IDR_GAMETYPE_CHESS,     VC(Chess::ChessVariant::Grand),             10, 10 } },  // Grand Chess
      {  75, { IDR_GAMETYPE_CHESS,     VC(Chess::ChessVariant::CapablancaRandom),  10,  8 } },  // Capablanca Random Chess
      {  76, { IDR_GAMETYPE_CHESS,     VC(Chess::ChessVariant::LosAlamos),          6,  6 } },  // Los Alamos Chess
      {  85, { IDR_GAMETYPE_CHESS,     VC(Chess::ChessVariant::Ambiguous),          8,  8 } },  // Ambiguous Chess
      {  93, { IDR_GAMETYPE_CHESS,     VC(Chess::ChessVariant::Cheversi),           8,  8 } },  // Cheversi
      {  95, { IDR_GAMETYPE_CHESS,     VC(Chess::ChessVariant::Dice),               8,  8 } },  // Dice Chess
      {  98, { IDR_GAMETYPE_CHESS,     VC(Chess::ChessVariant::Recycle),            8,  8 } },  // Recycle Chess
      { 100, { IDR_GAMETYPE_CHESS,     VC(Chess::ChessVariant::IceAge),             8,  8 } },  // Ice Age Chess
      { 104, { IDR_GAMETYPE_CHESS,     VC(Chess::ChessVariant::Behemoth),           8,  8 } },  // Behemoth Chess
      { 113, { IDR_GAMETYPE_CHESS,     VC(Chess::ChessVariant::CheshireCat),        8,  8 } },  // Cheshire Cat Chess
      { 116, { IDR_GAMETYPE_CHESS,     VC(Chess::ChessVariant::Knightmate),         8,  8 } },  // Knightmate Chess
      { 125, { IDR_GAMETYPE_CHESS,     VC(Chess::ChessVariant::RacingKings),        8,  8 } },  // Racing Kings
      { 127, { IDR_GAMETYPE_CHESS,     VC(Chess::ChessVariant::Dice10x10),         10, 10 } },  // Dice Chess 10x10
      { 128, { IDR_GAMETYPE_CHESS,     VC(Chess::ChessVariant::Massacre),           8,  8 } },  // Massacre Chess
 
    //{  93, { IDR_GAMETYPE_CHEVERSI,  '\0',                      8,  8 } },  // Cheversi
 
     // TODO: variants for Espionage
      {  55, { IDR_GAMETYPE_ESPIONAGE, '\0',                     10, 10 } },  // Espionage
      {  62, { IDR_GAMETYPE_ESPIONAGE, '\0',                      8,  8 } },  // Small Espionage
      {  63, { IDR_GAMETYPE_ESPIONAGE, '\0',                     10, 10 } },  // Fast Espionage
      {  87, { IDR_GAMETYPE_ESPIONAGE, '\0',                     10, 10 } },  // Open Fast Espionage
      {  88, { IDR_GAMETYPE_ESPIONAGE, '\0',                      8,  8 } },  // Small Fast Espionage
                                                                          
      {  73, { IDR_GAMETYPE_HASAMI,    '\0',                      8,  8 } },  // Hasami Shogi
                                                                          
      { 110, { IDR_GAMETYPE_KNIGHTF,   '\0',                      8,  8 } },  // Knight Fight
      // TODO: variants for LoA                                           
      {  83, { IDR_GAMETYPE_LOA,       '\0',                      8,  8 } },  // Lines of Action
      {  86, { IDR_GAMETYPE_LOA,       '\0',                      8,  8 } },  // Scrambled Eggs
      { 112, { IDR_GAMETYPE_LOA,       '\0',                      8,  8 } },  // Parachute Lines of Action
      { 114, { IDR_GAMETYPE_LOA,       '\0',                      8,  8 } },  // Cheshire Cat Lines of Action
                                                                          
      { 102, { IDR_GAMETYPE_LOGIK,     '\0',                      8,  8 } },  // Logik
                                                                          
      // TODO: variants for Shogi                                         
      {  69, { IDR_GAMETYPE_SHOGI,     '\0',                      8,  8 } },  // Japanese Chess
      {  91, { IDR_GAMETYPE_SHOGI,     'M',                       8,  8 } },  // Minishogi
                                                                          
      {  20, { IDR_GAMETYPE_TANKB,     '\0',                      8,  8 } },  // Tank Battle
                                                                          
      // TODO: other games                                                
      {   5, { IDR_GAMETYPE_XXXXX,     '\0',                      8,  8 } },  // Five in Line
      {   9, { IDR_GAMETYPE_XXXXX,     '\0',                      8,  8 } },  // Reversi 6x6
      {  10, { IDR_GAMETYPE_XXXXX,     '\0',                      8,  8 } },  // Reversi 8x8
      {  11, { IDR_GAMETYPE_XXXXX,     '\0',                      8,  8 } },  // Reversi 10x10
      {  12, { IDR_GAMETYPE_XXXXX,     '\0',                      8,  8 } },  // Anti Reversi
      {  19, { IDR_GAMETYPE_XXXXX,     '\0',                      8,  8 } },  // Tablut
      {  23, { IDR_GAMETYPE_XXXXX,     '\0',                      8,  8 } },  // Backgammon
      {  24, { IDR_GAMETYPE_XXXXX,     '\0',                      8,  8 } },  // Nackgammon
      {  25, { IDR_GAMETYPE_XXXXX,     '\0',                      8,  8 } },  // Anti Backgammon
      {  26, { IDR_GAMETYPE_XXXXX,     '\0',                      8,  8 } },  // Backgammon Race
      {  33, { IDR_GAMETYPE_XXXXX,     '\0',                      8,  8 } },  // Halma 8x8
      {  34, { IDR_GAMETYPE_XXXXX,     '\0',                      8,  8 } },  // Halma 10x10
      {  37, { IDR_GAMETYPE_XXXXX,     '\0',                      8,  8 } },  // Crowded Backgammon
      {  38, { IDR_GAMETYPE_XXXXX,     '\0',                      8,  8 } },  // Pente
      {  39, { IDR_GAMETYPE_XXXXX,     '\0',                      8,  8 } },  // Keryo Pente
      {  42, { IDR_GAMETYPE_XXXXX,     '\0',                      8,  8 } },  // Small Pente
      {  43, { IDR_GAMETYPE_XXXXX,     '\0',                      8,  8 } },  // Small Keryo Pente
      {  44, { IDR_GAMETYPE_XXXXX,     '\0',                      8,  8 } },  // Open Pente
      {  45, { IDR_GAMETYPE_XXXXX,     '\0',                      8,  8 } },  // Open Keryo Pente
      {  46, { IDR_GAMETYPE_XXXXX,     '\0',                      8,  8 } },  // Battleboats
      {  49, { IDR_GAMETYPE_XXXXX,     '\0',                      8,  8 } },  // Amazons
      {  50, { IDR_GAMETYPE_XXXXX,     '\0',                      8,  8 } },  // Battleboats Plus
      {  51, { IDR_GAMETYPE_XXXXX,     '\0',                      8,  8 } },  // Dark Battleboats
      {  52, { IDR_GAMETYPE_XXXXX,     '\0',                      8,  8 } },  // Jarmo
      {  53, { IDR_GAMETYPE_XXXXX,     '\0',                      8,  8 } },  // Alquerque
      {  54, { IDR_GAMETYPE_XXXXX,     '\0',                      8,  8 } },  // Froglet
      {  56, { IDR_GAMETYPE_XXXXX,     '\0',                      8,  8 } },  // Jungle
      {  60, { IDR_GAMETYPE_XXXXX,     '\0',                      8,  8 } },  // Anti Froglet
      {  64, { IDR_GAMETYPE_XXXXX,     '\0',                      8,  8 } },  // Pro Five in Line
      {  65, { IDR_GAMETYPE_XXXXX,     '\0',                      8,  8 } },  // Swap Five in Line
      {  66, { IDR_GAMETYPE_XXXXX,     '\0',                      8,  8 } },  // Hyper Backgammon
      {  68, { IDR_GAMETYPE_XXXXX,     '\0',                      8,  8 } },  // Chinese Chess
      {  72, { IDR_GAMETYPE_XXXXX,     '\0',                      8,  8 } },  // PahTum
      {  77, { IDR_GAMETYPE_XXXXX,     '\0',                      8,  8 } },  // Sphere Froglet
      {  78, { IDR_GAMETYPE_XXXXX,     '\0',                      8,  8 } },  // Ludo
      {  79, { IDR_GAMETYPE_XXXXX,     '\0',                      8,  8 } },  // Go
      {  80, { IDR_GAMETYPE_XXXXX,     '\0',                      8,  8 } },  // Go 9x9
      {  81, { IDR_GAMETYPE_XXXXX,     '\0',                      8,  8 } },  // Go 13x13
      {  82, { IDR_GAMETYPE_XXXXX,     '\0',                      8,  8 } },  // Connect6
      {  84, { IDR_GAMETYPE_XXXXX,     '\0',                      8,  8 } },  // Breakthrough
      {  92, { IDR_GAMETYPE_XXXXX,     '\0',                      8,  8 } },  // Cloning Backgammon
      {  96, { IDR_GAMETYPE_XXXXX,     '\0',                      8,  8 } },  // Grasshopper
      {  97, { IDR_GAMETYPE_XXXXX,     '\0',                      8,  8 } },  // Dice Poker
      {  99, { IDR_GAMETYPE_XXXXX,     '\0',                      8,  8 } },  // Triple Dice Poker
      { 101, { IDR_GAMETYPE_XXXXX,     '\0',                      8,  8 } },  // Frog Finder
      { 103, { IDR_GAMETYPE_XXXXX,     '\0',                      8,  8 } },  // Mancala
      { 105, { IDR_GAMETYPE_XXXXX,     '\0',                      8,  8 } },  // Frog Legs
      { 106, { IDR_GAMETYPE_XXXXX,     '\0',                      8,  8 } },  // Dice Poker 6D
      { 107, { IDR_GAMETYPE_XXXXX,     '\0',                      8,  8 } },  // Triple Dice Poker 6D
      { 108, { IDR_GAMETYPE_XXXXX,     '\0',                      8,  8 } },  // Big Jungle
      { 109, { IDR_GAMETYPE_XXXXX,     '\0',                      8,  8 } },  // Plakoto
      { 111, { IDR_GAMETYPE_XXXXX,     '\0',                      8,  8 } },  // Fevga
    };

    static const std::unordered_map<std::string, const Piece&> map_
    {
      { R"(blank35)",                 Piece::NoPiece },
      { R"(blank50)",                 Piece::NoPiece },

      // ##############################################
      // Chess pieces
      { R"(chess/wp35)",              Chess::ChessPiece::WP },               // White Pawn
      { R"(chess/wn35)",              Chess::ChessPiece::WN },               // White Knight
      { R"(chess/wb35)",              Chess::ChessPiece::WB },               // White Bishop
      { R"(chess/wr35)",              Chess::ChessPiece::WR },               // White Rook 
      { R"(chess/wa35)",              Chess::ChessPiece::WC },               // White Cardinal = Bishop + Knight (also: Archbishop, Princess, Janus)
      { R"(chess/wc35)",              Chess::ChessPiece::WM },               // White Marshall = Rook + Knight
      { R"(chess/wq35)",              Chess::ChessPiece::WQ },               // White Queen
      { R"(chess/wz35)",              Chess::ChessPiece::WA },               // White Amazon   = Queen + Knight
      { R"(chess/wk35)",              Chess::ChessPiece::WK },               // White King 

      { R"(chess/bp35)",              Chess::ChessPiece::BP },               // Black Pawn 
      { R"(chess/bn35)",              Chess::ChessPiece::BN },               // Black Knight
      { R"(chess/bb35)",              Chess::ChessPiece::BB },               // Black Bishop
      { R"(chess/br35)",              Chess::ChessPiece::BR },               // Black Rook 
      { R"(chess/ba35)",              Chess::ChessPiece::BC },               // Black Cardinal = Bishop + Knight (also: Archbishop, Princess, Janus)
      { R"(chess/bc35)",              Chess::ChessPiece::BM },               // Black Marshall = Rook + Knight
      { R"(chess/bq35)",              Chess::ChessPiece::BQ },               // Black Queen
      { R"(chess/bz35)",              Chess::ChessPiece::BA },               // Black Amazon   = Queen + Knight
      { R"(chess/bk35)",              Chess::ChessPiece::BK },               // Black King 

      { R"(chess/behemoth35)",        Chess::ChessPiece::RB },               // (Red) Behemoth
      { R"(chess/ice35)",             Chess::ChessPiece::GI },               // (Gray) Ice


       // taken pieces - ignore for now
      { R"(chess/wp1)",               Piece::NoTile },                      // White Pawn
      { R"(chess/wn1)",               Piece::NoTile },                      // White Knight
      { R"(chess/wb1)",               Piece::NoTile },                      // White Bishop
      { R"(chess/wr1)",               Piece::NoTile },                      // White Rook
      { R"(chess/wa1)",               Piece::NoTile },                      // White Cardinal = Bishop + Knight (also: Archbishop, Princess, Janus) 
      { R"(chess/wc1)",               Piece::NoTile },                      // White Marshall = Rook + Knight 
      { R"(chess/wq1)",               Piece::NoTile },                      // White Queen 
      { R"(chess/wz1)",               Piece::NoTile },                      // White Amazon   = Queen + Knight 
      { R"(chess/wk1)",               Piece::NoTile },                      // White King  
                                               
      { R"(chess/bp1)",               Piece::NoTile },                      // Black Pawn  
      { R"(chess/bn1)",               Piece::NoTile },                      // Black Knight 
      { R"(chess/bb1)",               Piece::NoTile },                      // Black Bishop 
      { R"(chess/br1)",               Piece::NoTile },                      // Black Rook 
      { R"(chess/ba1)",               Piece::NoTile },                      // Black Cardinal = Bishop + Knight (also: Archbishop, Princess, Janus) 
      { R"(chess/bc1)",               Piece::NoTile },                      // Black Marshall = Rook + Knight 
      { R"(chess/bq1)",               Piece::NoTile },                      // Black Queen 
      { R"(chess/bz1)",               Piece::NoTile },                      // Black Amazon   = Queen + Knight 
      { R"(chess/bk1)",               Piece::NoTile },                      // Black King 

      // ##############################################
      // Checkers
      { R"(checkers/wp35)",           Checkers::CheckersPiece::CheckersPieceW },
      { R"(checkers/bp35)",           Checkers::CheckersPiece::CheckersPieceB },

      // ##############################################
      // Connect Four
      { R"(line4/w35)",               CorePiece::WC },
      { R"(line4/b35)",               CorePiece::BC },

      // ##############################################
      // Espionage
      { R"(espionage/wg135)",         Espionage::EPiece::WGeneral1 },
      { R"(espionage/wg235)",         Espionage::EPiece::WGeneral2 },
      { R"(espionage/wg335)",         Espionage::EPiece::WGeneral3 },
      { R"(espionage/wg435)",         Espionage::EPiece::WGeneral4 },
      { R"(espionage/wg535)",         Espionage::EPiece::WGeneral5 },
      { R"(espionage/wy35)",          Espionage::EPiece::WSpy },
      { R"(espionage/ws35)",          Espionage::EPiece::WSapper },
      { R"(espionage/wm35)",          Espionage::EPiece::WMine },
      { R"(espionage/wh35)",          Espionage::EPiece::WHeadquarter },
      { R"(espionage/bg135)",         Espionage::EPiece::BGeneral1 },
      { R"(espionage/bg235)",         Espionage::EPiece::BGeneral2 },
      { R"(espionage/bg335)",         Espionage::EPiece::BGeneral3 },
      { R"(espionage/bg435)",         Espionage::EPiece::BGeneral4 },
      { R"(espionage/bg535)",         Espionage::EPiece::BGeneral5 },
      { R"(espionage/by35)",          Espionage::EPiece::BSpy },
      { R"(espionage/bs35)",          Espionage::EPiece::BSapper },
      { R"(espionage/bm35)",          Espionage::EPiece::BMine },
      { R"(espionage/bh35)",          Espionage::EPiece::BHeadquarter },
      { R"(espionage/volcano35)",     Espionage::EPiece::Volcano },

      // ##############################################
      // Shogi
      { R"(shogi/i-p35d)",            Shogi::ShogiPiece::ShogiWP },
      { R"(shogi/i-b35d)",            Shogi::ShogiPiece::ShogiWB },
      { R"(shogi/i-r35d)",            Shogi::ShogiPiece::ShogiWR },
      { R"(shogi/i-l35d)",            Shogi::ShogiPiece::ShogiWL },
      { R"(shogi/i-n35d)",            Shogi::ShogiPiece::ShogiWN },
      { R"(shogi/i-sg35d)",           Shogi::ShogiPiece::ShogiWS },
      { R"(shogi/i-gg35d)",           Shogi::ShogiPiece::ShogiWG },
      { R"(shogi/i-k35d)",            Shogi::ShogiPiece::ShogiWK },
      { R"(shogi/i-p35u)",            Shogi::ShogiPiece::ShogiBP },
      { R"(shogi/i-l35u)",            Shogi::ShogiPiece::ShogiBL },
      { R"(shogi/i-n35u)",            Shogi::ShogiPiece::ShogiBN },
      { R"(shogi/i-sg35u)",           Shogi::ShogiPiece::ShogiBS },
      { R"(shogi/i-gg35u)",           Shogi::ShogiPiece::ShogiBG },
      { R"(shogi/i-k35u)",            Shogi::ShogiPiece::ShogiBK },
      { R"(shogi/i-r35u)",            Shogi::ShogiPiece::ShogiBR },
      { R"(shogi/i-b35u)",            Shogi::ShogiPiece::ShogiBB },
      { R"(hshogi/b1)",               Piece::NoTile },                       // taken pieces - ignore for now
      { R"(hshogi/w1)",               Piece::NoTile },                       // taken pieces - ignore for now
      { R"(hshogi/w35)",              Hasami::HasamiPiece::HasamiPieceW },
      { R"(hshogi/b35)",              Hasami::HasamiPiece::HasamiPieceB },

      // ##############################################
      // Backgammon
      { R"(back/redo25)",             Piece::NoPiece },                      // backgammon: red, down row - ignore for now
      { R"(back/wredo25-1)",          Piece::NoPiece },                      // backgammon: red, down row, 1 white piece - ignore for now
      { R"(back/wredo25-2)",          Piece::NoPiece },                      // backgammon: red, down row, 2 white pieces - ignore for now
      { R"(back/wredo25-3)",          Piece::NoPiece },                      // backgammon: red, down row, 3 white pieces - ignore for now
      { R"(back/wredo25-4)",          Piece::NoPiece },                      // backgammon: red, down row, 4 white pieces - ignore for now
      { R"(back/bredo25-1)",          Piece::NoPiece },                      // backgammon: red, down row, 1 black piece - ignore for now
      { R"(back/bredo25-2)",          Piece::NoPiece },                      // backgammon: red, down row, 2 black pieces - ignore for now
      { R"(back/bredo25-3)",          Piece::NoPiece },                      // backgammon: red, down row, 3 black pieces - ignore for now
      { R"(back/bredo25-4)",          Piece::NoPiece },                      // backgammon: red, down row, 4 black pieces - ignore for now
      { R"(back/pudo25)",             Piece::NoPiece },                      // backgammon: purple, up(per) row - ignore for now
      { R"(back/wpudo25-1)",          Piece::NoPiece },                      // backgammon: purple, down row, 1 white piece - ignore for now
      { R"(back/wpudo25-2)",          Piece::NoPiece },                      // backgammon: purple, down row, 2 white pieces - ignore for now
      { R"(back/wpudo25-3)",          Piece::NoPiece },                      // backgammon: purple, down row, 2 white pieces - ignore for now
      { R"(back/wpudo25-4)",          Piece::NoPiece },                      // backgammon: purple, down row, 2 white pieces - ignore for now
      { R"(back/bpudo25-1)",          Piece::NoPiece },                      // backgammon: purple, down row, 1 black piece - ignore for now
      { R"(back/bpudo25-2)",          Piece::NoPiece },                      // backgammon: purple, down row, 2 black pieces - ignore for now
      { R"(back/bpudo25-3)",          Piece::NoPiece },                      // backgammon: purple, down row, 3 black pieces - ignore for now
      { R"(back/bpudo25-4)",          Piece::NoPiece },                      // backgammon: purple, down row, 4 black pieces - ignore for now
      { R"(back/reup25)",             Piece::NoPiece },                      // backgammon: red, up(per) row - ignore for now
      { R"(back/puup25)",             Piece::NoPiece },                      // backgammon: purple, up(per) row - ignore for now

      // ##############################################
      // others...
      { R"(dot25)",                   Piece::NoPiece },                      // used in Battle boats
      { R"(explode35)",               Piece::NoPiece },                      // disallowed field in Cheshire Cat variants, for now: treat like blank
    };

    std::vector<const Piece*> ListFromHTML(const std::string& s)
    {
      std::vector<const Piece*> list{};
      std::string::size_type p1{ 0 };

      static const std::string s1{ R"(<img src=https://i.brainking.com/)" };
      static const std::string s2{ R"(.gif)" };
      while ((p1 = s.find(s1, p1)) != std::string::npos)                       // move p1 to the next image (= instance of s1)
      {
        p1 += s1.size();                                                       // move p1 to behind the image string (s1)
        auto p2 = s.find(s2, p1);                                              // find the end of the image name
        const auto& it{ map_.find(s.substr(p1, p2 - p1)) };                    // lookup the image name
        assert(it != map_.end());                                              // if not found, stop right here - needs to be added in the map
        if (it->second != Piece::NoTile) list.push_back(&(it->second));        // add to Piece stack, but ignore NoTile
      }
      return list;
    }

    const std::string GetBetween(const std::string& s, const std::string& s0, const std::string& s1)
    {
      const std::string::size_type p0{ s.find(s0) + s0.size() };
      const std::string::size_type p1{ s.find(s1,p0) };
      return s.substr(p0, p1 - p0);
    }

  }

  void DownloadInput(const std::string& source, const std::string& target)
  {
    URL::Cookies cookies{};
    cookies.push_back({ "http://brainking.com","JSESSIONID","18j066c3wgj91cdeae9ohktd6" });

    std::string s = URL::GetHTMLFromURL(source, cookies);
    std::ofstream outf(target, std::ios::out);
    outf << s;
  }

  VariantChosen BKGame::LoadGame(std::vector<const Piece*>& list) const
  {
    DownloadInput(R"(http://brainking.com/en/ShowGame?g=)" + std::to_string(no_),
                  R"(D:\Projects\BoardGames\)"+ std::to_string(no_) + R"(.html)");


    // try to read a Current game
    const std::string urlc = R"(http://brainking.com/en/ShowGame?g=)" + std::to_string(no_);
    URL::Cookies cookies{};
    //cookies.push_back({ "http://brainking.com","JSESSIONID","18j066c3wgj91cdeae9ohktd6" });
    std::string html = URL::GetHTMLFromURL(urlc, cookies);
    if (html.size() < 256)    // if this results in a redirect, try to read as Archived game
    {
      const std::string urla = R"(http://brainking.com/en/ArchivedGame?g=)" + std::to_string(no_);
      html = URL::GetHTMLFromURL(urla, cookies);
    }

    // TODO: analyze HTML properly

    const std::string tp = GetBetween(html, R"(<a href="GameRules?tp=)", R"(&)");
    BKGameType gametype = stoi(tp);

    const auto& it{ BKGameMap.find(gametype) };
    assert(it != BKGameMap.end());        // stop immediately if unknown game type encountered                                 
    if (it->second.id)                    // if the game type is implemented, read the board
    {
      const std::string board = GetBetween(html, R"(<div id="game-board-section">)", R"(<div id="game-info-section">)");
      // TODO: recognize and skip/treat Captured Pieces
      list = ListFromHTML(board);
    }

    return it->second;
  }

}