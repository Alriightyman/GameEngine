#pragma once
#include "GameScreen.h"
#include <vector>
#include <algorithm>
#include <d3d11.h>
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include "Script.h"
#include "ObjectManager.h"

namespace Engine
{

	class Graphics;

	class ScreenManager
	{
	private:
		
		std::vector<GameScreen*> screens;
		std::vector<GameScreen*> screensToUpdate;
		std::vector<GameScreen*> screensToDelete;

		InputState* input;

		DirectX::SpriteBatch* spriteBatch;
		DirectX::SpriteFont* font;
		ID3D11ShaderResourceView* blankTexture;

		Engine::Graphics* m_graphics;
		std::unique_ptr<ObjectManager> m_objectManager;

		static bool isInitialized;
		bool traceEnabled;

		bool quit;
		//
		Script* m_script;

	public:
		DirectX::SpriteBatch* SpriteBatch()const { return spriteBatch; }
		DirectX::SpriteFont* Font()const { return font; }	
		Engine::Graphics* GetGraphicsDevice() const { return m_graphics; }
		Script* GetScript() const { return m_script; }
		ObjectManager* GetObjectManager() const { return m_objectManager.get(); }
		bool TraceEnabeld()const { return traceEnabled; }
		void TraceEnabled(bool value) { traceEnabled = value; }
		bool Quit()const { return quit;}
		void Exit() { quit = true; }

	public:
		ScreenManager(Engine::Graphics* graphics);
		~ScreenManager(void);

		void Load();
		void Unload();


		void Update(float deltaTime);
		void TraceScreens();
		void Draw(float deltaTime);

		void AddScreen(GameScreen* screen, int playerIndex);
		void RemoveScreen(GameScreen* screen);
		void CleanUpScreens();
		std::vector<GameScreen*> GetScreens();
		void FadeBackBufferToBlack(float alpha);
	public:
		static void LuaBindings(luabridge::lua_State* L);
	};


}