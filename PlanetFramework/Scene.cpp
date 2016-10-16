#include "stdafx.h"
#include "Scene.h"

#include "Camera.h"
#include "Planet.h"

Scene::Scene()
{
	m_pPlanet = new Planet();
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

	m_pPlanet->Init();

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

	m_pPlanet->Update();
}

void Scene::Draw()
{
	if (renderMode == SOLID || renderMode == OVERLAY)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDepthRange(0.00001, 1.0);
		m_pPlanet->Draw();
	}
	if (renderMode == WIREFRAME || renderMode == OVERLAY)
	{
		glDepthRange(0.0, 0.99999);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		m_pPlanet->DrawWire();
	}
}

Scene::~Scene()
{
	SafeDelete(m_pPlanet);

	SafeDelete(m_pTime);
	SafeDelete(m_pCamera);
	SafeDelete(m_pConObj);
}
