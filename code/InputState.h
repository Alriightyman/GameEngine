#pragma once
#define DIRECTINPUT_VERSION 0x800
#include <dinput.h>
#include <GamePad.h>
#include "PlayerIndex.h"
#include "Lua\LuaBridge\LuaBridge.h"
namespace Engine
{
	
	/// <summary>
	/// Helper for reading input from m_Keyboard, gamepad, and touch input. This class 
	/// tracks both the current and previous state of the input devices, and implements 
	/// query methods for high level input actions such as "move up through the menu"
	/// or "pause the game".
	/// </summary>
	class InputState
	{
	public:
		static void Bind(luabridge::lua_State* L);
	private:
		const int MAXINPUTS;
		LPDIRECTINPUT8 m_DInput;
		LPDIRECTINPUTDEVICE8 m_Keyboard;
		char m_CurrentKeyboardState[256];
		char m_PreviousKeyboardState[256];
		std::unique_ptr<DirectX::GamePad> m_gamepad;
		DirectX::GamePad::State m_currentGamepadState[DirectX::GamePad::MAX_PLAYER_COUNT];
		DirectX::GamePad::State m_prevGamePadState[DirectX::GamePad::MAX_PLAYER_COUNT];
		bool m_gamePadWasConnected[DirectX::GamePad::MAX_PLAYER_COUNT];
	public:
		DirectX::GamePad::State CurrentGamePadState(int index) { return m_currentGamepadState[index]; }
		bool GamePadWasConnected(int index);
	public:
		InputState(HWND hwnd);
		~InputState(void);
		void Update();

		// keyboard functions
		bool IsKeyDown(int key);
		bool IsNewKeyPress(int key);
		// gamepad functions
		// button presses
		bool IsNewAButtonPress(PlayerIndex controllingPlayer,PlayerIndex& playerIndex);
		bool IsNewBButtonPress(PlayerIndex controllingPlayer,PlayerIndex& playerIndex);
		bool IsNewXButtonPress(PlayerIndex controllingPlayer,PlayerIndex& playerIndex);
		bool IsNewYButtonPress(PlayerIndex controllingPlayer,PlayerIndex& playerIndex);
		bool IsNewStartButtonPress(PlayerIndex controllingPlayer,PlayerIndex& playerIndex);
		bool IsNewBackButtonPress(PlayerIndex controllingPlayer,PlayerIndex& playerIndex);
		bool IsNewDPADLeftButtonPress(PlayerIndex controllingPlayer,PlayerIndex& playerIndex);
		bool IsNewDPADRightButtonPress(PlayerIndex controllingPlayer,PlayerIndex& playerIndex);
		bool IsNewDPADUpButtonPress(PlayerIndex controllingPlayer,PlayerIndex& playerIndex);
		bool IsNewDPADDownButtonPress(PlayerIndex controllingPlayer,PlayerIndex& playerIndex);
		bool IsNewLTriggerPress(PlayerIndex controllingPlayer,PlayerIndex& playerIndex);
		bool IsNewRTriggerPress(PlayerIndex controllingPlayer,PlayerIndex& playerIndex);
		bool IsNewLButtonPress(PlayerIndex controllingPlayer,PlayerIndex& playerIndex);
		bool IsNewRButtonPress(PlayerIndex controllingPlayer,PlayerIndex& playerIndex);
		bool IsNewLeftStickUpPress(PlayerIndex controllingPlayer,PlayerIndex& playerIndex);
		bool IsNewLeftStickDownPress(PlayerIndex controllingPlayer,PlayerIndex& playerIndex);

		//
		bool IsMenuSelect(PlayerIndex controllingPlayer,PlayerIndex& playerIndex);
		bool IsMenuCancel(PlayerIndex controllingPlayer,PlayerIndex& playerIndex);
		bool IsMenuUp(PlayerIndex& controllingPlayer);
		bool IsMenuDown(PlayerIndex& controllingPlayer);
		bool IsPauseGame(PlayerIndex& controllingPlayer);
	};


}