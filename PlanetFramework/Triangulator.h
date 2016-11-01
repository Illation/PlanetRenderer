#pragma once

class Frustum;
class Planet;

enum TriNext
{
	CULL,
	LEAF,
	SPLIT,
	SPLITCULL
};

class Triangulator
{
public:
	Triangulator(Planet* pPlanet);
	~Triangulator();

	//Member functions
	void Init();
	bool Update();
	void GenerateGeometry();

	bool IsFrustumLocked() { return m_LockFrustum; }
	Frustum* GetFrustum() { return m_pFrustum; }
	int GetVertexCount() { return m_Positions.size(); }

private:
	friend class Planet;

	void Precalculate();
	bool DistanceExceeds(short level, glm::vec3& center);
	TriNext SplitHeuristic(glm::vec3 &a, glm::vec3 &b, glm::vec3 &c, short level, bool frustumCull);
	void RecursiveTriangle(glm::vec3 a, glm::vec3 b, glm::vec3 c, short level, bool frustumCull);

	//Triangulation paramenters
	float m_AllowedTriPx = 60.f;
	float m_AllowedScreenPerc;
	int m_MaxLevel = 15;

	std::vector<glm::vec3> m_Icosahedron;
	std::vector<float> m_TriLevelSizeLUT;
	std::vector<float> m_TriLevelDotLUT;
	std::vector<float> m_HeightMultLUT;

	Planet* m_pPlanet = nullptr;
	Frustum* m_pFrustum = nullptr;
	bool m_LockFrustum = false;

	std::vector<glm::vec3> m_Positions;
};

