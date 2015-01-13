#pragma once
#include <SimpleMath.h>
class Script;
namespace Engine
{
	class Graphics;
	enum ShaderType;

	class IObject
	{
	protected:
		std::wstring m_name;
		DirectX::SimpleMath::Vector3 m_position;
	
	public:
		IObject(std::wstring name) : m_name(name), m_position(0.0f) { } 
		virtual ~IObject(void) {};

		virtual bool Initialize(Graphics* graphics,std::string scriptfilename,Script* script) = 0;
		virtual void Update(float dt) = 0;
		virtual ShaderType GetShaderType() = 0;
	};

}
