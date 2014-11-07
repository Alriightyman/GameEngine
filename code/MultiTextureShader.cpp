#include "MultiTextureShader.h"
namespace Engine
{

	MultiTextureShader::MultiTextureShader()
		: m_sampleState(0)
	{
		 m_vertexShader = 0;
		 m_pixelShader = 0;
		 m_layout = 0;
	}	 

	MultiTextureShader::~MultiTextureShader() {  }

	bool MultiTextureShader::Initialize(Graphics* graphics)
	{
		// initialize our shaders
		if(!InitializeShader(graphics,L"Content/Shaders/MultiTextureVS.cso", L"Content/Shaders/MultiTexturePS.cso"))
			return false;

		return true;
	}

	void MultiTextureShader::Shutdown()
	{
		ShutdownShader();
	}

	void MultiTextureShader::Render(Graphics* graphics,int indexCount, Matrix worldMatrix,Matrix viewMatrix,
		Matrix projMatrix, ID3D11ShaderResourceView** textureArray)
	{
		SetShaderParameters(graphics,worldMatrix,viewMatrix,projMatrix,textureArray);

		// render the buffers
		RenderShader(graphics,indexCount);
	}


	bool MultiTextureShader::InitializeShader(Graphics* graphics,std::wstring vertexShaderFilename,std::wstring pixelShaderFilename)
	{
		// create a little structure to handle
		// shader buffer and size
		struct ShaderBuffer
		{
			unsigned int Size;
			void* Buffer;
		};

		ShaderBuffer vertexBuffer, pixelBuffer;

		D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
		unsigned int numElements;
		D3D11_SAMPLER_DESC samplerDesc;
		ID3D11Device* device = graphics->GetDevice();


		memset(&vertexBuffer,0,sizeof(ShaderBuffer));
		memset(&pixelBuffer,0,sizeof(ShaderBuffer));
		// load the compiled shaders
		vertexBuffer.Buffer = LoadCompiledShader(vertexShaderFilename, vertexBuffer.Size);
		pixelBuffer.Buffer = LoadCompiledShader(pixelShaderFilename, pixelBuffer.Size);
		// set the shaders
		if(FAILED(device->CreateVertexShader(vertexBuffer.Buffer,vertexBuffer.Size,nullptr,&m_vertexShader)))
			return false;

		if(FAILED(device->CreatePixelShader(pixelBuffer.Buffer,pixelBuffer.Size,nullptr,&m_pixelShader)))
			return false;

		// create the vertex input layout
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

		// get a count of the elements in the layout
		numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

		// create the vertex input layout
		if(FAILED(device->CreateInputLayout(polygonLayout,numElements,vertexBuffer.Buffer,
			vertexBuffer.Size,&m_layout)))
			return false;

		// released the vertex shader buffer and pixel shader buffers
		// since they are no longer needed
		delete vertexBuffer.Buffer;
		delete pixelBuffer.Buffer;

		// initialize our constant buffer
		m_matrixBuffer.Initialize(device);

		// create a texture sampler state description
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 1;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		//memset(&samplerDesc.BorderColor,0,sizeof(FLOAT)*4);
		samplerDesc.BorderColor[0] = 0.0f;
		samplerDesc.BorderColor[1] = 0.0f;
		samplerDesc.BorderColor[2] = 0.0f;
		samplerDesc.BorderColor[3] = 0.0f;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		// create the texture sampler
		if(FAILED(device->CreateSamplerState(&samplerDesc,&m_sampleState)))
			return false;

		return true;
	}

	void MultiTextureShader::ShutdownShader()
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
	}


	void MultiTextureShader::SetShaderParameters(Graphics* graphics,Matrix worldMatrix,Matrix viewMatrix,
		Matrix projMatrix, ID3D11ShaderResourceView** textureArray)
	{
		unsigned int bufferNumber;
		ID3D11DeviceContext* deviceContext = graphics->GetImmediateContex();;

		m_matrixBuffer.Data.World = worldMatrix.Transpose();
		m_matrixBuffer.Data.View = viewMatrix.Transpose();
		m_matrixBuffer.Data.Projection = projMatrix.Transpose();
		m_matrixBuffer.ApplyChanges(deviceContext);

		bufferNumber = 0;
		ID3D11Buffer* constBuffer[] = { m_matrixBuffer.Buffer() };
		deviceContext->VSSetConstantBuffers(bufferNumber,1,constBuffer);

		deviceContext->PSSetShaderResources(0,2,textureArray);

	}

	void MultiTextureShader::RenderShader(Graphics* graphics,int indexCount)
	{
		ID3D11DeviceContext* deviceContext = graphics->GetImmediateContex();
		deviceContext->IASetInputLayout(m_layout);

		// set the vertex and pixel shaders that will be used to render 
		deviceContext->VSSetShader(m_vertexShader,NULL,0);
		deviceContext->PSSetShader(m_pixelShader,NULL,0);

		// set the sampler state
		deviceContext->PSSetSamplers(0,1,&m_sampleState);

		// render
		deviceContext->DrawIndexed(indexCount,0,0);
	}

	void* MultiTextureShader::LoadCompiledShader(std::wstring filename,unsigned int& size)
	{
		// open the file
		std::ifstream ifs(filename, std::ios::binary);
		// make sure it is good
		if (ifs.bad() || ifs.fail())
		{
			std::wstring failmsg = L"Failed to load shader from ";
			failmsg.append(filename);
			return 0;
		}
		// get the size of the file
		ifs.seekg(0, std::ios::end);
		size = (UINT)ifs.tellg();
		// create some memory
		char* buffer = new char[size];
		// set position to the begining of the file
		ifs.seekg(0, std::ios::beg);
		// read the file
		ifs.read(buffer, size);
		// close the file
		ifs.close();
		// return the buffer
		return buffer;
	}

}