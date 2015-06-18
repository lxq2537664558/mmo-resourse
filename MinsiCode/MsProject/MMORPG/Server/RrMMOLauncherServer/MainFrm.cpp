// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// ("Fluent UI")����ʾ������Ϊ�ο������ṩ�� 
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ �������渽����ص����ĵ���
// ���ơ�ʹ�û�ַ� Fluent UI �����������ǵ����ṩ�ġ�
// ��Ҫ�˽��й� Fluent UI ���ɼƻ�����ϸ��Ϣ�������  
// http://msdn.microsoft.com/officeui��
//
// ��Ȩ���� (C) Microsoft Corporation
// ��������Ȩ����

// MainFrm.cpp : CMainFrame ���ʵ��
//

#include "Precompiled.h"
#include "RrMMOLauncherServer.h"

#include "MainFrm.h"

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWndEx)
    ON_WM_CREATE()
    ON_COMMAND(ID_WINDOW_MANAGER, &CMainFrame::OnWindowManager)
    ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_OFF_2007_AQUA, &CMainFrame::OnApplicationLook)
    ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_OFF_2007_AQUA, &CMainFrame::OnUpdateApplicationLook)
    ON_COMMAND(ID_VIEW_CAPTION_BAR, &CMainFrame::OnViewCaptionBar)
    ON_UPDATE_COMMAND_UI(ID_VIEW_CAPTION_BAR, &CMainFrame::OnUpdateViewCaptionBar)
END_MESSAGE_MAP()

// CMainFrame ����/����

CMainFrame::CMainFrame()
{
    // TODO: �ڴ����ӳ�Ա��ʼ������
    theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_OFF_2007_BLACK);
}

CMainFrame::~CMainFrame()
{
}

INT CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CMDIFrameWndEx::OnCreate(lpCreateStruct) == -1)
        return -1;

    BOOL bNameValid;
    // ���ڳ־�ֵ�����Ӿ�����������ʽ
    OnApplicationLook(theApp.m_nAppLook);

    CMDITabInfo mdiTabParams;
    mdiTabParams.m_style = CMFCTabCtrl::STYLE_3D_ONENOTE; // ����������ʽ...
    mdiTabParams.m_bActiveTabCloseButton = TRUE;      // ����Ϊ FALSE �Ὣ�رհ�ť������ѡ�������Ҳ�
    mdiTabParams.m_bTabIcons = FALSE;    // ����Ϊ TRUE ���� MDI ѡ��������ĵ�ͼ��
    mdiTabParams.m_bAutoColor = TRUE;    // ����Ϊ FALSE ������ MDI ѡ����Զ���ɫ
    mdiTabParams.m_bDocumentMenu = TRUE; // ��ѡ�������ұ�Ե�����ĵ��˵�
    EnableMDITabbedGroups(TRUE, mdiTabParams);

    m_wndRibbonBar.Create(this);
    InitializeRibbon();

    if (!m_wndStatusBar.Create(this))
    {
        TRACE0("δ�ܴ���״̬��\n");
        return -1;      // δ�ܴ���
    }

    CString strTitlePane1;
    CString strTitlePane2;
    bNameValid = strTitlePane1.LoadString(IDS_STATUS_PANE1);
    ASSERT(bNameValid);
    bNameValid = strTitlePane2.LoadString(IDS_STATUS_PANE2);
    ASSERT(bNameValid);
    m_wndStatusBar.AddElement(NEW CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE1, strTitlePane1, TRUE), strTitlePane1);
    m_wndStatusBar.AddExtendedElement(NEW CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE2, strTitlePane2, TRUE), strTitlePane2);

    // ���� Visual Studio 2005 ��ʽͣ��������Ϊ
    CDockingManager::SetDockingMode(DT_SMART);
    // ���� Visual Studio 2005 ��ʽͣ�������Զ�������Ϊ
    EnableAutoHidePanes(CBRS_ALIGN_ANY);

    // �������񽫴�������࣬��˽���ʱ��������ͣ��:
    EnableDocking(CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM | CBRS_ALIGN_RIGHT);

    // ���������á�Outlook��������:
    if (!CreateOutlookBar(m_wndNavigationBar, ID_VIEW_NAVIGATION, m_wndTree, m_wndCalendar, 250))
    {
        TRACE0("δ�ܴ�����������\n");
        return -1;      // δ�ܴ���
    }

    // ����������:
    if (!CreateCaptionBar())
    {
        TRACE0("δ�ܴ���������\n");
        return -1;      // δ�ܴ���
    }

    // �Ѵ��� Outlook ����Ӧ���������ͣ����
    EnableDocking(CBRS_ALIGN_LEFT);
    EnableAutoHidePanes(CBRS_ALIGN_RIGHT);

    // ���ز˵���ͼ��(�����κα�׼��������):
    CMFCToolBar::AddToolBarForImageCollection(IDR_MENU_IMAGES, theApp.m_bHiColorIcons ? IDB_MENU_IMAGES_24 : 0);

    // ����ͣ������
    if (!CreateDockingWindows())
    {
        TRACE0("δ�ܴ���ͣ������\n");
        return -1;
    }

    m_wndFileView.EnableDocking(CBRS_ALIGN_ANY);
    m_wndClassView.EnableDocking(CBRS_ALIGN_ANY);
    DockPane(&m_wndFileView);
    CDockablePane* pTabbedBar = NULL;
    m_wndClassView.AttachToTabWnd(&m_wndFileView, DM_SHOW, TRUE, &pTabbedBar);
    m_wndOutput.EnableDocking(CBRS_ALIGN_ANY);
    DockPane(&m_wndOutput);
    m_wndProperties.EnableDocking(CBRS_ALIGN_ANY);
    DockPane(&m_wndProperties);


    // ������ǿ�Ĵ��ڹ����Ի���
    EnableWindowsDialog(ID_WINDOW_MANAGER, IDS_WINDOWS_MANAGER, TRUE);

    return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
    if( !CMDIFrameWndEx::PreCreateWindow(cs) )
        return FALSE;
    // TODO: �ڴ˴�ͨ���޸�
    //  CREATESTRUCT cs ���޸Ĵ��������ʽ

    return TRUE;
}

