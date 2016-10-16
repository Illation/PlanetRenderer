#include "stdafx.h"
#include "Planet.h"

#include "Shader.h"
#include "Transform.h"
#include "Camera.h"

Planet::Planet()
{
	m_pTransform = new Transform();
	
	m_pTriangleShader = new Shader("./Shaders/triangle.glsl");
	m_pWireShader = new Shader("./Shaders/wire.glsl");
}

void Planet::Init()
{
	//Handle planet material
	m_pWireShader->Build();
	glUseProgram(m_pWireShader->GetProgram());
	m_uModelWire = glGetUniformLocation(m_pWireShader->GetProgram(), "model");
	m_uViewProjWire = glGetUniformLocation(m_pWireShader->GetProgram(), "viewProj");

	m_pTriangleShader->Build();
	glUseProgram(m_pTriangleShader->GetProgram());
	m_uModel = glGetUniformLocation(m_pTriangleShader->GetProgram(), "model");
	m_uViewProj = glGetUniformLocation(m_pTriangleShader->GetProgram(), "viewProj");

	//Generate buffers and arrays
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	//glGenBuffers(1, &m_EBO);

	GenerateGeometry();

	//bind
	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);

	//set data and attributes
	glBufferData(GL_ARRAY_BUFFER, m_Positions.size() * sizeof(glm::vec3), m_Positions.data(), GL_STATIC_DRAW);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(GLuint), m_Indices.data(), GL_STATIC_DRAW);
	
	//input layout
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Planet::Update()
{
	m_pTransform->SetPosition(0, 0, 0);
	m_pTransform->SetRotation(glm::rotate(m_pTransform->GetRotation(), -(GLfloat)TIME->DeltaTime() * 0.4f, glm::vec3(0.5f, 1.0f, 0.0f)));

	m_pTransform->UpdateTransforms();


	//Change Planet
	//***************
	bool geometryChanged = false;
	if (INPUT->IsKeyboardKeyPressed(SDL_SCANCODE_UP))
	{
		m_MaxLevel++;
		geometryChanged = true;
	}
	if (INPUT->IsKeyboardKeyPressed(SDL_SCANCODE_DOWN) && m_MaxLevel>0)
	{
		m_MaxLevel--;
		geometryChanged = true;
	}
	if (geometryChanged)
	{
		//Change the actual vertex positions
		GenerateGeometry();

		//Bind Object vertex array
		glBindVertexArray(m_VAO);

		//Send the vertex buffer again
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, m_Positions.size() * sizeof(glm::vec3), m_Positions.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//Done Modifying
		glBindVertexArray(0);
	}

	std::cout << "FPS: " << TIME->FPS() << " - Vertices: " << m_Positions.size() <<std::endl;
}

void Planet::GenerateGeometry()
{
	//Generate icosahedron
	float ratio = (1.f + sqrt(5.f)) / 2.f;
	float scale = m_Radius/glm::length(glm::vec2(ratio, 1.f));
	ratio *= scale;

	std::vector<glm::vec3> ico;

	//X plane
	ico.push_back(glm::vec3(ratio, 0, -scale));		//rf 0
	ico.push_back(glm::vec3(-ratio, 0, -scale));	//lf 1
	ico.push_back(glm::vec3(ratio, 0, scale));		//rb 2
	ico.push_back(glm::vec3(-ratio, 0, scale));		//lb 3
	//Y plane													 
	ico.push_back(glm::vec3(0, -scale, ratio));		//db 4
	ico.push_back(glm::vec3(0, -scale, -ratio));	//df 5
	ico.push_back(glm::vec3(0, scale, ratio));		//ub 6
	ico.push_back(glm::vec3(0, scale, -ratio));		//uf 7
	//Z plane													 
	ico.push_back(glm::vec3(-scale, ratio, 0));		//lu 8
	ico.push_back(glm::vec3(-scale, -ratio, 0));	//ld 9
	ico.push_back(glm::vec3(scale, ratio, 0));		//ru 10
	ico.push_back(glm::vec3(scale, -ratio, 0));		//rd 11

	m_Positions.clear();

	RecursiveTriangle(ico[1], ico[3], ico[8], 0);
	RecursiveTriangle(ico[1], ico[3], ico[9], 0);
	RecursiveTriangle(ico[0], ico[2], ico[10], 0);
	RecursiveTriangle(ico[0], ico[2], ico[11], 0);

	RecursiveTriangle(ico[5], ico[7], ico[0], 0);
	RecursiveTriangle(ico[5], ico[7], ico[1], 0);
	RecursiveTriangle(ico[4], ico[6], ico[2], 0);
	RecursiveTriangle(ico[4], ico[6], ico[3], 0);

	RecursiveTriangle(ico[9], ico[11], ico[4], 0);
	RecursiveTriangle(ico[9], ico[11], ico[5], 0);
	RecursiveTriangle(ico[8], ico[10], ico[6], 0);
	RecursiveTriangle(ico[8], ico[10], ico[7], 0);

	RecursiveTriangle(ico[1], ico[7], ico[8], 0);
	RecursiveTriangle(ico[1], ico[5], ico[9], 0);
	RecursiveTriangle(ico[0], ico[7], ico[10], 0);
	RecursiveTriangle(ico[0], ico[5], ico[11], 0);

	RecursiveTriangle(ico[3], ico[6], ico[8], 0);
	RecursiveTriangle(ico[3], ico[4], ico[9], 0);
	RecursiveTriangle(ico[2], ico[6], ico[10], 0);
	RecursiveTriangle(ico[2], ico[4], ico[11], 0);
}

