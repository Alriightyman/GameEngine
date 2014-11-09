////////////////////////////////////////////////////////////////////////////////
// Filename: LightShader.cpp
////////////////////////////////////////////////////////////////////////////////
#include "lightshader.h"
#include <DirectXColors.h>
#include "Texture.h"

namespace Engine
{

	LightShader::LightShader()
	{
		m_vertexShader = 0;
		m_pixelShader = 0;
		m_layout = 0;
		m_sampleState = 0;
		m_worldMatrix = Matrix::Identity;
		m_viewMatrix = Matrix::Identity;
		m_projectionMatrix = Matrix::Identity;
		m_texture = 0;
		m_lightDirection = Vector3::Zero;
		m_diffuseColor = Color(1,1,1,1);
		m_ambientColor = Color(1,1,1,1);
		m_cameraPosition = Vector3::Zero;
		m_specularColor = Color(1,1,1,1);
		m_specularPower = 1.0f;
	}


	LightShader::LightShader(const LightShader& other)
	{
	}


	LightShader::~LightShader()
	{
	}


	bool LightShader::Initialize(Graphics* graphics)
	{
		// Initialize the vertex and pixel shaders.
		if(!InitializeShader(graphics, L"Content/Shaders/lightVS.cso", L"Content/Shaders/lightPS.cso"))
			return false;

		return true;
	}


	void LightShader::Shutdown()
	{
		// Shutdown the vertex and pixel shaders as well as the related objects.
		ShutdownShader();
	}


	void LightShader::Render(Graphics* graphics, int indexCount)
	{

		// Set the shader parameters that it will use for rendering.
		SetShaderParameters(graphics);

		// Now render the prepared buffers with the shader.
		RenderShader(graphics, indexCount);
	}


	bool LightShader::InitializeShader(Graphics* graphics,  std::wstring vertShaderfilename,  std::wstring pixelShaderfilename)
	{
		HRESULT result;
		// create a little structure to handle
		// shader buffer and size
		struct ShaderBuffer
		{
			unsigned int Size;
			void* Buffer;
		};

		ShaderBuffer vertexShaderBuffer, pixelShaderBuffer;
		D3D11_INPUT_ELEMENT_DESC polygonLayout[3];
		unsigned int numElements;
		D3D11_SAMPLER_DESC samplerDesc;
		ID3D11Device* device = graphics->GetDevice();

		memset(&vertexShaderBuffer,0,sizeof(ShaderBuffer));
		memset(&pixelShaderBuffer,0,sizeof(ShaderBuffer));
		// load the compiled shaders
		vertexShaderBuffer.Buffer = LoadCompiledShader(vertShaderfilename, vertexShaderBuffer.Size);
		pixelShaderBuffer.Buffer = LoadCompiledShader(pixelShaderfilename, pixelShaderBuffer.Size);
		// set the shaders
		if(FAILED(device->CreateVertexShader(vertexShaderBuffer.Buffer,vertexShaderBuffer.Size,nullptr,&m_vertexShader)))
			return false;

		if(FAILED(device->CreatePixelShader(pixelShaderBuffer.Buffer,pixelShaderBuffer.Size,nullptr,&m_pixelShader)))
			return false;

		// Create the vertex input layout description.
		// This setup needs to match the VertexType stucture in the Model and in the shader.
		polygonLayout[0].SemanticName = "POSITION";
		polygonLayout[0].SemanticIndex = 0;
		polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		polygonLayout[0].InputSlot = 0;
		polygonLayout[0].AlignedByteOffset = 0;
		polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		polygonLayout[0].InstanceDataStepRate = 0;

		polygonLayout[1].SemanticName = "TEXCOORD";
		polygonLayout[1].SemanticIndex = 0;
		polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
		polygonLayout[1].InputSlot = 0;
		polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		polygonLayout[1].InstanceDataStepRate = 0;

		polygonLayout[2].SemanticName = "NORMAL";
		polygonLayout[2].SemanticIndex = 0;
		polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		polygonLayout[2].InputSlot = 0;
		polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		polygonLayout[2].InstanceDataStepRate = 0;

		// Get a count of the elements in the layout.
		numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

		// Create the vertex input layout.
		result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer.Buffer, vertexShaderBuffer.Size, 
			&m_layout);
		if(FAILED(result))
		{
			return false;
		}

		// Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
		delete vertexShaderBuffer.Buffer;
		delete pixelShaderBuffer.Buffer;

		// Create a texture sampler state description.
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 1;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		samplerDesc.BorderColor[0] = 0;
		samplerDesc.BorderColor[1] = 0;
		samplerDesc.BorderColor[2] = 0;
		samplerDesc.BorderColor[3] = 0;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		// Create the texture sampler state.
		result = device->CreateSamplerState(&samplerDesc, &m_sampleState);
		if(FAILED(result))
		{
			return false;
		}

		// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
		m_matrixBuffer.Initialize(device);
		m_cameraBuffer.Initialize(device);
		m_lightBuffer.Initialize(device);

