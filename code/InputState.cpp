#include "InputState.h"
#include "LuaEnum.h"

using namespace DirectX;
namespace Engine
{

	// binds the input into lua
	void InputState::Bind(luabridge::lua_State* L)
	{
		using namespace DirectX;
		using namespace luabridge;	
		getGlobalNamespace(L)
			.beginClass<GamePad::Buttons>("Buttons")
				.addData<bool>("a",&GamePad::Buttons::a)
				.addData<bool>("b",&GamePad::Buttons::b)
				.addData<bool>("x",&GamePad::Buttons::x)
				.addData<bool>("y",&GamePad::Buttons::y)
				.addData<bool>("leftStick",&GamePad::Buttons::leftStick)
				.addData<bool>("rightStick",&GamePad::Buttons::rightStick)
				.addData<bool>("leftShoulder",&GamePad::Buttons::leftShoulder)
				.addData<bool>("rightShoulder",&GamePad::Buttons::rightShoulder)
				.addData<bool>("back",&GamePad::Buttons::back)
				.addData<bool>("start",&GamePad::Buttons::start)
			.endClass()
			.beginClass<GamePad::DPad>("DPad")
				.addData<bool>("up",&GamePad::DPad::up)
				.addData<bool>("down",&GamePad::DPad::down)
				.addData<bool>("right",&GamePad::DPad::right)
				.addData<bool>("left",&GamePad::DPad::left)
			.endClass()
			.beginClass<GamePad::ThumbSticks>("ThumbSticks")
				.addData<float>("leftX",&GamePad::ThumbSticks::leftX)
				.addData<float>("leftY",&GamePad::ThumbSticks::leftY)
				.addData<float>("rightX",&GamePad::ThumbSticks::rightX)
				.addData<float>("rightY",&GamePad::ThumbSticks::rightY)
			.endClass()
			.beginClass<GamePad::Triggers>("Triggers")
				.addData<float>("left",&GamePad::Triggers::left)
				.addData<float>("right",&GamePad::Triggers::right)
			.endClass()
			.beginClass<GamePad::State>("State")
				.addData<GamePad::Buttons>("buttons",&GamePad::State::buttons)
				.addData<GamePad::DPad>("dpad",&GamePad::State::dpad)
				.addData<GamePad::ThumbSticks>("thumbSticks",&GamePad::State::thumbSticks)
				.addData<GamePad::Triggers>("triggers",&GamePad::State::triggers)
			.endClass()
			.beginClass<InputState>("Input")
				.addFunction("IsKeyDown",&InputState::IsKeyDown)
				.addFunction("IsNewKeyPress",&InputState::IsNewKeyPress)
				.addFunction("IsMenuSelect",&InputState::IsMenuSelect)
				.addFunction("IsMenuCancel",&InputState::IsMenuCancel)
				.addFunction("IsMenuUp",&InputState::IsMenuUp)
				.addFunction("IsMenuDown",&InputState::IsMenuDown)
				.addFunction("IsPauseGame",&InputState::IsPauseGame)
				.addFunction("CurrentGamePadState",&InputState::CurrentGamePadState)
				.addProperty("UseGamepad",&InputState::UseGamepad,&InputState::SetGamepadUse)

			.endClass();
	}


