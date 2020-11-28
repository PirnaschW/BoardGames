#include "Games.h"

#include "BrainKing.h"
#include "Hasami/HasamiGame.h"
#include "Shogi/ShogiGame.h"

#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
#undef _CXX17_DEPRECATE_CODECVT_HEADER
#define _CXX17_DEPRECATE_CODECVT_HEADER
#include <xstring>
#include <locale>
#include <codecvt>

namespace BoardGamesBK
{

  // local functions and data (not defined in .h):
  namespace Local
  {

    static const std::string GetBetween(const std::string& s, const std::string& s0, const std::string& s1)
    {
      const std::string::size_type p0{ s.find(s0) };
      const std::string::size_type p1{ s.find(s1,p0) };
      return s.substr(p0 + s0.size(), p1 - p0);
    }

    static std::vector<const Piece*> ListFromHTML(const std::string& s)
    {
      static const std::unordered_map<std::string, const Piece&> map_{
        { R"(blank35)",       Piece::NoPiece },
        { R"(blank50)",       Piece::NoPiece },
        { R"(dot25)",         Piece::NoPiece },     // used in Battle boats
        { R"(explode35)",     Piece::NoPiece },     // disallowed field in Cheshire Cat variants, for now: treat like blank
        { R"(line4/w35)",     CorePiece::WC },
        { R"(line4/b35)",     CorePiece::BC },
        { R"(chess/wk35)",    CorePiece::WK },
        { R"(chess/wq35)",    CorePiece::WQ },
        { R"(chess/wr35)",    CorePiece::WR },
        { R"(chess/wb35)",    CorePiece::WB },
        { R"(chess/wn35)",    CorePiece::WN },
        { R"(chess/wp35)",    CorePiece::WP },
        { R"(chess/bk35)",    CorePiece::BK },
        { R"(chess/bq35)",    CorePiece::BQ },
        { R"(chess/br35)",    CorePiece::BR },
        { R"(chess/bb35)",    CorePiece::BB },
        { R"(chess/bn35)",    CorePiece::BN },
        { R"(chess/bp35)",    CorePiece::BP },
        { R"(shogi/i-p35d)",  Shogi::ShogiPiece::ShogiWP },
        { R"(shogi/i-b35d)",  Shogi::ShogiPiece::ShogiWB },
        { R"(shogi/i-r35d)",  Shogi::ShogiPiece::ShogiWR },
        { R"(shogi/i-l35d)",  Shogi::ShogiPiece::ShogiWL },
        { R"(shogi/i-n35d)",  Shogi::ShogiPiece::ShogiWN },
        { R"(shogi/i-sg35d)", Shogi::ShogiPiece::ShogiWS },
        { R"(shogi/i-gg35d)", Shogi::ShogiPiece::ShogiWG },
        { R"(shogi/i-k35d)",  Shogi::ShogiPiece::ShogiWK },
        { R"(shogi/i-p35u)",  Shogi::ShogiPiece::ShogiBP },
        { R"(shogi/i-l35u)",  Shogi::ShogiPiece::ShogiBL },
        { R"(shogi/i-n35u)",  Shogi::ShogiPiece::ShogiBN },
        { R"(shogi/i-sg35u)", Shogi::ShogiPiece::ShogiBS },
        { R"(shogi/i-gg35u)", Shogi::ShogiPiece::ShogiBG },
        { R"(shogi/i-k35u)",  Shogi::ShogiPiece::ShogiBK },
        { R"(shogi/i-r35u)",  Shogi::ShogiPiece::ShogiBR },
        { R"(shogi/i-b35u)",  Shogi::ShogiPiece::ShogiBB },
        { R"(hshogi/b1)",     Piece::NoTile },
        { R"(hshogi/w1)",     Piece::NoTile },
        { R"(hshogi/w35)",    Hasami::HasamiPiece::HasamiPieceW },
        { R"(hshogi/b35)",    Hasami::HasamiPiece::HasamiPieceB },
        //<a href="GameRules?tp=13&
      };

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
        if (it->second != Piece::NoTile) list.push_back(&(it->second));        // add to Piece stack
      }
      return list;
    }

