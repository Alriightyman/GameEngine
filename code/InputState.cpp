#include "InputState.h"

using namespace DirectX;
namespace Engine
{

	// binds the input into lua
	void InputState::Bind(luabridge::lua_State* L)
	{
		using namespace luabridge;
		
		getGlobalNamespace(L)
			.beginClass<InputState>("Input")
				.addFunction("IsKeyDown",&InputState::IsKeyDown)
				.addFunction("IsNewKeyPress",&InputState::IsNewKeyPress)
			.endClass();
	}


	InputState::InputState(HWND hwnd) : MAXINPUTS(4)
	{
		DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, 
			(void**)&m_DInput, NULL);

		m_DInput->CreateDevice(GUID_SysKeyboard, &m_Keyboard, NULL);

		m_Keyboard->SetDataFormat(&c_dfDIKeyboard);
		m_Keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND|DISCL_NONEXCLUSIVE);
		m_Keyboard->Acquire();

		// get a pointer
		m_gamepad.reset( new GamePad() );

		memset(m_gamePadWasConnected,false,4);
	}


	InputState::~InputState(void)
	{
		m_Keyboard->Unacquire();
		m_Keyboard->Release();
		m_DInput->Release();
	}

	void InputState::Update()
	{
		memcpy(m_PreviousKeyboardState,m_CurrentKeyboardState,256);
		m_Keyboard->Poll();
		if(!SUCCEEDED(m_Keyboard->GetDeviceState(sizeof(m_CurrentKeyboardState),(LPVOID)&m_CurrentKeyboardState)))
		{
			m_Keyboard->Acquire();
		}

		for(int i = 0; i < GamePad::MAX_PLAYER_COUNT; i++)
		{
			m_prevGamePadState[i] = m_currentGamepadState[i];
			m_currentGamepadState[i] = m_gamepad->GetState(i);
		}
	}

	bool InputState::IsNewKeyPress(int key)
	{
		return (bool)(m_CurrentKeyboardState[key] & 0x80 &&
			!(bool)(m_PreviousKeyboardState[key] & 0x80));
	}

	bool InputState::IsKeyDown(int key)
	{
		return (bool)(m_CurrentKeyboardState[key] & 0x80);
	}

	bool InputState::IsNewAButtonPress(PlayerIndex controllingPlayer, PlayerIndex& playerIndex)
	{
		if(controllingPlayer != PlayerIndex::Null && controllingPlayer >= PlayerIndex::One && controllingPlayer < PlayerIndex::Four + 1)
		{
			playerIndex = controllingPlayer;

			int i = (int)playerIndex;
			return (m_currentGamepadState[i].IsAPressed() &&
				!m_prevGamePadState[i].IsAPressed());
		}
		else
		{
			return (IsNewAButtonPress(PlayerIndex::One,playerIndex) ||
				IsNewAButtonPress(PlayerIndex::Two,playerIndex) ||
				IsNewAButtonPress(PlayerIndex::Three,playerIndex) ||
				IsNewAButtonPress(PlayerIndex::Four,playerIndex));
		}
	}

	bool InputState::IsNewBButtonPress(PlayerIndex controllingPlayer, PlayerIndex& playerIndex)
	{
		if(controllingPlayer != PlayerIndex::Null && controllingPlayer >= PlayerIndex::One && controllingPlayer < PlayerIndex::Four + 1)
		{
			playerIndex = controllingPlayer;

			int i = (int)playerIndex;
			return (m_currentGamepadState[i].IsBPressed() &&
				!m_prevGamePadState[i].IsBPressed());
		}
		else
		{
			return (IsNewBButtonPress(PlayerIndex::One,playerIndex) ||
				IsNewBButtonPress(PlayerIndex::Two,playerIndex) ||
				IsNewBButtonPress(PlayerIndex::Three,playerIndex) ||
				IsNewBButtonPress(PlayerIndex::Four,playerIndex));
		}
	}

	bool InputState::IsNewXButtonPress(PlayerIndex controllingPlayer, PlayerIndex& playerIndex)
	{
		if(controllingPlayer != PlayerIndex::Null && controllingPlayer >= PlayerIndex::One && controllingPlayer < PlayerIndex::Four + 1)
		{
			playerIndex = controllingPlayer;

			int i = (int)playerIndex;
			return (m_currentGamepadState[i].IsXPressed() &&
				!m_prevGamePadState[i].IsXPressed());
		}
		else
		{
			return (IsNewXButtonPress(PlayerIndex::One,playerIndex) ||
				IsNewXButtonPress(PlayerIndex::Two,playerIndex) ||
				IsNewXButtonPress(PlayerIndex::Three,playerIndex) ||
				IsNewXButtonPress(PlayerIndex::Four,playerIndex));
		}
	}

	bool InputState::IsNewYButtonPress(PlayerIndex controllingPlayer, PlayerIndex& playerIndex)
	{
		if(controllingPlayer != PlayerIndex::Null && controllingPlayer >= PlayerIndex::One && controllingPlayer < PlayerIndex::Four + 1)
		{
			playerIndex = controllingPlayer;

			int i = (int)playerIndex;
			return (m_currentGamepadState[i].IsYPressed() &&
				!m_prevGamePadState[i].IsYPressed());
		}
		else
		{
			return (IsNewYButtonPress(PlayerIndex::One,playerIndex) ||
				IsNewYButtonPress(PlayerIndex::Two,playerIndex) ||
				IsNewYButtonPress(PlayerIndex::Three,playerIndex) ||
				IsNewYButtonPress(PlayerIndex::Four,playerIndex));
		}
	}

	bool InputState::IsNewStartButtonPress(PlayerIndex controllingPlayer, PlayerIndex& playerIndex)
	{
		if(controllingPlayer != PlayerIndex::Null && controllingPlayer >= PlayerIndex::One && controllingPlayer < PlayerIndex::Four + 1)
		{
			playerIndex = controllingPlayer;

			int i = (int)playerIndex;
			return (m_currentGamepadState[i].IsStartPressed() &&
				!m_prevGamePadState[i].IsStartPressed());
		}
		else
		{
			return (IsNewStartButtonPress(PlayerIndex::One,playerIndex) ||
				IsNewStartButtonPress(PlayerIndex::Two,playerIndex) ||
				IsNewStartButtonPress(PlayerIndex::Three,playerIndex) ||
				IsNewStartButtonPress(PlayerIndex::Four,playerIndex));
		}
	}

	bool InputState::IsNewBackButtonPress(PlayerIndex controllingPlayer, PlayerIndex& playerIndex)
	{
		if(controllingPlayer != PlayerIndex::Null && controllingPlayer >= PlayerIndex::One && controllingPlayer < PlayerIndex::Four + 1)
		{
			playerIndex = controllingPlayer;

			int i = (int)playerIndex;
			return (m_currentGamepadState[i].IsBackPressed() &&
				!m_prevGamePadState[i].IsBackPressed());
		}
		else
		{
			return (IsNewBackButtonPress(PlayerIndex::One,playerIndex) ||
				IsNewBackButtonPress(PlayerIndex::Two,playerIndex) ||
				IsNewBackButtonPress(PlayerIndex::Three,playerIndex) ||
				IsNewBackButtonPress(PlayerIndex::Four,playerIndex));
		}
	}

	bool InputState::IsNewDPADUpButtonPress(PlayerIndex controllingPlayer, PlayerIndex& playerIndex)
	{
		if(controllingPlayer != PlayerIndex::Null && controllingPlayer >= PlayerIndex::One && controllingPlayer < PlayerIndex::Four + 1)
		{
			playerIndex = controllingPlayer;

			int i = (int)playerIndex;
			return (m_currentGamepadState[i].IsDPadUpPressed() &&
				!m_prevGamePadState[i].IsDPadUpPressed());
		}
		else
		{
			return (IsNewDPADUpButtonPress(PlayerIndex::One,playerIndex) ||
				IsNewDPADUpButtonPress(PlayerIndex::Two,playerIndex) ||
				IsNewDPADUpButtonPress(PlayerIndex::Three,playerIndex) ||
				IsNewDPADUpButtonPress(PlayerIndex::Four,playerIndex));
		}
	}

	bool InputState::IsNewDPADDownButtonPress(PlayerIndex controllingPlayer, PlayerIndex& playerIndex)
	{
		if(controllingPlayer != PlayerIndex::Null && controllingPlayer >= PlayerIndex::One && controllingPlayer < PlayerIndex::Four + 1)
		{
			playerIndex = controllingPlayer;

			int i = (int)playerIndex;
			return (m_currentGamepadState[i].IsDPadDownPressed() &&
				!m_prevGamePadState[i].IsDPadDownPressed());
		}
		else
		{
			return (IsNewDPADDownButtonPress(PlayerIndex::One,playerIndex) ||
				IsNewDPADDownButtonPress(PlayerIndex::Two,playerIndex) ||
				IsNewDPADDownButtonPress(PlayerIndex::Three,playerIndex) ||
				IsNewDPADDownButtonPress(PlayerIndex::Four,playerIndex));
		}
	}

	bool InputState::IsNewDPADLeftButtonPress(PlayerIndex controllingPlayer, PlayerIndex& playerIndex)
	{
		if(controllingPlayer != PlayerIndex::Null && controllingPlayer >= PlayerIndex::One && controllingPlayer < PlayerIndex::Four + 1)
		{
			playerIndex = controllingPlayer;

			int i = (int)playerIndex;
			return (m_currentGamepadState[i].IsDPadLeftPressed() &&
				!m_prevGamePadState[i].IsDPadLeftPressed());
		}
		else
		{
			return (IsNewDPADLeftButtonPress(PlayerIndex::One,playerIndex) ||
				IsNewDPADLeftButtonPress(PlayerIndex::Two,playerIndex) ||
				IsNewDPADLeftButtonPress(PlayerIndex::Three,playerIndex) ||
				IsNewDPADLeftButtonPress(PlayerIndex::Four,playerIndex));
		}
	}

	bool InputState::IsNewDPADRightButtonPress(PlayerIndex controllingPlayer, PlayerIndex& playerIndex)
	{
		if(controllingPlayer != PlayerIndex::Null && controllingPlayer >= PlayerIndex::One && controllingPlayer < PlayerIndex::Four + 1)
		{
			playerIndex = controllingPlayer;

			int i = (int)playerIndex;
			return (m_currentGamepadState[i].IsDPadRightPressed() &&
				!m_prevGamePadState[i].IsDPadRightPressed());
		}
		else
		{
			return (IsNewDPADRightButtonPress(PlayerIndex::One,playerIndex) ||
				IsNewDPADRightButtonPress(PlayerIndex::Two,playerIndex) ||
				IsNewDPADRightButtonPress(PlayerIndex::Three,playerIndex) ||
				IsNewDPADRightButtonPress(PlayerIndex::Four,playerIndex));
		}
	}

	bool InputState::IsNewLButtonPress(PlayerIndex controllingPlayer, PlayerIndex& playerIndex)
	{
		if(controllingPlayer != PlayerIndex::Null && controllingPlayer >= PlayerIndex::One && controllingPlayer < PlayerIndex::Four + 1)
		{
			playerIndex = controllingPlayer;

			int i = (int)playerIndex;
			return (m_currentGamepadState[i].IsLeftShoulderPressed() &&
				!m_prevGamePadState[i].IsLeftShoulderPressed());
		}
		else
		{
			return (IsNewLButtonPress(PlayerIndex::One,playerIndex) ||
				IsNewLButtonPress(PlayerIndex::Two,playerIndex) ||
				IsNewLButtonPress(PlayerIndex::Three,playerIndex) ||
				IsNewLButtonPress(PlayerIndex::Four,playerIndex));
		}
	}

	bool InputState::IsNewRButtonPress(PlayerIndex controllingPlayer, PlayerIndex& playerIndex)
	{
		if(controllingPlayer != PlayerIndex::Null && controllingPlayer >= PlayerIndex::One && controllingPlayer < PlayerIndex::Four + 1)
		{
			playerIndex = controllingPlayer;

			int i = (int)playerIndex;
			return (m_currentGamepadState[i].IsRightShoulderPressed() &&
				!m_prevGamePadState[i].IsRightShoulderPressed());
		}
		else
		{
			return (IsNewRButtonPress(PlayerIndex::One,playerIndex) ||
				IsNewRButtonPress(PlayerIndex::Two,playerIndex) ||
				IsNewRButtonPress(PlayerIndex::Three,playerIndex) ||
				IsNewRButtonPress(PlayerIndex::Four,playerIndex));
		}
	}

	bool InputState::IsNewRTriggerPress(PlayerIndex controllingPlayer, PlayerIndex& playerIndex)
	{
		if(controllingPlayer != PlayerIndex::Null && controllingPlayer >= PlayerIndex::One && controllingPlayer < PlayerIndex::Four + 1)
		{
			playerIndex = controllingPlayer;

			int i = (int)playerIndex;
			return (m_currentGamepadState[i].IsRightTriggerPressed() &&
				!m_prevGamePadState[i].IsRightTriggerPressed());
		}
		else
		{
			return (IsNewRTriggerPress(PlayerIndex::One,playerIndex) ||
				IsNewRTriggerPress(PlayerIndex::Two,playerIndex) ||
				IsNewRTriggerPress(PlayerIndex::Three,playerIndex) ||
				IsNewRTriggerPress(PlayerIndex::Four,playerIndex));
		}
	}

	bool InputState::IsNewLTriggerPress(PlayerIndex controllingPlayer, PlayerIndex& playerIndex)
	{
		if(controllingPlayer != PlayerIndex::Null && controllingPlayer >= PlayerIndex::One && controllingPlayer < PlayerIndex::Four + 1)
		{
			playerIndex = controllingPlayer;

			int i = (int)playerIndex;
			return (m_currentGamepadState[i].IsLeftTriggerPressed() &&
				!m_prevGamePadState[i].IsLeftTriggerPressed());
		}
		else
		{
			return (IsNewLTriggerPress(PlayerIndex::One,playerIndex) ||
				IsNewLTriggerPress(PlayerIndex::Two,playerIndex) ||
				IsNewLTriggerPress(PlayerIndex::Three,playerIndex) ||
				IsNewLTriggerPress(PlayerIndex::Four,playerIndex));
		}
	}

	bool InputState::IsNewLeftStickUpPress(PlayerIndex controllingPlayer, PlayerIndex& playerIndex)
	{
		if(controllingPlayer != PlayerIndex::Null && controllingPlayer >= PlayerIndex::One && controllingPlayer < PlayerIndex::Four + 1)
		{
			playerIndex = controllingPlayer;

			int i = (int)playerIndex;
			return (m_currentGamepadState[i].IsLeftThumbStickUp() &&
				!m_prevGamePadState[i].IsLeftThumbStickUp());
		}
		else
		{
			return (IsNewLeftStickUpPress(PlayerIndex::One,playerIndex) ||
				IsNewLeftStickUpPress(PlayerIndex::Two,playerIndex) ||
				IsNewLeftStickUpPress(PlayerIndex::Three,playerIndex) ||
				IsNewLeftStickUpPress(PlayerIndex::Four,playerIndex));
		}
	}

	bool InputState::IsNewLeftStickDownPress(PlayerIndex controllingPlayer, PlayerIndex& playerIndex)
	{
		if(controllingPlayer != PlayerIndex::Null && controllingPlayer >= PlayerIndex::One && controllingPlayer < PlayerIndex::Four + 1)
		{
			playerIndex = controllingPlayer;

			int i = (int)playerIndex;
			return (m_currentGamepadState[i].IsLeftThumbStickDown() &&
				!m_prevGamePadState[i].IsLeftThumbStickDown());
		}
		else
		{
			return (IsNewLeftStickDownPress(PlayerIndex::One,playerIndex) ||
				IsNewLeftStickDownPress(PlayerIndex::Two,playerIndex) ||
				IsNewLeftStickDownPress(PlayerIndex::Three,playerIndex) ||
				IsNewLeftStickDownPress(PlayerIndex::Four,playerIndex));
		}
	}

	// universal selections
	bool InputState::IsMenuSelect(PlayerIndex controllingPlayer,PlayerIndex& playerIndex)
	{
		return IsNewKeyPress(DIK_RETURN) || 
			IsNewKeyPress(DIK_SPACE) ||
			IsNewAButtonPress(controllingPlayer,playerIndex) ||
			IsNewStartButtonPress(controllingPlayer,playerIndex);
	}
	bool InputState::IsMenuCancel(PlayerIndex controllingPlayer,PlayerIndex& playerIndex)
	{
		return IsNewKeyPress(DIK_ESCAPE) ||
			IsNewBButtonPress(controllingPlayer,playerIndex) ||
			IsNewBackButtonPress(controllingPlayer,playerIndex);
	}
	bool InputState::IsMenuUp(PlayerIndex& controllingPlayer)
	{
		PlayerIndex playerIndex;

		return IsNewKeyPress(DIK_UPARROW) ||
			IsNewDPADUpButtonPress(controllingPlayer,playerIndex) ||
			IsNewLeftStickUpPress(controllingPlayer,playerIndex);
	}

	bool InputState::IsMenuDown(PlayerIndex& controllingPlayer)
	{
		PlayerIndex playerIndex;
		return IsNewKeyPress(DIK_DOWNARROW) ||
			IsNewDPADDownButtonPress(controllingPlayer,playerIndex) ||
			IsNewLeftStickDownPress(controllingPlayer,playerIndex);
	}
	bool InputState::IsPauseGame(PlayerIndex& controllingPlayer)
	{
		PlayerIndex playerIndex;
		return IsNewKeyPress(DIK_ESCAPE) || IsNewStartButtonPress(controllingPlayer,playerIndex);
	}

	bool InputState::GamePadWasConnected(int index)
	{
		if ( index >= DirectX::GamePad::MAX_PLAYER_COUNT)
			return false; 
		else 
			return m_gamePadWasConnected[index]; 
	} 
}