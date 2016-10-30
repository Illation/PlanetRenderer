#pragma once

class Shader;
class Transform;
class Frustum;
class Texture;

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
	int GetVertexCount() { return m_Positions.size(); }

private:
	//Member functions
	void GenerateGeometry();
	void Precalculate();
	bool DistanceExceeds(short level, glm::vec3& center);
	TriNext SplitHeuristic(glm::vec3 &a, glm::vec3 &b, glm::vec3 &c, short level, bool frustumCull);
	void RecursiveTriangle(glm::vec3 a, glm::vec3 b, glm::vec3 c, short level, bool frustumCull);

	//DataMembers
	float m_Radius = 1737.1f;
	float m_MaxHeight = 10.7f;
	int m_MaxLevel = 15;
	float m_SplitDist = 3*sqrtf(m_Radius);
	float m_AllowedTriPx = 60.f;

	std::vector<float> m_TriLevelSizeLUT;
	float m_AllowedScreenPerc;
	std::vector<float> m_TriLevelDotLUT;
	std::vector<float> m_HeightMultLUT;

	Transform *m_pTransform = nullptr;
	bool m_Rotate = false;

	Frustum* m_pFrustum = nullptr;
	bool m_LockFrustum = false;

	Texture* m_pDiffuse = nullptr;

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