// BGSelect dialog

class BGSelect : public CDialogEx
{
  DECLARE_DYNAMIC(BGSelect)

public:
  BGSelect(CWnd* pParent = nullptr);   // standard constructor
  virtual ~BGSelect();

  // Dialog Data
#ifdef AFX_DESIGN_TIME
  enum { IDD = IDD_BGSELECT };
#endif

protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnSelchangeGameType();
  afx_msg void OnGameP1H();
  afx_msg void OnGameP1C();
  afx_msg void OnSelchangeGameVariant();
  afx_msg void OnClickedGameSizeLock();

  virtual BOOL OnInitDialog();
  void OnNewWWW();
  void AddGame(BoardGamesCore::GameID resourceID);
  void SetControlsForGame();
  BoardGamesCore::GameDispatch* CreateGame();

  // access to controls
  CListBox m_ctrl_game_type;      // Game Type
  CListBox m_ctrl_game_variant;   // Rule Variant
  CSpinButtonCtrl m_ctrl_spin_x;  // Size X
  CSpinButtonCtrl m_ctrl_spin_y;  // Size Y

// data members
  BoardGamesCore::VariantChosen v{ 0, '\0', 8, 8 };        // Game Resource ID, Size X, Size Y, Variant Code
  int m_x{ v.x };
  int m_y{ v.y };
  int m_player1{ 0 };                                  // Player 1
  int m_player2{ 1 };                                  // Player 2
  BOOL m_size_lock{ true };                            // Lock X to Y sizes
  int m_var_index{ 0 };                                // Variant index
  int m_www_no{ 0 };                                   // WWW Game Number
  std::vector<BoardGamesCore::PieceIndex> m_setup{};   // current Board's Piece list
  BoardGamesCore::VariantList var{};
};
