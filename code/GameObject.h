#pragma once
#include <SimpleMath.h>
class Script;
namespace Engine
{
	class Graphics;

	class IGameObject
	{
	public:
		std::string m_name;
		DirectX::SimpleMath::Vector3 m_position;
		DirectX::SimpleMath::Vector3 m_velocity;
		DirectX::SimpleMath::Vector3 m_direction;
		
	public:
		IGameObject(void) : m_name("NULL"), m_position(0.0f),m_velocity(0.0f),m_direction(DirectX::SimpleMath::Vector3::UnitY) { } 
		virtual ~IGameObject(void) {};

		virtual bool Initialize(Graphics* graphics,Script* script) = 0;
		virtual void Update(float dt) = 0;
		virtual void Render(Graphics* graphics) = 0;
	};

}
