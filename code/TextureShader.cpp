#include "TextureShader.h"
#include "HelperUtilities.h"
#include "Texture.h"
namespace Engine
{

	TextureShader::TextureShader() 
		:m_sampleState(0)
	{
		m_vertexShader = 0;
		m_pixelShader = 0;
		m_layout = 0;

		m_worldMatrix = Matrix::Identity;
		m_viewMatrix = Matrix::Identity;
		m_projectionMatrix = Matrix::Identity;


	}

	TextureShader::~TextureShader()
	{
	}

	bool TextureShader::Initialize(Graphics* graphics)
	{

		if(!InitializeShader(graphics,L"Content/Shaders/TextureVS.cso",L"Content/Shaders/TexturePS.cso"))
			return false;

		return true;
	}

	void TextureShader::Shutdown()
	{
		ShutdownShader();
	}

	void TextureShader::Render(Graphics* graphics)
	{
		SetShaderParameters(graphics);

		RenderShader(graphics);
	}


	bool TextureShader::InitializeShader(Graphics* graphics,std::wstring vsFilename, std::wstring psFilename)
	{
		D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
		unsigned int numElements;

		struct ShaderBuffer
		{
			unsigned int Size;
			void* Buffer;
			ShaderBuffer() { Buffer = 0; }
		};

		ShaderBuffer vertexShaderBuffer,pixelShaderBuffer;
		ID3D11Device* device = graphics->GetDevice();

		memset(&vertexShaderBuffer,0,sizeof(ShaderBuffer));
		memset(&pixelShaderBuffer,0,sizeof(ShaderBuffer));

		vertexShaderBuffer.Buffer = LoadCompiledShader(vsFilename, vertexShaderBuffer.Size);
		pixelShaderBuffer.Buffer = LoadCompiledShader(psFilename, pixelShaderBuffer.Size);


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
		polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		polygonLayout[1].InputSlot = 0;
		polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		polygonLayout[1].InstanceDataStepRate = 0;

		// get a count of the elements in the layout
		numElements = sizeof(polygonLayout)/sizeof(polygonLayout[0]);

		// create the vertex input layout
		BHR(device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer.Buffer, vertexShaderBuffer.Size, 
			&m_layout));

		// Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
		delete vertexShaderBuffer.Buffer;
		delete pixelShaderBuffer.Buffer;

		// setup the description of the dynamic matrix constant buffer that is 
		// in the vertex shader
		m_matrixBuffer.Initialize(device);
		// create a texture sampler state description
		D3D11_SAMPLER_DESC samplerDesc;
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 1;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;

		memset(samplerDesc.BorderColor,0,4);

		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		// create the texture sampler state
		BHR(graphics->GetDevice()->CreateSamplerState(&samplerDesc,&m_sampleState));

		return true;

	}

	void TextureShader::ShutdownShader()
	{
		// release sampler state
		ReleaseCOM(m_sampleState);
		// release the layout
		ReleaseCOM(m_layout);
		// release the pixel shader
		ReleaseCOM(m_pixelShader);
		// release the vertex shader
		ReleaseCOM(m_vertexShader);
	}

	void TextureShader::SetShaderParameters(Graphics* graphics)
	{
		unsigned int bufferNumber;
		ID3D11Buffer* buffer;
		Matrix worldCopy,viewCopy,projCopy;
		ID3D11DeviceContext* context = graphics->GetImmediateContex();

		worldCopy.Transpose(m_worldMatrix);
		viewCopy.Transpose(m_viewMatrix);
		projCopy.Transpose(m_projectionMatrix);

		m_matrixBuffer.Data.world = worldCopy;
		m_matrixBuffer.Data.view = viewCopy;
		m_matrixBuffer.Data.projection = projCopy;
		m_matrixBuffer.ApplyChanges(context);

		// set the position of constant buffer in the vertex shader
		bufferNumber = 0;
		// finally set the constant buffer in the vertex shader with the updated values
		buffer = m_matrixBuffer.Buffer();
		context->VSSetConstantBuffers(bufferNumber,1,&buffer);

		context->PSSetShaderResources(0,1,m_textures->GetTextures());
	}

	void TextureShader::RenderShader(Graphics* graphics)
	{
		// set the vertex layout
		ID3D11DeviceContext* context = graphics->GetImmediateContex();

		context->IASetInputLayout(m_layout);
		// set the vertex and pixel shaders that will be used to render this triangle
		context->VSSetShader(m_vertexShader,0,0);
		context->PSSetShader(m_pixelShader,0,0);

		context->PSSetSamplers(0,1,&m_sampleState);
	} 
}