VOID CMainFrame::InitializeRibbon()
{
    CMFCRibbonButton* pBtnPrint             = NULL;
    CMFCRibbonButton* pBtnPaste             = NULL;
    CMFCRibbonButton* pBtnStatusBar         = NULL;
    CMFCRibbonButton* pBtnCaptionBar        = NULL;
    CMFCRibbonButton* pBtnWindows           = NULL;
    CMFCRibbonButton* pVisualStyleButton    = NULL;
    _ENTER_FUN_C;

    BOOL bNameValid;

    CString strTemp;
    bNameValid = strTemp.LoadString(IDS_RIBBON_FILE);
    ASSERT(bNameValid);

    // �������ͼ��:
    m_PanelImages.SetImageSize(CSize(16, 16));
    m_PanelImages.Load(IDB_BUTTONS);

    // ��ʼ����ť:
    m_MainButton.SetImage(IDB_MAIN);
    m_MainButton.SetText(_T("\nf"));
    m_MainButton.SetToolTipText(strTemp);

    m_wndRibbonBar.SetApplicationButton(&m_MainButton, CSize (45, 45));
    CMFCRibbonMainPanel* pMainPanel = m_wndRibbonBar.AddMainCategory(strTemp, IDB_FILESMALL, IDB_FILELARGE);

    bNameValid = strTemp.LoadString(IDS_RIBBON_NEW);
    ASSERT(bNameValid);
    pMainPanel->Add(new CMFCRibbonButton(ID_FILE_NEW, strTemp, 0, 0));
    bNameValid = strTemp.LoadString(IDS_RIBBON_OPEN);
    ASSERT(bNameValid);
    pMainPanel->Add(new CMFCRibbonButton(ID_FILE_OPEN, strTemp, 1, 1));
    bNameValid = strTemp.LoadString(IDS_RIBBON_SAVE);
    ASSERT(bNameValid);
    pMainPanel->Add(new CMFCRibbonButton(ID_FILE_SAVE, strTemp, 2, 2));
    bNameValid = strTemp.LoadString(IDS_RIBBON_SAVEAS);
    ASSERT(bNameValid);
    pMainPanel->Add(new CMFCRibbonButton(ID_FILE_SAVE_AS, strTemp, 3, 3));

    bNameValid = strTemp.LoadString(IDS_RIBBON_PRINT);
    ASSERT(bNameValid);
    pBtnPrint = new CMFCRibbonButton(ID_FILE_PRINT, strTemp, 6, 6);
    pBtnPrint->SetKeys(_T("p"), _T("w"));
    bNameValid = strTemp.LoadString(IDS_RIBBON_PRINT_LABEL);
    ASSERT(bNameValid);
    pBtnPrint->AddSubItem(new CMFCRibbonLabel(strTemp));
    bNameValid = strTemp.LoadString(IDS_RIBBON_PRINT_QUICK);
    ASSERT(bNameValid);
    pBtnPrint->AddSubItem(new CMFCRibbonButton(ID_FILE_PRINT_DIRECT, strTemp, 7, 7, TRUE));
    bNameValid = strTemp.LoadString(IDS_RIBBON_PRINT_PREVIEW);
    ASSERT(bNameValid);
    pBtnPrint->AddSubItem(new CMFCRibbonButton(ID_FILE_PRINT_PREVIEW, strTemp, 8, 8, TRUE));
    bNameValid = strTemp.LoadString(IDS_RIBBON_PRINT_SETUP);
    ASSERT(bNameValid);
    pBtnPrint->AddSubItem(new CMFCRibbonButton(ID_FILE_PRINT_SETUP, strTemp, 11, 11, TRUE));
    pMainPanel->Add(pBtnPrint);
    pMainPanel->Add(new CMFCRibbonSeparator(TRUE));

    bNameValid = strTemp.LoadString(IDS_RIBBON_CLOSE);
    ASSERT(bNameValid);
    pMainPanel->Add(new CMFCRibbonButton(ID_FILE_CLOSE, strTemp, 9, 9));

    bNameValid = strTemp.LoadString(IDS_RIBBON_RECENT_DOCS);
    ASSERT(bNameValid);
    pMainPanel->AddRecentFilesList(strTemp);

    bNameValid = strTemp.LoadString(IDS_RIBBON_EXIT);
    ASSERT(bNameValid);
    pMainPanel->AddToBottom(new CMFCRibbonMainPanelButton(ID_APP_EXIT, strTemp, 15));

    // Ϊ�������塱������ӡ��������:
    bNameValid = strTemp.LoadString(IDS_RIBBON_HOME);
    ASSERT(bNameValid);
    CMFCRibbonCategory* pCategoryHome = m_wndRibbonBar.AddCategory(strTemp, IDB_WRITESMALL, IDB_WRITELARGE);

    // �����������塱���:
    bNameValid = strTemp.LoadString(IDS_RIBBON_CLIPBOARD);
    ASSERT(bNameValid);
    CMFCRibbonPanel* pPanelClipboard = pCategoryHome->AddPanel(strTemp, m_PanelImages.ExtractIcon(27));

    bNameValid = strTemp.LoadString(IDS_RIBBON_PASTE);
    ASSERT(bNameValid);
    pBtnPaste = new CMFCRibbonButton(ID_EDIT_PASTE, strTemp, 0, 0);
    pPanelClipboard->Add(pBtnPaste);

    bNameValid = strTemp.LoadString(IDS_RIBBON_CUT);
    ASSERT(bNameValid);
    pPanelClipboard->Add(new CMFCRibbonButton(ID_EDIT_CUT, strTemp, 1));
    bNameValid = strTemp.LoadString(IDS_RIBBON_COPY);
    ASSERT(bNameValid);
    pPanelClipboard->Add(new CMFCRibbonButton(ID_EDIT_COPY, strTemp, 2));
    bNameValid = strTemp.LoadString(IDS_RIBBON_SELECTALL);
    ASSERT(bNameValid);
    pPanelClipboard->Add(new CMFCRibbonButton(ID_EDIT_SELECT_ALL, strTemp, -1));

    // ���������ӡ���ͼ�����:
    bNameValid = strTemp.LoadString(IDS_RIBBON_VIEW);
    ASSERT(bNameValid);
    CMFCRibbonPanel* pPanelView = pCategoryHome->AddPanel(strTemp, m_PanelImages.ExtractIcon (7));

    bNameValid = strTemp.LoadString(IDS_RIBBON_STATUSBAR);
    ASSERT(bNameValid);
    pBtnStatusBar = new CMFCRibbonCheckBox(ID_VIEW_STATUS_BAR, strTemp);
    pPanelView->Add(pBtnStatusBar);
    bNameValid = strTemp.LoadString(IDS_RIBBON_CAPTIONBAR);
    ASSERT(bNameValid);
    pBtnCaptionBar = new CMFCRibbonCheckBox(ID_VIEW_CAPTION_BAR, strTemp);
    pPanelView->Add(pBtnCaptionBar);

    // ���������ӡ����ڡ����:
    bNameValid = strTemp.LoadString(IDS_RIBBON_WINDOW);
    ASSERT(bNameValid);
    CMFCRibbonPanel* pPanelWindow = pCategoryHome->AddPanel(strTemp, m_PanelImages.ExtractIcon (7));

    bNameValid = strTemp.LoadString(IDS_RIBBON_WINDOWS);
    ASSERT(bNameValid);
    pBtnWindows = new CMFCRibbonButton(ID_WINDOW_MANAGER, strTemp, -1, 1);
    pBtnWindows->SetMenu(IDR_WINDOWS_MENU, TRUE);
    pPanelWindow->Add(pBtnWindows);

    // ��Ԫ�����ӵ�ѡ��Ҳ�:
    bNameValid = strTemp.LoadString(IDS_RIBBON_STYLE);
    ASSERT(bNameValid);
    pVisualStyleButton = new CMFCRibbonButton(INVALID_UID, strTemp, -1, -1);

    pVisualStyleButton->SetMenu(IDR_THEME_MENU, FALSE /* ��Ĭ������*/, TRUE /* �Ҷ���*/);

    bNameValid = strTemp.LoadString(IDS_RIBBON_STYLE_TIP);
    ASSERT(bNameValid);
    pVisualStyleButton->SetToolTipText(strTemp);
    bNameValid = strTemp.LoadString(IDS_RIBBON_STYLE_DESC);
    ASSERT(bNameValid);
    pVisualStyleButton->SetDescription(strTemp);
    m_wndRibbonBar.AddToTabs(pVisualStyleButton);

    // ���ӿ��ٷ��ʹ���������:
    CList<UINT, UINT> lstQATCmds;

    lstQATCmds.AddTail(ID_FILE_NEW);
    lstQATCmds.AddTail(ID_FILE_OPEN);
    lstQATCmds.AddTail(ID_FILE_SAVE);
    lstQATCmds.AddTail(ID_FILE_PRINT_DIRECT);

    m_wndRibbonBar.SetQuickAccessCommands(lstQATCmds);

    m_wndRibbonBar.AddToTabs(new CMFCRibbonButton(ID_APP_ABOUT, _T("\na"), m_PanelImages.ExtractIcon (0)));

    _DISPOSE_END_C
    {
        SAFE_DELETE(pBtnPrint);
        SAFE_DELETE(pBtnPaste);
        SAFE_DELETE(pBtnStatusBar);
        SAFE_DELETE(pBtnCaptionBar);
        SAFE_DELETE(pBtnWindows);
        SAFE_DELETE(pVisualStyleButton);
    }_LEAVE_FUN;
}

