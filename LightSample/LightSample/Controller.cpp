
#include "stdafx.h"
#include "controller.h"


using namespace graphic;


cController::cController()
{
	m_lights.resize(3);

	m_lights[ 0].Init(cLight::LIGHT_DIRECTIONAL);
	m_lights[ 0].SetPosition(Vector3(0,100,0));
	m_lights[ 0].m_light.Range = 200;

	m_lights[ 1].Init(cLight::LIGHT_POINT);
	m_lights[ 1].SetPosition(Vector3(0,100,0));
	m_lights[ 1].m_light.Range = 200;
	
	m_lights[ 2].Init(cLight::LIGHT_SPOT);
	m_lights[ 2].SetPosition(Vector3(0,100,0));


}

cController::~cController()
{

}


void cController::Init()
{
	GetDevice()->LightEnable(0, TRUE);
	GetDevice()->SetRenderState( D3DRS_LIGHTING, TRUE );

	m_cube.SetCube(Vector3(-10,-10,-10), Vector3(10,10,10));
	Matrix44 T;
	T.SetTranslate(Vector3(0,20,0));
	m_cube.SetTransform(T);
	m_cube.GetMaterial().InitRed();

	m_grid.Create(128, 128, 10);

}


void cController::Render()
{
	GetDevice()->LightEnable(0, TRUE);
	GetDevice()->SetRenderState( D3DRS_LIGHTING, TRUE );

	m_lights[ 0].Bind(0);

	m_cube.Render(Matrix44::Identity);

	GetDevice()->SetTransform( D3DTS_WORLD, (D3DXMATRIX*)&Matrix44::Identity);
	m_grid.Render();

}


void cController::Update(const float elapseTime)
{
	Matrix44 rx;
	rx.SetRotationX(elapseTime * 1.f);
	m_cube.SetTransform(m_cube.GetTransform() * rx);
}

