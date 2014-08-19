#pragma once


namespace graphic
{

	// OBB �浹�ڽ�.
	class cBoundingBox
	{
	public:
		cBoundingBox();
		cBoundingBox(const cCube &cube);
		void SetBoundingBox(const Vector3 &vMin, const Vector3 &vMax );
		void SetTransform( const Matrix44 &tm );
		bool Collision( cBoundingBox &box );
		bool Pick(const Vector3 &orig, const Vector3 &dir);

		cBoundingBox& operator=(const cCube &cube);


		Vector3 m_min;
		Vector3 m_max;
		Matrix44 m_tm;
	};

}
