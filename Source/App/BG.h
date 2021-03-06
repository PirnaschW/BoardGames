// BG.h : main header file for the BG application
//

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

// CBGApp:
// See BG.cpp for the implementation of this class
//

class CBGApp : public CWinAppEx
{
public:
	CBGApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
  UINT  m_nAppLook{};
  BOOL  m_bHiColorIcons{};

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CBGApp theApp;
