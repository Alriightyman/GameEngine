////////////////////////////////////////////////////////////////////////////////
// Filename: Model.cpp
////////////////////////////////////////////////////////////////////////////////
#include "model.h"

namespace Engine
{

	Model::Model()
	{
		m_vertexBuffer = 0;
		m_indexBuffer = 0;
		m_Textures = 0;
		m_model = 0;
	}


	Model::Model(const Model& other)
	{
	}


	Model::~Model()
	{
	}


	bool Model::Initialize(Graphics* graphics,char* modelFilename, std::wstring textureFilename1,std::wstring textureFilename2)
	{
		bool result;
		// load the model data
		result = LoadModel(modelFilename);
		if(!result)
			return false;
		// Initialize the vertex and index buffers.
		result = InitializeBuffers(graphics);
		if(!result)
		{
			return false;
		}

		// Load the texture for this model.
		result = LoadTextures(graphics, textureFilename1,textureFilename2);
		if(!result)
		{
			return false;
		}

		return true;
	}


	void Model::Shutdown()
	{
		// Release the model texture.
		ReleaseTextures();

		// Shutdown the vertex and index buffers.
		ShutdownBuffers();

		// release the model data
		ReleaseModel();

		return;
	}


	void Model::Render(Graphics* graphics,Shader* shader)
	{
		// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
		RenderBuffers(graphics);

		if(shader != nullptr)
			shader->Render(graphics,m_indexCount);
	}


	int Model::GetIndexCount()
	{
		return m_indexCount;
	}


	ID3D11ShaderResourceView** Model::GetTextures()
	{
		return m_Textures->GetTextures();
	}


	bool Model::InitializeBuffers(Graphics* graphics)
	{
		StaticVertex* vertices;
		unsigned long* indices;
		D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
		D3D11_SUBRESOURCE_DATA vertexData, indexData;
		HRESULT result;
		ID3D11Device* device = graphics->GetDevice();

		// Create the vertex array.
		vertices = new StaticVertex[m_vertexCount];
		if(!vertices)
		{
			return false;
		}

		// Create the index array.
		indices = new unsigned long[m_indexCount];
		if(!indices)
		{
			return false;
		}

		// Load the vertex array with data.
		for (int i = 0; i < m_vertexCount; i++)
		{
			vertices[i].position = Vector3(m_model[i].x, m_model[i].y, m_model[i].z); 
			vertices[i].texture = Vector2(m_model[i].tu, m_model[i].tv);
			vertices[i].normal = Vector3(m_model[i].nx, m_model[i].ny, m_model[i].nz);

			indices[i] = i;
		}


		// Set up the description of the static vertex buffer.
		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.ByteWidth = sizeof(StaticVertex) * m_vertexCount;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.MiscFlags = 0;
		vertexBufferDesc.StructureByteStride = 0;

		// Give the subresource structure a pointer to the vertex data.
		vertexData.pSysMem = vertices;
		vertexData.SysMemPitch = 0;
		vertexData.SysMemSlicePitch = 0;

		// Now create the vertex buffer.
		result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
		if(FAILED(result))
		{
			return false;
		}

		// Set up the description of the static index buffer.
		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.CPUAccessFlags = 0;
		indexBufferDesc.MiscFlags = 0;
		indexBufferDesc.StructureByteStride = 0;

		// Give the subresource structure a pointer to the index data.
		indexData.pSysMem = indices;
		indexData.SysMemPitch = 0;
		indexData.SysMemSlicePitch = 0;

		// Create the index buffer.
		result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
		if(FAILED(result))
		{
			return false;
		}

		// Release the arrays now that the vertex and index buffers have been created and loaded.
		delete [] vertices;
		vertices = 0;

		delete [] indices;
		indices = 0;

		return true;
	}


	void Model::ShutdownBuffers()
	{
		// Release the index buffer.
		if(m_indexBuffer)
		{
			m_indexBuffer->Release();
			m_indexBuffer = 0;
		}

		// Release the vertex buffer.
		if(m_vertexBuffer)
		{
			m_vertexBuffer->Release();
			m_vertexBuffer = 0;
		}

		return;
	}


	void Model::RenderBuffers(Graphics* graphics)
	{
		unsigned int stride;
		unsigned int offset;

		ID3D11DeviceContext* deviceContext = graphics->GetImmediateContex();
		// Set vertex buffer stride and offset.
		stride = sizeof(StaticVertex); 
		offset = 0;

		// Set the vertex buffer to active in the input assembler so it can be rendered.
		deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

		// Set the index buffer to active in the input assembler so it can be rendered.
		deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

		// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		return;
	}


