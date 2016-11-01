#pragma once

class Shader;
class Transform;
class Frustum;
class Texture;
class Triangulator;

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
	int GetVertexCount();
	Transform* GetTransform() { return m_pTransform; }

private:
	//Planet parameters
	float m_Radius = 1737.1f;
	float m_MaxHeight = 10.7f;

	Transform *m_pTransform = nullptr;
	bool m_Rotate = false;
	Texture* m_pDiffuse = nullptr;

	Triangulator* m_pTriangulator;

	//OpenGl stuff
	GLuint m_VAO;
	GLuint m_VBO;

	Shader *m_pTriangleShader = nullptr;
	GLint m_uModel;
	GLint m_uViewProj;

	Shader *m_pWireShader = nullptr;
	GLint m_uModelWire;
	GLint m_uViewProjWire;
};