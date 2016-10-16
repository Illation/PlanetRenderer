#pragma once

#pragma region
//SDL and opengl Header files
#include "staticDependancies/glad/glad.h"
#include <SDL.h>
#include <SDL_opengl.h>
#pragma endregion sdl-opengl

#pragma region
//C RunTime Header Files
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#pragma endregion stl

#pragma region
#pragma warning(push, 0)
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#pragma warning(pop)
#define GLM_LEFT_HANDED
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