#include "Frustum.h"
namespace Engine
{

	Frustum::Frustum() {}

	Frustum::~Frustum() {}

	void Frustum::Construct(float screenDepth,Matrix projMatrix,Matrix viewMatrix)
	{
		float zMin, r;
		Matrix matrix;
		// calculate the minimum z distance in the frustum
		zMin = -projMatrix._43 / projMatrix._33;
		r = screenDepth / (screenDepth - zMin);
		projMatrix._33 = r;
		projMatrix._43 = -r * zMin;

		// create the frustum matrix from the view matrix and update projection matrix
		matrix = viewMatrix * projMatrix;

		// calculate near plane
		m_planes[0].x = matrix._14 + matrix._13;
		m_planes[0].y = matrix._24 + matrix._23;
		m_planes[0].z = matrix._34 + matrix._33;
		m_planes[0].w = matrix._44 + matrix._43;
		m_planes[0].Normalize();

		// calculate far plane
		m_planes[1].x = matrix._14 - matrix._13;
		m_planes[1].y = matrix._24 - matrix._23;
		m_planes[1].z = matrix._34 - matrix._33;
		m_planes[1].w = matrix._44 - matrix._43;
		m_planes[1].Normalize();

		// calculate left plane
		m_planes[2].x = matrix._14 + matrix._11;
		m_planes[2].y = matrix._24 + matrix._21;
		m_planes[2].z = matrix._34 + matrix._31;
		m_planes[2].w = matrix._44 + matrix._41;
		m_planes[2].Normalize();

		// calculate right plane
		m_planes[3].x = matrix._14 - matrix._11;
		m_planes[3].y = matrix._24 - matrix._21;
		m_planes[3].z = matrix._34 - matrix._31;
		m_planes[3].w = matrix._44 - matrix._41;
		m_planes[3].Normalize();

		// calculate top plane
		m_planes[4].x = matrix._14 - matrix._12;
		m_planes[4].y = matrix._24 - matrix._22;
		m_planes[4].z = matrix._34 - matrix._32;
		m_planes[4].w = matrix._44 - matrix._42;
		m_planes[4].Normalize();

		// calculate bottom
		m_planes[5].x = matrix._14 + matrix._12;
		m_planes[5].y = matrix._24 + matrix._22;
		m_planes[5].z = matrix._34 + matrix._32;
		m_planes[5].w = matrix._44 + matrix._42;
		m_planes[5].Normalize();

	}

	bool Frustum::CheckPoint(float x, float y, float z)
	{
		// check if the point is inside all six planes
		// of the view frustum
		for(int i = 0; i < 6; i++)
		{
			if(m_planes[i].DotCoordinate(Vector3(x,y,z) )< 0.0f)
				return false;
		}

		return true;
	}

	bool Frustum::CheckCube(float xCenter,float yCenter,float zCenter,float radius)
	{
		// check if any one point of the cube is in the frustum
		for(int i = 0; i < 6; i++)
		{
			if(m_planes[i].DotCoordinate(Vector3(xCenter - radius,yCenter - radius,zCenter - radius)) >= 0.0f)
				continue;
			if(m_planes[i].DotCoordinate(Vector3(xCenter + radius,yCenter - radius,zCenter -radius)) >= 0.0f)
				continue;
			if(m_planes[i].DotCoordinate(Vector3(xCenter - radius,yCenter + radius,zCenter -radius)) >= 0.0f)
				continue;
			if(m_planes[i].DotCoordinate(Vector3(xCenter - radius,yCenter - radius,zCenter -radius)) >= 0.0f)
				continue;
			if(m_planes[i].DotCoordinate(Vector3(xCenter - radius,yCenter - radius,zCenter + radius)) >= 0.0f)
				continue;
			if(m_planes[i].DotCoordinate(Vector3(xCenter + radius,yCenter - radius,zCenter + radius)) >= 0.0f)
				continue;
			if(m_planes[i].DotCoordinate(Vector3(xCenter - radius,yCenter + radius,zCenter + radius)) >= 0.0f)
				continue;
			if(m_planes[i].DotCoordinate(Vector3(xCenter + radius,yCenter + radius,zCenter + radius)) >= 0.0f)
				continue;

			return false;
		}

		return true;
	}

	bool Frustum::CheckSphere(float xCenter,float yCenter,float zCenter, float radius)
	{
		// check if radius of sphere is inside the view frustum
		for(int i = 0; i < 6; i++)
		{
			if(m_planes[i].DotCoordinate(Vector3(xCenter,yCenter,zCenter)) < -radius)
				return false;
		}

		return true;
	}

	bool Frustum::CheckRectangle(float xCenter, float yCenter, float zCenter, float xSize, float ySize, float zSize)
	{
		// Check if any of the 6 planes of the rectangle are inside the view frustum.
		for(int i=0; i<6; i++)
		{
			if(m_planes[i].DotCoordinate( Vector3((xCenter - xSize), (yCenter - ySize), (zCenter - zSize))) >= 0.0f)
				continue;

			if(m_planes[i].DotCoordinate(Vector3((xCenter + xSize), (yCenter - ySize), (zCenter - zSize))) >= 0.0f)
				continue;

			if(m_planes[i].DotCoordinate(Vector3((xCenter - xSize), (yCenter + ySize), (zCenter - zSize))) >= 0.0f)
				continue;

			if(m_planes[i].DotCoordinate( Vector3( xCenter - xSize, yCenter - ySize, zCenter + zSize)) >= 0.0f)
				continue;

			if(m_planes[i].DotCoordinate(Vector3((xCenter + xSize), (yCenter + ySize), (zCenter - zSize))) >= 0.0f)
				continue;

			if(m_planes[i].DotCoordinate(Vector3((xCenter + xSize), (yCenter - ySize), (zCenter + zSize))) >= 0.0f)
				continue;

			if(m_planes[i].DotCoordinate(Vector3((xCenter - xSize), (yCenter + ySize), (zCenter + zSize))) >= 0.0f)
				continue;

			if(m_planes[i].DotCoordinate(Vector3((xCenter + xSize), (yCenter + ySize), (zCenter + zSize))) >= 0.0f)
				continue;

			return false;
		}

		return true;
	} 
}