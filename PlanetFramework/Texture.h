#pragma once

#include <string>

#include "stdafx.h"

class Texture
{
public:
	Texture(const std::string &filename);
	~Texture();

	void Load(bool useSRGB = false);
	GLuint GetHandle() { return m_Handle; }

	glm::vec2 GetDimension() { return glm::vec2(m_Width, m_Height); }
private:
	std::string m_Name;

	GLuint m_Handle;
	int m_Width;
	int m_Height;
};

