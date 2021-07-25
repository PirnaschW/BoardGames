

namespace Chess
{

  // Display Dimensions
  constexpr unsigned int FieldSizeX = 50;  // pixels per tile
  constexpr unsigned int FieldSizeY = 50;  // pixels per tile

  constexpr unsigned int FieldSizeSX = 18;  // pixels per small tile
  constexpr unsigned int FieldSizeSY = 20;  // pixels per small tile

  constexpr unsigned int BoardStartX = 30;  // offset for drawing the board
  constexpr unsigned int BoardStartY = 30;  // offset for drawing the board

  using namespace BoardGamesCore;

  enum ChessVariant : VariantCode   // recognized variants of Chess
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


  class Pawn : public Kind
  {
  private:
    Pawn(void) noexcept : Kind('P') {}
  public:
    void CollectMoves(const MainPosition&, const Location&, Moves&) const noexcept override;
    unsigned int GetValue(const MainPosition& /*p*/, const Location& /*l*/) const noexcept override { return 100; }

  public:
    static const Pawn ThePawn;
  };

  class Knight : public Kind
  {
  private:
    Knight(void) noexcept : Kind('N') {}
  public:
    void CollectMoves(const MainPosition&, const Location&, Moves&) const noexcept override;
    unsigned int GetValue(const MainPosition& /*p*/, const Location& /*l*/) const noexcept override { return 300; }

  public:
    static const Knight TheKnight;
  };

  class Bishop : public Kind
  {
  private:
    Bishop(void) noexcept : Kind('B') {}
  public:
    void CollectMoves(const MainPosition&, const Location&, Moves&) const noexcept override;
    unsigned int GetValue(const MainPosition& /*p*/, const Location& /*l*/) const noexcept override { return 300; }

  public:
    static const Bishop TheBishop;
  };

  class Rook : public Kind
  {
  private:
    Rook(void) noexcept : Kind('R') {}
  public:
    void CollectMoves(const MainPosition&, const Location&, Moves&) const noexcept override;
    unsigned int GetValue(const MainPosition& /*p*/, const Location& /*l*/) const noexcept override { return 500; }

  public:
    static const Rook TheRook;
  };

  class Cardinal : public Kind  // Bishop + Knight (also: Archbishop, Princess, Janus)
  {
  private:
    Cardinal(void) noexcept : Kind('C') {}
  public:
    virtual void CollectMoves(const MainPosition&, const Location&, Moves&) const noexcept override;
    virtual unsigned int GetValue(const MainPosition& /*p*/, const Location& /*l*/) const noexcept override { return 900; }

  public:
    static const Cardinal TheCardinal;
  };

  class Marshall : public Kind // Rook + Knight
  {
  private:
    Marshall(void) noexcept : Kind('M') {}
  public:
    virtual void CollectMoves(const MainPosition&, const Location&, Moves&) const noexcept override;
    virtual unsigned int GetValue(const MainPosition& /*p*/, const Location& /*l*/) const noexcept override { return 900; }

  public:
    static const Marshall TheMarshall;
  };

  class Queen : public Kind
  {
  private:
    Queen(void) noexcept : Kind('Q') {}
  public:
    virtual void CollectMoves(const MainPosition&, const Location&, Moves&) const noexcept override;
    virtual unsigned int GetValue(const MainPosition& /*p*/, const Location& /*l*/) const noexcept override { return 900; }

  public:
    static const Queen TheQueen;
  };

  class Amazon : public Kind // Queen + Knight
  {
  private:
    Amazon(void) noexcept : Kind('A') {}
  public:
    virtual void CollectMoves(const MainPosition&, const Location&, Moves&) const noexcept override;
    virtual unsigned int GetValue(const MainPosition& /*p*/, const Location& /*l*/) const noexcept override { return 900; }

  public:
    static const Amazon TheAmazon;
  };

  class King : public Kind
  {
  private:
    King(void) noexcept : Kind('K') {}
  public:
    virtual void CollectMoves(const MainPosition&, const Location&, Moves&) const noexcept override;
    virtual unsigned int GetValue(const MainPosition& /*p*/, const Location& /*l*/) const noexcept override { return 500; }

  public:
    static const King TheKing;
  };


  class ChessPiece : public Piece
  {
  private:
    inline ChessPiece(const Kind& k, const PieceColor& c, unsigned int ID) : Piece(k, c, ID) {}
    ChessPiece(const ChessPiece&) = delete;
    ChessPiece& operator=(const ChessPiece&) = delete;

  public:
    static const ChessPiece WP;  // White Pawn
    static const ChessPiece WN;  // White Knight
    static const ChessPiece WB;  // White Bishop
    static const ChessPiece WR;  // White Rook 
    static const ChessPiece WC;  // White Cardinal = Bishop + Knight (also: Archbishop, Princess, Janus)
    static const ChessPiece WM;  // White Marshall = Rook + Knight
    static const ChessPiece WQ;  // White Queen
    static const ChessPiece WA;  // White Amazon   = Queen + Knight
    static const ChessPiece WK;  // White King 

    static const ChessPiece BP;  // Black Pawn 
    static const ChessPiece BN;  // Black Knight
    static const ChessPiece BB;  // Black Bishop
    static const ChessPiece BR;  // Black Rook 
    static const ChessPiece BC;  // Black Cardinal = Bishop + Knight (also: Archbishop, Princess, Janus)
    static const ChessPiece BM;  // Black Marshall = Rook + Knight
    static const ChessPiece BQ;  // Black Queen
    static const ChessPiece BA;  // Black Amazon   = Queen + Knight
    static const ChessPiece BK;  // Black King 
  };


  class ChessPosition : public MainPosition
  {
  public:
    ChessPosition(const VariantChosen& v, const PieceMapP& p, const Dimensions& d) noexcept : MainPosition(v, p, d) {};
    virtual MainPosition* Clone(void) const noexcept override { return new ChessPosition(*this); }
    virtual void SetStartingPosition() noexcept override;
    virtual bool AddIfLegal(Moves& m, const Location& fr, const Location& to) const noexcept override;
    virtual unsigned int GetMoveCountFactor(void) const noexcept override { return 1000; }

// extensions
  protected:
    void SetPawns(Coordinate row, const PieceColor& c) noexcept;
  };

  class ChessGame : public Game
  {
  private:
    ChessGame(void) = delete;
    static MainPosition* GetNewPosition(const VariantChosen& v, const PieceMapP& m, const Dimensions& d) noexcept;

  public:
    ChessGame(const VariantChosen& v, const PieceMapP& m, const Dimensions& d) noexcept : Game(v, m, GetNewPosition(v, m, d), new MainLayout(d)) {}
    static const VariantList& GetVariants(void) noexcept;
    static const PieceMapP& GetPieces(const VariantChosen& v) noexcept;
    static const Dimensions GetDimensions(const VariantChosen& v) noexcept;
  };

}
