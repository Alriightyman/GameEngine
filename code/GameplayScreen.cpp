#include <ctime>

#include "GameplayScreen.h"
#include "ScreenManager.h"
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

using namespace DirectX;
using namespace SimpleMath;
namespace Engine
{

	GameplayScreen::GameplayScreen(void)
		: m_Font(0),m_camera(0),
		m_light(0),m_frustum(0)//,m_model(0)
	{
		rotationX = rotationZ = rotation = 0.0f;
		rotationY = 0.0f;

		std::srand((UINT)std::time(0)); // use current time as seed for random generator
		m_TransitionOnTime = 1.5f;
		m_TransitionOffTime = 0.5f;
		//	m_EnemyPosition = m_PlayerPosition = m_PlayerMovement = Vector2(0,0);
	}

	GameplayScreen::~GameplayScreen(void)
	{
	}

	void GameplayScreen::Load()
	{
		// get the font for the game
		m_Font = new SpriteFont(m_ScreenManager->GetGraphicsDevice()->GetDevice(),L"Content/Font/gameFont.spritefont");
		Graphics* graphics = m_ScreenManager->GetGraphicsDevice();

		// create the camera object
		m_camera = new Camera();

		Script* script = m_ScreenManager->GetScript();
		script->LoadScript("Content/Scripts/LevelData.lua");
		script->RunFunction("SetCameraZ",0,1);
		float z = (float)script->GetResultNumber();

		// set the initial position of the camera
		m_camera->SetPosition(0.0f,0.0f,z);

		// create the model object
		m_model.reset( graphics->CreateModel(L"Content/Models/cube.txt",graphics->CreateTexture(L"Content/Textures/stone01.dds",L"Content/Textures/bump01.dds",L"")));

		m_light = new Light();
		float r,g,b;
		script->RunFunction("SetLightingR");
		r = script->GetGlobalNumber("diffuse");
		script->RunFunction("SetLightingG");
		g = script->GetGlobalNumber("diffuse");
		script->RunFunction("SetLightingB");
		b = script->GetGlobalNumber("diffuse");

		m_light->SetAmbientColor(0.15f,0.15f,0.15f,1.0f);
		m_light->SetDiffuseColor(r, g,b,1.0f);
		m_light->SetDirection(1.0f,1.0f,1.0f);
		m_light->SetSpecularColor(1.0f,1.0f,1.0f,1.0f);
		m_light->SetSpecularPower(32.0f);

		m_frustum = new Frustum();
	}

