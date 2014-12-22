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

using namespace DirectX;
using namespace SimpleMath;
namespace Engine
{

	GameplayScreen::GameplayScreen(void)
		: m_Font(0),obj(0)//,m_camera(0),
		//m_light(0),m_frustum(0)//,m_model(0)
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
		UnLoad();
	}

	void GameplayScreen::Load()
	{
		// get the font for the game
		m_Font = new SpriteFont(m_ScreenManager->GetGraphicsDevice()->GetDevice(),L"Content/Font/gameFont.spritefont");
		Graphics* graphics = m_ScreenManager->GetGraphicsDevice();

		Script* script = m_ScreenManager->GetScript();
		Camera::BindLua(script->GetState());
		Light::Bind(script);
		Model::Bind(script);
		DemoObject::Bind(script);

		// create the camera object
		m_camera.reset( new Camera(script));

		
		script->LoadScript("Content/Scripts/LevelData.lua");
		script->RunFunction("SetCameraZ",0,1);
		float z = (float)script->GetResultNumber();

		// set the initial position of the camera
		//m_camera->SetPosition(0.0f,-1.0f,z);
		m_camera->SetPosition(Vector3(0.0f,-1.0f,z));
		script->RunFunction("LoadModelData",0,1);

		std::vector<std::wstring> modelData = script->ReturnArray();

		// create the model object
		//m_model = graphics->CreateModel(modelData[0]);
		obj = new DemoObject();
		obj->Initialize(graphics,script);

		m_light.LoadScript(script);
		
		//m_light = new Light();
		/*float r,g,b;
		script->RunFunction("SetLightingR");
		r = script->GetGlobalNumber("diffuse");
		script->RunFunction("SetLightingG");
		g = script->GetGlobalNumber("diffuse");
		script->RunFunction("SetLightingB");
		b = script->GetGlobalNumber("diffuse");*/

		//m_light.SetAmbientColor(0.15f,0.15f,0.15f,1.0f);
		//m_light.SetDiffuseColor(r, g,b,1.0f);
		//m_light.SetDirection(0.0f,0.0f,1.0f);
		//m_light.SetSpecularColor(1.0f,1.0f,1.0f,1.0f);
		//m_light.SetSpecularPower(96.0f);

		//m_frustum = new Frustum();
	}

	void GameplayScreen::UnLoad()
	{
		if(obj)
		{
			//obj->Shutdown();
			delete obj;
			obj = 0;
		}
		if(m_Font)
		{
			delete m_Font;
			m_Font = 0;
		}

		/*if(m_frustum)
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
		}*/
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
			
			
			obj->Input(input);

			script->LoadScript("Content/Scripts/DemoInput.lua");
			// light controls
			if(input->IsKeyDown(DIK_R))
			{
				Color c = m_light.GetDiffuseColor();
				float r = c.R();
				if(input->IsKeyDown(DIK_LSHIFT))
				{
					script->RunFunction("SubtractColorValue",r,1,1);
					r = (float)script->GetResultNumber();
					//m_light.SetDiffuseColor(r,c.G(),c.B(),c.A());
				}
				else
				{
					script->RunFunction("AddColorValue",r,1,1);
					r = (float)script->GetResultNumber();
					//m_light.SetDiffuseColor(r,c.G(),c.B(),c.A());
				}

			}

			if(input->IsKeyDown(DIK_G))
			{
				Color c = m_light.GetDiffuseColor();
				float g = c.G();
				if(input->IsKeyDown(DIK_LSHIFT))
				{
					script->RunFunction("SubtractColorValue",g,1,1);
					g = (float)script->GetResultNumber();
					//m_light.SetDiffuseColor(c.R(),g,c.B(),c.A());
				}
				else
				{
					script->RunFunction("AddColorValue",g,1,1);
					g = (float)script->GetResultNumber();
					//m_light.SetDiffuseColor(c.R(),g,c.B(),c.A());
				}

			}

			if(input->IsKeyDown(DIK_U))
				rotationX += 1.0;

			if(input->IsKeyDown(DIK_B))
			{
				Color c = m_light.GetDiffuseColor();
				float b = c.B();
				if(input->IsKeyDown(DIK_LSHIFT))
				{
					script->RunFunction("SubtractColorValue",b,1,1);
					b = (float)script->GetResultNumber();
					//m_light.SetDiffuseColor(c.B(),c.G(),b,c.A());
				}
				else
				{
					script->RunFunction("AddColorValue",b,1,1);
					b = (float)script->GetResultNumber();
					//m_light.SetDiffuseColor(c.B(),c.G(),b,c.A());
				}

			}
			// check for joystick movements
			Vector2 thumbstick(gamepadState.thumbSticks.leftX,gamepadState.thumbSticks.leftY);

			movement.x += thumbstick.x;
			movement.y -= thumbstick.y;

			if(movement.Length() > 1)
				movement.Normalize();

			rotation += -thumbstick.x * static_cast<float>(*g_XMPi) * 1.5f;

			m_camera->MoveCamera(m_ScreenManager->GetScript(), input);

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

			//m_camera.SetRotation(rotationX,rotationY,0.0f);
			obj->Update(deltaTime);

//			m_light.SetDirection(Vector3(rotationX,rotationY,rotationZ));
			Vector3 v = m_light.GetDirection();
			Color c = m_light.GetDiffuseColor();
			printf("Direction (%f,%f,%f)\n",v.x,v.y,v.z);
			printf("Diffuse (%f,%f,%f)\n",c.x,c.y,c.z);
		}
	}

	void GameplayScreen::Draw(float deltaTime)
	{
		// get the spritebatch object
		SpriteBatch* spriteBatch = m_ScreenManager->SpriteBatch();
		// get the graphics object
		Graphics* graphics = m_ScreenManager->GetGraphicsDevice();

		// must be set or there will be depth problems
		graphics->GetImmediateContex()->OMSetDepthStencilState(graphics->GetDepthStencilState(),1);
		// Now set the rasterizer state.
		graphics->GetImmediateContex()->RSSetState(graphics->GetRasterState());

		// get the font object
		SpriteFont* font = m_Font;
		Matrix viewMatrix,projMatrix,worldMatrix;

		// generate the view matrix based on the camera's position
		m_camera->Render();

		// get the world,view, and projection matrices from the camera
		viewMatrix = m_camera->GetViewMatrix();
		worldMatrix = obj->GetWorldMatrix(); // m_ScreenManager->GetGraphicsDevice()->GetWorldMatrix();
		projMatrix = m_ScreenManager->GetGraphicsDevice()->GetProjectionMatrix();
		//worldMatrix *= Matrix::CreateRotationX(rotationZ);
		// set up the frustum
		m_frustum.Construct(m_ScreenManager->GetGraphicsDevice()->GetScreenDepth(),projMatrix,viewMatrix);
		
		NormalMapShader* normalMap = m_ScreenManager->GetGraphicsDevice()->GetNormalMapShader();
		normalMap->SetWorldViewProjMatrices(worldMatrix,viewMatrix,projMatrix);
		normalMap->SetDiffuseColor(m_light.GetDiffuseColor());
		normalMap->SetLightDiretion(m_light.GetDirection());
		// clear the screen to a different color
		graphics->Clear(Colors::Blue);

		// render the model
		//graphics->Render(m_model);
		obj->Render(graphics);

//		spriteBatch->Begin();

//		spriteBatch->End();

		// If the game is transitioning on or off, fade it out to black.
		if (m_TransitionPosition > 0 || m_PauseAlpha > 0)
		{
			float alpha =  MathHelper::Lerp(1.0f - TransitionAlpha(), 1.0f, m_PauseAlpha / 2) + 0.30f;
			m_ScreenManager->FadeBackBufferToBlack(alpha);
		}
	}

}