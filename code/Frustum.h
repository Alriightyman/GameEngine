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

		void Construct(float,Matrix,Matrix);

		bool CheckPoint(float,float,float);
		bool CheckCube(float,float,float,float);
		bool CheckSphere(float,float,float,float);
		bool CheckRectangle(float,float,float,float,float,float);
	}; 
}