////////////////////////////////////////////////////////////////////////////////
// Filename: Light.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _Light_H_
#define _Light_H_


//////////////
// INCLUDES //
//////////////
#include <SimpleMath.h>
#include "Script.h"
using namespace DirectX::SimpleMath;

namespace Engine
{
	//class Script;

	class Light
	{
	public:
		static bool isInitialized;
		static void Bind(Script* script);
	public:
		Light();
		Light(const Light&);
		~Light();
		void LoadScript(Script* script);
		void SetAmbientColor(Color color);
		void SetDiffuseColor(Color color);
		void SetDirection(Vector3 direction);
		void SetSpecularColor(Color color);
		void SetSpecularPower(float);

		Color GetAmbientColor() const;
		Color GetDiffuseColor() const;
		Vector3 GetDirection() const;
		Color GetSpecularColor() const;
		float GetSpecularPower() const;

	private:
		Color m_ambientColor;
		Color m_diffuseColor;
		Vector3 m_direction;
		Color m_specularColor;
		float m_specularPower;
	};

}
#endif