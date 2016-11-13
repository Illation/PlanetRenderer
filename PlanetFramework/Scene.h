#pragma once

class Time;
class Camera;
class ContextObjects;

class SpriteFont;

class Planet;

class Scene
{
public:
	Scene();
	~Scene();

	void Init();

	void Update();

	void Draw();
	void PostDraw();

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
	bool m_DrawUI = true;

	SpriteFont* m_pDebugFont = nullptr;

	//Planet tech
	Planet *m_pPlanet;
};

