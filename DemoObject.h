#pragma once
#include "code\GameObject.h"
#include <memory>
#include <map>
namespace luabridge
{
	class LuaRef;
}

namespace Engine
{
	class InputState;
	class Model;
	enum PlayerIndex;
	enum ShaderType;

	class DemoObject : public GameObject
	{
	public:
		static void Bind(Script* script);
		static bool isInitialized;

	public: // properties
		DirectX::SimpleMath::Vector3 GetPosition() const { return m_position; }
		DirectX::SimpleMath::Vector3 GetVelocity() const { return m_velocity; }
		DirectX::SimpleMath::Vector3 GetDirection() const { return m_direction; }
		DirectX::SimpleMath::Vector3 GetDesiredDirection() const { return m_desiredDirection; }
		DirectX::SimpleMath::Vector3 GetRotation() const { return m_rotation; }

		std::wstring GetName() const { return m_name; }

		void SetName(std::wstring name) { m_name = name; }
		void SetPosition(DirectX::SimpleMath::Vector3 pos) { m_position = pos; }
		void SetVelocity(DirectX::SimpleMath::Vector3  vel) { m_velocity = vel; }
		void SetDirection(DirectX::SimpleMath::Vector3 dir) { m_desiredDirection = dir; }
		void SetDesiredDirection(DirectX::SimpleMath::Vector3 dir) { m_desiredDirection = dir; }
		void SetRotation(DirectX::SimpleMath::Vector3 rot) { m_rotation = rot; }
		
		
		ShaderType GetShaderType() override;

		DemoObject(std::wstring name);
		~DemoObject(void);

		bool Initialize(Graphics* graphics,std::string scriptfilename, Script* script) override;
		void Update(float dt) override;
		void Render(Graphics* graphics) override;
		void Input(InputState* input, int currentIndex);

	private:
		DirectX::SimpleMath::Vector3 m_rotation;
		Model* m_model;
		std::map<std::string,std::shared_ptr<luabridge::LuaRef> > functions;
		DirectX::SimpleMath::Vector3 m_desiredDirection;
	}; 
}

