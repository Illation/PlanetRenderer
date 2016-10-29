#pragma once

class Camera;
class Camera;
class Transform;

struct Plane
{
	Plane()
	{
		n = glm::vec3(0, 1, 0);
		d = glm::vec3(0, 0, 0);
	}
	Plane(glm::vec3 normal, glm::vec3 det)
	{
		n = normal;
		d = det;
	}
	Plane(glm::vec3 a, glm::vec3 b, glm::vec3 c)
	{
		d = a;
		n = glm::normalize(glm::cross(-b + a, c - a));
	}
	glm::vec3 n;
	glm::vec3 d;
};

enum class VolumeTri
{
	OUTSIDE,
	INTERSECT,
	CONTAINS
};

class Frustum
{
public:
	Frustum();
	~Frustum();

	void Init();
	void Update();
	void Draw();
	void SetShaderAccessors(GLint model, GLint viewProj)
	{
		m_uModelWire = model;
		m_uViewProjWire = viewProj;
	}

	void SetToCamera(Camera* pCamera);
	void SetCullTransform(glm::mat4 objectWorld);

	bool Contains(glm::vec3 p);
	VolumeTri ContainsTriangle(glm::vec3 &a, glm::vec3 &b, glm::vec3 &c);

	const glm::vec3 &GetPositionOS() { return m_PositionObject; }
	const float GetFOV() { return m_FOV; }
	const float GetRadInvFOV() { return m_RadInvFOV; }

private:
	//transform to the culled objects object space and back to world space
	glm::mat4 m_CullWorld, m_CullInverse;

	//stuff in the culled objects object space
	std::vector<Plane> m_Planes;
	glm::vec3 m_PositionObject;

	float m_RadInvFOV;

	//camera parameters for locking
	glm::vec3 m_Position;
	glm::vec3 m_Forward;
	glm::vec3 m_Up;
	glm::vec3 m_Right;
	float m_NearPlane, m_FarPlane, m_FOV;

	//debug rendering
	std::vector<glm::vec3> m_Corners;
	GLuint m_VAO;
	GLuint m_VBO;
	GLint m_uModelWire;
	GLint m_uViewProjWire;
};

