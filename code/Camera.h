////////////////////////////////////////////////////////////////////////////////
// Filename: Camera.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _Camera_H_
#define _Camera_H_


//////////////
// INCLUDES //
//////////////
#include <SimpleMath.h>
#include "InputState.h"
#include "Script.h"
#include <map>

using namespace DirectX::SimpleMath;


namespace Engine
{

	class Camera
	{
	public:
		static void BindLua(Script* script);
		static bool isInitialized;
	public:
		Camera(Script* script);
		Camera(const Camera&);
		~Camera();

		void SetPosition(Vector3);
		void SetRotation(Vector3);

		Vector3 GetPosition() const;
		Vector3 GetRotation() const;

		void Update();
		static Matrix GetViewMatrix();
		void MoveCamera(Script* script,InputState* input);
	private:
		bool loadScript;
		void LoadScript(Script* script);
	private:
		Vector3 m_position;
		Vector3 m_rotation;
		static Matrix m_viewMatrix;
		std::map<std::string, std::shared_ptr<luabridge::LuaRef> > funcs;
	}; 
}

#endif