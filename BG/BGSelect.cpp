// BGSelect.cpp : implementation file
//

#include "stdafx.h"

// BGSelect dialog

IMPLEMENT_DYNAMIC(BGSelect, CDialogEx)

BGSelect::BGSelect(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_BGSELECT, pParent)
  , m_size_x(8)
  , m_size_y(8)
  , m_size_lock(TRUE)
  , m_player1(0)
  , m_player2(1)
{
}

BGSelect::~BGSelect()
{
}

void BGSelect::DoDataExchange(CDataExchange* pDX)
{
  CDialogEx::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_GAME_SELECT_TYPE, m_ctrl_game_type);
  DDX_Control(pDX, IDC_GAME_SELECT_VARIANT, m_ctrl_game_variant);
  DDX_Control(pDX, IDC_GAME_SELECT_SIZE_X, m_ctrl_size_x);
  DDX_Control(pDX, IDC_GAME_SELECT_SIZE_Y, m_ctrl_size_y);
  DDX_Check(pDX, IDC_GAME_SELECT_SIZE_LOCK, m_size_lock);
  DDX_Radio(pDX, IDC_GAME_SELECT_P1_HUMAN, m_player1);
  DDX_Radio(pDX, IDC_GAME_SELECT_P2_HUMAN, m_player2);
  DDX_Slider(pDX, IDC_GAME_SELECT_SIZE_X, m_size_x);
  DDX_Slider(pDX, IDC_GAME_SELECT_SIZE_Y, m_size_y);
}


BEGIN_MESSAGE_MAP(BGSelect, CDialogEx)
  ON_LBN_SELCHANGE(IDC_GAME_SELECT_TYPE, &BGSelect::OnSelchangeGameType)
  ON_LBN_SELCHANGE(IDC_GAME_SELECT_VARIANT, &BGSelect::OnSelchangeGameVariant)
  ON_BN_CLICKED(IDC_GAME_SELECT_SIZE_LOCK, &BGSelect::OnClickedGameSizeLock)
  ON_COMMAND(IDC_GAME_SELECT_P1_HUMAN, &BGSelect::OnGameP1H)
  ON_COMMAND(IDC_GAME_SELECT_P1_COMPUTER, &BGSelect::OnGameP1C)
  ON_COMMAND(IDC_GAME_SELECT_P2_HUMAN, &BGSelect::OnGameP2H)
  ON_COMMAND(IDC_GAME_SELECT_P2_COMPUTER, &BGSelect::OnGameP2C)
  ON_BN_CLICKED(IDOK, &BGSelect::OnBnClickedOk)
END_MESSAGE_MAP()


// BGSelect message handlers
BOOL BGSelect::OnInitDialog()
{
  CDialogEx::OnInitDialog();

  // TODO:  Add extra initialization here
  AddGames();
  m_ctrl_game_type.SetCurSel(0); // default
  m_game_id = m_ctrl_game_type.GetItemData(m_ctrl_game_type.GetCurSel());
  SetControlsForGame();
  return TRUE;  // return TRUE unless you set the focus to a control
                // EXCEPTION: OCX Property Pages should return FALSE
}

void BGSelect::AddGame(int resourceID)
{
  const CString str0{ MAKEINTRESOURCE(resourceID) };
  const CString str1{ str0.Mid(1, str0.Find(L"\n", 1) - 1) };
  m_ctrl_game_type.SetItemData(m_ctrl_game_type.AddString(str1), resourceID);
}


void BGSelect::OnSelchangeGameType()
{
  m_game_id = m_ctrl_game_type.GetItemData(m_ctrl_game_type.GetCurSel());
  SetControlsForGame();
  UpdateData(FALSE);
}

void BGSelect::OnSelchangeGameVariant()
{
}

void BGSelect::OnGameP1H() { m_player1 = 0; m_player2 = 1; UpdateData(FALSE); }
void BGSelect::OnGameP1C() { m_player1 = 1; m_player2 = 0; UpdateData(FALSE); }
void BGSelect::OnGameP2H() { m_player1 = 1; m_player2 = 0; UpdateData(FALSE); }
void BGSelect::OnGameP2C() { m_player1 = 0; m_player2 = 1; UpdateData(FALSE); }
void BGSelect::OnClickedGameSizeLock() { if ((m_size_lock = !m_size_lock) == true) m_size_y = m_size_x; UpdateData(FALSE); }

void BGSelect::OnBnClickedOk()
{
  UpdateData(TRUE); 
  m_game_id = m_ctrl_game_type.GetItemData(m_ctrl_game_type.GetCurSel());
  m_rule_id = m_ctrl_game_variant.GetItemData(m_ctrl_game_variant.GetCurSel());
  CDialogEx::OnOK();
}
