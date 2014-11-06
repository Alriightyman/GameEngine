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
		void PlayGameMenuEntrySelected(void* sender,PlayerIndex playerIndex);
		void OptionsMenuSelected(void* sender,PlayerIndex playerIndex);
		void OnCancel(void* sender,PlayerIndex playerIndex);
		void ConfirmExitMessageBoxAccepted(void* sender,PlayerIndex playerIndex);
#pragma endregion
	};

}