		return true;
	}


	void LightShader::ShutdownShader()
	{

		// Release the sampler state.
		if(m_sampleState)
		{
			m_sampleState->Release();
			m_sampleState = 0;
		}

		// Release the layout.
		if(m_layout)
		{
			m_layout->Release();
			m_layout = 0;
		}

		// Release the pixel shader.
		if(m_pixelShader)
		{
			m_pixelShader->Release();
			m_pixelShader = 0;
		}

		// Release the vertex shader.
		if(m_vertexShader)
		{
			m_vertexShader->Release();
			m_vertexShader = 0;
		}

		return;
	}

	void LightShader::SetShaderParameters(Graphics* graphics)
	{

		unsigned int bufferNumber;
		ID3D11DeviceContext* deviceContext = graphics->GetImmediateContex();
		ID3D11Buffer* buffer;

		Matrix worldMatrixCopy = m_worldMatrix;
		Matrix viewMatrixCopy = m_viewMatrix;
		Matrix projectionMatrixCopy = m_projectionMatrix;

		// Transpose the matrices to prepare them for the shader.

		worldMatrixCopy = m_worldMatrix.Transpose();
		viewMatrixCopy = m_viewMatrix.Transpose();
		projectionMatrixCopy = m_projectionMatrix.Transpose();

		// Copy the matrices into the constant buffer.
		m_matrixBuffer.Data.world = worldMatrixCopy;
		m_matrixBuffer.Data.view = viewMatrixCopy;
		m_matrixBuffer.Data.projection = projectionMatrixCopy;
		m_matrixBuffer.ApplyChanges(deviceContext);

		// Set the position of the constant buffer in the vertex shader.
		bufferNumber = 0;

		buffer = m_matrixBuffer.Buffer();
		// Now set the constant buffer in the vertex shader with the updated values.
		deviceContext->VSSetConstantBuffers(bufferNumber, 1, &buffer);

		// Copy the camera variables into the constant buffer.
		m_cameraBuffer.Data.cameraPosition = m_cameraPosition;
		m_cameraBuffer.Data.padding = 0.0f;

		m_cameraBuffer.ApplyChanges(deviceContext);

		// Set the position of the camera constant buffer in the vertex shader.
		bufferNumber = 1;

		// Finally set the camera constant buffer in the vertex shader with the updated values.
		buffer = m_cameraBuffer.Buffer();
		deviceContext->VSSetConstantBuffers(bufferNumber, 1, &buffer);

		// Set shader texture resource in the pixel shader.
		deviceContext->PSSetShaderResources(0, 1, &m_texture);

		// Copy the lighting variables into the constant buffer.
		m_lightBuffer.Data.ambientColor = m_ambientColor;
		m_lightBuffer.Data.diffuseColor = m_diffuseColor;
		m_lightBuffer.Data.lightDirection = m_lightDirection;
		m_lightBuffer.Data.specularColor = m_specularColor;
		m_lightBuffer.Data.specularPower = m_specularPower;
		m_lightBuffer.ApplyChanges(deviceContext);

		// Set the position of the light constant buffer in the pixel shader.
		bufferNumber = 0;

		// Finally set the light constant buffer in the pixel shader with the updated values.
		buffer = m_lightBuffer.Buffer();
		deviceContext->PSSetConstantBuffers(bufferNumber, 1, &buffer);

	}


	void LightShader::RenderShader(Graphics* graphics, int indexCount)
	{
		ID3D11DeviceContext* deviceContext = graphics->GetImmediateContex();
		// Set the vertex input layout.
		deviceContext->IASetInputLayout(m_layout);

		// Set the vertex and pixel shaders that will be used to render this triangle.
		deviceContext->VSSetShader(m_vertexShader, NULL, 0);
		deviceContext->PSSetShader(m_pixelShader, NULL, 0);

		// Set the sampler state in the pixel shader.
		deviceContext->PSSetSamplers(0, 1, &m_sampleState);

		// Render the triangle.
		deviceContext->DrawIndexed(indexCount, 0, 0);
	} 


	void LightShader::SetWorldMatrix(Matrix& world)
	{
		m_worldMatrix = world;
	}
	void LightShader::SetViewMatrix(Matrix& view)
	{
		m_viewMatrix = view;
	}
	void LightShader::SetProjectionMatrix(Matrix& proj)
	{
		m_projectionMatrix = proj;
	}
	void LightShader::SetWolrdViewProjMatrix(Matrix& world,Matrix& view,Matrix& proj)
	{
		m_worldMatrix = world;
		m_viewMatrix = view; 
		m_projectionMatrix = proj;
	}
	void LightShader::SetTexture(Texture* texture)
	{
		m_texture = texture->GetTextures()[0];
	}
	void LightShader::SetLightDirection(const Vector3& lightDirection)
	{
		m_lightDirection = lightDirection;
	}
	void LightShader::SetAmbientColor(const Color& ambientColor)
	{
		m_ambientColor = ambientColor;
	}
	void LightShader::SetDiffuseColor(const Color& diffuseColor)
	{
		m_diffuseColor = diffuseColor;
	}
	void LightShader::SetCameraPosition(Vector3& cameraPosition)
	{
		m_cameraPosition = cameraPosition;
	}
	void LightShader::SetSpecularColor(Color& spcularColor)
	{
		m_specularColor = spcularColor;
	}
	void LightShader::SetSpecularPower(float specularPower)
	{
		m_specularPower = specularPower;
	}


}