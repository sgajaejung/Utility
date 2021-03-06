#pragma once


namespace graphic
{

	namespace TERRAIN_EDIT_MODE 
	{
		enum TYPE
		{
			NONE,
			UP,
			DOWN,
		};
	};


	class cTerrainCursor
	{
		enum {
			CURSOR_VERTEX_COUNT = 64,
		};


	public:
		cTerrainCursor(void);
		virtual ~cTerrainCursor(void);

		void RenderBrush();
		void RenderModel();
		void RenderTerrainBrush();
		void UpdateCursor( graphic::cTerrain &terrain, const Vector3 &cursorPos);

		float GetInnerBrushRadius() const;
		float GetOuterBrushRadius() const;
		float GetInnerBrushAlpha() const;
		void SetInnerBrushRadius(float radius);
		void SetOuterBrushRadius(float radius);
		void SetInnerBrushAlpha(float alpha);
		const Vector3& GetCursorPos() const;

		// terrain
		void SetTerrainEditMode(const TERRAIN_EDIT_MODE::TYPE type);
		TERRAIN_EDIT_MODE::TYPE GetTerrainEditMode() const;
		void SetBrushSpeed(const float offset);
		float GetBrushSpeed() const;

		// brush
		void EnableEraseMode(const bool erase);
		bool IsEraseMode() const;
		void SelectBrushTexture(const string &fileName );
		const cTexture* GetBrushTexture() const;

		// model
		void SelectModel(const string &fileName);
		void SelectModel(cModel *model);
		const cModel* GetSelectModel() const;
		void CancelSelectModel();
		bool IsSelectModel() const;


	private:
		Vector3 m_pos;
		
		// terrain edit
		TERRAIN_EDIT_MODE::TYPE m_editMode;
		float m_brushSpeed; // 지형 높낮이 조절 속도 값. (defalut: 20)

		// brush
		bool m_isEraseMode;
		float m_innerRadius;
		float m_outerRadius;
		float m_innerAlpha;
		cVertexBuffer m_innerCircle;
		cVertexBuffer m_outerCircle;
		cTexture *m_brushTexture; // reference

		// model
		cModel *m_selectModel;
	};


	inline bool cTerrainCursor::IsEraseMode() const { return m_isEraseMode; }
	inline float cTerrainCursor::GetInnerBrushRadius() const { return m_innerRadius; }
	inline float cTerrainCursor::GetOuterBrushRadius() const { return m_outerRadius; }
	inline void cTerrainCursor::SetInnerBrushRadius(float radius) { m_innerRadius = radius; }
	inline void cTerrainCursor::SetOuterBrushRadius(float radius) { m_outerRadius = radius; }
	inline float cTerrainCursor::GetInnerBrushAlpha() const { return m_innerAlpha; }
	inline void cTerrainCursor::SetInnerBrushAlpha(float alpha) { m_innerAlpha = alpha; }
	inline const cTexture* cTerrainCursor::GetBrushTexture() const { return m_brushTexture; }
	inline const Vector3& cTerrainCursor::GetCursorPos() const { return m_pos; }
	inline const cModel* cTerrainCursor::GetSelectModel() const { return m_selectModel; }
	inline bool cTerrainCursor::IsSelectModel() const { return m_selectModel? true : false; }
	inline void cTerrainCursor::SelectModel(cModel *model) { m_selectModel = model; }
	inline void cTerrainCursor::SetTerrainEditMode(const TERRAIN_EDIT_MODE::TYPE type) { m_editMode = type; }
	inline TERRAIN_EDIT_MODE::TYPE cTerrainCursor::GetTerrainEditMode() const { return m_editMode; }
	inline void cTerrainCursor::SetBrushSpeed(const float offset) { m_brushSpeed = offset; }
	inline float cTerrainCursor::GetBrushSpeed() const { return m_brushSpeed; }
}
