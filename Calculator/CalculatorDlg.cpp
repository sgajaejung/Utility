
// CalculatorDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "Calculator.h"
#include "CalculatorDlg.h"
#include "afxdialogex.h"
#include <MMSystem.h>
#include "D3DView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif



CCalculatorDlg::CCalculatorDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCalculatorDlg::IDD, pParent)
,	m_loop(true)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CCalculatorDlg::~CCalculatorDlg()
{
	graphic::ReleaseRenderer();
}

void CCalculatorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_SOURCE, m_EditSource);
	DDX_Control(pDX, IDC_BUTTON_APPLY, m_applyButton);
}

BEGIN_MESSAGE_MAP(CCalculatorDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CCalculatorDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CCalculatorDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, &CCalculatorDlg::OnBnClickedButtonApply)
END_MESSAGE_MAP()


// CCalculatorDlg 메시지 처리기

BOOL CCalculatorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
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

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	MoveWindow(CRect(0,0,REAL_WINDOW_WIDTH,REAL_WINDOW_HEIGHT));

	// Create Main Model View
	m_view = new CD3DView();
	m_view->Create(NULL, _T("CView"), WS_CHILDWINDOW, 
		CRect(0,0, VIEW_WIDTH, VIEW_HEIGHT), this, 0);

	// Create Direct
	graphic::cRenderer::Get()->CreateDirectX(
		m_view->GetSafeHwnd(), VIEW_WIDTH, VIEW_HEIGHT);
	
	m_view->Init();
	m_view->ShowWindow(SW_SHOW);

	m_EditSource.MoveWindow(CRect(0,REAL_WINDOW_HEIGHT-135,REAL_WINDOW_WIDTH-20, REAL_WINDOW_HEIGHT));
	m_applyButton.MoveWindow(CRect(REAL_WINDOW_WIDTH-200,REAL_WINDOW_HEIGHT-155,
		REAL_WINDOW_WIDTH-20, REAL_WINDOW_HEIGHT-135));

	CString source = L"// T(x,y,z), S(sx,sy,sz), Rx(angle), Ry(angle), Rz(angle) \r\n\
VOut = V(100,0,100) \r\n\
MOut = T(0,50,0) * S(1,2,1) * Rx(0.2) * Ry(0.6)";

	m_EditSource.SetWindowText( source );

	m_view->ParseSource( wstr2str((wstring)source) );

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CCalculatorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CCalculatorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CCalculatorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CCalculatorDlg::MainLoop()
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


void CCalculatorDlg::OnBnClickedOk()
{
}


void CCalculatorDlg::OnBnClickedCancel()
{
	m_loop = false;
	CDialogEx::OnCancel();
}


void CCalculatorDlg::OnBnClickedButtonApply()
{
	CString wstr;
	m_EditSource.GetWindowText(wstr);
	string str = wstr2str((wstring)wstr);
	m_view->ParseSource(str);
}
