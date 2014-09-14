
#include "stdafx.h"
#include "light.h"

using namespace graphic;

cLight::cLight()
{

}


cLight::~cLight()
{

}


void cLight::Init(TYPE type, 
	const Vector4 &ambient, // Vector4(1, 1,13, 1),
	const Vector4 &diffuse, // Vector4(0.2, 0.2, 0.2, 1)
	const Vector4 &specular, // Vector4(1,1,1,1)
	const Vector3 &direction) // Vector3(0,-1,0)
{
	ZeroMemory(&m_light, sizeof(m_light));
	m_light.Type = (D3DLIGHTTYPE)type;
	m_light.Ambient = *(D3DCOLORVALUE*)&ambient;
	m_light.Diffuse = *(D3DCOLORVALUE*)&diffuse;
	m_light.Specular = *(D3DCOLORVALUE*)&specular;
	m_light.Direction = *(D3DXVECTOR3*)&direction;
}


void cLight::Bind(int lightIndex)
{
	GetDevice()->SetLight(lightIndex, &m_light); // ���� ����.
}


void cLight::SetDirection( const Vector3 &direction )
{
	m_light.Direction = *(D3DXVECTOR3*)&direction;
}


void cLight::SetPosition( const Vector3 &pos )
{
	m_light.Position = *(D3DXVECTOR3*)&pos;
}


// �׸��ڸ� ����ϱ� ���� ������ �����Ѵ�.
// modelPos : �׸��ڸ� ����� ���� ��ġ (�������)
// lightPos : ������ ��ġ�� ����Ǿ� ����.
// view : �������� ���� �ٶ󺸴� �� ���
// proj : �������� ���� �ٶ󺸴� ���� ���
// tt : ���� ��ǥ���� �ؽ��� ��ǥ�� ��ȯ�ϴ� ���.
void cLight::GetShadowMatrix( const Vector3 &modelPos, 
	OUT Vector3 &lightPos, OUT Matrix44 &view, OUT Matrix44 &proj, 
	OUT Matrix44 &tt )
{
	lightPos = *(Vector3*)&m_light.Position;

	view.SetView2( lightPos, modelPos, Vector3(0,1,0));

	proj.SetProjection( D3DX_PI/8.f, 1, 0.1f, 10000);

	D3DXMATRIX mTT= D3DXMATRIX(0.5f, 0.0f, 0.0f, 0.0f
		, 0.0f,-0.5f, 0.0f, 0.0f
		, 0.0f, 0.0f, 1.0f, 0.0f
		, 0.5f, 0.5f, 0.0f, 1.0f);
	tt = *(Matrix44*)&mTT;
}

