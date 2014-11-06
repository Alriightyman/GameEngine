////////////////////////////////////////////////////////////////////////////////
// Filename: Camera.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _Camera_H_
#define _Camera_H_


//////////////
// INCLUDES //
//////////////
#include <SimpleMath.h>
using namespace DirectX::SimpleMath;


namespace Engine
{

	class Camera
	{
	public:
		Camera();
		Camera(const Camera&);
		~Camera();

		void SetPosition(float, float, float);
		void SetRotation(float, float, float);

		Vector3 GetPosition();
		Vector3 GetRotation();

		void Render();
		Matrix GetViewMatrix();

	private:
		float m_positionX, m_positionY, m_positionZ;
		float m_rotationX, m_rotationY, m_rotationZ;
		Matrix m_viewMatrix;
	}; 
}

#endif