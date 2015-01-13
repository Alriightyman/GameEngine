#pragma once

#include "IObject.h"

namespace Engine
{
	class InputState;

	class GameObject : public IObject
	{
	protected:
		DirectX::SimpleMath::Vector3 m_velocity;
		DirectX::SimpleMath::Vector3 m_direction;
		bool m_isAlive, m_isOnScreen;
		DirectX::SimpleMath::Matrix m_worldMatrix;
	public:
		GameObject(std::wstring name) : IObject(name),m_velocity(0.0f),m_direction(0.0f),m_isAlive(true) {}
		virtual ~GameObject() {}
		void SetAlive(bool value) { m_isAlive = value; }
		bool IsAlive() const { return m_isAlive; }
		void SetOnScreenFlag(bool value) { m_isOnScreen = value; }
		bool IsOnScreen() const { return m_isOnScreen; }

		virtual bool Initialize(Graphics* graphics,std::string filename, Script* script) override { return true;}
		virtual void Update(float deltaTime) override {}
		virtual void Render(Graphics* graphics) {}
		virtual ShaderType GetShaderType() override { return (ShaderType)0; }
		DirectX::SimpleMath::Matrix GetWorldMatrix() const { return m_worldMatrix;	}
	};
}