// 전역 컨트롤 클래스

#pragma once
#include "../../graphic/base/cube2.h"


class cController : public common::cSingleton<cController>
{
public:
	cController();
	virtual ~cController();

	vector<graphic::cLight>& GetLights();
	graphic::cLight& GetLight(const int index);
	graphic::cCube2& GetCube();

	void Init();
	void Render();
	void Update(const float elapseTime);


protected:


private:
	graphic::cCube2 m_cube;
	graphic::cGrid2 m_grid;

	vector<graphic::cLight> m_lights;
};


inline vector<graphic::cLight>& cController::GetLights() { return m_lights; }
inline graphic::cLight& cController::GetLight(const int index) { return m_lights[ index]; }
inline graphic::cCube2& cController::GetCube() { return m_cube; }
