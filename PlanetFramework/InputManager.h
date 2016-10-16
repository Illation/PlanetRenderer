#pragma once
#include <map>
#include <SDL.h>
#include <glm\glm.hpp>
#include "Singleton.h"
//----------------------------
//Event Manager class definintion
//----------------------------
class InputManager : public Singleton<InputManager>
{
public:
	//Constructor Destructor
	InputManager();
	virtual ~InputManager();
	//----------------------------
	//Member functions
	//----------------------------
	//Keyboard

	// True if user starts pressing key
	bool IsKeyboardKeyPressed(SDL_Scancode key);
	// True if user starts pressing key
	bool IsKeyboardKeyPressed(char key);
	// True if user is pressing key
	bool IsKeyboardKeyDown(SDL_Scancode key);
	// True if user is pressing key
	bool IsKeyboardKeyDown(char key);
	// True if user stops pressing key
	bool IsKeyboardKeyReleased(SDL_Scancode key);
	// True if user stops pressing key
	bool IsKeyboardKeyReleased(char key);

	//Mouse

	//True if user starts pressing button
	bool IsMouseButtonPressed(int button);
	//True if user is pressing button
	bool IsMouseButtonDown(int button);
	//True if user stops pressing button
	bool IsMouseButtonReleased(int button);
	//Returns mouse position as vector
	glm::vec2 GetMousePosition();
	glm::vec2 GetMouseMovement() { return m_MouseMove; }

public:
	void Init();//call after SDL init
	void UpdateEvents();//call before all GUI ticks
	bool IsExitRequested();
private:
	friend class AbstractFramework;
	//----------------------------
	// Member functions
	//----------------------------

	//Getters

	// Gets the SDL_Scancode for a char
	// Returns false if char is not mapped
	bool GetScancode(SDL_Scancode &code, char key);

	//----------------------------
	// Data Members
	//----------------------------

	//Key Input
	Uint8 *m_pKeyMapNew = nullptr,
		*m_pKeyMapOld = nullptr;
	const Uint8 *m_pKeyMapSdl = nullptr;
	int m_KeyboardLength;
	std::map<char, SDL_Scancode> m_CharToSdlMap;

	//Mouse Input
	int m_MousePosX = 0;
	int m_MousePosY = 0;
	glm::vec2 m_MouseMove = glm::vec2();
	Uint32 m_MouseMapNew,
		m_MouseMapOld;
	//Application flow
	bool m_ExitRequested = false;
};

