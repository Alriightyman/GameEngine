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


	bool Model::Initialize(Graphics* graphics,std::wstring modelFilename, Texture* texture)
	{
		bool result;
		// load the model data
		result = LoadModel(modelFilename);
		if(!result)
			return false;

		// Calculate the tangent, binormal, and normals
		CalculateModelVectors();

		// Initialize the vertex and index buffers.
		result = InitializeBuffers(graphics);
		if(!result)
		{
			return false;
		}

		// Load the texture for this model.
		m_Textures = texture;

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


	Texture* Model::GetTextures()
	{
		return m_Textures;
	}


	bool Model::InitializeBuffers(Graphics* graphics)
	{
		Vertex* vertices;
		unsigned long* indices;
		D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
		D3D11_SUBRESOURCE_DATA vertexData, indexData;
		HRESULT result;
		ID3D11Device* device = graphics->GetDevice();

		// Create the vertex array.
		vertices = new Vertex[m_vertexCount];
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
			vertices[i].tangent = Vector3(m_model[i].tx, m_model[i].ty, m_model[i].tz);
			vertices[i].binormal = Vector3(m_model[i].bx, m_model[i].by, m_model[i].bz);

			indices[i] = i;
		}


		// Set up the description of the static vertex buffer.
		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.ByteWidth = sizeof(Vertex) * m_vertexCount;
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
		stride = sizeof(Vertex); 
		offset = 0;

		// Set the vertex buffer to active in the input assembler so it can be rendered.
		deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

		// Set the index buffer to active in the input assembler so it can be rendered.
		deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

		// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		return;
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

	bool Model::LoadModel(std::wstring filename)
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

	void Model::CalculateModelVectors()
	{
		int faceCount, index;
		TempVertex v1,v2,v3;
		Vector3 tangent,binormal,normal;

		// calculate the number of faces
		faceCount = m_vertexCount / 3;

		// initializee the index to the model data
		index = 0;
		// go through all the faces and calculate the tangent, binormal, and normal vectors
		for(int i = 0; i < faceCount; i++)
		{
			// get the three vertices for this face
			v1.x = m_model[index].x;
			v1.y = m_model[index].y;
			v1.z = m_model[index].z;
			v1.tu = m_model[index].tu;
			v1.tv = m_model[index].tv;
			v1.nx = m_model[index].nx;
			v1.ny = m_model[index].ny;
			v1.nz = m_model[index].nz;
			index++;

			v2.x = m_model[index].x;
			v2.y = m_model[index].y;
			v2.z = m_model[index].z;
			v2.tu = m_model[index].tu;
			v2.tv = m_model[index].tv;
			v2.nx = m_model[index].nx;
			v2.ny = m_model[index].ny;
			v2.nz = m_model[index].nz;
			index++;

			v3.x = m_model[index].x;
			v3.y = m_model[index].y;
			v3.z = m_model[index].z;
			v3.tu = m_model[index].tu;
			v3.tv = m_model[index].tv;
			v3.nx = m_model[index].nx;
			v3.ny = m_model[index].ny;
			v3.nz = m_model[index].nz;
			index++;

			// calculate the tangent and binormals
			CalculateTangentBinormal(v1,v2,v3,tangent,binormal);

			// calculate the normals based on the tangents and binormals
			CalculateNormal(tangent,binormal,normal);

			m_model[index - 1].nx = normal.x;
			m_model[index - 1].ny = normal.y;
			m_model[index - 1].nz = normal.z;
			m_model[index - 1].tx = tangent.x;
			m_model[index - 1].ty = tangent.y;
			m_model[index - 1].tz = tangent.z;
			m_model[index - 1].bx = binormal.x;
			m_model[index - 1].by = binormal.y;
			m_model[index - 1].bz = binormal.z;

			m_model[index - 2].nx = normal.x;
			m_model[index - 2].ny = normal.y;
			m_model[index - 2].nz = normal.z;
			m_model[index - 2].tx = tangent.x;
			m_model[index - 2].ty = tangent.y;
			m_model[index - 2].tz = tangent.z;
			m_model[index - 2].bx = binormal.x;
			m_model[index - 2].by = binormal.y;
			m_model[index - 2].bz = binormal.z;

			m_model[index - 3].nx = normal.x;
			m_model[index - 3].ny = normal.y;
			m_model[index - 3].nz = normal.z;
			m_model[index - 3].tx = tangent.x;
			m_model[index - 3].ty = tangent.y;
			m_model[index - 3].tz = tangent.z;
			m_model[index - 3].bx = binormal.x;
			m_model[index - 3].by = binormal.y;
			m_model[index - 3].bz = binormal.z;
		}
	}

	void Model::CalculateTangentBinormal(TempVertex v1,TempVertex v2,TempVertex v3, Vector3& tangent,Vector3& binormal)
	{
		float vector1[3], vector2[3];
		float tuVector[2], tvVector[2];
		float den;
		float length;

		// caclulate the two vectors for this face
		vector1[0] = v2.x - v1.x;
		vector1[1] = v2.y - v1.y;
		vector1[2] = v2.z - v1.z;

		vector2[0] = v3.x - v1.x;
		vector2[1] = v3.y - v1.y;
		vector2[2] = v3.z - v1.z;

		// calculate the tu and tv texture space vectors
		tuVector[0] = v2.tu - v1.tu;
		tvVector[0] = v2.tv - v1.tv;

		tuVector[1] = v3.tu - v1.tu;
		tvVector[1] = v3.tv - v1.tv;

		// calculate the denominator of the tangent/binormal equation
		den = 1.0f/ (tuVector[0] * tvVector[1] - tuVector[1]* tvVector[0]);

		// Calculate the cross products and multiply by the coefficient to get the tangent and binormal.
		tangent.x = (tvVector[1] * vector1[0] - tvVector[0] * vector2[0]) * den;
		tangent.y = (tvVector[1] * vector1[1] - tvVector[0] * vector2[1]) * den;
		tangent.z = (tvVector[1] * vector1[2] - tvVector[0] * vector2[2]) * den;

		binormal.x = (tuVector[0] * vector2[0] - tuVector[1] * vector1[0]) * den;
		binormal.y = (tuVector[0] * vector2[1] - tuVector[1] * vector1[1]) * den;
		binormal.z = (tuVector[0] * vector2[2] - tuVector[1] * vector1[2]) * den;

		// Calculate the length of this normal.
		length = sqrt((tangent.x * tangent.x) + (tangent.y * tangent.y) + (tangent.z * tangent.z));
			
		// Normalize the normal and then store it
		tangent.x = tangent.x / length;
		tangent.y = tangent.y / length;
		tangent.z = tangent.z / length;

		// Calculate the length of this normal.
		length = sqrt((binormal.x * binormal.x) + (binormal.y * binormal.y) + (binormal.z * binormal.z));
			
		// Normalize the normal and then store it
		binormal.x = binormal.x / length;
		binormal.y = binormal.y / length;
		binormal.z = binormal.z / length;

	}

	void Model::CalculateNormal(Vector3 tangent, Vector3 binormal, Vector3& normal)
	{
		float length = 0;

		// calculate the cross product of the tangent and binormal to get normal
		normal = tangent.Cross(binormal);

		// normalize the normal
		normal.Normalize();
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