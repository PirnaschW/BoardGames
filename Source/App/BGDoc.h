// BGDoc.h : interface of the CBGDoc class
//

//using BoardGamesMFC::CmdUI;
#include "Dispatch.h"

class CBGDoc : public CDocument
{
protected: // create from serialization only
	CBGDoc() noexcept;
	DECLARE_DYNCREATE(CBGDoc)

// Attributes
public:
  BoardGamesCore::GameDispatch* g_{nullptr};

// Operations
public:
  void CallBack();

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	~CBGDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
	void OnCommand(UINT nID);
	void OnUpdateCommandUI(CCmdUI* pCmdUI);

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
