#include "PauseMenuScreen.h"
#include "MessageBoxScreen.h"
#include "ScreenManager.h"
#include "LoadingScreen.h"
#include "BackgroundScreen.h"
#include "MainMenuScreen.h"
#include "Debug.h"
namespace Engine
{

	PauseMenuScreen::PauseMenuScreen(void) : MenuScreen("Paused")
	{
		debug.Print("PauseMenuScreen Loaded");

		MenuEntry* resumeGameMenuEntry = new MenuEntry("Resume Game");
		MenuEntry* quitGameMenuEntry = new MenuEntry("Quit Game");
		using namespace std::placeholders;
		resumeGameMenuEntry->Selected = std::bind(&PauseMenuScreen::OnCancel,this,_1,_2);
		quitGameMenuEntry->Selected = std::bind(&PauseMenuScreen::QuitGameMenuEntrySelected,this,_1,_2);

		m_MenuEntries.push_back(resumeGameMenuEntry);
		m_MenuEntries.push_back(quitGameMenuEntry);
	}


	PauseMenuScreen::~PauseMenuScreen(void)
	{

	}


	void PauseMenuScreen::QuitGameMenuEntrySelected(void* sender,PlayerIndex playerIndex)
	{
		const std::string message = "Are you sure you want to quit this game?";
		MessageBoxScreen* confirmQuitMessageBox = new MessageBoxScreen(message);

		using namespace std::placeholders;
		confirmQuitMessageBox->Accepted = std::bind(&PauseMenuScreen::ConfirmQuitMessageBoxAccepted,this,_1,_2);

		m_ScreenManager->AddScreen(confirmQuitMessageBox,playerIndex);
	}
	void PauseMenuScreen::ConfirmQuitMessageBoxAccepted(void* sender,PlayerIndex playerIndex)
	{
		std::vector<GameScreen*> screensToLoad; 
		screensToLoad.push_back( new BackgroundScreen());
		screensToLoad.push_back( new MainMenuScreen());
		LoadingScreen::Load(m_ScreenManager, false,playerIndex,screensToLoad);
	}

}