	bool Model::LoadTextures(Graphics* graphics,std::wstring filename1,std::wstring filename2)
	{
		bool result;


		// Create the texture object.
		m_Textures = new Texture();
		if(!m_Textures)
		{
			return false;
		}

		// Initialize the texture object.
		result = m_Textures->Initialize(graphics, filename1,filename2);
		if(!result)
		{
			return false;
		}

		return true;
	}


	void Model::ReleaseTextures()
	{
		// Release the texture object.
		if(m_Textures)
		{
			m_Textures->Shutdown();
			delete m_Textures;
			m_Textures = 0;
		}

		return;
	}

	bool Model::LoadModel(char* filename)
	{
		std::ifstream fin;
		char input;

		// open the model file
		fin.open(filename);

		// if it could not open the file then exit
		if(fin.fail())
			return false;

		fin.get(input);

		while(input != ':')
			fin.get(input);

		// read the vertex count

		fin >> m_vertexCount;

		// set the number of indices to be the same as the vertex count
		m_indexCount = m_vertexCount;

		// create the model using the vertex count that was read in
		m_model = new ModelData[m_vertexCount];

		if(!m_model)
			return false;

		// read up to the beginning of the data
		fin.get(input);

		while(input != ':')
			fin.get(input);
		fin.get(input);
		fin.get(input);

		// read the vertex data
		for(int i = 0; i < m_vertexCount; i++)
		{
			fin >> m_model[i].x >> m_model[i].y >> m_model[i].z;
			fin >> m_model[i].tu >> m_model[i].tv;
			fin >> m_model[i].nx >> m_model[i].ny >> m_model[i].nz;
		}

		fin.close();

		return true;

	}


	void Model::ReleaseModel()
	{
		if(m_model)
		{
			delete[] m_model;
			m_model = 0;
		}
	}

#pragma region MODEL_LIST
	/////////////////////////////////////////////////////
	/////////// MODEL LIST CLASS ///////////////////////
	///////////////////////////////////////////////////


	ModelListClass::ModelListClass()
	{
		m_ModelInfoList = 0;
	}


	ModelListClass::ModelListClass(const ModelListClass& other)
	{
	}


	ModelListClass::~ModelListClass()
	{
	}


	bool ModelListClass::Initialize(int numModels)
	{
		int i;
		float red, green, blue;

		// Store the number of models.
		m_modelCount = numModels;

		// Create a list array of the model information.
		m_ModelInfoList = new ModelInfoType[m_modelCount];
		if(!m_ModelInfoList)
		{
			return false;
		}
		// Seed the random generator with the current time.
		srand((unsigned int)time(NULL));

		// Go through all the models and randomly generate the model color and position.
		for(i=0; i<m_modelCount; i++)
		{
			// Generate a random color for the model.
			red = (float)rand() / RAND_MAX;
			green = (float)rand() / RAND_MAX;
			blue = (float)rand() / RAND_MAX;

			m_ModelInfoList[i].color = Color(red, green, blue, 1.0f);

			// Generate a random position in front of the viewer for the mode.
			m_ModelInfoList[i].positionX = (((float)rand()-(float)rand())/RAND_MAX) * 10.0f;
			m_ModelInfoList[i].positionY = (((float)rand()-(float)rand())/RAND_MAX) * 10.0f;
			m_ModelInfoList[i].positionZ = ((((float)rand()-(float)rand())/RAND_MAX) * 10.0f) + 5.0f;
		}

		return true;
	}

	void ModelListClass::Shutdown()
	{
		// Release the model information list.
		if(m_ModelInfoList)
		{
			delete [] m_ModelInfoList;
			m_ModelInfoList = 0;
		}

		return;
	}

	int ModelListClass::GetModelCount()
	{
		return m_modelCount;
	}

	void ModelListClass::GetData(int index, float& positionX, float& positionY, float& positionZ, Color& color)
	{
		positionX = m_ModelInfoList[index].positionX;
		positionY = m_ModelInfoList[index].positionY;
		positionZ = m_ModelInfoList[index].positionZ;

		color = m_ModelInfoList[index].color;

		return;
	} 
#pragma endregion

}