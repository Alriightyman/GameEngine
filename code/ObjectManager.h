#pragma once

#include "GameObject.h"
#include <list>

// macros
#define MAIN_PLAYER 0

namespace DirectX
{
	namespace SimpleMath
	{
		struct Matrix;
	}
}

namespace Engine
{
	
	class Frustum;

	class ObjectManager
	{
	public: // static meths
		static void Bind(Script* script);
		static bool isInitialized;

	public:
		ObjectManager(Graphics* graphics);
		~ObjectManager();
		void Clear();

		template<typename T>
		T* CreateObject(Script* script, std::string scriptfilename, std::wstring name)
		{
			T* g = new T(name);
			if(!g->Initialize(m_graphics,scriptfilename,script))
				return nullptr;

			m_objects.push_back(static_cast<GameObject*>(g));
			return g;
		}
		
		void Update(float deltaTime, Frustum* frustum);
		void Render(DirectX::SimpleMath::Matrix& world,DirectX::SimpleMath::Matrix& view, DirectX::SimpleMath::Matrix& proj);
		GameObject* GetGameObject(int index);

	private:
		// prevent copying
		ObjectManager(ObjectManager& o) {};

		Graphics* m_graphics;
		std::list<GameObject*> m_objects;

	};

}