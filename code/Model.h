#pragma once


#include <d3d11.h>
#include <SimpleMath.h>
#include <fstream>
#include "texture.h"
#include "Shader.h"
#include <vector> 

using namespace DirectX::SimpleMath;

/* 
|
*/
namespace Engine
{

	class Model
	{
		// make friends with the Graphics class
		friend class Graphics;
	private:
		// static vertex buffer
		struct StaticVertex
		{
			Vector3 position;
			Vector2 texture;
			Vector3 normal;

			StaticVertex()
			{
				position	= Vector3(0,0,0);
				texture		= Vector2(0,0);
				normal		= Vector3(0,0,0);
			}
		};

		// used for normal, tangent, binormal calculation
		struct TempVertex
		{
			float x,y,z;
			float tu,tv;
			float nx,ny,nz;
		};

		struct Vertex
		{
			Vector3 position;
			Vector2 texture;
			Vector3 normal;
			Vector3 tangent;
			Vector3 binormal;
		};


		struct ModelData
		{
			float x,y,z;
			float tu,tv;
			float nx,ny,nz;
			float tx,ty,tz;
			float bx,by,bz;
		};
		struct Mesh
		{
			int Start;
			int Count;
			//std::vector<Vertex> Vertices;
			int IndexCount;
			ModelData* Model;
			ID3D11Buffer *VertexBuffer, *IndexBuffer;
			Texture* Textures;
			Mesh();
			~Mesh();
		};
	private:
		int m_vertexCount;
		std::vector<Mesh*> m_meshes;

	public:
		Model();
		Model(const Model&);
		~Model();

		void Shutdown();
	protected:
		bool Initialize(Graphics* graphics,std::wstring modelFilename, Texture* texture);
		void Render(Graphics* graphics, Shader* shader);
	private:
		bool InitializeBuffers(Graphics*);
		void ShutdownBuffers();
		void RenderBuffers(Graphics*,Mesh* mesh);

		void CalculateModelVectors();
		void CalculateTangentBinormal(TempVertex vertex1, TempVertex vertex2, TempVertex vertex3, Vector3& tangent,Vector3& binormal);
		void CalculateNormal(Vector3 tangent, Vector3 binormal, Vector3& normal);

		bool LoadModel(std::wstring filename);

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