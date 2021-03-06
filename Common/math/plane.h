#pragma once


namespace common
{

	struct Plane
	{
		Plane();
		Plane( float a, float b, float c, float d );
		Plane( Vector3& vN, float d );
		Plane( const Vector3& vA, const Vector3& vB, const Vector3& vC );

		void Init( Vector3& vA, Vector3& vB, Vector3& vC );
		float Distance( Vector3& vP ) const;
		Vector3 Pick( Vector3& vOrig, Vector3& vDir ) const;
		float GetX( float fY, float fZ ) const;
		float GetY( float fX, float fZ ) const;
		float GetZ( float fX, float fY ) const;
		float Collision( Vector3& vP );
		BOOL CollisionSphere( Vector3& vP, float fRadius );
		int LineCross( Vector3& v0, Vector3& v1, Vector3* pvOut ) const;


		Vector3 N; // Normal
		float	D; // D
	};

}
