#pragma once
#include "menuscreen.h"
namespace Engine
{
	class PauseMenuScreen :
		public MenuScreen
	{
	public:
		PauseMenuScreen(void);
		~PauseMenuScreen(void);

		void QuitGameMenuEntrySelected(void* sender,int playerIndex);
		void ConfirmQuitMessageBoxAccepted(void* sender,int playerIndex);
	};

}
