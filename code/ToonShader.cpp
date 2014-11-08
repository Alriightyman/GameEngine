#include "ToonShader.h"
#include "Texture.h"

namespace Engine
{

	ToonShader::ToonShader()
		: m_outlinePixelShader(0),m_outlineVertexShader(0),
		m_sampleState(0)
	{ 
		m_vertexShader = 0;
		m_pixelShader = 0;
	}

	ToonShader::~ToonShader()
	{
	}

	bool ToonShader::Initialize(Graphics* graphics)
	{
		if(!InitializeShader(graphics,L"Content/Shaders/ToonVS.cso",
			L"Content/Shaders/ToonPS.cso",
			L"Content/Shaders/ToonOutlineVS.cso",
			L"Content/Shaders/ToonOutlinePS.cso"))
		{
			return false;
		} // end if

		return true;
	}

	void ToonShader::Shutdown()
	{
		ShutdownShader();
	}

	void ToonShader::Render(Graphics* graphics, int indexCount)
	{

		SetShaderParameters(graphics);
			

		RenderShader(graphics,indexCount);
	}


	bool ToonShader::InitializeShader(Graphics* graphics, std::wstring vertexShaderFilename, std::wstring pixelShaderFilename,
		std::wstring vertexOutlineShaderFilename, std::wstring pixelOutlineShaderFilename)
	{
		HRESULT result;
		D3D11_INPUT_ELEMENT_DESC polygonLayout[3];
		unsigned int numElements;
		D3D11_SAMPLER_DESC samplerDesc;
		ID3D11Device* device = graphics->GetDevice();

		struct ShaderBuffer
		{
			unsigned int size;
			void* data;
		};

		ShaderBuffer VSBuffer;
		ShaderBuffer PSBuffer;
		memset(&VSBuffer,0,sizeof(ShaderBuffer));
		memset(&PSBuffer,0,sizeof(ShaderBuffer));

		VSBuffer.data = LoadCompiledShader(vertexShaderFilename.c_str(),VSBuffer.size);
		PSBuffer.data = LoadCompiledShader(pixelShaderFilename.c_str(),PSBuffer.size);

		// Create the vertex shader from the buffer.
		result = device->CreateVertexShader(VSBuffer.data, VSBuffer.size, NULL, &m_vertexShader);
		if(FAILED(result))
		{
			return false;
		}
		// Create the pixel shader from the buffer.
		result = device->CreatePixelShader(PSBuffer.data, PSBuffer.size, NULL, &m_pixelShader);
		if(FAILED(result))
		{
			return false;
		}

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
		result = device->CreateInputLayout(polygonLayout, numElements,VSBuffer.data,VSBuffer.size, &m_layout);
		if(FAILED(result))
		{
			return false;
		}
		delete VSBuffer.data;
		delete PSBuffer.data;
		memset(&VSBuffer,0,sizeof(ShaderBuffer));
		memset(&PSBuffer,0,sizeof(ShaderBuffer));
		// now for the outline shaders....
		VSBuffer.data = LoadCompiledShader(vertexOutlineShaderFilename.c_str(),VSBuffer.size);
		PSBuffer.data = LoadCompiledShader(pixelOutlineShaderFilename.c_str(),PSBuffer.size);
		// Create the vertex shader from the buffer.
		result = device->CreateVertexShader(VSBuffer.data,VSBuffer.size, NULL, &m_outlineVertexShader);
		if(FAILED(result))
		{
			return false;
		}

		// Create the pixel shader from the buffer.
		result = device->CreatePixelShader(PSBuffer.data, PSBuffer.size, NULL, &m_outlinePixelShader);
		if(FAILED(result))
		{
			return false;
		}

		if(FAILED(result))
		{
			return false;
		}

		// Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
		delete VSBuffer.data;
		delete PSBuffer.data;
		memset(&VSBuffer,0,sizeof(ShaderBuffer));
		memset(&PSBuffer,0,sizeof(ShaderBuffer));


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

		// initialize the constant buffers
		m_matrixBuffer.Initialize(device);
		m_lightBuffer.Initialize(device);

		return true;

	}

