//
//  ���� ���� ����Ʈ
// http://www.directxtutorial.com/Lesson.aspx?lessonid=9-4-9
//

#include "stdafx.h"
#include "D3DView.h"


// CD3DView
CD3DView::CD3DView():
	m_LButtonDown(false),
	m_RButtonDown(false),
	m_MButtonDown(false)
{

}

CD3DView::~CD3DView()
{
}

BEGIN_MESSAGE_MAP(CD3DView, CView)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_MOUSEWHEEL()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()


// CD3DView �׸����Դϴ�.

void CD3DView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: ���⿡ �׸��� �ڵ带 �߰��մϴ�.
}


// CD3DView �����Դϴ�.

#ifdef _DEBUG
void CD3DView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CD3DView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CD3DView �޽��� ó�����Դϴ�.


bool CD3DView::Init()
{
	m_camera.SetCamera(Vector3(100,100,-500), Vector3(0,0,0), Vector3(0,1,0));
	m_camera.SetProjection( D3DX_PI / 4.f, (float)VIEW_WIDTH / (float) VIEW_HEIGHT, 1.f, 10000.0f);

	m_lineMtrl.InitBlack();
	m_lineMtrl.GetMtrl().Emissive = *(D3DXCOLOR*)&Vector4(1,1,0,1);

	SetFocus();
	return true;
}


void CD3DView::Render()
{
	//ȭ�� û��
	if (SUCCEEDED(graphic::GetDevice()->Clear( 
		0,			//û���� ������ D3DRECT �迭 ����		( ��ü Ŭ���� 0 )
		NULL,		//û���� ������ D3DRECT �迭 ������		( ��ü Ŭ���� NULL )
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,	//û�ҵ� ���� �÷��� ( D3DCLEAR_TARGET �÷�����, D3DCLEAR_ZBUFFER ���̹���, D3DCLEAR_STENCIL ���ٽǹ���
		D3DCOLOR_XRGB(0, 0, 0),			//�÷����۸� û���ϰ� ä���� ����( 0xAARRGGBB )
		1.0f,				//���̹��۸� û���Ұ� ( 0 ~ 1 0 �� ī�޶󿡼� ���ϰ���� 1 �� ī�޶󿡼� ���� �� )
		0					//���ٽ� ���۸� ä�ﰪ
		)))
	{
		//ȭ�� û�Ұ� ���������� �̷�� ���ٸ�... ������ ����
		graphic::GetDevice()->BeginScene();
		graphic::GetRenderer()->RenderFPS();

		cController::Get()->Render();

		if (m_LButtonDown)
		{
			m_lineMtrl.Bind();
			m_lightLine.Render();
		}

		//������ ��
		graphic::GetDevice()->EndScene();
		//�������� �������� �������� ���� ȭ������ ����
		graphic::GetDevice()->Present( NULL, NULL, NULL, NULL );
	}

}


void CD3DView::Update(float elapseT)
{
	graphic::GetRenderer()->Update(elapseT);
	cController::Get()->Update(elapseT);
}


void CD3DView::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetFocus();
	SetCapture();
	m_LButtonDown = true;
	m_curPos = point;
	CView::OnLButtonDown(nFlags, point);
}


void CD3DView::OnLButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();
	m_LButtonDown = false;
	CView::OnLButtonUp(nFlags, point);
}


void CD3DView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_LButtonDown)
	{
		Ray ray(point.x, point.y, 800, 600, m_camera.GetProjectionMatrix(), m_camera.GetViewMatrix());
		Vector3 pickPos;
		if (cController::Get()->GetGrid().Pick( ray.orig, ray.dir, pickPos ))
		{
			const Vector3 lightPos = *(Vector3*)&cController::Get()->GetSelectLight().m_light.Position;
			Vector3 dir = pickPos - lightPos;
			dir.Normalize();
			m_lightLine.SetLine(lightPos, pickPos, 1);

			cController::Get()->UpdateLightDirection(dir);
		}
	}
	else if (m_RButtonDown)
	{
		CPoint pos = point - m_curPos;
		m_curPos = point;
		m_camera.Pitch2(pos.y * 0.005f); 
		m_camera.Yaw2(pos.x * 0.005f); 
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

	CView::OnMouseMove(nFlags, point);
}


void CD3DView::OnRButtonDown(UINT nFlags, CPoint point)
{
	SetFocus();
	SetCapture();
	m_RButtonDown = true;
	m_curPos = point;
	CView::OnRButtonDown(nFlags, point);
}


void CD3DView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();
	m_RButtonDown = false;
	CView::OnRButtonUp(nFlags, point);
}


BOOL CD3DView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	const float len = m_camera.GetDistance();
	float zoomLen = (len > 100)? 50 : (len/4.f);
	if (nFlags & 0x4)
		zoomLen = zoomLen/10.f;

	m_camera.Zoom( (zDelta<0)? -zoomLen : zoomLen );	

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


void CD3DView::OnMButtonUp(UINT nFlags, CPoint point)
{
	m_MButtonDown = false;
	ReleaseCapture();
	CView::OnMButtonUp(nFlags, point);
}


void CD3DView::OnMButtonDown(UINT nFlags, CPoint point)
{
	SetFocus();
	SetCapture();
	m_MButtonDown = true;
	CView::OnMButtonDown(nFlags, point);
}


void CD3DView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
	case VK_TAB:
		{
			static bool flag = false;
			graphic::GetDevice()->SetRenderState(D3DRS_CULLMODE, flag? D3DCULL_CCW : D3DCULL_NONE);
			graphic::GetDevice()->SetRenderState(D3DRS_FILLMODE, flag? D3DFILL_SOLID : D3DFILL_WIREFRAME);
			flag = !flag;
		}
		break;
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CD3DView::Update(int type)
{
	// ���� �ƹ��ϵ� ����.
}
