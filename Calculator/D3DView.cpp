// D3DView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Calculator.h"
#include "D3DView.h"
#include "parser/ExpressionParser.h"


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
	ON_WM_KEYDOWN()
	ON_WM_MOUSEWHEEL()
	ON_WM_MBUTTONUP()
	ON_WM_MBUTTONDOWN()
END_MESSAGE_MAP()



// CD3DView 메시지 처리기입니다.


bool CD3DView::Init()
{
	m_camera.SetCamera(Vector3(100,100,-500), Vector3(0,0,0), Vector3(0,1,0));
	m_camera.SetProjection( D3DX_PI / 4.f, (float)VIEW_WIDTH / (float) VIEW_HEIGHT, 1.f, 10000.0f);

	m_cube.SetCube(Vector3(-10,-10,-10), Vector3(10,10,10));

	SetFocus();
	return true;
}


void CD3DView::Render()
{
	//화면 청소
	if (SUCCEEDED(graphic::GetDevice()->Clear( 
		0,			//청소할 영역의 D3DRECT 배열 갯수		( 전체 클리어 0 )
		NULL,		//청소할 영역의 D3DRECT 배열 포인터		( 전체 클리어 NULL )
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,	//청소될 버퍼 플레그 ( D3DCLEAR_TARGET 컬러버퍼, D3DCLEAR_ZBUFFER 깊이버퍼, D3DCLEAR_STENCIL 스텐실버퍼
		D3DCOLOR_XRGB(150, 150, 150),			//컬러버퍼를 청소하고 채워질 색상( 0xAARRGGBB )
		1.0f,				//깊이버퍼를 청소할값 ( 0 ~ 1 0 이 카메라에서 제일가까운 1 이 카메라에서 제일 먼 )
		0					//스텐실 버퍼를 채울값
		)))
	{
		//화면 청소가 성공적으로 이루어 졌다면... 랜더링 시작
		graphic::GetDevice()->BeginScene();
		graphic::GetRenderer()->RenderFPS();
		graphic::GetRenderer()->RenderGrid();
		graphic::GetRenderer()->RenderAxis();

		Matrix44 matIdentity;
		m_cube.Render(matIdentity);


		//랜더링 끝
		graphic::GetDevice()->EndScene();
		//랜더링이 끝났으면 랜더링된 내용 화면으로 전송
		graphic::GetDevice()->Present( NULL, NULL, NULL, NULL );
	}

}


void CD3DView::Update(float elapseT)
{
	graphic::GetRenderer()->Update(elapseT);

}


void CD3DView::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetFocus();
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
	else if (m_MButtonDown)
	{
		CPoint pos = point - m_curPos;
		m_curPos = point;

		Vector3 dir = m_camera.GetDirection();
		dir.y = 0;
		dir.Normalize();

		const float len = m_camera.GetDistance();
		m_camera.MoveRight( -pos.x * len * 0.001f );
		m_camera.MoveAxis( dir, pos.y * len * 0.001f );
	}
	else
	{
		m_curPos = point;
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


BOOL CD3DView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	const float len = m_camera.GetDistance();
	float zoomLen = (len > 100)? 50 : (len/4.f);
	if (nFlags & 0x4)
		zoomLen = zoomLen/10.f;

	m_camera.Zoom( (zDelta<0)? -zoomLen : zoomLen );	

	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}


void CD3DView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CD3DView::OnMButtonUp(UINT nFlags, CPoint point)
{
	m_MButtonDown = false;
	ReleaseCapture();
	CWnd::OnMButtonUp(nFlags, point);
}


void CD3DView::OnMButtonDown(UINT nFlags, CPoint point)
{
	SetFocus();
	SetCapture();
	m_MButtonDown = true;
	CWnd::OnMButtonDown(nFlags, point);
}


void CD3DView::ParseSource( const string &source )
{
	parser::CExpressionParser ps;
	ps.Parse((BYTE*)source.c_str(), source.length() );
	m_cube.SetTransform( ps.m_mat );
}
