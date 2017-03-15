#pragma once
#include "Singleton.h"
#include <string>
#ifdef PLATFORM_Win
	#include <SDL.h>
#else
	#include <SDL2/SDL.h>
#endif

struct Settings : public Singleton<Settings>
{
public:
	Settings():Window(WindowSettings())
	{

	}
	~Settings() {}

	struct WindowSettings
	{
		WindowSettings() :
			Fullscreen(true),
			VSyncEnabled(false),
			AspectRatio(Width / (float)Height),
			Title("OpenGl Planet Renderer"),
			pWindow(nullptr)
		{
			#if defined(DEBUG) | defined(_DEBUG)
				Fullscreen = false;
			#endif
			Width = Fullscreen ? 1920 : 1280;
			Height = Fullscreen ? 1080 : 720;
		}
		bool Fullscreen;
		bool VSyncEnabled;
		int Width;
		int Height;
		float AspectRatio;
		std::string Title;
		SDL_Window* pWindow;
	}Window;
};

