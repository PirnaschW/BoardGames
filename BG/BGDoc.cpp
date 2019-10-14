// BGDoc.cpp : implementation of the CBGDoc class
//

#include "stdafx.h"
#include "BGSelect.h"

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
  // TODO: add one-time construction code here
}

CBGDoc::~CBGDoc()
{
  delete game;
}

BOOL CBGDoc::OnNewDocument()
{
  if (!CDocument::OnNewDocument())
    return FALSE;

  // TODO: add reinitialization code here
  // (SDI documents will reuse this document)

  BGSelect dlg;
  game = dlg.CreateGame();
  if (game == nullptr) return FALSE;

  game->SetUpdateCallBack(std::bind(&CBGDoc::CallBack, this));
  return TRUE;
}

// CBGDoc serialization

void CBGDoc::Serialize(CArchive& ar)
{
  if (ar.IsStoring())
  {
    game->Serialize(&ar);
  }
  else
  {
    game->Serialize(&ar);
  }
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

// CBGDoc commands

void CBGDoc::CallBack(void)
{
  UpdateAllViews(nullptr);

  CBGView* v;
  for (POSITION p = GetFirstViewPosition(); (v = dynamic_cast<CBGView*>(GetNextView(p))) != nullptr; )
    v->UpdateWindow();
}
