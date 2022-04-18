
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
    virtual PositionValue GetValue(const Board& /*p*/, const Location& /*l*/) const noexcept override { return 100; };
    virtual void CollectMoves(const Board&, const Location&, Moves&) const noexcept override;
    bool CollectJumps(const Board& p, const Location& fr, const Actions& a, bool charge, const PieceColor& c, Moves& m) const noexcept;

  public:
    static const Pawn ThePawn;
  };

  class Knight : public Pawn
  {
  private:
    constexpr Knight() noexcept : Pawn('N') {}
  public:
    virtual PositionValue GetValue(const Board& /*p*/, const Location& /*l*/) const noexcept override { return 400; }
    virtual void CollectMoves(const Board&, const Location&, Moves&) const noexcept override;

  public:
    static const Knight TheKnight;
  };


  class CamPiece : public Piece
  {
  private:
    CamPiece(const Kind& k, const PieceColor& c, unsigned int ID) noexcept : Piece(k, c, ID) {}
    CamPiece(const CamPiece&) = delete;
    CamPiece& operator=(const CamPiece&) = delete;
    virtual PositionValue GetValue(const Board& p, const Location& l) const noexcept override;

  public:
    static const CamPiece WP;
    static const CamPiece WN;
    static const CamPiece BP;
    static const CamPiece BN;
  };


  class CamBoard : public Board
  {
  public:
    CamBoard(const VariantChosen& v, const BoardPartDimensions& d) noexcept : Board(v, d) {};
    virtual Board* Clone() const noexcept override { return new CamBoard(*this); }
    virtual void SetStartingBoard() noexcept override;
    //virtual void GetAllMoves() const noexcept override;

    // extensions:
  public:
//    bool CollectJumps(const Location& fr, const Actions& a, bool charge, const PieceColor& c, Moves& m) const;
  private:
//    bool IsRepeat(const Actions& a, const Piece& p, const Location& fr, const Location& to) const noexcept;
  };

  class CamGame : public Game
  {
  private:
    CamGame() = delete;
  public:
    CamGame(const VariantChosen& v, const BoardPartDimensions& d) noexcept : Game(v,new CamBoard(v, d)) {}
    static void Register() noexcept;
    static const VariantList& GetVariants() noexcept;
    static const BoardPartDimensions GetDimensions(const VariantChosen& v) noexcept;
  };

}