void Planet::RecursiveTriangle(glm::vec3 a, glm::vec3 b, glm::vec3 c, short level)
{
	//check if triangle in view
	bool visible = true;
	if (visible)
	{
		//check if subdivision is needed based on camera distance
		bool subdivide = level < m_MaxLevel;
		if (subdivide)
		{
			//find midpoints
			glm::vec3 A = b + ((c - b)*0.5f);
			glm::vec3 B = c + ((a - c)*0.5f);
			glm::vec3 C = a + ((b - a)*0.5f);
			//make the distance from center larger according to planet radius
			A *= m_Radius / glm::length(A);
			B *= m_Radius / glm::length(B);
			C *= m_Radius / glm::length(C);
			//Make 4 new triangles
			short nLevel = level + 1;
			RecursiveTriangle(a, B, C, nLevel);
			RecursiveTriangle(A, b, C, nLevel);
			RecursiveTriangle(A, B, c, nLevel);
			RecursiveTriangle(A, B, C, nLevel);
		}
		else //put the triangle in the buffer
		{
			m_Positions.push_back(a);
			m_Positions.push_back(b);
			m_Positions.push_back(c);
		}
	}
}

void Planet::Draw()
{
	//Enable this objects shader
	glUseProgram(m_pTriangleShader->GetProgram());
	
	// Pass transformations to the shader
	glUniformMatrix4fv(m_uModel, 1, GL_FALSE, glm::value_ptr(m_pTransform->GetTransform()));
	glUniformMatrix4fv(m_uViewProj, 1, GL_FALSE, glm::value_ptr(CAMERA->GetViewProj()));

	//Bind Object vertex array
	glBindVertexArray(m_VAO);

	//Draw the object
	glDrawArrays(GL_TRIANGLES, 0, m_Positions.size());

	//unbind vertex array
	glBindVertexArray(0);
}
void Planet::DrawWire()
{
	//Enable this objects shader
	glUseProgram(m_pWireShader->GetProgram());

	// Pass transformations to the shader
	glUniformMatrix4fv(m_uModelWire, 1, GL_FALSE, glm::value_ptr(m_pTransform->GetTransform()));
	glUniformMatrix4fv(m_uViewProjWire, 1, GL_FALSE, glm::value_ptr(CAMERA->GetViewProj()));

	//Bind Object vertex array
	glBindVertexArray(m_VAO);

	//Draw the object
	glDrawArrays(GL_TRIANGLES, 0, m_Positions.size());

	//unbind vertex array
	glBindVertexArray(0);
}

Planet::~Planet()
{
	glUseProgram(m_pTriangleShader->GetProgram());
	SafeDelete(m_pTriangleShader);
	glUseProgram(m_pWireShader->GetProgram());
	SafeDelete(m_pWireShader);
	SafeDelete(m_pTransform);

	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
}
