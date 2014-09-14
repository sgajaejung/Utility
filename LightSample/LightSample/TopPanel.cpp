// TopPanel.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "TopPanel.h"
#include "LightPanel.h"


// CTopPanel
CTopPanel::CTopPanel()
{

}

CTopPanel::~CTopPanel()
{
}


BEGIN_MESSAGE_MAP(CTopPanel, CMiniFrameWnd)
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
