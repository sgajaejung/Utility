// TabPanel.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TabPanel.h"
#include "afxdialogex.h"


// CTabPanel 대화 상자입니다.
CTabPanel::CTabPanel(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTabPanel::IDD, pParent)
{

}

CTabPanel::~CTabPanel()
{
}

void CTabPanel::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_tab);
}


BEGIN_MESSAGE_MAP(CTabPanel, CDialogEx)
	ON_BN_CLICKED(IDOK, &CTabPanel::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CTabPanel::OnBnClickedCancel)
END_MESSAGE_MAP()


// CTabPanel 메시지 처리기입니다.


void CTabPanel::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnOK();
}


void CTabPanel::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnCancel();
}


BOOL CTabPanel::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	return TRUE;
}
