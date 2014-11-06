////////////////////////////////////////////////////////////////////////////////
// Filename: Light.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _Light_H_
#define _Light_H_


//////////////
// INCLUDES //
//////////////
#include <SimpleMath.h>
using namespace DirectX::SimpleMath;

namespace Engine
{
	class Light
	{
	public:
		Light();
		Light(const Light&);
		~Light();

		void SetAmbientColor(float,float,float,float);
		void SetDiffuseColor(float, float, float, float);
		void SetDirection(float, float, float);
		void SetSpecularColor(float,float,float,float);
		void SetSpecularPower(float);

		Color GetAmbientColor();
		Color GetDiffuseColor();
		Vector3 GetDirection();
		Color GetSpecularColor();
		float GetSpecularPower();

	private:
		Color m_ambientColor;
		Color m_diffuseColor;
		Vector3 m_direction;
		Color m_specularColor;
		float m_specularPower;
	};

}
#endif