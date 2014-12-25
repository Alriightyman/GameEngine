#pragma once
#include "gamescreen.h"
#include <vector>
namespace Engine
{

	class ScreenManager;
	/// <summary>
	/// The loading screen coordinates transitions between the menu system and the
	/// game itself. Normally one screen will transition off at the same time as
	/// the next screen is transitioning on, but for larger transitions that can
	/// take a longer time to load their data, we want the menu system to be entirely
	/// gone before we start loading the game. This is done as follows:
	/// 
	/// - Tell all the existing screens to transition off.
	/// - Activate a loading screen, which will transition on at the same time.
	/// - The loading screen watches the state of the previous screens.
	/// - When it sees they have finished transitioning off, it activates the real
	///   next screen, which may take a long time to load its data. The loading
	///   screen will be the only thing displayed while this load is taking place.
	/// </summary>

	class LoadingScreen :
		public GameScreen
	{
#pragma region Fields

		bool m_LoadingIsSlow;
		bool m_OtherScreensAreGone;
		std::vector<GameScreen*> m_ScreensToLoad;

#pragma endregion

#pragma region Initialization

	protected:
		LoadingScreen(Engine::ScreenManager* m, bool m_LoadingIsSlow,std::vector<GameScreen*>& screenToLoad);

	public:
		~LoadingScreen(void);
		static void Load(Engine::ScreenManager* screenManager,bool loadingIsSlow,int& playerIndex,std::vector<GameScreen*>& screensToLoad);
#pragma endregion

#pragma region Update and Draw
		void Update(float deltaTime,bool otherScreenHasFocus,bool coveredByOtherScreen);
		void Draw(float deltaTime);
#pragma endregion
	};


}