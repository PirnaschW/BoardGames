// BGSelect.cpp : implementation file
//

#include "pch.h"
#include "BGSelect.h"

// BGSelect dialog

IMPLEMENT_DYNAMIC(BGSelect, CDialogEx)

BGSelect::BGSelect(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_BGSELECT, pParent)
  , m_player1(0)
  , m_player2(1)
  , m_size_x(8)
  , m_size_y(8)
  , m_size_lock(TRUE)
{
}

BGSelect::~BGSelect()
{
}

void BGSelect::DoDataExchange(CDataExchange* pDX)
{
  CDialogEx::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_GAME_SELECT_TYPE, m_ctrl_game_type);
  DDX_Radio(pDX, IDC_GAME_SELECT_P1_HUMAN, m_player1);
  DDX_Radio(pDX, IDC_GAME_SELECT_P2_HUMAN, m_player2);
  DDX_Control(pDX, IDC_GAME_SELECT_VARIANT, m_ctrl_game_variant);
  DDX_Control(pDX, IDC_GAME_SELECT_X_SPIN, m_ctrl_spin_x);
  DDX_Text(pDX, IDC_GAME_SELECT_X_EDIT, m_size_x);
  DDX_Check(pDX, IDC_GAME_SELECT_SIZE_LOCK, m_size_lock);
  DDX_Control(pDX, IDC_GAME_SELECT_Y_SPIN, m_ctrl_spin_y);
  DDX_Text(pDX, IDC_GAME_SELECT_Y_EDIT, m_size_y);
}


BEGIN_MESSAGE_MAP(BGSelect, CDialogEx)
  ON_LBN_SELCHANGE(IDC_GAME_SELECT_TYPE, &BGSelect::OnSelchangeGameType)
  ON_COMMAND(IDC_GAME_SELECT_P1_HUMAN, &BGSelect::OnGameP1H)
  ON_COMMAND(IDC_GAME_SELECT_P1_COMPUTER, &BGSelect::OnGameP1C)
  ON_COMMAND(IDC_GAME_SELECT_P2_HUMAN, &BGSelect::OnGameP1C)
  ON_COMMAND(IDC_GAME_SELECT_P2_COMPUTER, &BGSelect::OnGameP1H)
  ON_LBN_SELCHANGE(IDC_GAME_SELECT_VARIANT, &BGSelect::OnSelchangeGameVariant)
  ON_BN_CLICKED(IDC_GAME_SELECT_SIZE_LOCK, &BGSelect::OnClickedGameSizeLock)
END_MESSAGE_MAP()


// BGSelect message handlers
BOOL BGSelect::OnInitDialog()
{
  CDialogEx::OnInitDialog();

  // TODO:  Add extra initialization here
  AddGames();
  m_ctrl_game_type.SetCurSel(0); // default
  m_game_id = static_cast<int>(m_ctrl_game_type.GetItemData(m_ctrl_game_type.GetCurSel()));
  GetDlgItem(IDC_GAME_SELECT_V_TEXT)->SetWindowTextW(L"Variant:");
  GetDlgItem(IDC_GAME_SELECT_X_TEXT)->SetWindowTextW(L"Horizontal Size:");
  GetDlgItem(IDC_GAME_SELECT_Y_TEXT)->SetWindowTextW(L"Vertical Size:");
  var = GetVariants(m_game_id);
  SetControlsForGame();
  UpdateData(FALSE);
  return TRUE;  // return TRUE unless you set the focus to a control
                // EXCEPTION: OCX Property Pages should return FALSE
}

void BGSelect::AddGame(int resourceID)
{
  const CString str0{ MAKEINTRESOURCE(resourceID) };
  const CString str1{ str0.Mid(1, str0.Find(L"\n", 1) - 1) };
  m_ctrl_game_type.SetItemData(m_ctrl_game_type.AddString(str1), resourceID);
}

void BGSelect::SetControlsForGame()
{
  assert(var.size() > 0);
  GetDlgItem(IDC_GAME_SELECT_V_TEXT)->ShowWindow(SW_HIDE);
  m_ctrl_game_variant.ResetContent();
  m_ctrl_game_variant.ShowWindow(SW_HIDE);
  for (unsigned int i = 0; i<var.size(); i++)
  {
    const auto& v{ var[i] };
    {
      m_size_x = v.xDef_;
      BOOL st = v.xMin_ == 0 ? SW_HIDE : SW_SHOW;
      GetDlgItem(IDC_GAME_SELECT_X_TEXT)->ShowWindow(st);
      GetDlgItem(IDC_GAME_SELECT_X_EDIT)->ShowWindow(st);
      m_ctrl_spin_x.ShowWindow(st);
      if (v.xMin_ > 0) m_ctrl_spin_x.SetRange32(v.xMin_, v.xMax_);
    }

    {
      m_size_y = v.yDef_;
      BOOL st = v.yMin_ == 0 ? SW_HIDE : SW_SHOW;
      GetDlgItem(IDC_GAME_SELECT_Y_TEXT)->ShowWindow(st);
      GetDlgItem(IDC_GAME_SELECT_Y_EDIT)->ShowWindow(st);
      m_ctrl_spin_y.ShowWindow(st);
      if (v.yMin_ > 0)
      {
        m_ctrl_spin_y.SetRange32(v.yMin_, v.yMax_);
        GetDlgItem(IDC_GAME_SELECT_SIZE_LOCK)->ShowWindow(SW_SHOW);
        m_size_lock = v.xDef_ == v.yDef_ ? TRUE : FALSE;
      }
      else
      {
        GetDlgItem(IDC_GAME_SELECT_SIZE_LOCK)->ShowWindow(SW_HIDE);
        m_size_lock = TRUE;
      }
    }

    if (v.name_ != nullptr)
    {
      GetDlgItem(IDC_GAME_SELECT_V_TEXT)->ShowWindow(SW_SHOW);
      m_ctrl_game_variant.ShowWindow(SW_SHOW);
      const std::string s(v.name_);
      m_ctrl_game_variant.SetItemData(m_ctrl_game_variant.AddString(std::wstring(s.cbegin(),s.cend()).c_str()), i);
      {
        m_ctrl_game_variant.SetCurSel(0);
        m_size_x = var[0].xDef_;
        m_size_y = var[0].yDef_;
      }
    }
  }
}


void BGSelect::OnSelchangeGameType()
{
  m_game_id = static_cast<int>(m_ctrl_game_type.GetItemData(m_ctrl_game_type.GetCurSel()));
  var = GetVariants(m_game_id);
  SetControlsForGame();
  UpdateData(FALSE);
}

void BGSelect::OnSelchangeGameVariant()
{
  m_rule_id = static_cast<int>(m_ctrl_game_variant.GetItemData(m_ctrl_game_variant.GetCurSel()));
  m_size_x = var[m_rule_id].xDef_;
  m_size_y = var[m_rule_id].yDef_;
  UpdateData(FALSE);
}

void BGSelect::OnGameP1H() { m_player1 = 0; m_player2 = 1; UpdateData(FALSE); }
void BGSelect::OnGameP1C() { m_player1 = 1; m_player2 = 0; UpdateData(FALSE); }
void BGSelect::OnClickedGameSizeLock() { if ((m_size_lock = !m_size_lock) == true) m_size_y = m_size_x; UpdateData(FALSE); }
