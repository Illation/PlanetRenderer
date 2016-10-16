#pragma once


class Shader
{
public:
	Shader(std::string filename, bool build = false);
	~Shader();

	void Build();
	GLuint GetProgram() { return m_ShaderProgram; }

	std::string GetSourceLocation() { return m_FileName; }
private:
	GLuint CompileShader(const std::string &shaderSourceStr, GLenum type);

	GLuint m_VertexShader;
	GLuint m_GeometryShader;
	GLuint m_FragmentShader;
	GLuint m_ShaderProgram;
	bool m_UseGS = false;

	std::string m_FileName;
};

