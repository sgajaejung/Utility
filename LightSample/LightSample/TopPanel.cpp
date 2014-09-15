// TopPanel.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "TopPanel.h"
#include "LightPanel.h"


// CTopPanel
CTopPanel::CTopPanel()
:	m_lightPanel(NULL)
{

}

CTopPanel::~CTopPanel()
{
}


BEGIN_MESSAGE_MAP(CTopPanel, CMiniFrameWnd)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CTopPanel �޽��� ó�����Դϴ�.
void CTopPanel::Init()
{
	CRect cr;
	GetClientRect(cr);

	m_lightPanel = new CLightPanel(this);
	m_lightPanel->Create( CLightPanel::IDD, this );
	m_lightPanel->MoveWindow(cr);
	m_lightPanel->ShowWindow(SW_SHOW);
}


void CTopPanel::OnSize(UINT nType, int cx, int cy)
{
	CMiniFrameWnd::OnSize(nType, cx, cy);
	if (m_lightPanel)
		m_lightPanel->MoveWindow(0, 0, cx, cy);
}
