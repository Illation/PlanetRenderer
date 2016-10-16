#pragma once

class Shader;
class Transform;

class Planet
{
public:
	Planet();
	~Planet();

	void Init();
	void Update();
	void Draw();
	void DrawWire();

private:
	//Member functions
	void GenerateGeometry();
	void RecursiveTriangle(glm::vec3 a, glm::vec3 b, glm::vec3 c, short level);

	//DataMembers

	float m_Radius = 1;
	float m_MaxLevel = 0;

	Transform *m_pTransform = nullptr;

	//OpenGl stuff
	std::vector<glm::vec3> m_Positions;
	std::vector<GLuint> m_Indices;

	GLuint m_VAO;
	GLuint m_VBO;

	Shader *m_pTriangleShader = nullptr;
	GLint m_uModel;
	GLint m_uViewProj;

	Shader *m_pWireShader = nullptr;
	GLint m_uModelWire;
	GLint m_uViewProjWire;
};

