
#include "App.h"

// BGView.cpp : implementation of the CBGView class
//

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
{	// add construction code here (none at this time)
}

CBGView::~CBGView()
{
}

BOOL CBGView::PreCreateWindow(CREATESTRUCT& cs)
{
	// Modify the Window class or styles here by modifying the CREATESTRUCT cs (none at this time)
	return CView::PreCreateWindow(cs);
}

// CBGView drawing

void CBGView::OnDraw(CDC* pDC)
{
	CBGDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

  // add draw code for native data here
	DC dc{ pDC };                                  // create a non-MFC object (with a pointer to the MFC object),
	                                               // to disconnect MFC from the rest of the code (->fast compile, no dependency)
  if (pDoc->g_ != nullptr) pDoc->g_->Draw(&dc);  // draw the game if it exists
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
{	// add extra initialization before printing (none at this time)
}

void CBGView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{	// add cleanup after printing (none at this time)
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

#endif //_DEBUG
CBGDoc* CBGView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBGDoc)));
	return (CBGDoc*)m_pDocument;
}
//#endif //_DEBUG


// CBGView message handlers

void CBGView::OnLButtonDown(UINT nFlags, CPoint point)
{
  if (GetDocument()->g_->React(WM_LBUTTONDOWN, nFlags, Point(point.x,point.y))) GetDocument()->UpdateAllViews(nullptr);
}

void CBGView::OnLButtonUp(UINT nFlags, CPoint point)
{
  if (GetDocument()->g_->React(WM_LBUTTONUP, nFlags, Point(point.x, point.y))) GetDocument()->UpdateAllViews(nullptr);
}

void CBGView::OnRButtonUp(UINT nFlags, CPoint point)
{
  if (GetDocument()->g_->React(WM_RBUTTONUP, nFlags, Point(point.x, point.y))) GetDocument()->UpdateAllViews(nullptr);
  else
  {
  	ClientToScreen(&point);
	  OnContextMenu(this, point);
  }
}

void CBGView::OnMouseMove(UINT nFlags, CPoint point)
{
  if (GetDocument()->g_->React(WM_MOUSEMOVE, nFlags, Point(point.x, point.y))) GetDocument()->UpdateAllViews(nullptr);
}

void CBGView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
  if (GetDocument()->g_->React(nChar, nRepCnt, nFlags)) GetDocument()->UpdateAllViews(nullptr);
}
