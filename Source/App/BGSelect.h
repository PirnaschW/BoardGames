// BGSelect dialog

using class BoardGamesCore::Piece;

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
  void OnNewWWW(void);
  void AddGame(int resourceID);
  void SetControlsForGame();
  BoardGamesCore::GameDispatch* CreateGame(void);

  // access to controls
  CListBox m_ctrl_game_type;      // Game Type
  CListBox m_ctrl_game_variant;   // Rule Variant
  CSpinButtonCtrl m_ctrl_spin_x;  // Size X
  CSpinButtonCtrl m_ctrl_spin_y;  // Size Y

// data members
  BoardGamesCore::GameID m_game_id{ 0 };                   // Game Resource ID
  int m_player1{ 0 };                                      // Player 1
  int m_player2{ 1 };                                      // Player 2
  int m_size_x{ 8 };                                       // Size X
  int m_size_y{ 8 };                                       // Size Y
  BOOL m_size_lock{ true };                                // Lock X to Y sizes
  int m_var_id{ 0 };                                       // Variant #
  BoardGamesCore::VariantCode m_var{ '\0' };               // Variant Code
  std::vector<const Piece*> m_setup{};     // current Position's Piece list
  const BoardGamesCore::VariantList* var;

};