	void ToonShader::ShutdownShader()
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
		if(m_outlinePixelShader)
		{
			m_outlinePixelShader->Release();
			m_outlinePixelShader = 0;
		}

		// Release the vertex shader.
		if(m_outlineVertexShader)
		{
			m_outlineVertexShader->Release();
			m_outlineVertexShader = 0;
		}

	}

	void ToonShader::SetShaderParameters(Graphics* graphics)
	{
		unsigned int bufferNumber;
		ID3D11DeviceContext* deviceContext = graphics->GetImmediateContex();
		// Transpose the matrices and 		
		// update data in the constant buffer
		m_matrixBuffer.Data.World = m_worldMatrix.Transpose();
		m_matrixBuffer.Data.WorldInverseTranspose = m_worldMatrix.Invert().Transpose();
		m_matrixBuffer.Data.View = m_viewMatrix.Transpose();
		m_matrixBuffer.Data.Projection = m_projectionMatrix.Transpose();
		// set the constant buufer
		m_matrixBuffer.ApplyChanges(deviceContext);

		// Set the position of the constant buffer in the vertex shader.
		bufferNumber = 0;
		ID3D11Buffer* cBuffer[1] = {m_matrixBuffer.Buffer() };
		// Now set the constant buffer in the vertex shader with the updated values.
		deviceContext->VSSetConstantBuffers(bufferNumber, 1, cBuffer);

		// Set shader texture resource in the pixel shader.
		deviceContext->PSSetShaderResources(0, 1, &m_texture);

		// Copy the lighting variables into the constant buffer.
		m_lightBuffer.Data.Intensity = 1.0f;
		m_lightBuffer.Data.DiffuseColor = m_diffuseColor;
		m_lightBuffer.Data.Direction = m_lightDirection;
		// set the constant buffer
		m_lightBuffer.ApplyChanges(deviceContext);

		// Set the position of the light constant buffer in the pixel shader.
		bufferNumber = 0;
		ID3D11Buffer* cLightBuffer[1] = {m_lightBuffer.Buffer() };
		// Finally set the light constant buffer in the pixel shader with the updated values.
		deviceContext->PSSetConstantBuffers(bufferNumber, 1, cLightBuffer);

	}

	void ToonShader::RenderShader(Graphics* graphics, int indexCount)
	{
		// get our graphics devices
		auto deviceContext = graphics->GetImmediateContex();
		auto device = graphics->GetDevice();
		// set the layout
		deviceContext->IASetInputLayout(m_layout);
		if(m_drawLine)
		{
			// draw the outline first
			deviceContext->RSSetState(graphics->GetOutlineRasterState());
			deviceContext->VSSetShader(m_outlineVertexShader,0,0);
			deviceContext->PSSetShader(m_outlinePixelShader,0,0);
			deviceContext->DrawIndexed(indexCount,0,0);
		}
		// draw the rest of the model in a toony way
		deviceContext->RSSetState(graphics->GetRasterState()); // set the renderstate
		deviceContext->VSSetShader(m_vertexShader,0,0);    // set vertexShader 
		deviceContext->PSSetShader(m_pixelShader,0,0);     // set pixelShader
		deviceContext->PSSetSamplers(0,1,&m_sampleState);      // set the sampler State
		deviceContext->DrawIndexed(indexCount,0,0);			   // draw the indexed model
	}

	void ToonShader::SetTexture(Texture* texture)
	{
		m_texture = texture->GetTextures()[0];
	}
	void ToonShader::SetLightDirection(Vector3& lightDirection)
	{
		m_lightDirection = lightDirection;
	}
	void ToonShader::SetDiffuseColor(Color& diffuseColor)
	{
		m_diffuseColor = diffuseColor;
	}
	void ToonShader::IsDrawLine(bool isDrawLine)
	{
		m_drawLine = isDrawLine;
	}
}