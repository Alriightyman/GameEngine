////////////////////////////////////////////////////////////////////////////////
// Filename: Light.cpp
////////////////////////////////////////////////////////////////////////////////
#include "light.h"
#include <memory>

namespace Engine
{
	bool Light::isInitialized = false;

	Light::Light()
	{
	}


	Light::Light(const Light& other)
	{
	}


	Light::~Light()
	{
	}

	void Light::LoadScript(Script* script)
	{
		using namespace luabridge;
		script->LoadScript("Content/Scripts/Light/Light.lua");
		// run initialization function.
		LuaRef f = getGlobal(script->GetState(),"Initialize");

		if (f.isFunction())
		{
			std::shared_ptr<luabridge::LuaRef> func = std::make_shared<LuaRef>(f); 
			(*func)(this);
		}
	}

	void Light::SetSpecularColor(Color color)
	{
		m_specularColor = color;
	}

	void Light::SetSpecularPower(float power)
	{
		m_specularPower = power;
	}

	void Light::SetAmbientColor(Color color)
	{
		m_ambientColor = color;
	}


	void Light::SetDiffuseColor(Color color)
	{
		m_diffuseColor = color;
		return;
	}


	void Light::SetDirection(Vector3 direction)
	{
		m_direction = direction;
	}


	Color Light::GetAmbientColor() const
	{
		return m_ambientColor;
	}

	Color Light::GetDiffuseColor() const
	{
		return m_diffuseColor;
	}


	Vector3 Light::GetDirection() const
	{
		return m_direction;
	}

	Color Light::GetSpecularColor() const
	{
		return m_specularColor;
	}

	float Light::GetSpecularPower() const
	{
		return m_specularPower;
	} 

	void Light::Bind(Script* script)
	{
		using namespace luabridge;
		if(!isInitialized)
		{
			getGlobalNamespace(script->GetState())
				.beginClass<Light>("Light")
					.addProperty("Direction",&Light::GetDirection,&Light::SetDirection)
					.addProperty("AmbientColor",&Light::GetAmbientColor,&Light::SetAmbientColor)
					.addProperty("DiffuseColor",&Light::GetDiffuseColor,&Light::SetDiffuseColor)
					.addProperty("SpecularColor",&Light::GetSpecularColor,&Light::SetSpecularColor)
					.addProperty("SpecularPower",&Light::GetSpecularPower,&Light::SetSpecularPower)
				.endClass();
		}
	}
}