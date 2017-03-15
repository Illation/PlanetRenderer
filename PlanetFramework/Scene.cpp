#include "stdafx.h"
#include "Scene.h"

#include "Camera.h"
#include "./PlanetTech/Types/Moon.h"
#include "./PlanetTech/Types/Earth.h"
#include "SpriteFont.h"
#include "TextRenderer.h"

#include <sstream>
#include <iomanip>

//#include "Screenshot.h"

Scene::Scene()
{
	m_pPlanet = new Moon();
	m_pDebugFont = new SpriteFont();
}

void Scene::Init()
{
	//Set scene context
	m_pTime = new Time();
	m_pCamera = new Camera();
	m_pConObj = new ContextObjects();
	m_pConObj->pTime = m_pTime;
	m_pConObj->pCamera = m_pCamera;
	m_pConObj->pScene = this;
	CONTEXT->SetContext(m_pConObj);

	//Initialize text rendering
	TextRenderer::GetInstance()->Init();
	m_pDebugFont->Load("./Fonts/Consolas_32.fnt");

	//Init screenshot capturer
	//Screenshot::GetInstance()->Init("./Screenshots/");

	//Init planet stuff
	m_pPlanet->Init();
	m_pCamera->SetPlanet(m_pPlanet);

	//start time
	m_pConObj->pTime->Start();
}

void Scene::Update()
{
	m_pConObj->pTime->Update();
	m_pCamera->Update();

	if (INPUT->IsKeyboardKeyPressed('z'))
	{
		switch (renderMode)
		{
		case Scene::SOLID:
			renderMode=WIREFRAME;
			break;
		case Scene::WIREFRAME:
			renderMode = OVERLAY;
			break;
		case Scene::OVERLAY:
			renderMode = SOLID;
			break;
		}
	}
	if (INPUT->IsKeyboardKeyPressed(SDL_SCANCODE_F2))m_DrawUI = !m_DrawUI;
	if (INPUT->IsKeyboardKeyPressed(SDL_SCANCODE_F1))
	{
	//	Screenshot::GetInstance()->Take();
	}

	m_pPlanet->Update();
}

void Scene::Draw()
{
	//Draw debug info
	TextRenderer::GetInstance()->SetFont(m_pDebugFont);
	TextRenderer::GetInstance()->SetColor(glm::vec4(1, 0.3f, 0.3f, 1));
	auto fpsStr = std::string("FPS: ")+std::to_string((int)TIME->FPS());
	TextRenderer::GetInstance()->DrawText(fpsStr, glm::vec2(20, 20));

	TextRenderer::GetInstance()->SetColor(glm::vec4(1, 1, 1, 1));
	auto vertStr = std::string("vertex count: ") + std::to_string(m_pPlanet->GetVertexCount());
	TextRenderer::GetInstance()->DrawText(vertStr, glm::vec2(20, 60));

	std::stringstream ss;
	ss << std::fixed << std::setprecision(3) << m_pCamera->GetAltitude();
	auto altStr = std::string("altitude: ") + ss.str() + "km";
	TextRenderer::GetInstance()->DrawText(altStr, glm::vec2(20, 100));

	auto fovStr = std::string("FOV: ") + std::to_string(m_pCamera->GetFOV());
	TextRenderer::GetInstance()->DrawText(fovStr, glm::vec2(20, 140));

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDepthRange(0.00001, 1.0);
	if (renderMode == SOLID || renderMode == OVERLAY)
	{
		m_pPlanet->Draw();
	}

	//Debug Drawing
	glDepthRange(0.0, 0.999999);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (renderMode == WIREFRAME || renderMode == OVERLAY)
	{
		//m_pPlanet->Draw();
		m_pPlanet->DrawWire();
	}

	//Draw text
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDepthRange(0.000001, 1.0);
	if(m_DrawUI)TextRenderer::GetInstance()->Draw();
}

void Scene::PostDraw()
{
	//Take a screenshot
	//Screenshot::GetInstance()->Capture();
}

Scene::~Scene()
{
	SafeDelete(m_pPlanet);

	SafeDelete(m_pDebugFont);
	TextRenderer::GetInstance()->DestroyInstance();
	//Screenshot::GetInstance()->DestroyInstance();

	SafeDelete(m_pTime);
	SafeDelete(m_pCamera);
	SafeDelete(m_pConObj);
}
