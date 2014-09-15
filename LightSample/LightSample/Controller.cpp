
#include "stdafx.h"
#include "controller.h"


using namespace graphic;


cController::cController()
: m_selectLight(0)
{
	m_lights.resize(3);

	m_lights[ 0].Init(cLight::LIGHT_DIRECTIONAL);
	m_lights[ 0].SetPosition(Vector3(0,300,0));
	m_lights[ 0].m_light.Range = 200;

	m_lights[ 1].Init(cLight::LIGHT_POINT);
	m_lights[ 1].SetPosition(Vector3(0,100,0));
	m_lights[ 1].m_light.Range = 380;
	m_lights[ 1].m_light.Attenuation0 = 0;
	m_lights[ 1].m_light.Attenuation1 = 0.006f;
	m_lights[ 1].m_light.Attenuation2 = 0;
	
	m_lights[ 2].Init(cLight::LIGHT_SPOT);
	m_lights[ 2].SetPosition(Vector3(0,100,0));
	m_lights[ 2].m_light.Range = 500;
	m_lights[ 2].m_light.Falloff = 1.f;
	m_lights[ 2].m_light.Theta = 1.25f;
	m_lights[ 2].m_light.Phi = 3.14f;
	m_lights[ 2].m_light.Attenuation0 = 0;
	m_lights[ 2].m_light.Attenuation1 = 0.006f;
	m_lights[ 2].m_light.Attenuation2 = 0;
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


	m_sphereMtrl.InitBlack();
	m_sphereMtrl.GetMtrl().Emissive = *(D3DCOLORVALUE*)&Vector4(0,0,1,1);
	m_sphere.Create(5, 10, 10);

	m_grid.Create(128, 128, 10);

}


void cController::Render()
{
	GetDevice()->LightEnable(0, TRUE);
	GetDevice()->SetRenderState( D3DRS_LIGHTING, TRUE );

	m_lights[ m_selectLight].Bind(0);

	m_cube.Render(Matrix44::Identity);

	m_sphereMtrl.Bind();
	m_sphere.Render(Matrix44::Identity);
	
	GetDevice()->SetTransform( D3DTS_WORLD, (D3DXMATRIX*)&Matrix44::Identity);
	m_grid.Render();

}


void cController::Update(const float elapseTime)
{
	static float angle = 0;
	angle += elapseTime;

	Matrix44 t;
	t.SetTranslate(Vector3(0,20,0));
	Matrix44 rx;
	rx.SetRotationX(angle);

	m_cube.SetTransform(rx * t);


	Matrix44 lightTm;
	lightTm.SetTranslate( *(Vector3*)&m_lights[ m_selectLight].m_light.Position );
	m_sphere.SetTransform(lightTm);
}

