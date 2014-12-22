#include "DemoObject.h"
#include "code/Model.h"
#include "code/InputState.h"
#include "code/Script.h"
#include "code\NormalMapShader.h"
#include <iostream>
namespace Engine
{
	bool DemoObject::isInitialized = false;
	void DemoObject::Bind(Script* script)
	{
		using namespace luabridge;
		luabridge::lua_State* L = script->GetState();
		if(!isInitialized)
		{
			isInitialized = true;
			getGlobalNamespace(L)
				.beginClass<DemoObject>("DemoObject")
					.addProperty("position",&DemoObject::GetPosition,&DemoObject::SetPosition)
					.addProperty("velocity",&DemoObject::GetVelocity,&DemoObject::SetVelocity)
					.addData<Model*>("model",&DemoObject::m_model)
					.addData<std::string>("name",&IGameObject::m_name)					
				.endClass();
		}
	}

	DemoObject::DemoObject(void) : IGameObject(),m_worldMatrix(Matrix::Identity)
	{

	}

	DemoObject::~DemoObject(void)
	{
	}

#ifdef _DEBUG
	Script* dScript;
#endif
	bool DemoObject::Initialize(Graphics* graphics, Script* script)
	{
		using namespace luabridge;
#ifdef _DEBUG
		dScript = script;
#endif
		script->LoadScript("Content/Scripts/GameObjects/DemoObject.lua");
		luabridge::lua_State* L = script->GetState();
		LuaRef f = getGlobal(L,"Init");

		if (f.isFunction())
			functions["Init"] = std::make_shared<LuaRef>(f);

		LuaRef a = getGlobal(L,"Input");

		if (a.isFunction())
			functions["Input"] = std::make_shared<LuaRef>(a);

		LuaRef u = getGlobal(L,"Update");

		if (u.isFunction())
			functions["Update"] = std::make_shared<LuaRef>(u);

		try
		{
			LuaRef r = (*functions["Init"])(this);
			std::string s(r.cast<std::string>());
			m_model = graphics->CreateModel(std::wstring(s.begin(),s.end()));

		}
		catch( LuaException const& e)
		{
			return false;
		}


		return true;
	}

	void DemoObject::Input(InputState* input)
	{
#ifdef _DEBUG
		if(input->IsNewKeyPress(DIK_R))
		{
			using namespace luabridge;
			dScript->LoadScript("Content/Scripts/GameObjects/DemoObject.lua");
			luabridge::lua_State* L = dScript->GetState();
			LuaRef f = getGlobal(L,"Init");

			if (f.isFunction())
				functions["Init"] = std::make_shared<LuaRef>(f);

			LuaRef a = getGlobal(L,"Input");

			if (a.isFunction())
				functions["Input"] = std::make_shared<LuaRef>(a);

			LuaRef u = getGlobal(L,"Update");

			if (u.isFunction())
				functions["Update"] = std::make_shared<LuaRef>(u);
			std::cout << "Reload!\n";
		}
#endif

		try
		{
			(*functions["Input"])(this,input);
		}
		catch(luabridge::LuaException const &e)
		{
			return;
		}
	}

	void DemoObject::Update(float dt)
	{
		try
		{
			(*functions["Update"])(this,dt);
		}
		catch(luabridge::LuaException const &e)
		{
			return;
		}

		m_worldMatrix = DirectX::SimpleMath::Matrix::CreateTranslation(m_position);
		
	}

	DirectX::SimpleMath::Matrix DemoObject::GetWorldMatrix() const
	{
		//using namespace DirectX::SimpleMath;
		return m_worldMatrix;
	}

	void DemoObject::Render(Graphics* graphics)
	{
		
		graphics->Render(m_model);
	}
}