#include <ctime>

#include "GameplayScreen.h"
#include "MathHelper.h"
#include "PauseMenuScreen.h"
#include "Debug.h"
#include "Graphics.h"
#include "LightShader.h"
#include "ToonShader.h"
#include "MultiTextureShader.h"
#include "LightMapShader.h"
#include "AlphaMapShader.h"
#include "NormalMapShader.h"
#include "ScreenManager.h"
#include "Model.h"
#include <iostream>
#include "../DemoObject.h"

using namespace DirectX;
using namespace SimpleMath;
namespace Engine
{

	GameplayScreen::GameplayScreen(void)
		: m_Font(0)//,m_camera(0),
		//m_light(0),m_frustum(0)//,m_model(0)
	{
		rotationX = rotationZ = rotation = 0.0f;
		rotationY = 0.0f;

		std::srand((UINT)std::time(0)); // use current time as seed for random generator
		m_TransitionOnTime = 1.5f;
		m_TransitionOffTime = 0.5f;
	}

	GameplayScreen::~GameplayScreen(void)
	{
		UnLoad();
	}

	void GameplayScreen::Load()
	{
		// get the font for the game
		m_Font = new SpriteFont(m_ScreenManager->GetGraphicsDevice()->GetDevice(),L"Content/Font/gameFont.spritefont");
		Graphics* graphics = m_ScreenManager->GetGraphicsDevice();

		Script* script = m_ScreenManager->GetScript();
		//Camera::BindLua(script);
		Light::Bind(script);
		Model::Bind(script);
		DemoObject::Bind(script);

		// create the camera object
		m_camera.reset( new Camera(script));

		// create the model object
		m_ScreenManager->GetObjectManager()->CreateObject<DemoObject>(script,"DemoObject.lua",L"DemoMan");
		
		m_light.LoadScript(script);
		
	}

	void GameplayScreen::UnLoad()
	{

		if(m_Font)
		{
			delete m_Font;
			m_Font = 0;
		}

		m_ScreenManager->GetObjectManager()->Clear();

	}
#undef GetObject
	void GameplayScreen::HandleInput(InputState* input)
	{

		int playerIndex = m_controllingPlayer;

		// don't need to get the keyboard state, but retrieve the
		// gamepad state
		GamePad::State gamepadState = input->CurrentGamePadState(playerIndex);

		bool gamePadDisconnected = !gamepadState.IsConnected() &&
			input->GamePadWasConnected(playerIndex);

		if ( input->IsPauseGame(m_controllingPlayer) || gamePadDisconnected )
		{
			m_ScreenManager->AddScreen(new PauseMenuScreen(),m_controllingPlayer);
		}
		else
		{			
			Vector2 movement = Vector2::Zero;

			Script* script = m_ScreenManager->GetScript();
			
			// get the player object to handle input
			DemoObject* obj = static_cast<DemoObject*>((m_ScreenManager->GetObjectManager()->GetGameObject(0)));
			obj->Input(input,playerIndex);

			// light controls
			script->LoadScript("Content/Scripts/DemoInput.lua");
			luabridge::LuaRef func = getGlobal(script->GetState(),"Input");
			
			if (func.isFunction())
			{
				std::shared_ptr<luabridge::LuaRef> f = std::make_shared<luabridge::LuaRef>(func);
				try
				{
					(*f)(input,playerIndex,&m_light);
				}
				catch(luabridge::LuaException const& e)
				{
					std::cout << "LuaException: " << e.what() << std::endl;
				}
			}

			// check for joystick movements

	/*		Vector2 thumbstick(gamepadState.thumbSticks.leftX,gamepadState.thumbSticks.leftY);

			movement.x += thumbstick.x;
			movement.y -= thumbstick.y;

			if(movement.Length() > 1)
				movement.Normalize();

			rotation += -thumbstick.x * static_cast<float>(*g_XMPi) * 1.5f;*/

			// camera movment
			m_camera->MoveCamera(m_ScreenManager->GetScript(), input);

		}

	}

	void GameplayScreen::Update(float deltaTime, bool otherScreenHasFocus, bool coveredByOtherScreen)
	{
		GameScreen::Update(deltaTime,otherScreenHasFocus,false);

		// generate the view matrix based on the camera's position
		m_camera->Update();

		if(coveredByOtherScreen)
			m_PauseAlpha = MathHelper::Min(m_PauseAlpha + 1.0f / 32.0f,1.0f);
		else
			m_PauseAlpha = MathHelper::Max(m_PauseAlpha - 1.0f /32.0f, 0.0f);

		if(IsActive())
		{
			Matrix viewMatrix, projMatrix;

			// get the world,view, and projection matrices from the camera
			viewMatrix = m_camera->GetViewMatrix();
			projMatrix = m_ScreenManager->GetGraphicsDevice()->GetProjectionMatrix();

			// set up the frustum
			m_frustum.Construct(m_ScreenManager->GetGraphicsDevice()->GetScreenDepth(),projMatrix,viewMatrix);

			m_ScreenManager->GetObjectManager()->Update(deltaTime,&m_frustum);
		}
	}

	void GameplayScreen::Draw(float deltaTime)
	{
		// get the spritebatch object
		SpriteBatch* spriteBatch = m_ScreenManager->SpriteBatch();
		// get the graphics object
		Graphics* graphics = m_ScreenManager->GetGraphicsDevice();
		ObjectManager* objectManager = m_ScreenManager->GetObjectManager();
		// must be set or there will be depth problems
		graphics->GetImmediateContex()->OMSetDepthStencilState(graphics->GetDepthStencilState(),1);
		// Now set the rasterizer state.
		graphics->GetImmediateContex()->RSSetState(graphics->GetRasterState());

		// get the font object
		SpriteFont* font = m_Font;
		Matrix viewMatrix, projMatrix, worldMatrix;

		// get the world,view, and projection matrices from the camera
		viewMatrix = m_camera->GetViewMatrix();
		projMatrix = m_ScreenManager->GetGraphicsDevice()->GetProjectionMatrix();
		
		NormalMapShader* normalMap = m_ScreenManager->GetGraphicsDevice()->GetNormalMapShader();
		normalMap->SetDiffuseColor(m_light.GetDiffuseColor());
		normalMap->SetLightDiretion(m_light.GetDirection());
		// clear the screen to a different color
		graphics->Clear(Colors::Blue);

		// render the model
		objectManager->Render(worldMatrix,viewMatrix,projMatrix);

		spriteBatch->Begin();
		std::wstring str = L"Move Light: Up - I, Down - K,Left - J,Right - L\nLightDirection: (" + std::to_wstring( m_light.GetDirection().x) 
			+ L"," + std::to_wstring(m_light.GetDirection().y) + L"," + std::to_wstring(m_light.GetDirection().z) + L")\n" ;
		font->DrawString(spriteBatch,
			str.c_str(),
			Vector2(10.0f),
			Colors::Yellow,0.0f,g_XMZero,0.5f);
		spriteBatch->End();

		// If the game is transitioning on or off, fade it out to black.
		if (m_TransitionPosition > 0 || m_PauseAlpha > 0)
		{
			float alpha =  MathHelper::Lerp(1.0f - TransitionAlpha(), 1.0f, m_PauseAlpha / 2) + 0.30f;
			m_ScreenManager->FadeBackBufferToBlack(alpha);
		}
	}

}