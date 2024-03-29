#include "App.h"

// BG.cpp : Defines the class behaviors for the application.
//

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBGApp

BEGIN_MESSAGE_MAP(CBGApp, CWinAppEx)
  ON_COMMAND(ID_APP_ABOUT, &CBGApp::OnAppAbout)
  // Standard file based document commands
  ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
  ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
  // Standard print setup command
  ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()


// CBGApp construction

CBGApp::CBGApp() noexcept
{
  m_bHiColorIcons = TRUE;

  // support Restart Manager
  m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;

  // recommended format for string is CompanyName.ProductName.SubProduct.VersionInformation
  SetAppID(_T("BG.BG.0001"));

  // add construction code here,
  // Place all significant initialization in InitInstance

  //srand((unsigned)time(nullptr));
  std::srand(1);
}

// The one and only CBGApp object

CBGApp theApp;


// CBGApp initialization

BOOL CBGApp::InitInstance()
{
  // InitCommonControlsEx() is required on Windows XP if an application
  // manifest specifies use of ComCtl32.dll version 6 or later to enable
  // visual styles.  Otherwise, any window creation will fail.
  INITCOMMONCONTROLSEX InitCtrls;
  InitCtrls.dwSize = sizeof(InitCtrls);
  // Set this to include all the common control classes you want to use
  // in your application.
  InitCtrls.dwICC = ICC_WIN95_CLASSES;
  InitCommonControlsEx(&InitCtrls);

  CWinAppEx::InitInstance();


  EnableTaskbarInteraction(FALSE);

  // AfxInitRichEdit2() is required to use RichEdit control
  // AfxInitRichEdit2();

  // Standard initialization
  // If you are not using these features and wish to reduce the size
  // of your final executable, you should remove from the following
  // the specific initialization routines you do not need

  // Change the registry key under which our settings are stored
  SetRegistryKey(_T("BoardGames"));
  LoadStdProfileSettings(16);  // Load standard INI file options (including MRU)

  InitContextMenuManager();

  InitKeyboardManager();

  InitTooltipManager();
  CMFCToolTipInfo ttParams;
  ttParams.m_bVislManagerTheme = TRUE;
  theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
    RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

  // Register the application's document templates.  Document templates
  //  serve as the connection between documents, frame windows and views

  CMultiDocTemplate* pDocTemplate;

  pDocTemplate = new CMultiDocTemplate(IDR_GAMETYPE,
    RUNTIME_CLASS(CBGDoc),
    RUNTIME_CLASS(CChildFrame), // custom MDI child frame
    RUNTIME_CLASS(CBGView));
  if (!pDocTemplate) return FALSE;
  AddDocTemplate(pDocTemplate);

  // create main MDI Frame window
  CMainFrame* pMainFrame = new CMainFrame;
  if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
  {
    delete pMainFrame;
    return FALSE;
  }
  m_pMainWnd = pMainFrame;

  // call DragAcceptFiles only if there's a suffix
  //  In an MDI app, this should occur immediately after setting m_pMainWnd
  // Enable drag/drop open
  m_pMainWnd->DragAcceptFiles();

  // Parse command line for standard shell commands, DDE, file open
  CCommandLineInfo cmdInfo;
  ParseCommandLine(cmdInfo);

  // Enable DDE Execute open
  EnableShellOpen();
  RegisterShellFileTypes(TRUE);


  // Dispatch commands specified on the command line.  Will return FALSE if
  // app was launched with /RegServer, /Register, /Unregserver or /Unregister.
  if (!ProcessShellCommand(cmdInfo))
    return FALSE;
  // The main window has been initialized, so show and update it
  pMainFrame->ShowWindow(m_nCmdShow);
  pMainFrame->UpdateWindow();

  return TRUE;
}

int CBGApp::ExitInstance()
{
  // handle additional resources you may have added: (none at this time)

  return CWinAppEx::ExitInstance();
}

// CBGApp message handlers


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
  CAboutDlg() noexcept;

  // Dialog Data
#ifdef AFX_DESIGN_TIME
  enum { IDD = IDD_ABOUTBOX };
#endif

protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
  DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() noexcept : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// App command to run the dialog
void CBGApp::OnAppAbout()
{
  CAboutDlg aboutDlg;
  aboutDlg.DoModal();
}

// CBGApp customization load/save methods

void CBGApp::PreLoadState()
{
  BOOL bNameValid;
  CString strName;
  bNameValid = strName.LoadString(IDS_EDIT_MENU);
  ASSERT(bNameValid);
  GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}

void CBGApp::LoadCustomState()
{
}

void CBGApp::SaveCustomState()
{
}
