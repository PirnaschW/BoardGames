
namespace Cam
{

  // Display Dimensions
  constexpr unsigned int FieldSizeX = 50;   // pixels per tile
  constexpr unsigned int FieldSizeY = 50;   // pixels per tile

  constexpr unsigned int FieldSizeSX = 20;  // pixels per small tile
  constexpr unsigned int FieldSizeSY = 18;  // pixels per small tile

  constexpr unsigned int BoardStartX = 30;  // offset for drawing the board
  constexpr unsigned int BoardStartY = 30;  // offset for drawing the board

  using namespace BoardGamesCore;

  class Pawn : public Kind
  {
  protected:
    constexpr Pawn(const char& k = 'P') noexcept : Kind(k) {}

  public:
    virtual unsigned int GetValue(const MainPosition& /*p*/, const Location& /*l*/) const noexcept override { return 100; };
    virtual void CollectMoves(const MainPosition&, const Location&, Moves&) const noexcept override;
    bool CollectJumps(const MainPosition& p, const Location& fr, const Actions& a, bool charge, const PieceColor& c, Moves& m) const noexcept;

  public:
    static const Pawn ThePawn;
  };

  class Knight : public Pawn
  {
  private:
    constexpr Knight(void) noexcept : Pawn('N') {}
  public:
    virtual unsigned int GetValue(const MainPosition& /*p*/, const Location& /*l*/) const noexcept override { return 400; }
    virtual void CollectMoves(const MainPosition&, const Location&, Moves&) const noexcept override;

  public:
    static const Knight TheKnight;
  };


  class CamPiece : public Piece
  {
  private:
    CamPiece(const Kind& k, const PieceColor& c, unsigned int ID) noexcept : Piece(k, c, ID) {}
    CamPiece(const CamPiece&) = delete;
    CamPiece& operator=(const CamPiece&) = delete;
    virtual unsigned int GetValue(const MainPosition& p, const Location& l) const noexcept override;

  public:
    static const CamPiece WP;
    static const CamPiece WN;
    static const CamPiece BP;
    static const CamPiece BN;
  };


  class CamPosition : public MainPosition
  {
  public:
    CamPosition(VariantCode c, const PieceMapP& p, const Dimensions& d) noexcept;
    virtual MainPosition* Clone(void) const noexcept override { return new CamPosition(*this); }
    virtual void GetAllMoves(void) const noexcept override;

    // extensions:
  public:
//    bool CollectJumps(const Location& fr, const Actions& a, bool charge, const PieceColor& c, Moves& m) const;
  private:
//    bool IsRepeat(const Actions& a, const Piece& p, const Location& fr, const Location& to) const noexcept;
  };

  class CamGame : public Game
  {
  private:
    CamGame(void) = delete;
  public:
    CamGame(const VariantChosen& v, const PieceMapP& m, const Dimensions& d) noexcept : Game(v, m, new CamPosition(v.c, m, d), new MainLayout(d)) {}
    static const VariantList& GetVariants(void) noexcept;
    static const PieceMapP& GetPieces(const VariantChosen& v) noexcept;
    static const Dimensions GetDimensions(const VariantChosen& v) noexcept;
  };

}
