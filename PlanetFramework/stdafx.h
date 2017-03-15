#pragma once

#pragma region
//SDL and opengl Header files
#include "StaticDependancies/glad/glad.h"
#ifdef PLATFORM_Win
	#include <SDL.h>
	#include <SDL_opengl.h>
#else
	#include <SDL2/SDL.h>
	#include <SDL2/SDL_opengl.h>
#endif
#pragma endregion sdl-opengl

#pragma region
//C RunTime Header Files
#include <string>
#include <iostream>
#include <fstream>
#include <istream>
#include <vector>
#pragma endregion stl

#pragma region
#pragma warning(push, 0)
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#pragma warning(pop)
#ifndef GLM_LEFT_HANDED
	#define GLM_LEFT_HANDED
#endif
#pragma endregion glm

#pragma region
//*****************************************************************************
//Declare templates for releasing interfaces and deleting objects 
//*****************************************************************************
template<class T>
inline void SafeDelete(T &pObjectToDelete)
{
	if (pObjectToDelete != 0)
	{
		delete(pObjectToDelete);
		pObjectToDelete = 0;
	}
}
#pragma endregion Templates

#pragma region
#include "Settings.h"
#include "Context.h"
#include "InputManager.h"
#define SETTINGS Settings::GetInstance()
#define WINDOW Settings::GetInstance()->Window
#define CONTEXT Context::GetInstance()
#define TIME Context::GetInstance()->pTime
#define SCENE Context::GetInstance()->pScene
#define CAMERA Context::GetInstance()->pCamera
#define INPUT InputManager::GetInstance()
#pragma endregion Macros

//TODO move this to a custom file with a solid ruleset
#define UINT32 unsigned int
#define UINT unsigned int
#define BYTE char
#define USHORT unsigned short
#define UCHAR unsigned char
