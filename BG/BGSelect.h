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
  afx_msg void OnGameP2H();
  afx_msg void OnGameP2C();
  afx_msg void OnClickedGameSizeLock();
  afx_msg void OnSelchangeGameVariant();
  afx_msg void OnBnClickedOk();

  virtual BOOL OnInitDialog();
  void AddGame(int resourceID);

  // access to controls
  CListBox m_ctrl_game_type;      // Game Type
  CListBox m_ctrl_game_variant;   // Rule Variant
  CSliderCtrl m_ctrl_size_x;      // Size X
  CSliderCtrl m_ctrl_size_y;      // Size Y

// data members
  int m_game_id{ 0 };             // Game Resource ID
  int m_size_x{ 8 };              // Size X
  int m_size_y{ 8 };              // Size Y
  BOOL m_size_lock{ true };       // Lock X to Y sizes
  int m_player1{ 0 };             // Player 1
  int m_player2{ 1 };             // Player 2
  int m_rule_id{ 0 };             // Rule Variant ID

  // #### methods with Game specific code ####
public:
  BoardGamesCore::Game* CreateGame(void);
private:
  void AddGames(void);
  void SetControlsForGame(void);
};
