#pragma once

class Time;
class Camera;
class ContextObjects;

class Planet;

class Scene
{
public:
	Scene();
	~Scene();

	void Init();

	void Update();

	void Draw();

private:
	//Context
	Time *m_pTime = nullptr;
	Camera *m_pCamera = nullptr;
	ContextObjects *m_pConObj = nullptr;

	//Rendering
	enum RenderMode
	{
		SOLID,
		WIREFRAME,
		OVERLAY
	} renderMode = OVERLAY;

	//Planet tech
	Planet *m_pPlanet;
};

