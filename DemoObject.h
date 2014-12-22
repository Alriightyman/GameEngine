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

	class DemoObject : public IGameObject
	{
	public:
		static void Bind(Script* script);
		static bool isInitialized;

	public: // properties
		DirectX::SimpleMath::Vector3 GetPosition() const { return m_position; }
		DirectX::SimpleMath::Vector3 GetVelocity() const { return m_velocity; }
		void SetPosition(DirectX::SimpleMath::Vector3 pos) { m_position = pos; }
		void SetVelocity(DirectX::SimpleMath::Vector3  vel) { m_velocity = vel; }
		DirectX::SimpleMath::Matrix GetWorldMatrix() const;
		DemoObject(void);
		~DemoObject(void);

		bool Initialize(Graphics* graphics, Script* script) override;
		void Update(float dt) override;
		void Render(Graphics* graphics) override;
		void Input(InputState* input);

	private:
		std::map<std::string,std::shared_ptr<luabridge::LuaRef> > functions;
		Model* m_model;
		DirectX::SimpleMath::Matrix m_worldMatrix;
	}; 
}

