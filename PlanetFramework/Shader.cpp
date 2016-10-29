#include "stdafx.h"
#include "Shader.h"


Shader::Shader(std::string filename, bool build)
	:m_FileName(filename)
{
	if (build) Build();
}

void Shader::Build()
{
	std::cout << "Building Shader: " << m_FileName << " . . . ";

	std::string vertSource;
	std::string geoSource;
	std::string fragSource;

	enum ParseState {
		INIT,
		VERT,
		GEO,
		FRAG
	} state = ParseState::INIT;
	bool useGeo = false;

	std::string extractedLine;
	std::ifstream shaderFile;
	shaderFile.open(m_FileName);
	if (!shaderFile)
	{
		std::cout << "  . . . FAILED!" << std::endl;
		std::cout << "    Opening shader file failed." << std::endl;
		return;
	}
	while (shaderFile.eof() == false)
	{
		//Get the line
		getline(shaderFile, extractedLine);
		//Precompile types
		switch (state)
		{
		case INIT:
			if (extractedLine.find("<VERTEX>") != std::string::npos)
			{
				state = ParseState::VERT;
			}
			if (extractedLine.find("<GEOMETRY>") != std::string::npos)
			{
				useGeo = true;
				state = ParseState::GEO;
			}
			if (extractedLine.find("<FRAGMENT>") != std::string::npos)
			{
				state = ParseState::FRAG;
			}
			break;
		case VERT:
			if (extractedLine.find("</VERTEX>") != std::string::npos)
			{
				state = ParseState::INIT;
				break;
			}
			vertSource += extractedLine;
			vertSource += "\n";
			break;
		case GEO:
			if (extractedLine.find("</GEOMETRY>") != std::string::npos)
			{
				state = ParseState::INIT;
				break;
			}
			geoSource += extractedLine;
			geoSource += "\n";
			break;
		case FRAG:
			if (extractedLine.find("</FRAGMENT>") != std::string::npos)
			{
				state = ParseState::INIT;
				break;
			}
			fragSource += extractedLine;
			fragSource += "\n";
			break;
		}
	}
	shaderFile.close();

	//Compile
	m_VertexShader = CompileShader(vertSource, GL_VERTEX_SHADER);
	if (useGeo)m_GeometryShader = CompileShader(geoSource, GL_GEOMETRY_SHADER);
	m_FragmentShader = CompileShader(fragSource, GL_FRAGMENT_SHADER);

	//Combine Shaders
	m_ShaderProgram = glCreateProgram();
	glAttachShader(m_ShaderProgram, m_VertexShader);
	if (useGeo)glAttachShader(m_ShaderProgram, m_GeometryShader);
	glAttachShader(m_ShaderProgram, m_FragmentShader);
	glBindFragDataLocation(m_ShaderProgram, 0, "outColor");
	glLinkProgram(m_ShaderProgram);

	// Print linking errors if any
	GLint status;
	glGetProgramiv(m_ShaderProgram, GL_LINK_STATUS, &status);
	if (!(status == GL_TRUE))
	{
		char buffer[512];
		glGetProgramInfoLog(m_ShaderProgram, 512, NULL, buffer);
		std::cout << "[ERROR] Shader>Build: Program linking failed\n" << buffer << std::endl;
		std::cout << "  . . . FAILED!" << std::endl;
	}
	else std::cout << "  . . . SUCCESS!" << std::endl;

	//Delete those shaders
	glDeleteShader(m_VertexShader);
	if (useGeo)glDeleteShader(m_GeometryShader);
	glDeleteShader(m_FragmentShader);
}

GLuint Shader::CompileShader(const std::string &shaderSourceStr, GLenum type)
{
	const char *shaderSource = shaderSourceStr.c_str();
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &shaderSource, NULL);

	GLint status;
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (!(status == GL_TRUE))
	{
		char buffer[512];
		glGetShaderInfoLog(shader, 512, NULL, buffer);
		std::cout << "  . . . FAILED!" << std::endl;
		std::string sName;
		switch (type)
		{
		case GL_VERTEX_SHADER:
			sName = "vertex";
			break;
		case GL_GEOMETRY_SHADER:
			sName = "geometry";
			break;
		case GL_FRAGMENT_SHADER:
			sName = "fragment";
			break;
		default:
			sName = "invalid type";
			break;
		}
		std::cout << "    Compiling " << sName << " shader failed." << std::endl;
		std::cout << "    " << buffer << std::endl;
	}

	return shader;
}

Shader::~Shader()
{
	glDeleteProgram(m_ShaderProgram);
	//glDeleteShader(m_FragmentShader);
	//if (m_UseGS)glDeleteShader(m_GeometryShader);
	//glDeleteShader(m_VertexShader);
}
