#pragma once

#include <SimpleMath.h>
using namespace DirectX::SimpleMath;

namespace Engine
{
	class Frustum
	{
	private:
		Plane m_planes[6];

	public:
		Frustum();
		~Frustum();

		void Construct(float screenDepth,Matrix projMatrix,Matrix viewMatrix);

		bool CheckPoint(float x, float y, float z);
		bool CheckCube(float xCenter,float yCenter,float zCenter,float radius);
		bool CheckSphere(float xCenter,float yCenter,float zCenter, float radius);
		bool CheckRectangle(float xCenter, float yCenter, float zCenter, float xSize, float ySize, float zSize);
	}; 
}