	void GameplayScreen::UnLoad()
	{
		if(m_frustum)
		{
			delete m_frustum;
			m_frustum = 0;
		}

		if(m_light)
		{
			delete m_light;
			m_light = 0;
		}

		if(m_camera)
		{
			delete m_camera;
			m_camera = 0;
		}
	}
	void GameplayScreen::HandleInput(InputState* input)
	{

		PlayerIndex playerIndex = m_controllingPlayer;

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
			script->LoadScript("Content/Scripts/DemoInput.lua");
			
			// check keyboard movements first
			if(input->IsKeyDown(DIK_LEFTARROW))
			{
				script->SetGlobalNumber("RotX",rotationX);
				script->RunFunction("RotateXLeft",rotationX,1); // rotationX += static_cast<float>(*g_XMPi) * 0.1f;
				rotationX = script->GetGlobalNumber("RotX");
			}
			if(input->IsKeyDown(DIK_RIGHTARROW))
			{
				script->SetGlobalNumber("RotX",rotationX);
				script->RunFunction("RotateXRight",rotationX,1); // rotationX -= static_cast<float>(*g_XMPi) * 0.1f;
				rotationX = script->GetGlobalNumber("RotX");
			}
			if(input->IsKeyDown(DIK_UPARROW))
			{
				script->SetGlobalNumber("RotY",rotationY);
				script->RunFunction("RotateYLeft",rotationY,1); // rotationY -= static_cast<float>(*g_XMPi) * 0.1f;
				rotationY = script->GetGlobalNumber("RotY");
			}
			if(input->IsKeyDown(DIK_DOWNARROW))
			{
				script->SetGlobalNumber("RotY",rotationY);
				script->RunFunction("RotateYRight",rotationY,1); // rotationY += static_cast<float>(*g_XMPi) * 0.1f;
				rotationY = script->GetGlobalNumber("RotY");
			}
			if(input->IsKeyDown(DIK_Q))
			{
				script->SetGlobalNumber("RotZ",rotationZ);
				script->RunFunction("RotateZLeft",rotationZ,1); // rotationZ -= static_cast<float>(*g_XMPi) * 0.1f;
				rotationZ = script->GetGlobalNumber("RotZ");
			
			}
			if(input->IsKeyDown(DIK_W))
			{
				script->SetGlobalNumber("RotZ",rotationZ);
				script->RunFunction("RotateZRight",rotationZ,1); // rotationZ += static_cast<float>(*g_XMPi) * 0.1f;
				rotationZ = script->GetGlobalNumber("RotZ");
			}

			if(input->IsKeyDown(DIK_R))
			{
				Color c = m_light->GetDiffuseColor();
				float r = c.R();
				if(input->IsKeyDown(DIK_LSHIFT))
				{
					script->RunFunction("SubtractColorValue",r,1,1);
					r = (float)script->GetResultNumber();
					m_light->SetDiffuseColor(r,c.G(),c.B(),c.A());
				}
				else
				{
					script->RunFunction("AddColorValue",r,1,1);
					r = (float)script->GetResultNumber();
					m_light->SetDiffuseColor(r,c.G(),c.B(),c.A());
				}

			}

			if(input->IsKeyDown(DIK_G))
			{
				Color c = m_light->GetDiffuseColor();
				float g = c.G();
				if(input->IsKeyDown(DIK_LSHIFT))
				{
					script->RunFunction("SubtractColorValue",g,1,1);
					g = (float)script->GetResultNumber();
					m_light->SetDiffuseColor(c.R(),g,c.B(),c.A());
				}
				else
				{
					script->RunFunction("AddColorValue",g,1,1);
					g = (float)script->GetResultNumber();
					m_light->SetDiffuseColor(c.R(),g,c.B(),c.A());
				}

			}

			if(input->IsKeyDown(DIK_B))
			{
				Color c = m_light->GetDiffuseColor();
				float b = c.B();
				if(input->IsKeyDown(DIK_LSHIFT))
				{
					script->RunFunction("SubtractColorValue",b,1,1);
					b = (float)script->GetResultNumber();
					m_light->SetDiffuseColor(c.B(),c.G(),b,c.A());
				}
				else
				{
					script->RunFunction("AddColorValue",b,1,1);
					b = (float)script->GetResultNumber();
					m_light->SetDiffuseColor(c.B(),c.G(),b,c.A());
				}

			}
			// check for joystick movements
			Vector2 thumbstick(gamepadState.thumbSticks.leftX,gamepadState.thumbSticks.leftY);

			movement.x += thumbstick.x;
			movement.y -= thumbstick.y;

			if(movement.Length() > 1)
				movement.Normalize();

			rotation += -thumbstick.x * static_cast<float>(*g_XMPi) * 1.5f;

			

		}

	}

	void GameplayScreen::Update(float deltaTime, bool otherScreenHasFocus, bool coveredByOtherScreen)
	{
		GameScreen::Update(deltaTime,otherScreenHasFocus,false);

		if(coveredByOtherScreen)
			m_PauseAlpha = MathHelper::Min(m_PauseAlpha + 1.0f / 32.0f,1.0f);
		else
			m_PauseAlpha = MathHelper::Max(m_PauseAlpha - 1.0f /32.0f, 0.0f);

		if(IsActive())
		{
		//	rotationX *= deltaTime;
		//	rotationY *= deltaTime;

			m_camera->SetRotation(rotationX,rotationY,0.0f);
			
		}
	}

	void GameplayScreen::Draw(float deltaTime)
	{
		// get the spritebatch object
		SpriteBatch* spriteBatch = m_ScreenManager->SpriteBatch();
		// get the graphics object
		Graphics* graphics = m_ScreenManager->GetGraphicsDevice();
		// get the font object
		SpriteFont* font = m_ScreenManager->Font();
		Matrix viewMatrix,projMatrix,worldMatrix;

			// generate the view matrix based on the camera's position
		m_camera->Render();

		// get the world,view, and projection matrices from the camera
		viewMatrix = m_camera->GetViewMatrix();
		worldMatrix = m_ScreenManager->GetGraphicsDevice()->GetWorldMatrix();
		projMatrix = m_ScreenManager->GetGraphicsDevice()->GetProjectionMatrix();
		worldMatrix = Matrix::CreateRotationX(rotationZ);
		// set up the frustum
		m_frustum->Construct(m_ScreenManager->GetGraphicsDevice()->GetScreenDepth(),projMatrix,viewMatrix);
		NormalMapShader* normalMap = m_ScreenManager->GetGraphicsDevice()->GetNormalMapShader();
		normalMap->SetTexture(m_model->GetTextures());
		normalMap->SetWorldViewProjMatrices(worldMatrix,viewMatrix,projMatrix);
		normalMap->SetDiffuseColor(m_light->GetDiffuseColor());
		normalMap->SetLightDiretion(m_light->GetDirection());

		// clear the screen to a different color
		graphics->Clear(Colors::Black);

		// render the model
		graphics->Render(m_model.get(), ShaderType::NORMALMAP);


		spriteBatch->Begin();

		spriteBatch->End();

		// If the game is transitioning on or off, fade it out to black.
		if (m_TransitionPosition > 0 || m_PauseAlpha > 0)
		{
			float alpha =  MathHelper::Lerp(1.0f - TransitionAlpha(), 1.0f, m_PauseAlpha / 2) + 0.30f;
			m_ScreenManager->FadeBackBufferToBlack(alpha);
		}
	}

}