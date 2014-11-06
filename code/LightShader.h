////////////////////////////////////////////////////////////////////////////////
// Filename: LightShader.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _LightShader_H_
#define _LightShader_H_


//////////////
// INCLUDES //
//////////////
#include "Shader.h"
using namespace std;

namespace Engine
{

	class LightShader : public Shader
	{
	private:
		struct MatrixBuffer
		{
			Matrix world;
			Matrix view;
			Matrix projection;
		};

		struct CameraBuffer
		{
			Vector3 cameraPosition;
			float padding;
		};

		struct LightBuffer
		{
			Color ambientColor;
			Color diffuseColor;
			Vector3 lightDirection;
			float specularPower;
			Color specularColor;
		};


	public:
		LightShader();
		LightShader(const LightShader&);
		~LightShader() override;

		bool Initialize(Graphics* graphics) override;
		void Shutdown() override;
		void Render(Graphics* graphics, int indexCount) override;
		// set params
		void SetWorldMatrix(Matrix& world);
		void SetViewMatrix(Matrix& view);
		void SetProjectionMatrix(Matrix& proj);
		void SetWolrdViewProjMatrix(Matrix& world,Matrix& view,Matrix& proj);
		void SetTexture(ID3D11ShaderResourceView* texture);
		void SetLightDirection(const Vector3& lightDirection); 
		void SetAmbientColor(const Color& ambientColor);
		void SetDiffuseColor(const Color& diffuseColor);
		void SetCameraPosition(Vector3& cameraPosition);
		void SetSpecularColor(Color& spcularColor);
		void SetSpecularPower(float specularPower);
	private:
		bool InitializeShader(Graphics* graphics,  std::wstring vertShaderfilename,  std::wstring pixelShaderfilename) override;
		void ShutdownShader() override;
//		void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

		void SetShaderParameters(Graphics* graphics) override;
		void RenderShader(Graphics* graphics, int indexCount) override;

	private:

		ID3D11SamplerState* m_sampleState;
		ConstantBuffer<MatrixBuffer> m_matrixBuffer;
		ConstantBuffer<CameraBuffer> m_cameraBuffer;
		ConstantBuffer<LightBuffer> m_lightBuffer;

		// values
		Matrix m_world;
		Matrix m_view;
		Matrix m_projection;
		ID3D11ShaderResourceView* m_texture;
		Vector3 m_lightDirection;
		Color m_diffuseColor;
		Color m_ambientColor;
		Vector3 m_cameraPosition;
		Color m_specularColor;
		float m_specularPower;

	}; 
}

#endif