// D3DView.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Calculator.h"
#include "D3DView.h"


// CD3DView
CD3DView::CD3DView() :
	m_LButtonDown(false),
	m_RButtonDown(false),
	m_MButtonDown(false)
{

}

CD3DView::~CD3DView()
{
}


BEGIN_MESSAGE_MAP(CD3DView, CWnd)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()



// CD3DView �޽��� ó�����Դϴ�.


bool CD3DView::Init()
{
	m_camera.SetCamera(Vector3(100,100,-500), Vector3(0,0,0), Vector3(0,1,0));
	m_camera.SetProjection( D3DX_PI / 4.f, (float)VIEW_WIDTH / (float) VIEW_HEIGHT, 1.f, 10000.0f);

	m_cube.SetCube(Vector3(-10,-10,-10), Vector3(10,10,10));


	return true;
}


void CD3DView::Render()
{
	//ȭ�� û��
	if (SUCCEEDED(graphic::GetDevice()->Clear( 
		0,			//û���� ������ D3DRECT �迭 ����		( ��ü Ŭ���� 0 )
		NULL,		//û���� ������ D3DRECT �迭 ������		( ��ü Ŭ���� NULL )
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,	//û�ҵ� ���� �÷��� ( D3DCLEAR_TARGET �÷�����, D3DCLEAR_ZBUFFER ���̹���, D3DCLEAR_STENCIL ���ٽǹ���
		D3DCOLOR_XRGB(150, 150, 150),			//�÷����۸� û���ϰ� ä���� ����( 0xAARRGGBB )
		1.0f,				//���̹��۸� û���Ұ� ( 0 ~ 1 0 �� ī�޶󿡼� ���ϰ���� 1 �� ī�޶󿡼� ���� �� )
		0					//���ٽ� ���۸� ä�ﰪ
		)))
	{
		//ȭ�� û�Ұ� ���������� �̷�� ���ٸ�... ������ ����
		graphic::GetDevice()->BeginScene();
		graphic::GetRenderer()->RenderFPS();
		graphic::GetRenderer()->RenderGrid();
		graphic::GetRenderer()->RenderAxis();

		Matrix44 matIdentity;
		m_cube.Render(matIdentity);


		//������ ��
		graphic::GetDevice()->EndScene();
		//�������� �������� �������� ���� ȭ������ ����
		graphic::GetDevice()->Present( NULL, NULL, NULL, NULL );
	}

}


void CD3DView::Update(float elapseT)
{
	graphic::GetRenderer()->Update(elapseT);

}


void CD3DView::OnLButtonDown(UINT nFlags, CPoint point)
{

	CWnd::OnLButtonDown(nFlags, point);
}


void CD3DView::OnLButtonUp(UINT nFlags, CPoint point)
{

	CWnd::OnLButtonUp(nFlags, point);
}


void CD3DView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_RButtonDown)
	{
		CPoint pos = point - m_curPos;
		m_curPos = point;
		m_camera.Pitch2(pos.y * -0.005f); 
		m_camera.Yaw2(pos.x * -0.005f); 
	}

	CWnd::OnMouseMove(nFlags, point);
}


void CD3DView::OnRButtonDown(UINT nFlags, CPoint point)
{
	SetFocus();
	SetCapture();
	m_RButtonDown = true;
	m_curPos = point;
	CWnd::OnRButtonDown(nFlags, point);
}


void CD3DView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();
	m_RButtonDown = false;
	CWnd::OnRButtonUp(nFlags, point);
}
