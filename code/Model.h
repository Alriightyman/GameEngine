#pragma once


#include <d3d11.h>
#include <SimpleMath.h>
#include <fstream>
#include "texture.h"
#include "Shader.h"
using namespace DirectX::SimpleMath;

/*
| Model format is an extension of the obj format.
| The ascii format is the same as the original obj format,
| but now has added info for animation data. The obx format is
| just the same as the ascii format except it is in binary form.
*/
namespace Engine
{

	class Model
	{
	private:

		struct StaticVertex
		{
			Vector3 position;
			Vector2 texture;
			Vector3 normal;

			StaticVertex()
			{
				position = Vector3(0,0,0);
				texture = Vector2(0,0);
				normal = Vector3(0,0,0);
			}
		};

		struct Vertex
		{
			Vector3 position;
			Vector2 texture;
			Vector3 normal;
			Vector4 weight;
			Vector4 weightIndex;

			Vertex()
			{
				position = Vector3(0,0,0);
				texture = Vector2(0,0);
				normal = Vector3(0,0,0);
			}
		};

		struct ModelData
		{
			float x,y,z;
			float tu,tv;
			float nx,ny,nz;
		};

	private:
		ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
		int m_vertexCount, m_indexCount;
		Texture* m_Textures;
		ModelData* m_model;

	public:
		Model();
		Model(const Model&);
		~Model();

		bool Initialize(Graphics* graphics,std::wstring modelFilename, Texture* texture);
		void Shutdown();
		void Render(Graphics* graphics, Shader* shader);

		int GetIndexCount();
		ID3D11ShaderResourceView** GetTextures();


	private:
		bool InitializeBuffers(Graphics*);
		void ShutdownBuffers();
		void RenderBuffers(Graphics*);

		bool LoadModel(std::wstring filename);
		void ReleaseModel();

		//bool LoadTextures(Graphics*, std::wstring,std::wstring);
		void ReleaseTextures();
	};






	//////////////
	// INCLUDES //
	//////////////
	//#include <SimpleMath.h>
#include <stdlib.h>
#include <time.h>


	///////////////////////////////////////////////////////////////////////////////
	// Class name: ModelListClass
	///////////////////////////////////////////////////////////////////////////////
	class ModelListClass
	{
	private:
		struct ModelInfoType
		{
			Color color;
			float positionX, positionY, positionZ;
		};

	public:
		ModelListClass();
		ModelListClass(const ModelListClass&);
		~ModelListClass();

		bool Initialize(int);
		void Shutdown();

		int GetModelCount();
		void GetData(int, float&, float&, float&, Color&);

	private:
		int m_modelCount;
		ModelInfoType* m_ModelInfoList;
	};




}