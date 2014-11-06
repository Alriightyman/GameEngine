////////////////////////////////////////////////////////////////////////////////
// Filename: Camera.cpp
////////////////////////////////////////////////////////////////////////////////
#include "camera.h"

namespace Engine
{

	Camera::Camera()
	{
		m_positionX = 0.0f;
		m_positionY = 0.0f;
		m_positionZ = 0.0f;

		m_rotationX = 0.0f;
		m_rotationY = 0.0f;
		m_rotationZ = 0.0f;
	}


	Camera::Camera(const Camera& other)
	{
	}


	Camera::~Camera()
	{
	}


	void Camera::SetPosition(float x, float y, float z)
	{
		m_positionX = x;
		m_positionY = y;
		m_positionZ = z;
		return;
	}


	void Camera::SetRotation(float x, float y, float z)
	{
		m_rotationX = x;
		m_rotationY = y;
		m_rotationZ = z;
		return;
	}


	Vector3 Camera::GetPosition()
	{
		return Vector3( m_positionX, m_positionY, m_positionZ );
	}


	Vector3 Camera::GetRotation()
	{
		return Vector3( m_rotationX, m_rotationY, m_rotationZ );
	}


	void Camera::Render()
	{
		Vector3 up, position, lookAt;
		float yaw, pitch, roll;
		Matrix rotationMatrix;

		// Setup the vector that points upwards.
		up = Vector3(0.0f,1.0f,0.0f);

		// Setup the position of the camera in the world.
		position = Vector3(m_positionX,m_positionY,m_positionZ);
		// Setup where the camera is looking by default.
		lookAt = Vector3(0.0f,0.f,1.0f);

		// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
		pitch = m_rotationX * 0.0174532925f;
		yaw   = m_rotationY * 0.0174532925f;
		roll  = m_rotationZ * 0.0174532925f;

		// Create the rotation matrix from the yaw, pitch, and roll values.
		rotationMatrix = Matrix::CreateFromYawPitchRoll( pitch, yaw, roll );
		// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.

		lookAt = Vector3::Transform( lookAt, rotationMatrix );
		up =  Vector3::Transform( up, rotationMatrix );

		// Translate the rotated camera position to the location of the viewer.
		lookAt = position + lookAt;

		// Finally create the view matrix from the three updated vectors.
		m_viewMatrix = Matrix::CreateLookAt( position, lookAt, up );

		return;
	}


	Matrix Camera::GetViewMatrix( )
	{
		return m_viewMatrix;
	} 
}