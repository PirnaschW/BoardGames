
#include "App.h"
#include <assert.h>

// BGSelect dialog

IMPLEMENT_DYNAMIC(BGSelect, CDialogEx)

BGSelect::BGSelect(CWnd* pParent /*=nullptr*/) : CDialogEx(IDD_BGSELECT, pParent) {}

BGSelect::~BGSelect() {}

void BGSelect::DoDataExchange(CDataExchange* pDX)
{
  CDialogEx::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_GAME_SELECT_TYPE, m_ctrl_game_type);
  DDX_Radio(pDX, IDC_GAME_SELECT_P1_HUMAN, m_player1);
  DDX_Radio(pDX, IDC_GAME_SELECT_P2_HUMAN, m_player2);
  DDX_Control(pDX, IDC_GAME_SELECT_VARIANT, m_ctrl_game_variant);
  DDX_Control(pDX, IDC_GAME_SELECT_X_SPIN, m_ctrl_spin_x);
  DDX_Text(pDX, IDC_GAME_SELECT_X_EDIT, m_x);
  DDX_Check(pDX, IDC_GAME_SELECT_SIZE_LOCK, m_size_lock);
  DDX_Control(pDX, IDC_GAME_SELECT_Y_SPIN, m_ctrl_spin_y);
  DDX_Text(pDX, IDC_GAME_SELECT_Y_EDIT, m_y);
  DDX_Text(pDX, IDC_GAME_WWW_NO_EDIT, m_www_no);
}


BEGIN_MESSAGE_MAP(BGSelect, CDialogEx)
  ON_LBN_SELCHANGE(IDC_GAME_SELECT_TYPE, &BGSelect::OnSelchangeGameType)
  ON_COMMAND(IDC_GAME_SELECT_P1_HUMAN, &BGSelect::OnGameP1H)
  ON_COMMAND(IDC_GAME_SELECT_P1_COMPUTER, &BGSelect::OnGameP1C)
  ON_COMMAND(IDC_GAME_SELECT_P2_HUMAN, &BGSelect::OnGameP1C)
  ON_COMMAND(IDC_GAME_SELECT_P2_COMPUTER, &BGSelect::OnGameP1H)
  ON_LBN_SELCHANGE(IDC_GAME_SELECT_VARIANT, &BGSelect::OnSelchangeGameVariant)
  ON_BN_CLICKED(IDC_GAME_SELECT_SIZE_LOCK, &BGSelect::OnClickedGameSizeLock)
  ON_COMMAND(ID_NEW_WWW, &BGSelect::OnNewWWW)
END_MESSAGE_MAP()

// BGSelect message handlers
BOOL BGSelect::OnInitDialog()
{
  CDialogEx::OnInitDialog();

  // get list of all games and add to dropdown
  std::vector<BoardGamesCore::GameID> list;
  BoardGamesCore::GetGames(list);
  for (auto g : list) AddGame(g);

  // select first one
  m_ctrl_game_type.SetCurSel(0); // default
  v.id = list[0];
  GetDlgItem(IDC_GAME_SELECT_V_TEXT)->SetWindowTextW(L"Variant:");
  GetDlgItem(IDC_GAME_SELECT_X_TEXT)->SetWindowTextW(L"Horizontal Size:");
  GetDlgItem(IDC_GAME_SELECT_Y_TEXT)->SetWindowTextW(L"Vertical Size:");

  // show variants for the selected game
  var = BoardGamesCore::Variants.GetVariantList(v.id);
  SetControlsForGame();
  UpdateData(FALSE);
  return TRUE;  // return TRUE unless you set the focus to a control
                // EXCEPTION: OCX Property Pages should return FALSE
}

void BGSelect::AddGame(BoardGamesCore::GameID resourceID)
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
    const auto& v0{ var[i] };
    {
      m_x = v.x = v0.xDef_;
      BOOL st = v0.xMin_ == 0 ? SW_HIDE : SW_SHOW;
      GetDlgItem(IDC_GAME_SELECT_X_TEXT)->ShowWindow(st);
      GetDlgItem(IDC_GAME_SELECT_X_EDIT)->ShowWindow(st);
      m_ctrl_spin_x.ShowWindow(st);
      if (v0.xMin_ > 0) m_ctrl_spin_x.SetRange32(v0.xMin_, v0.xMax_);
    }

    {
      m_y = v.y = v0.yDef_;
      BOOL st = v0.yMin_ == 0 ? SW_HIDE : SW_SHOW;
      GetDlgItem(IDC_GAME_SELECT_Y_TEXT)->ShowWindow(st);
      GetDlgItem(IDC_GAME_SELECT_Y_EDIT)->ShowWindow(st);
      m_ctrl_spin_y.ShowWindow(st);
      if (v0.yMin_ > 0)
      {
        m_ctrl_spin_y.SetRange32(v0.yMin_, v0.yMax_);
        GetDlgItem(IDC_GAME_SELECT_SIZE_LOCK)->ShowWindow(SW_SHOW);
        m_size_lock = v0.xDef_ == v0.yDef_ ? TRUE : FALSE;
      }
      else
      {
        GetDlgItem(IDC_GAME_SELECT_SIZE_LOCK)->ShowWindow(SW_HIDE);
        m_size_lock = TRUE;
      }
    }

    if (v0.vName_ != "")
    {
      GetDlgItem(IDC_GAME_SELECT_V_TEXT)->ShowWindow(SW_SHOW);
      m_ctrl_game_variant.ShowWindow(SW_SHOW);
      const std::string s(v0.vName_);
      m_ctrl_game_variant.SetItemData(m_ctrl_game_variant.AddString(std::wstring(s.cbegin(),s.cend()).c_str()), i);
      m_ctrl_game_variant.SetCurSel(0);
      m_x = v.x = var[0].xDef_;
      m_y = v.y = var[0].yDef_;
      v.c = var[0].vCode_;
    }
  }
}


void BGSelect::OnSelchangeGameType()
{
  v.id = static_cast<int>(m_ctrl_game_type.GetItemData(m_ctrl_game_type.GetCurSel()));
  var = BoardGamesCore::Variants.GetVariantList(v.id);
  SetControlsForGame();
  UpdateData(FALSE);
}

void BGSelect::OnSelchangeGameVariant()
{
  m_var_index = static_cast<int>(m_ctrl_game_variant.GetItemData(m_ctrl_game_variant.GetCurSel()));
  v.c = var[m_var_index].vCode_;
  m_x = v.x = var[m_var_index].xDef_;
  m_y = v.y = var[m_var_index].yDef_;
  UpdateData(FALSE);
}

void BGSelect::OnGameP1H() { m_player1 = 0; m_player2 = 1; UpdateData(FALSE); }
void BGSelect::OnGameP1C() { m_player1 = 1; m_player2 = 0; UpdateData(FALSE); }
void BGSelect::OnClickedGameSizeLock() { if ((m_size_lock = !m_size_lock) == true) m_y = v.y = v.x; UpdateData(FALSE); }

void BGSelect::OnNewWWW()
{
  UpdateData(TRUE);

//  v = BoardGamesCore::LoadFromWWW(m_www_no,m_setup);
  if (v.id) EndDialog(IDOK);
}

BoardGamesCore::GameDispatch* BGSelect::CreateGame()
{
  if (DoModal() != IDOK) return nullptr;

  v.x = m_x;
  v.y = m_y;
  BoardGamesCore::GameDispatch* d = new BoardGamesCore::GameDispatch(v, m_setup);
  return d;
}
