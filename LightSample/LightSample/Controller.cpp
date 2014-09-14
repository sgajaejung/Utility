
#include "stdafx.h"
#include "controller.h"


using namespace graphic;


cController::cController()
{
	m_lights.resize(3);

	m_lights[ 0].Init(cLight::LIGHT_DIRECTIONAL);
	m_lights[ 1].Init(cLight::LIGHT_POINT);
	m_lights[ 2].Init(cLight::LIGHT_SPOT);


}

cController::~cController()
{

}


void cController::Init()
{
	graphic::GetDevice()->LightEnable(0, TRUE);
	graphic::GetDevice()->SetRenderState( D3DRS_LIGHTING, TRUE );

	m_cube.SetCube(Vector3(-10,-10,-10), Vector3(10,10,10));

}


void cController::Render()
{
	m_lights[ 0].Bind(0);

	m_cube.Render(Matrix44::Identity);

}


void cController::Update(const float elapseTime)
{

}

