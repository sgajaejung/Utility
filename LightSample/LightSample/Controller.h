// 전역 컨트롤 클래스

#pragma once
#include "../../graphic/base/cube2.h"


class cController : public common::cSingleton<cController>
{
public:
	cController();
	virtual ~cController();

	void SelectLight(const int lightIndex);
	vector<graphic::cLight>& GetLights();
	graphic::cLight& GetLight(const int index);
	graphic::cLight& GetSelectLight();
	graphic::cCube2& GetCube();
	graphic::cGrid2& GetGrid();

	void Init();
	void Render();
	void Update(const float elapseTime);


private:
	graphic::cCube2 m_cube;
	graphic::cGrid2 m_grid;
	graphic::cSphere m_sphere;
	graphic::cMaterial m_sphereMtrl;

	int m_selectLight;
	vector<graphic::cLight> m_lights;
};


inline vector<graphic::cLight>& cController::GetLights() { return m_lights; }
inline graphic::cLight& cController::GetLight(const int index) { return m_lights[ index]; }
inline graphic::cCube2& cController::GetCube() { return m_cube; }
inline void cController::SelectLight(const int lightIndex) { m_selectLight = lightIndex; }
inline graphic::cGrid2& cController::GetGrid() { return m_grid; }
inline graphic::cLight& cController::GetSelectLight() { return m_lights[ m_selectLight]; }
