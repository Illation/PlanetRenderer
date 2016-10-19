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

private:
	std::vector<Plane> m_Planes;

	glm::mat4 m_CullWorld, m_CullInverse;
	Transform* m_pTransform;
	float m_NearPlane, m_FarPlane, m_FOV;

	std::vector<glm::vec3> m_Corners;
	GLuint m_VAO;
	GLuint m_VBO;
	GLint m_uModelWire;
	GLint m_uViewProjWire;
};

