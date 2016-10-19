#pragma once

class Shader;
class Transform;
class Frustum;

enum TriNext
{
	CULL,
	LEAF,
	SPLIT,
	SPLITCULL
};

class Planet
{
public:
	Planet();
	~Planet();

	void Init();
	void Update();
	void Draw();
	void DrawWire();

	float GetRadius(){ return m_Radius; }

private:
	//Member functions
	void GenerateGeometry();
	TriNext SplitHeuristic(glm::vec3 &a, glm::vec3 &b, glm::vec3 &c, int level, bool frustumCull);
	void RecursiveTriangle(glm::vec3 a, glm::vec3 b, glm::vec3 c, int level, bool frustumCull);

	glm::mat4 m_ToWorld;
	glm::vec3 m_CamForward;
	glm::vec3 m_CamPos;

	//DataMembers
	float m_Radius = 1;
	int m_MaxLevel = 0;

	Transform *m_pTransform = nullptr;
	bool m_Rotate = false;

	Frustum* m_pFrustum = nullptr;
	bool m_LockFrustum = false;

	std::vector<glm::vec3> m_Icosahedron;

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

