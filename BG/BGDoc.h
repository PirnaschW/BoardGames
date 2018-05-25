// BGDoc.h : interface of the CBGDoc class
//

BoardGamesCore::Game* GetGame(UINT);

class CBGDoc : public CDocument
{
protected: // create from serialization only
	CBGDoc() noexcept;
	DECLARE_DYNCREATE(CBGDoc)

// Attributes
public:
  BoardGamesCore::Game* game{nullptr};

// Operations
public:

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
	virtual ~CBGDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
  void OnCommand(UINT nID) { if (game->React(nID)) UpdateAllViews(NULL); };
  void OnUpdateCommandUI(CCmdUI* pCmdUI) { game->React(pCmdUI); }

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
