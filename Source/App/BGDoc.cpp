#include "App.h"

// BGDoc.cpp : implementation of the CBGDoc class
//

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CBGDoc

IMPLEMENT_DYNCREATE(CBGDoc, CDocument)

BEGIN_MESSAGE_MAP(CBGDoc, CDocument)
  ON_COMMAND_RANGE(ID_COMMAND_RANGE_MIN, ID_COMMAND_RANGE_MAX, &CBGDoc::OnCommand)
  ON_UPDATE_COMMAND_UI_RANGE(ID_COMMAND_RANGE_MIN, ID_COMMAND_RANGE_MAX, &CBGDoc::OnUpdateCommandUI)
  ON_COMMAND_RANGE(ID_EDIT_CLEAR, ID_EDIT_REDO, &CBGDoc::OnCommand)
  ON_UPDATE_COMMAND_UI_RANGE(ID_EDIT_CLEAR, ID_EDIT_REDO, &CBGDoc::OnUpdateCommandUI)
END_MESSAGE_MAP()

// CBGDoc construction/destruction

CBGDoc::CBGDoc() noexcept
{
  // add one-time construction code here (none at this time)
}

CBGDoc::~CBGDoc()
{
  delete g_;
}

BOOL CBGDoc::OnNewDocument()
{
  if (!CDocument::OnNewDocument())
    return FALSE;

  // add reinitialization code here (SDI documents will reuse this document)

  if (g_) delete g_;                   // remove potential previous game
  BGSelect dlg;                        
  g_ = dlg.CreateGame();               // have user select game type & variant
  if (g_ == nullptr) return FALSE;     // user aborted or load failed

  g_->SetUpdateCallBack(std::bind(&CBGDoc::CallBack, this));   // allows to re-draw screen while AI is thinking
  return TRUE;
}

// CBGDoc serialization

void CBGDoc::Serialize(CArchive& ar)
{
  Archive a(&ar);

  if (ar.IsLoading()) // read the game type upfront, and create the respective empty game
  {
    BoardGamesCore::VariantChosen v;
    a.Serialize(v);
    g_ = new BoardGamesCore::GameDispatch(v);
  }
  g_->Serialize(a); // load or store rest of the game
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CBGDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
  // Modify this code to draw the document's data
  dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

  CString strText = _T("TODO: implement thumbnail drawing here");
  LOGFONT lf;

  CFont* pDefaultGUIFont = CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT));
  pDefaultGUIFont->GetLogFont(&lf);
  lf.lfHeight = 36;

  CFont fontDraw;
  fontDraw.CreateFontIndirect(&lf);

  CFont* pOldFont = dc.SelectObject(&fontDraw);
  dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
  dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CBGDoc::InitializeSearchContent()
{
  CString strSearchContent;
  // Set search contents from document's data.
  // The content parts should be separated by ";"

  // For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
  SetSearchContent(strSearchContent);
}

void CBGDoc::SetSearchContent(const CString& value)
{
  if (value.IsEmpty())
  {
    RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
  }
  else
  {
    CMFCFilterChunkValueImpl *pChunk = nullptr;
    ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
    if (pChunk != nullptr)
    {
      pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
      SetChunkValue(pChunk);
    }
  }
}

#endif // SHARED_HANDLERS

// CBGDoc diagnostics

#ifdef _DEBUG
void CBGDoc::AssertValid() const
{
  CDocument::AssertValid();
}

void CBGDoc::Dump(CDumpContext& dc) const
{
  CDocument::Dump(dc);
}
#endif //_DEBUG


void CBGDoc::OnCommand(UINT nID) { if (g_->React(nID)) UpdateAllViews(nullptr); };
void CBGDoc::OnUpdateCommandUI(CCmdUI* pCmdUI) { CmdUI c{ pCmdUI };  g_->React(&c); }

// CBGDoc commands
void CBGDoc::CallBack()
{
  UpdateAllViews(nullptr);

  CBGView* v;
  for (POSITION p = GetFirstViewPosition(); (v = static_cast<CBGView*>(GetNextView(p))) != nullptr; )
    v->UpdateWindow();
}
