//
//
//  ���� ���� ����Ʈ
// http://www.directxtutorial.com/Lesson.aspx?lessonid=9-4-9
//

#include "stdafx.h"
#include "resource.h"		// �� ��ȣ�Դϴ�.
#include "D3DView.h"
#include "TopPanel.h"
#include "Controller.h"
#include <MMSystem.h>


class CLightSampleApp : public CWinApp
{
public:
	CLightSampleApp();
	virtual BOOL InitInstance();
	DECLARE_MESSAGE_MAP()
};

extern CLightSampleApp theApp;


// CLightSampleDlg ��ȭ ����
class CLightSampleDlg : public CDialogEx
{
public:
	CLightSampleDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

	enum { IDD = IDD_LIGHTSAMPLE_DIALOG };

	void MainLoop();


protected:
	HICON m_hIcon;
	CD3DView *m_view;
	bool m_loop;

	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};




#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CLightSampleApp

BEGIN_MESSAGE_MAP(CLightSampleApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CLightSampleApp ����

CLightSampleApp::CLightSampleApp()
{
	// �ٽ� ���� ������ ����
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
}

CLightSampleApp theApp;


// CLightSampleApp �ʱ�ȭ

BOOL CLightSampleApp::InitInstance()
{
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();
	AfxEnableControlContainer();

	CLightSampleDlg *dlg = new CLightSampleDlg();
	dlg->Create(CLightSampleDlg::IDD);
	m_pMainWnd = dlg;

	dlg->ShowWindow(SW_SHOW);
	dlg->MainLoop();

	delete dlg;
	return FALSE;
}




// CLightSampleDlg ��ȭ ����

CLightSampleDlg::CLightSampleDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLightSampleDlg::IDD, pParent)
,	m_loop(true)
,	m_view(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLightSampleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CLightSampleDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CLightSampleDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CLightSampleDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CLightSampleDlg �޽��� ó����

BOOL CLightSampleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.



	MoveWindow(CRect(0,0,REAL_WINDOW_WIDTH,REAL_WINDOW_HEIGHT));

	// Create Main Model View
	m_view = new CD3DView();
	m_view->Create(NULL, _T("CView"), WS_CHILDWINDOW, 
		CRect(0,0, VIEW_WIDTH, VIEW_HEIGHT), this, 0);

	// Create Direct
	graphic::cRenderer::Get()->CreateDirectX(
		m_view->GetSafeHwnd(), VIEW_WIDTH, VIEW_HEIGHT);

	m_view->Init();
	cController::Get()->Init();
	m_view->ShowWindow(SW_SHOW);


	// TopPanel ����.
	{
		const int PANEL_WIDTH = 420;
		const int PANEL_HEIGHT = 600;

		CTopPanel *dlg = new CTopPanel();
		const CString StrClassName = AfxRegisterWndClass( CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,
			LoadCursor(NULL, IDC_ARROW), (HBRUSH)GetStockObject(COLOR_BTNFACE+1), 
			LoadIcon(NULL, IDI_APPLICATION) );

		dlg->CreateEx(0, StrClassName, L"Top Panel", 
			WS_POPUP | WS_CAPTION | WS_SYSMENU | MFS_THICKFRAME, 
			CRect(0, 0, PANEL_WIDTH, PANEL_HEIGHT), this );

		dlg->Init();

		// TopPanel Positioning
		{
			CRect panelR;
			dlg->GetWindowRect(panelR);

			const int screenCX = GetSystemMetrics(SM_CXSCREEN);
			const int screenCY = GetSystemMetrics(SM_CYSCREEN);
			int x = screenCX/2 - REAL_WINDOW_WIDTH/2 + REAL_WINDOW_WIDTH - panelR.Width()/2;
			const int y = screenCY/2 - REAL_WINDOW_HEIGHT/2;

			if ((x+panelR.Width()) > screenCX)
				x = screenCX - panelR.Width();

			dlg->MoveWindow(x, y, panelR.Width(), panelR.Height());


			// Main Dialog RePositioning
			int px = screenCX/2 - REAL_WINDOW_WIDTH/2 - panelR.Width()/2;
			px = max(0, px);
			MoveWindow(px, y, REAL_WINDOW_WIDTH,REAL_WINDOW_HEIGHT);
		}

		dlg->ShowWindow(SW_SHOW);
	}


	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void CLightSampleDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}


void CLightSampleDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CLightSampleDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CLightSampleDlg::OnBnClickedOk()
{
	CDialogEx::OnOK();
}


void CLightSampleDlg::OnBnClickedCancel()
{
	m_loop = false;
	CDialogEx::OnCancel();
}


void CLightSampleDlg::MainLoop()
{
	while (m_loop)
	{
		MSG msg;
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			if (!GetMessage(&msg, NULL, 0, 0)) 
			{
				break;
			}
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}

		const int curT = timeGetTime();
		static int oldT = curT;
		const int elapseT = curT - oldT;
		const float t = elapseT * 0.001f;
		oldT = curT;

		if (m_view)
		{
			m_view->Update(t);
			m_view->Render();
		}

		Sleep(0);
	}
}
