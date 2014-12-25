#pragma once
#include "menuscreen.h"
namespace Engine
{
	class MainMenuScreen :
		public MenuScreen
	{
#pragma region Initialization
	public:
		MainMenuScreen(void);
		~MainMenuScreen(void);
#pragma endregion

#pragma region Handle Input
		void PlayGameMenuEntrySelected(void* sender,int playerIndex);
		void OptionsMenuSelected(void* sender,int playerIndex);
		void OnCancel(void* sender,int playerIndex);
		void ConfirmExitMessageBoxAccepted(void* sender,int playerIndex);
#pragma endregion
	};

}