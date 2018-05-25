// BGView.cpp : implementation of the CBGView class
//

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBGView

IMPLEMENT_DYNCREATE(CBGView, CView)

BEGIN_MESSAGE_MAP(CBGView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CBGView::OnFilePrintPreview)
  ON_WM_LBUTTONDOWN()
  ON_WM_LBUTTONUP()
  ON_WM_RBUTTONUP()
  ON_WM_MOUSEMOVE()
  ON_WM_CHAR()
END_MESSAGE_MAP()

// CBGView construction/destruction

CBGView::CBGView() noexcept
{
	// TODO: add construction code here

}

CBGView::~CBGView()
{
}

BOOL CBGView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CBGView drawing

void CBGView::OnDraw(CDC* pDC)
{
	CBGDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

  // TODO: add draw code for native data here
  pDoc->game->Draw(pDC);
}


// CBGView printing


void CBGView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CBGView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CBGView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CBGView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CBGView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CBGView diagnostics

#ifdef _DEBUG
void CBGView::AssertValid() const
{
	CView::AssertValid();
}

void CBGView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CBGDoc* CBGView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBGDoc)));
	return (CBGDoc*)m_pDocument;
}
#endif //_DEBUG


// CBGView message handlers

void CBGView::OnLButtonDown(UINT nFlags, CPoint point)
{
  if (GetDocument()->game->React(WM_LBUTTONDOWN, nFlags, point)) GetDocument()->UpdateAllViews(NULL);
}

void CBGView::OnLButtonUp(UINT nFlags, CPoint point)
{
  if (GetDocument()->game->React(WM_LBUTTONUP, nFlags, point)) GetDocument()->UpdateAllViews(NULL);
}

void CBGView::OnRButtonUp(UINT nFlags, CPoint point)
{
  if (GetDocument()->game->React(WM_RBUTTONUP, nFlags, point)) GetDocument()->UpdateAllViews(NULL);
  else
  {
  	ClientToScreen(&point);
	  OnContextMenu(this, point);
  }
}

void CBGView::OnMouseMove(UINT nFlags, CPoint point)
{
  if (GetDocument()->game->React(WM_MOUSEMOVE, nFlags, point)) GetDocument()->UpdateAllViews(NULL);
}

void CBGView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
  if (GetDocument()->game->React(nChar, nRepCnt, nFlags)) GetDocument()->UpdateAllViews(NULL);
}
