// 전역 컨트롤 클래스

#pragma once
#include "../../graphic/base/cube2.h"


class cController : public common::cSingleton<cController>
						, public common::cObservable2
{
public:
	cController();
	virtual ~cController();

	int GetSelectModel();
	void SetSelectModel(const int index);
	void SelectLight(const int lightIndex);
	void EnableSpecularEffect(const bool enable);
	vector<graphic::cLight>& GetLights();
	graphic::cLight& GetLight(const int index);
	graphic::cLight& GetSelectLight();
	graphic::cCube2& GetCube();
	graphic::cSphere& GetSphere();
	graphic::cGrid2& GetGrid();


	void Init();
	void Render();
	void Update(const float elapseTime);

	void UpdateLightDirection(const Vector3 &direction);


private:
	int m_selectModel;
	graphic::cCube2 m_cube;
	graphic::cSphere m_sphere;

	graphic::cGrid2 m_grid;
	graphic::cSphere m_lightSphere;

	int m_selectLight;
	vector<graphic::cLight> m_lights;

	bool m_enableSpecularEffect;
};


inline vector<graphic::cLight>& cController::GetLights() { return m_lights; }
inline graphic::cLight& cController::GetLight(const int index) { return m_lights[ index]; }
inline graphic::cCube2& cController::GetCube() { return m_cube; }
inline void cController::SelectLight(const int lightIndex) { m_selectLight = lightIndex; }
inline graphic::cGrid2& cController::GetGrid() { return m_grid; }
inline graphic::cLight& cController::GetSelectLight() { return m_lights[ m_selectLight]; }
inline graphic::cSphere& cController::GetSphere() { return m_sphere; }
inline int cController::GetSelectModel() { return m_selectModel; }
inline void cController::SetSelectModel(const int index) { m_selectModel = index; }
inline void cController::EnableSpecularEffect(const bool enable) { m_enableSpecularEffect = enable; }
