////////////////////////////////////////////////////////////////////////////////
// Filename: Camera.cpp
////////////////////////////////////////////////////////////////////////////////
#include "camera.h"
#include <iostream>

namespace Engine
{
	Matrix Camera::m_viewMatrix = Matrix::Identity;

	bool Camera::isInitialized = false;
	void Camera::BindLua(Script* script)
	{
		
	}

	Camera::Camera(Script* script) : loadScript(false)
	{
		// set up lua binding
		luabridge::lua_State* L = script->GetState();

		if (!isInitialized)
		{
			isInitialized = true;

			try 
			{
				using namespace luabridge;
				getGlobalNamespace(L)
					.beginClass<Camera>("Camera")
						.addProperty("position",&Camera::GetPosition,&Camera::SetPosition)
						.addProperty("rotation",&Camera::GetRotation,&Camera::SetRotation)
						.addData<bool>("reloadScript",&Camera::loadScript)
					.endClass();
			}
			catch(luabridge::LuaException const& e)
			{
				std::cout << "ERR: " << e.what() << std::endl;
			}
		}

		m_position = Vector3::Zero;
		m_rotation = Vector3::Zero;
		LoadScript(script);
	}

	void Camera::LoadScript(Script* script)
	{
		using namespace luabridge;
		script->LoadScript("Content/Scripts/Camera/Camera.lua");
		LuaRef function = getGlobal(script->GetState(),"HandleInput");

		if (function.isFunction())
		{
			funcs["Input"] = std::make_shared<LuaRef>(function);
		}

		function = getGlobal(script->GetState(),"Initialize");

		if (function.isFunction())
		{
			funcs["Initialize"] = std::make_shared<LuaRef>(function);
			(*funcs["Initialize"])(this);
		}
	}

	Camera::Camera(const Camera& other)
	{
	}


	Camera::~Camera()
	{
	}


	void Camera::SetPosition(Vector3 position)
	{
		m_position = position;

	}


	void Camera::SetRotation(Vector3 rotation)
	{
		m_rotation = rotation;

	}


	Vector3 Camera::GetPosition() const
	{
		return m_position;
	}


	Vector3 Camera::GetRotation() const
	{
		return m_rotation;
	}


	void Camera::Update()
	{
		Vector3 up, position, lookAt;
		float yaw, pitch, roll;
		Matrix rotationMatrix;

		// Setup the vector that points upwards.
		up = Vector3(0.0f,1.0f,0.0f);

		// Setup the position of the camera in the world.
		position = m_position;
		// Setup where the camera is looking by default.
		lookAt = Vector3(0.0f,0.f,1.0f);

		// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
		pitch = m_rotation.x * 0.0174532925f;
		yaw   = m_rotation.y * 0.0174532925f;
		roll  = m_rotation.z * 0.0174532925f;

		// Create the rotation matrix from the yaw, pitch, and roll values.
		rotationMatrix = Matrix::CreateFromYawPitchRoll( pitch, yaw, roll );
		// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.

		lookAt = Vector3::Transform( lookAt, rotationMatrix );
		up =  Vector3::Transform( up, rotationMatrix );

		// Translate the rotated camera position to the location of the viewer.
		lookAt = position + lookAt;

		// Finally create the view matrix from the three updated vectors.
		Camera::m_viewMatrix = Matrix::CreateLookAt( position, lookAt, up );
	}


	Matrix Camera::GetViewMatrix( )
	{
		return Camera::m_viewMatrix;
	} 

	void Camera::MoveCamera(Script* script,InputState* input)
	{
		if(funcs["Input"])
		{
			try
			{
				(*funcs["Input"])(this,input);
				//std::cout << "C++ Position: (" << m_position.x << "," << m_position.y << ","<<m_position.z << ")\n"; 
			}
			catch (luabridge::LuaException const& e)
			{
				std::cout << "LuaException: " << e.what() << std::endl;
			}
		}

		if (loadScript)
		{
			loadScript = false;
			LoadScript(script);
		}
	}
}