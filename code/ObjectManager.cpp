#include "ObjectManager.h"
#include "../DemoObject.h"
#include <SimpleMath.h>
#include "Graphics.h"
#include "NormalMapShader.h"
#include "Frustum.h"

namespace Engine
{
	typedef std::list<GameObject*>::iterator GameObjectIter;

	bool ObjectManager::isInitialized = false;

	void ObjectManager::Bind(Script* script)
	{

	}

	ObjectManager::ObjectManager(Graphics* graphics) : m_graphics(graphics)
	{

	}
	// release objects
	ObjectManager::~ObjectManager()
	{
		Clear();
	}

	void ObjectManager::Clear()
	{
		while(!m_objects.empty())
		{
			GameObject* o = m_objects.back();
			if (o)
			{
				delete o;
				o = 0;
				m_objects.pop_back();
			}
		}

	}
	

	void ObjectManager::Update(float dt,Frustum* frustum)
	{
		for (GameObjectIter obj = m_objects.begin(); obj != m_objects.end(); obj++)
		{
			(*obj)->Update(dt);

			// TODO check frustum to object.

			if( !(*obj)->IsAlive() )
				m_objects.remove((*obj));
		}
	}

	void ObjectManager::Render(DirectX::SimpleMath::Matrix& world,DirectX::SimpleMath::Matrix& view, DirectX::SimpleMath::Matrix& proj)
	{
		for (GameObjectIter obj = m_objects.begin(); obj != m_objects.end(); obj++)
		{
			if( (*obj)->IsOnScreen() )
			{
				world = (*obj)->GetWorldMatrix();
				NormalMapShader* normalMap = m_graphics->GetNormalMapShader();
				normalMap->SetWorldViewProjMatrices(world,view,proj);
				(*obj)->Render(m_graphics);
			}
		}
	}

	GameObject* ObjectManager::GetGameObject(int index)
	{
		GameObjectIter iter = std::next(m_objects.begin() , index);

		return (*iter);
	}

}