BOOL CMainFrame::CreateDockingWindows()
{
    BOOL bNameValid;

    // ��������ͼ
    CString strClassView;
    bNameValid = strClassView.LoadString(IDS_CLASS_VIEW);
    ASSERT(bNameValid);
    if (!m_wndClassView.Create(strClassView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_CLASSVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
    {
        TRACE0("δ�ܴ���������ͼ������\n");
        return FALSE; // δ�ܴ���
    }

    // �����ļ���ͼ
    CString strFileView;
    bNameValid = strFileView.LoadString(IDS_FILE_VIEW);
    ASSERT(bNameValid);
    if (!m_wndFileView.Create(strFileView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_FILEVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT| CBRS_FLOAT_MULTI))
    {
        TRACE0("δ�ܴ������ļ���ͼ������\n");
        return FALSE; // δ�ܴ���
    }

    // �����������
    CString strOutputWnd;
    bNameValid = strOutputWnd.LoadString(IDS_OUTPUT_WND);
    ASSERT(bNameValid);
    if (!m_wndOutput.Create(strOutputWnd, this, CRect(0, 0, 100, 100), TRUE, ID_VIEW_OUTPUTWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
    {
        TRACE0("δ�ܴ����������\n");
        return FALSE; // δ�ܴ���
    }

    // �������Դ���
    CString strPropertiesWnd;
    bNameValid = strPropertiesWnd.LoadString(IDS_PROPERTIES_WND);
    ASSERT(bNameValid);
    if (!m_wndProperties.Create(strPropertiesWnd, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_PROPERTIESWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
    {
        TRACE0("δ�ܴ��������ԡ�����\n");
        return FALSE; // δ�ܴ���
    }

    SetDockingWindowIcons(theApp.m_bHiColorIcons);
    return TRUE;
}

VOID CMainFrame::SetDockingWindowIcons(BOOL bHiColorIcons)
{
    HICON hFileViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_FILE_VIEW_HC : IDI_FILE_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
    m_wndFileView.SetIcon(hFileViewIcon, FALSE);

    HICON hClassViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_CLASS_VIEW_HC : IDI_CLASS_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
    m_wndClassView.SetIcon(hClassViewIcon, FALSE);

    HICON hOutputBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_OUTPUT_WND_HC : IDI_OUTPUT_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
    m_wndOutput.SetIcon(hOutputBarIcon, FALSE);

    HICON hPropertiesBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_PROPERTIES_WND_HC : IDI_PROPERTIES_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
    m_wndProperties.SetIcon(hPropertiesBarIcon, FALSE);

    UpdateMDITabbedBarsIcons();
}

BOOL CMainFrame::CreateOutlookBar(CMFCOutlookBar& bar, UINT uiID, CMFCShellTreeCtrl& tree, CCalendarBar& calendar, INT nInitialWidth)
{
    CWindowDC dc(NULL);

    bar.SetMode2003();

    BOOL bNameValid;
    CString strTemp;
    bNameValid = strTemp.LoadString(IDS_SHORTCUTS);
    ASSERT(bNameValid);
    if (!bar.Create(strTemp, this, CRect(0, 0, nInitialWidth, 32000), uiID, WS_CHILD | WS_VISIBLE | CBRS_LEFT))
    {
        return FALSE; // δ�ܴ���
    }

    CMFCOutlookBarTabCtrl* pOutlookBar = (CMFCOutlookBarTabCtrl*)bar.GetUnderlyingWindow();

    if (pOutlookBar == NULL)
    {
        ASSERT(FALSE);
        return FALSE;
    }

    pOutlookBar->EnableInPlaceEdit(TRUE);

    static UINT uiPageID = 1;

    DWORD dwPaneStyle = AFX_DEFAULT_TOOLBAR_STYLE | CBRS_FLOAT_MULTI;
    USE_PARAM(dwPaneStyle);

    // �ɸ��������Զ����أ��ɵ�����С�������ܹر�
    DWORD dwStyle = AFX_CBRS_FLOAT | AFX_CBRS_AUTOHIDE | AFX_CBRS_RESIZE;

    CRect rectDummy(0, 0, 0, 0);
    CONST DWORD dwTreeStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS;

    tree.Create(dwTreeStyle, rectDummy, &bar, 1200);
    bNameValid = strTemp.LoadString(IDS_FOLDERS);
    ASSERT(bNameValid);
    pOutlookBar->AddControl(&tree, strTemp, 2, TRUE, dwStyle);

    calendar.Create(rectDummy, &bar, 1201);
    bNameValid = strTemp.LoadString(IDS_CALENDAR);
    ASSERT(bNameValid);
    pOutlookBar->AddControl(&calendar, strTemp, 3, TRUE, dwStyle);

    bar.SetPaneStyle(bar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

    pOutlookBar->SetImageList(theApp.m_bHiColorIcons ? IDB_PAGES_HC : IDB_PAGES, 24);
    pOutlookBar->SetToolbarImageList(theApp.m_bHiColorIcons ? IDB_PAGES_SMALL_HC : IDB_PAGES_SMALL, 16);
    pOutlookBar->RecalcLayout();

    BOOL bAnimation = theApp.GetInt(_T("OutlookAnimation"), TRUE);
    CMFCOutlookBarTabCtrl::EnableAnimation(bAnimation);

    bar.SetButtonsFont(&afxGlobalData.fontBold);

    return TRUE;
}

BOOL CMainFrame::CreateCaptionBar()
{
    if (!m_wndCaptionBar.Create(WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, this, ID_VIEW_CAPTION_BAR, -1, TRUE))
    {
        TRACE0("δ�ܴ���������\n");
        return FALSE;
    }

    BOOL bNameValid;

    CString strTemp, strTemp2;
    bNameValid = strTemp.LoadString(IDS_CAPTION_BUTTON);
    ASSERT(bNameValid);
    m_wndCaptionBar.SetButton(strTemp, ID_TOOLS_OPTIONS, CMFCCaptionBar::ALIGN_LEFT, FALSE);
    bNameValid = strTemp.LoadString(IDS_CAPTION_BUTTON_TIP);
    ASSERT(bNameValid);
    m_wndCaptionBar.SetButtonToolTip(strTemp);

    bNameValid = strTemp.LoadString(IDS_CAPTION_TEXT);
    ASSERT(bNameValid);
    m_wndCaptionBar.SetText(strTemp, CMFCCaptionBar::ALIGN_LEFT);

    m_wndCaptionBar.SetBitmap(IDB_INFO, RGB(255, 255, 255), FALSE, CMFCCaptionBar::ALIGN_LEFT);
    bNameValid = strTemp.LoadString(IDS_CAPTION_IMAGE_TIP);
    ASSERT(bNameValid);
    bNameValid = strTemp2.LoadString(IDS_CAPTION_IMAGE_TEXT);
    ASSERT(bNameValid);
    m_wndCaptionBar.SetImageToolTip(strTemp, strTemp2);

    return TRUE;
}

// CMainFrame ���

#ifdef _DEBUG
VOID CMainFrame::AssertValid() CONST
{
    CMDIFrameWndEx::AssertValid();
}

VOID CMainFrame::Dump(CDumpContext& dc) CONST
{
    CMDIFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame ��Ϣ��������

VOID CMainFrame::OnWindowManager()
{
    ShowWindowsDialog();
}

VOID CMainFrame::OnApplicationLook(UINT id)
{
    CWaitCursor wait;

    theApp.m_nAppLook = id;

    switch (theApp.m_nAppLook)
    {
    case ID_VIEW_APPLOOK_WIN_2000:
        CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
        break;

    case ID_VIEW_APPLOOK_OFF_XP:
        CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
        break;

    case ID_VIEW_APPLOOK_WIN_XP:
        CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
        CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
        break;

    case ID_VIEW_APPLOOK_OFF_2003:
        CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
        CDockingManager::SetDockingMode(DT_SMART);
        break;

    case ID_VIEW_APPLOOK_VS_2005:
        CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
        CDockingManager::SetDockingMode(DT_SMART);
        break;

    default:
        switch (theApp.m_nAppLook)
        {
        case ID_VIEW_APPLOOK_OFF_2007_BLUE:
            CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
            break;

        case ID_VIEW_APPLOOK_OFF_2007_BLACK:
            CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
            break;

        case ID_VIEW_APPLOOK_OFF_2007_SILVER:
            CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
            break;

        case ID_VIEW_APPLOOK_OFF_2007_AQUA:
            CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
            break;
        }

        CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
        CDockingManager::SetDockingMode(DT_SMART);
    }

    RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

    theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

VOID CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
    pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}

VOID CMainFrame::OnViewCaptionBar()
{
    m_wndCaptionBar.ShowWindow(m_wndCaptionBar.IsVisible() ? SW_HIDE : SW_SHOW);
    RecalcLayout(FALSE);
}

VOID CMainFrame::OnUpdateViewCaptionBar(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(m_wndCaptionBar.IsVisible());
}