	InputState::InputState(HWND hwnd) : MAXINPUTS(4),m_useGamepad(false)
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
		return ( (m_CurrentKeyboardState[key] & 0x80) != 0 &&
			!((m_PreviousKeyboardState[key] & 0x80) != 0) );
	}

	bool InputState::IsKeyDown(int key)
	{
		return (m_CurrentKeyboardState[key] & 0x80) != 0;
	}

	bool InputState::IsNewAButtonPress(int controllingPlayer )
	{
		if(controllingPlayer != PlayerIndex::Null && controllingPlayer >= PlayerIndex::One && controllingPlayer < PlayerIndex::Four + 1)
		{
			int playerIndex = controllingPlayer;

			int i = (int)playerIndex;
			return (m_currentGamepadState[i].IsAPressed() &&
				!m_prevGamePadState[i].IsAPressed());
		}
		else
		{
			return (IsNewAButtonPress(PlayerIndex::One) ||
				IsNewAButtonPress(PlayerIndex::Two) ||
				IsNewAButtonPress(PlayerIndex::Three) ||
				IsNewAButtonPress(PlayerIndex::Four));
		}
	}

	bool InputState::IsNewBButtonPress(int controllingPlayer )
	{
		if(controllingPlayer != PlayerIndex::Null && controllingPlayer >= PlayerIndex::One && controllingPlayer < PlayerIndex::Four + 1)
		{
			int playerIndex = controllingPlayer;

			int i = (int)playerIndex;
			return (m_currentGamepadState[i].IsBPressed() &&
				!m_prevGamePadState[i].IsBPressed());
		}
		else
		{
			return (IsNewBButtonPress(PlayerIndex::One) ||
				IsNewBButtonPress(PlayerIndex::Two) ||
				IsNewBButtonPress(PlayerIndex::Three) ||
				IsNewBButtonPress(PlayerIndex::Four));
		}
	}

	bool InputState::IsNewXButtonPress(int controllingPlayer )
	{
		if(controllingPlayer != PlayerIndex::Null && controllingPlayer >= PlayerIndex::One && controllingPlayer < PlayerIndex::Four + 1)
		{
			int playerIndex = controllingPlayer;

			int i = (int)playerIndex;
			return (m_currentGamepadState[i].IsXPressed() &&
				!m_prevGamePadState[i].IsXPressed());
		}
		else
		{
			return (IsNewXButtonPress(PlayerIndex::One) ||
				IsNewXButtonPress(PlayerIndex::Two) ||
				IsNewXButtonPress(PlayerIndex::Three) ||
				IsNewXButtonPress(PlayerIndex::Four));
		}
	}

	bool InputState::IsNewYButtonPress(int controllingPlayer )
	{
		if(controllingPlayer != PlayerIndex::Null && controllingPlayer >= PlayerIndex::One && controllingPlayer < PlayerIndex::Four + 1)
		{
			int playerIndex = controllingPlayer;

			int i = (int)playerIndex;
			return (m_currentGamepadState[i].IsYPressed() &&
				!m_prevGamePadState[i].IsYPressed());
		}
		else
		{
			return (IsNewYButtonPress(PlayerIndex::One) ||
				IsNewYButtonPress(PlayerIndex::Two) ||
				IsNewYButtonPress(PlayerIndex::Three) ||
				IsNewYButtonPress(PlayerIndex::Four));
		}
	}

	bool InputState::IsNewStartButtonPress(int controllingPlayer )
	{
		if(controllingPlayer != PlayerIndex::Null && controllingPlayer >= PlayerIndex::One && controllingPlayer < PlayerIndex::Four + 1)
		{
			int playerIndex = controllingPlayer;

			int i = (int)playerIndex;
			return (m_currentGamepadState[i].IsStartPressed() &&
				!m_prevGamePadState[i].IsStartPressed());
		}
		else
		{
			return (IsNewStartButtonPress(PlayerIndex::One) ||
				IsNewStartButtonPress(PlayerIndex::Two) ||
				IsNewStartButtonPress(PlayerIndex::Three) ||
				IsNewStartButtonPress(PlayerIndex::Four));
		}
	}

	bool InputState::IsNewBackButtonPress(int controllingPlayer )
	{
		if(controllingPlayer != PlayerIndex::Null && controllingPlayer >= PlayerIndex::One && controllingPlayer < PlayerIndex::Four + 1)
		{
			int playerIndex = controllingPlayer;

			int i = (int)playerIndex;
			return (m_currentGamepadState[i].IsBackPressed() &&
				!m_prevGamePadState[i].IsBackPressed());
		}
		else
		{
			return (IsNewBackButtonPress(PlayerIndex::One) ||
				IsNewBackButtonPress(PlayerIndex::Two) ||
				IsNewBackButtonPress(PlayerIndex::Three) ||
				IsNewBackButtonPress(PlayerIndex::Four));
		}
	}

	bool InputState::IsNewDPADUpButtonPress(int controllingPlayer)
	{
		if(controllingPlayer != PlayerIndex::Null && controllingPlayer >= PlayerIndex::One && controllingPlayer < PlayerIndex::Four + 1)
		{
			//int playerIndex = controllingPlayer;

			int i = (int)controllingPlayer;
			return (m_currentGamepadState[i].IsDPadUpPressed() &&
				!m_prevGamePadState[i].IsDPadUpPressed());
		}
		else
		{
			return (IsNewDPADUpButtonPress(PlayerIndex::One) ||
				IsNewDPADUpButtonPress(PlayerIndex::Two) ||
				IsNewDPADUpButtonPress(PlayerIndex::Three) ||
				IsNewDPADUpButtonPress(PlayerIndex::Four));
		}
	}

	bool InputState::IsNewDPADDownButtonPress(int controllingPlayer )
	{
		if(controllingPlayer != PlayerIndex::Null && controllingPlayer >= PlayerIndex::One && controllingPlayer < PlayerIndex::Four + 1)
		{
			int playerIndex = controllingPlayer;

			int i = (int)playerIndex;
			return (m_currentGamepadState[i].IsDPadDownPressed() &&
				!m_prevGamePadState[i].IsDPadDownPressed());
		}
		else
		{
			return (IsNewDPADDownButtonPress(PlayerIndex::One) ||
				IsNewDPADDownButtonPress(PlayerIndex::Two) ||
				IsNewDPADDownButtonPress(PlayerIndex::Three) ||
				IsNewDPADDownButtonPress(PlayerIndex::Four));
		}
	}

	bool InputState::IsNewDPADLeftButtonPress(int controllingPlayer )
	{
		if(controllingPlayer != PlayerIndex::Null && controllingPlayer >= PlayerIndex::One && controllingPlayer < PlayerIndex::Four + 1)
		{
			int playerIndex = controllingPlayer;

			int i = (int)playerIndex;
			return (m_currentGamepadState[i].IsDPadLeftPressed() &&
				!m_prevGamePadState[i].IsDPadLeftPressed());
		}
		else
		{
			return (IsNewDPADLeftButtonPress(PlayerIndex::One) ||
				IsNewDPADLeftButtonPress(PlayerIndex::Two) ||
				IsNewDPADLeftButtonPress(PlayerIndex::Three) ||
				IsNewDPADLeftButtonPress(PlayerIndex::Four));
		}
	}

	bool InputState::IsNewDPADRightButtonPress(int controllingPlayer )
	{
		if(controllingPlayer != PlayerIndex::Null && controllingPlayer >= PlayerIndex::One && controllingPlayer < PlayerIndex::Four + 1)
		{
			int playerIndex = controllingPlayer;

			int i = (int)playerIndex;
			return (m_currentGamepadState[i].IsDPadRightPressed() &&
				!m_prevGamePadState[i].IsDPadRightPressed());
		}
		else
		{
			return (IsNewDPADRightButtonPress(PlayerIndex::One) ||
				IsNewDPADRightButtonPress(PlayerIndex::Two) ||
				IsNewDPADRightButtonPress(PlayerIndex::Three) ||
				IsNewDPADRightButtonPress(PlayerIndex::Four));
		}
	}

	bool InputState::IsNewLButtonPress(int controllingPlayer )
	{
		if(controllingPlayer != PlayerIndex::Null && controllingPlayer >= PlayerIndex::One && controllingPlayer < PlayerIndex::Four + 1)
		{
			int playerIndex = controllingPlayer;

			int i = (int)playerIndex;
			return (m_currentGamepadState[i].IsLeftShoulderPressed() &&
				!m_prevGamePadState[i].IsLeftShoulderPressed());
		}
		else
		{
			return (IsNewLButtonPress(PlayerIndex::One) ||
				IsNewLButtonPress(PlayerIndex::Two) ||
				IsNewLButtonPress(PlayerIndex::Three) ||
				IsNewLButtonPress(PlayerIndex::Four));
		}
	}

	bool InputState::IsNewRButtonPress(int controllingPlayer )
	{
		if(controllingPlayer != PlayerIndex::Null && controllingPlayer >= PlayerIndex::One && controllingPlayer < PlayerIndex::Four + 1)
		{
			int playerIndex = controllingPlayer;

			int i = (int)playerIndex;
			return (m_currentGamepadState[i].IsRightShoulderPressed() &&
				!m_prevGamePadState[i].IsRightShoulderPressed());
		}
		else
		{
			return (IsNewRButtonPress(PlayerIndex::One) ||
				IsNewRButtonPress(PlayerIndex::Two) ||
				IsNewRButtonPress(PlayerIndex::Three) ||
				IsNewRButtonPress(PlayerIndex::Four));
		}
	}

	bool InputState::IsNewRTriggerPress(int controllingPlayer )
	{
		if(controllingPlayer != PlayerIndex::Null && controllingPlayer >= PlayerIndex::One && controllingPlayer < PlayerIndex::Four + 1)
		{
			int playerIndex = controllingPlayer;

			int i = (int)playerIndex;
			return (m_currentGamepadState[i].IsRightTriggerPressed() &&
				!m_prevGamePadState[i].IsRightTriggerPressed());
		}
		else
		{
			return (IsNewRTriggerPress(PlayerIndex::One) ||
				IsNewRTriggerPress(PlayerIndex::Two) ||
				IsNewRTriggerPress(PlayerIndex::Three) ||
				IsNewRTriggerPress(PlayerIndex::Four));
		}
	}

	bool InputState::IsNewLTriggerPress(int controllingPlayer )
	{
		if(controllingPlayer != PlayerIndex::Null && controllingPlayer >= PlayerIndex::One && controllingPlayer < PlayerIndex::Four + 1)
		{
			int playerIndex = controllingPlayer;

			int i = (int)playerIndex;
			return (m_currentGamepadState[i].IsLeftTriggerPressed() &&
				!m_prevGamePadState[i].IsLeftTriggerPressed());
		}
		else
		{
			return (IsNewLTriggerPress(PlayerIndex::One) ||
				IsNewLTriggerPress(PlayerIndex::Two) ||
				IsNewLTriggerPress(PlayerIndex::Three) ||
				IsNewLTriggerPress(PlayerIndex::Four));
		}
	}

	bool InputState::IsNewLeftStickUpPress(int controllingPlayer )
	{
		if(controllingPlayer != PlayerIndex::Null && controllingPlayer >= PlayerIndex::One && controllingPlayer < PlayerIndex::Four + 1)
		{
			int playerIndex = controllingPlayer;

			int i = (int)playerIndex;
			return (m_currentGamepadState[i].IsLeftThumbStickUp() &&
				!m_prevGamePadState[i].IsLeftThumbStickUp());
		}
		else
		{
			return (IsNewLeftStickUpPress(PlayerIndex::One) ||
				IsNewLeftStickUpPress(PlayerIndex::Two) ||
				IsNewLeftStickUpPress(PlayerIndex::Three) ||
				IsNewLeftStickUpPress(PlayerIndex::Four));
		}
	}

	bool InputState::IsNewLeftStickDownPress(int controllingPlayer )
	{
		if(controllingPlayer != PlayerIndex::Null && controllingPlayer >= PlayerIndex::One && controllingPlayer < PlayerIndex::Four + 1)
		{
			int playerIndex = controllingPlayer;

			int i = (int)playerIndex;
			return (m_currentGamepadState[i].IsLeftThumbStickDown() &&
				!m_prevGamePadState[i].IsLeftThumbStickDown());
		}
		else
		{
			return (IsNewLeftStickDownPress(PlayerIndex::One) ||
				IsNewLeftStickDownPress(PlayerIndex::Two) ||
				IsNewLeftStickDownPress(PlayerIndex::Three) ||
				IsNewLeftStickDownPress(PlayerIndex::Four));
		}
	}

	// universal selections
	bool InputState::IsMenuSelect(int controllingPlayer)
	{
		return IsNewKeyPress(DIK_RETURN) || 
			IsNewKeyPress(DIK_SPACE) ||
			IsNewAButtonPress(controllingPlayer) ||
			IsNewStartButtonPress(controllingPlayer);
	}
	bool InputState::IsMenuCancel(int controllingPlayer)
	{
		return IsNewKeyPress(DIK_ESCAPE) ||
			IsNewBButtonPress(controllingPlayer) ||
			IsNewBackButtonPress(controllingPlayer);
	}
	bool InputState::IsMenuUp(int& controllingPlayer)
	{
		int playerIndex;
		return IsNewKeyPress(DIK_UPARROW) ||
			IsNewDPADUpButtonPress(controllingPlayer) ||
			IsNewLeftStickUpPress(controllingPlayer);
	}

	bool InputState::IsMenuDown(int& controllingPlayer)
	{
		int playerIndex;
		return IsNewKeyPress(DIK_DOWNARROW) ||
			IsNewDPADDownButtonPress(controllingPlayer) ||
			IsNewLeftStickDownPress(controllingPlayer);
	}
	bool InputState::IsPauseGame(int& controllingPlayer)
	{
		int playerIndex;
		return IsNewKeyPress(DIK_ESCAPE) || IsNewStartButtonPress(controllingPlayer);
	}

	bool InputState::GamePadWasConnected(int index)
	{
		if ( index >= DirectX::GamePad::MAX_PLAYER_COUNT)
			return false; 
		else 
			return m_gamePadWasConnected[index]; 
	} 
}