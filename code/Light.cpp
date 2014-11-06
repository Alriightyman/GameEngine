////////////////////////////////////////////////////////////////////////////////
// Filename: Light.cpp
////////////////////////////////////////////////////////////////////////////////
#include "light.h"

namespace Engine
{

	Light::Light()
	{
	}


	Light::Light(const Light& other)
	{
	}


	Light::~Light()
	{
	}

	void Light::SetSpecularColor(float red, float green, float blue, float alpha)
	{
		m_specularColor = Color(red, green, blue, alpha);
	}

	void Light::SetSpecularPower(float power)
	{
		m_specularPower = power;
	}

	void Light::SetAmbientColor(float red, float green, float blue, float alpha)
	{
		m_ambientColor = Color(red, green, blue, alpha);
	}


	void Light::SetDiffuseColor(float red, float green, float blue, float alpha)
	{
		m_diffuseColor = Color(red, green, blue, alpha);
		return;
	}


	void Light::SetDirection(float x, float y, float z)
	{
		m_direction = Vector3(x, y, z);
		return;
	}


	Color Light::GetAmbientColor()
	{
		return m_ambientColor;
	}

	Color Light::GetDiffuseColor()
	{
		return m_diffuseColor;
	}


	Vector3 Light::GetDirection()
	{
		return m_direction;
	}

	Color Light::GetSpecularColor()
	{
		return m_specularColor;
	}

	float Light::GetSpecularPower()
	{
		return m_specularPower;
	} 
}