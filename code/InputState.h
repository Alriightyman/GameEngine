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
		bool m_useGamepad;
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
		void SetGamepadUse(bool value) { m_useGamepad = value;}
		bool UseGamepad() const { return m_useGamepad; }
	public:
		InputState(HWND hwnd);
		~InputState(void);
		void Update();

		// keyboard functions
		bool IsKeyDown(int key);
		bool IsNewKeyPress(int key);
		// gamepad functions
		// button presses
		bool IsNewAButtonPress(int controllingPlayer);
		bool IsNewBButtonPress(int controllingPlayer);
		bool IsNewXButtonPress(int controllingPlayer);
		bool IsNewYButtonPress(int controllingPlayer);
		bool IsNewStartButtonPress(int controllingPlayer);
		bool IsNewBackButtonPress(int controllingPlayer);
		bool IsNewDPADLeftButtonPress(int controllingPlayer);
		bool IsNewDPADRightButtonPress(int controllingPlayer);
		bool IsNewDPADUpButtonPress(int controllingPlayer);
		bool IsNewDPADDownButtonPress(int controllingPlayer);
		bool IsNewLTriggerPress(int controllingPlayer);
		bool IsNewRTriggerPress(int controllingPlayer);
		bool IsNewLButtonPress(int controllingPlayer);
		bool IsNewRButtonPress(int controllingPlayer);
		bool IsNewLeftStickUpPress(int controllingPlayer);
		bool IsNewLeftStickDownPress(int controllingPlayer);

		//
		bool IsMenuSelect(int controllingPlayer);
		bool IsMenuCancel(int controllingPlayer);
		bool IsMenuUp(int& controllingPlayer);
		bool IsMenuDown(int& controllingPlayer);
		bool IsPauseGame(int& controllingPlayer);
	};


}