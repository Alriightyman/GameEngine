#include "MainMenuScreen.h"
#include "ScreenManager.h"
#include "Debug.h"

#include "LoadingScreen.h"
#include "GameplayScreen.h"
#include "MessageBoxScreen.h"
#include "OptionsMenuScreen.h"
namespace Engine
{

	MainMenuScreen::MainMenuScreen(void) : MenuScreen("Main Menu")
	{
		debug.Print("MainMenuScreen Loaded!");

		// create out menu entries 
		MenuEntry* playGameMenuEntry = new MenuEntry("Play Game");
		MenuEntry* optionsMenuEntry = new MenuEntry("Options");
		MenuEntry* exitMenuEntry = new MenuEntry("Exit");

		using std::placeholders::_1;
		using std::placeholders::_2;
		playGameMenuEntry->Selected = std::bind(&MainMenuScreen::PlayGameMenuEntrySelected,this,_1,_2);
		optionsMenuEntry->Selected = std::bind(&MainMenuScreen::OptionsMenuSelected,this,_1,_2);
		exitMenuEntry->Selected = std::bind(&MainMenuScreen::OnCancel,this,_1,_2);

		m_MenuEntries.push_back(playGameMenuEntry);
		m_MenuEntries.push_back(optionsMenuEntry);
		m_MenuEntries.push_back(exitMenuEntry);

	}

	MainMenuScreen::~MainMenuScreen(void)
	{

	}

	void MainMenuScreen::PlayGameMenuEntrySelected(void* sender,int playerIndex)
	{
		std::vector<GameScreen*> gameplayScreen;
		gameplayScreen.push_back(new GameplayScreen());
		LoadingScreen::Load(m_ScreenManager,true,playerIndex,gameplayScreen);
	}

	void MainMenuScreen::OptionsMenuSelected(void* sender,int playerIndex)
	{
		m_ScreenManager->AddScreen(new OptionsMenuScreen(m_ScreenManager->GetScript()),playerIndex);
	}

	void MainMenuScreen::OnCancel(void* sender,int playerIndex)
	{
		const std::string message = "Are you sure you want to exit this sample?";
		MessageBoxScreen* confirmExitMessageBox = new MessageBoxScreen(message);
		using std::placeholders::_1;
		using std::placeholders::_2;
		confirmExitMessageBox->Accepted = std::bind(&MainMenuScreen::ConfirmExitMessageBoxAccepted,this,_1,_2);

		m_ScreenManager->AddScreen(confirmExitMessageBox,playerIndex);
	}

	void MainMenuScreen::ConfirmExitMessageBoxAccepted(void* sender,int playerIndex)
	{
		m_ScreenManager->Exit();
	}

}