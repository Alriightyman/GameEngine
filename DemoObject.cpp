#include "DemoObject.h"
#include <iostream>
#include "code/Model.h"
#include "code/InputState.h"
#include "code/Script.h"
#include "code/NormalMapShader.h"
#include "code/MathHelper.h"

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
					.addProperty("rotation",&DemoObject::GetRotation,&DemoObject::SetRotation)
					.addProperty("desiredDirection",&DemoObject::GetDesiredDirection,&DemoObject::SetDesiredDirection)					
					.addData<Model*>("model",&DemoObject::m_model)
					.addData<DirectX::SimpleMath::Matrix>("worldMatrix",&DemoObject::m_worldMatrix)
					.addData<DirectX::SimpleMath::Vector3>("direction",&DemoObject::m_direction)
					.addProperty("name",&DemoObject::GetName,&DemoObject::SetName)
					.addProperty("isAlive",&DemoObject::IsAlive,&DemoObject::SetAlive)
				.endClass();
		}
	}

	DemoObject::DemoObject(std::wstring name) : GameObject(name)
	{
		m_worldMatrix = Matrix::Identity;
	}

	DemoObject::~DemoObject(void)
	{
		if(m_model)
		{
			m_model->Shutdown();
			delete m_model;
			m_model = 0;
		}
	}

#ifdef _DEBUG
	Script* dScript;
#endif
	bool DemoObject::Initialize(Graphics* graphics,std::string scriptfilename, Script* script)
	{
		using namespace luabridge;
#ifdef _DEBUG
		dScript = script;
#endif
		scriptfilename = "Content/Scripts/GameObjects/" + scriptfilename;
		script->LoadScript(scriptfilename.c_str());
		luabridge::lua_State* L = script->GetState();
		luabridge::LuaRef f = getGlobal(L,"Init");

		if (f.isFunction())
			functions["Init"] = std::make_shared<luabridge::LuaRef>(f);

		luabridge::LuaRef a = getGlobal(L,"Input");

		if (a.isFunction())
			functions["Input"] = std::make_shared<luabridge::LuaRef>(a);

		luabridge::LuaRef u = getGlobal(L,"Update");

		if (u.isFunction())
			functions["Update"] = std::make_shared<luabridge::LuaRef>(u);

		try
		{
			luabridge::LuaRef r = (*functions["Init"])(this);
			std::string s(r.cast<std::string>());
			m_model = graphics->CreateModel(std::wstring(s.begin(),s.end()));

		}
		catch( LuaException const& e)
		{
			std::cout << "DemoObject::Initialize() LuaException!\n" << e.what() << std::endl;
			return false;
		}


		return true;
	}

	void DemoObject::Input(InputState* input,int currentIndex)
	{
#ifdef _DEBUG
		if(input->IsNewKeyPress(DIK_R))
		{
			using namespace luabridge;
			dScript->LoadScript("Content/Scripts/GameObjects/DemoObject.lua");
			luabridge::lua_State* L = dScript->GetState();
			luabridge::LuaRef f = getGlobal(L,"Init");

			if (f.isFunction())
				functions["Init"] = std::make_shared<luabridge::LuaRef>(f);

			luabridge::LuaRef a = getGlobal(L,"Input");

			if (a.isFunction())
				functions["Input"] = std::make_shared<luabridge::LuaRef>(a);

			luabridge::LuaRef u = getGlobal(L,"Update");

			if (u.isFunction())
				functions["Update"] = std::make_shared<luabridge::LuaRef>(u);
			std::cout << "Reload!\n";
		}
#endif
		int playerIndex = 0;

		try
		{
			(*functions["Input"])(this, input,currentIndex,playerIndex);

		}
		catch(luabridge::LuaException const &e)
		{
			std::cout << "DemoObject::Input LuaException!\n" << e.what() << std::endl;
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
			std::cout << "DemoObject::Update LuaException!\n" << e.what() << std::endl;
			return;
		}		
	}


	void DemoObject::Render(Graphics* graphics)
	{
		
		graphics->Render(m_model);
	}

	ShaderType DemoObject::GetShaderType() { return m_model->GetShaderType(); }
}