    struct GameData {
    public:
      int tp;
      GameID id;
      VariantCode c;
      Coordinate x;
      Coordinate y;
    };
    static constexpr GameData map[]{
      {   0, IDR_GAMETYPE_XXXXX,     '\0',  0,  0 },  // dummy for 'unknown game type'
      {   1, IDR_GAMETYPE_CHESS,     '\0',  8,  8 },  // Chess
      {   2, IDR_GAMETYPE_CHESS,     '\0',  8,  8 },  // Corner Chess
      {   3, IDR_GAMETYPE_CHESS,     '\0',  8,  8 },  // Fortress Chess
      {   4, IDR_GAMETYPE_CHESS,     '\0',  8,  8 },  // Horde Chess
      {   5, IDR_GAMETYPE_XXXXX,     '\0',  8,  8 },  // Five in Line
      {   6, IDR_GAMETYPE_CHESS,     '\0',  8,  8 },  // Loop Chess
      {   7, IDR_GAMETYPE_XXXXX,     '\0',  8,  8 },  // Checkers
      {   8, IDR_GAMETYPE_XXXXX,     '\0',  8,  8 },  // Anti Checkers
      {   9, IDR_GAMETYPE_XXXXX,     '\0',  8,  8 },  // Reversi 6x6
      {  10, IDR_GAMETYPE_XXXXX,     '\0',  8,  8 },  // Reversi 8x8
      {  11, IDR_GAMETYPE_XXXXX,     '\0',  8,  8 },  // Reversi 10x10
      {  12, IDR_GAMETYPE_XXXXX,     '\0',  8,  8 },  // Anti Reversi
      {  13, IDR_GAMETYPE_CFOUR,     'L',   8,  8 },  // Line4
      {  14, IDR_GAMETYPE_CFOUR,     'A',   8,  8 },  // Anti Line4
      {  15, IDR_GAMETYPE_CFOUR,     'T',   8,  8 },  // Linetris
      {  16, IDR_GAMETYPE_CFOUR,     'S',   8,  8 },  // Spider Line4
      {  17, IDR_GAMETYPE_CHESS,     '\0',  8,  8 },  // Anti Chess
      {  18, IDR_GAMETYPE_CHESS,     '\0',  8,  8 },  // Extinction Chess
      {  19, IDR_GAMETYPE_XXXXX,     '\0',  8,  8 },  // Tablut
      {  20, IDR_GAMETYPE_TANKB,     '\0',  8,  8 },  // Tank Battle
      {  21, IDR_GAMETYPE_CHESS,     '\0',  8,  8 },  // Maharajah Chess
      {  22, IDR_GAMETYPE_CHESS,     '\0',  8,  8 },  // Three Checks Chess
      {  23, IDR_GAMETYPE_XXXXX,     '\0',  8,  8 },  // Backgammon
      {  24, IDR_GAMETYPE_XXXXX,     '\0',  8,  8 },  // Nackgammon
      {  25, IDR_GAMETYPE_XXXXX,     '\0',  8,  8 },  // Anti Backgammon
      {  26, IDR_GAMETYPE_XXXXX,     '\0',  8,  8 },  // Backgammon Race
      {  27, IDR_GAMETYPE_CHESS,     '\0',  8,  8 },  // Dark Chess
      {  28, IDR_GAMETYPE_CHESS,     '\0',  8,  8 },  // Atomic Chess
      {  29, IDR_GAMETYPE_CHECKERS,  '\0',  8,  8 },  // Czech Checkers
      {  30, IDR_GAMETYPE_CHECKERS,  '\0',  8,  8 },  // Corner Checkers
      {  31, IDR_GAMETYPE_CHECKERS,  '\0',  8,  8 },  // One Way Checkers
      {  32, IDR_GAMETYPE_CHECKERS,  '\0',  8,  8 },  // Parachute Checkers
      {  33, IDR_GAMETYPE_XXXXX,     '\0',  8,  8 },  // Halma 8x8
      {  34, IDR_GAMETYPE_XXXXX,     '\0',  8,  8 },  // Halma 10x10
      {  35, IDR_GAMETYPE_CHECKERS,  '\0',  8,  8 },  // Gothic Checkers
      {  36, IDR_GAMETYPE_CHECKERS,  '\0',  8,  8 },  // Turkish Checkers< / select>
      {  37, IDR_GAMETYPE_XXXXX,     '\0',  8,  8 },  // Crowded Backgammon
      {  38, IDR_GAMETYPE_XXXXX,     '\0',  8,  8 },  // Pente
      {  39, IDR_GAMETYPE_XXXXX,     '\0',  8,  8 },  // Keryo Pente
      {  40, IDR_GAMETYPE_CHESS,     '\0',  8,  8 },  // Janus Chess
      {  41, IDR_GAMETYPE_CHESS,     '\0',  8,  8 },  // Embassy Chess
      {  42, IDR_GAMETYPE_XXXXX,     '\0',  8,  8 },  // Small Pente
      {  43, IDR_GAMETYPE_XXXXX,     '\0',  8,  8 },  // Small Keryo Pente
      {  44, IDR_GAMETYPE_XXXXX,     '\0',  8,  8 },  // Open Pente
      {  45, IDR_GAMETYPE_XXXXX,     '\0',  8,  8 },  // Open Keryo Pente
      {  46, IDR_GAMETYPE_XXXXX,     '\0',  8,  8 },  // Battleboats
      {  47, IDR_GAMETYPE_CHESS,     '\0',  8,  8 },  // Screen Chess
      {  48, IDR_GAMETYPE_CHESS,     '\0',  8,  8 },  // Crazy Screen Chess
      {  49, IDR_GAMETYPE_XXXXX,     '\0',  8,  8 },  // Amazons
      {  50, IDR_GAMETYPE_XXXXX,     '\0',  8,  8 },  // Battleboats Plus
      {  51, IDR_GAMETYPE_XXXXX,     '\0',  8,  8 },  // Dark Battleboats
      {  52, IDR_GAMETYPE_XXXXX,     '\0',  8,  8 },  // Jarmo
      {  53, IDR_GAMETYPE_XXXXX,     '\0',  8,  8 },  // Alquerque
      {  54, IDR_GAMETYPE_XXXXX,     '\0',  8,  8 },  // Froglet
      {  55, IDR_GAMETYPE_ESPIONAGE, '\0',  8,  8 },  // Espionage
      {  56, IDR_GAMETYPE_XXXXX,     '\0',  8,  8 },  // Jungle
      {  57, IDR_GAMETYPE_CHESS,     '\0',  8,  8 },  // Cylinder Chess
      {  58, IDR_GAMETYPE_CHESS,     '\0',  8,  8 },  // Amazon Chess
      {  59, IDR_GAMETYPE_CHESS,     '\0',  8,  8 },  // Berolina Chess
      {  60, IDR_GAMETYPE_XXXXX,     '\0',  8,  8 },  // Anti Froglet
      {  61, IDR_GAMETYPE_CFOUR,     'X',   8,  8 },  // Spider Linetris
      {  62, IDR_GAMETYPE_XXXXX,     '\0',  8,  8 },  // Small Espionage
      {  63, IDR_GAMETYPE_XXXXX,     '\0',  8,  8 },  // Fast Espionage
      {  64, IDR_GAMETYPE_XXXXX,     '\0',  8,  8 },  // Pro Five in Line
      {  65, IDR_GAMETYPE_XXXXX,     '\0',  8,  8 },  // Swap Five in Line
      {  66, IDR_GAMETYPE_XXXXX,     '\0',  8,  8 },  // Hyper Backgammon
      {  67, IDR_GAMETYPE_XXXXX,     '\0',  8,  8 },  // Fischer Random Chess
      {  68, IDR_GAMETYPE_XXXXX,     '\0',  8,  8 },  // Chinese Chess
      {  69, IDR_GAMETYPE_SHOGI,     '\0',  8,  8 },  // Japanese Chess
      {  70, IDR_GAMETYPE_CHESS,     '\0',  8,  8 },  // Legan Chess
      {  71, IDR_GAMETYPE_CHESS,     '\0',  8,  8 },  // Knight Relay Chess
      {  72, IDR_GAMETYPE_XXXXX,     '\0',  8,  8 },  // PahTum
      {  73, IDR_GAMETYPE_HASAMI,    '\0',  8,  8 },  // Hasami Shogi
      {  74, IDR_GAMETYPE_CHESS,     '\0',  8,  8 },  // Grand Chess
      {  75, IDR_GAMETYPE_CHESS,     '\0',  8,  8 },  // Capablanca Random Chess
      {  76, IDR_GAMETYPE_CHESS,     '\0',  8,  8 },  // Los Alamos Chess
      {  77, IDR_GAMETYPE_XXXXX,     '\0',  8,  8 },  // Sphere Froglet
      {  78, IDR_GAMETYPE_XXXXX,     '\0',  8,  8 },  // Ludo
      {  79, IDR_GAMETYPE_XXXXX,     '\0',  8,  8 },  // Go
      {  80, IDR_GAMETYPE_XXXXX,     '\0',  8,  8 },  // Go 9x9
      {  81, IDR_GAMETYPE_XXXXX,     '\0',  8,  8 },  // Go 13x13
      {  82, IDR_GAMETYPE_XXXXX,     '\0',  8,  8 },  // Connect6
      {  83, IDR_GAMETYPE_XXXXX,     '\0',  8,  8 },  // Lines of Action
      {  84, IDR_GAMETYPE_XXXXX,     '\0',  8,  8 },  // Breakthrough
      {  85, IDR_GAMETYPE_CHESS,     '\0',  8,  8 },  // Ambiguous Chess
      {  86, IDR_GAMETYPE_LOA,       '\0',  8,  8 },  // Scrambled Eggs
      {  87, IDR_GAMETYPE_ESPIONAGE, '\0',  8,  8 },  // Open Fast Espionage
      {  88, IDR_GAMETYPE_ESPIONAGE, '\0',  8,  8 },  // Small Fast Espionage
      {  89, IDR_GAMETYPE_ATAXX,     '\0',  8,  8 },  // Assimilation
      {  90, IDR_GAMETYPE_ATAXX,     '\0',  8,  8 },  // Ataxx
      {  91, IDR_GAMETYPE_SHOGI,     'M',   8,  8 },  // Minishogi
      {  92, IDR_GAMETYPE_XXXXX,     '\0',  8,  8 },  // Cloning Backgammon
      {  93, IDR_GAMETYPE_CHEVERSI,  '\0',  8,  8 },  // Cheversi
      {  94, IDR_GAMETYPE_XXXXX,     '\0',  8,  8 },  // Hawaiian Checkers
      {  95, IDR_GAMETYPE_XXXXX,     '\0',  8,  8 },  // Dice Chess
      {  96, IDR_GAMETYPE_XXXXX,     '\0',  8,  8 },  // Grasshopper
      {  97, IDR_GAMETYPE_XXXXX,     '\0',  8,  8 },  // Dice Poker
      {  98, IDR_GAMETYPE_CHESS,     '\0',  8,  8 },  // Recycle Chess
      {  99, IDR_GAMETYPE_XXXXX,     '\0',  8,  8 },  // Triple Dice Poker
      { 100, IDR_GAMETYPE_CHESS,     '\0',  8,  8 },  // Ice Age Chess
      { 101, IDR_GAMETYPE_XXXXX,     '\0',  8,  8 },  // Frog Finder
      { 102, IDR_GAMETYPE_LOGIK,     '\0',  8,  8 },  // Logik
      { 103, IDR_GAMETYPE_XXXXX,     '\0',  8,  8 },  // Mancala
      { 104, IDR_GAMETYPE_CHESS,     '\0',  8,  8 },  // Behemoth Chess
      { 105, IDR_GAMETYPE_XXXXX,     '\0',  8,  8 },  // Frog Legs
      { 106, IDR_GAMETYPE_XXXXX,     '\0',  8,  8 },  // Dice Poker 6D
      { 107, IDR_GAMETYPE_XXXXX,     '\0',  8,  8 },  // Triple Dice Poker 6D
      { 108, IDR_GAMETYPE_XXXXX,     '\0',  8,  8 },  // Big Jungle
      { 109, IDR_GAMETYPE_XXXXX,     '\0',  8,  8 },  // Plakoto
      { 110, IDR_GAMETYPE_KNIGHTF,   '\0',  8,  8 },  // Knight Fight
      { 111, IDR_GAMETYPE_XXXXX,     '\0',  8,  8 },  // Fevga
      { 112, IDR_GAMETYPE_LOA,       '\0',  8,  8 },  // Parachute Lines of Action
      { 113, IDR_GAMETYPE_CHESS,     '\0',  8,  8 },  // Cheshire Cat Chess
      { 114, IDR_GAMETYPE_LOA,       '\0',  8,  8 },  // Cheshire Cat Lines of Action
      { 115, IDR_GAMETYPE_CHECKERS,  '\0',  8,  8 },  // International Checkers
      { 116, IDR_GAMETYPE_CHECKERS,  '\0',  8,  8 },  // Knightmate Chess
      { 117, IDR_GAMETYPE_CHECKERS,  '\0',  8,  8 },  // Russian Checkers
      { 118, IDR_GAMETYPE_CHECKERS,  '\0',  8,  8 },  // Canadian Checkers
      { 119, IDR_GAMETYPE_CHECKERS,  '\0',  8,  8 },  // Thai Checkers
      { 120, IDR_GAMETYPE_CHECKERS,  '\0',  8,  8 },  // Brazilian Checkers
      { 121, IDR_GAMETYPE_CHECKERS,  '\0',  8,  8 },  // Portuguese Checkers
      { 122, IDR_GAMETYPE_CHECKERS,  '\0',  8,  8 },  // Italian Checkers
      { 123, IDR_GAMETYPE_XXXXX,     '\0',  8,  8 },  // Dameo
      { 124, IDR_GAMETYPE_CAMELOT,   '\0',  8,  8 },  // Camelot
      { 125, IDR_GAMETYPE_XXXXX,     '\0',  8,  8 },  // Racing Kings
      { 126, IDR_GAMETYPE_CAMELOT,   '\0',  8,  8 },  // Cam
      { 127, IDR_GAMETYPE_CHESS,     '\0',  8,  8 },  // Dice Chess 10x10
      { 128, IDR_GAMETYPE_MCHESS,    '\0',  8,  8 },  // Massacre Chess
    };
    constexpr GameData GetGameData(int tp)
    {
      for (const auto& g : map)
        if (g.tp == tp) return g;
      return map[0];
    }
  }

  GameID BKGame::LoadGame(BoardGamesCore::VariantCode& c, BoardGamesCore::Coordinate& xSize, BoardGamesCore::Coordinate& ySize, std::vector<const Piece*>& list) const
  {
    // try to read a Current game
    const std::wstring urlc = LR"(http://brainking.com/en/ShowGame?g=)" + std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(nr_);
    std::string html = URL::GetHTMLFromURL(urlc);
    if (strlen(html.c_str()) < 256)    // if this results in a redirect, try to read an Archived game
    {
      const std::wstring urla = LR"(http://brainking.com/en/ArchivedGame?g=)" + std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(nr_);
      html = URL::GetHTMLFromURL(urla);
    }

    const std::string tp = Local::GetBetween(html, R"(<a href="GameRules?tp=)", R"(&)");
    int gametype = stoi(tp);
    Local::GameData g = Local::GetGameData(gametype);
    if (g.id == 0) return 0; // not a valid or recognized game type / variant

    c = g.c;
    xSize = g.x;
    ySize = g.y;

    const std::string board = Local::GetBetween(html, R"(<div id="game-board-section">)", R"(<div id="game-info-section">)");
    list = Local::ListFromHTML(board);

    return g.id;
  }

}