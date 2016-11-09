#include "stdafx.h"
#include "Planet.h"

#include <limits>

#include "Shader.h"
#include "Transform.h"
#include "Camera.h"
#include "Frustum.h"
#include "Triangulator.h"
#include "Texture.h"

Planet::Planet()
{
	m_pTransform = new Transform();

	m_pTriangulator = new Triangulator(this);
	
	m_pTriangleShader = new Shader("./Shaders/triangle.glsl");
	m_pWireShader = new Shader("./Shaders/wire.glsl");

	m_pDiffuse = new Texture("./Textures/moon8k.jpg");
	m_pHeight = new Texture("./Textures/MoonHeight.jpg");
}

void Planet::Init()
{
	m_pTransform->SetRotation(glm::rotate(m_pTransform->GetRotation(), glm::radians(270.f), glm::vec3(0.0f, 1.0f, 0.0f)));

	//Handle planet material
	m_pWireShader->Build();
	glUseProgram(m_pWireShader->GetProgram());
	m_uModelWire = glGetUniformLocation(m_pWireShader->GetProgram(), "model");
	m_uViewProjWire = glGetUniformLocation(m_pWireShader->GetProgram(), "viewProj");

	m_pTriangleShader->Build();
	glUseProgram(m_pTriangleShader->GetProgram());
	m_uModel = glGetUniformLocation(m_pTriangleShader->GetProgram(), "model");
	m_uViewProj = glGetUniformLocation(m_pTriangleShader->GetProgram(), "viewProj");

	//LoadTextures
	m_pDiffuse->Load();
	glUniform1i(glGetUniformLocation(m_pTriangleShader->GetProgram(), "texDiffuse"), 0);
	m_pHeight->Load();
	glUniform1i(glGetUniformLocation(m_pTriangleShader->GetProgram(), "texHeight"), 1);
	glUseProgram(m_pWireShader->GetProgram());
	glUniform1i(glGetUniformLocation(m_pWireShader->GetProgram(), "texHeight"), 1);

	m_pTriangulator->Init();

	//Buffer Initialisation
	//*********************
	//Generate buffers and arrays
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	//bind
	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
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
	//m_pTransform->SetPosition(m_Radius*sinf(TIME->GetTime()), 0, 0);
	if (INPUT->IsKeyboardKeyPressed('r'))m_Rotate = !m_Rotate;
	if(m_Rotate)m_pTransform->SetRotation(glm::rotate(m_pTransform->GetRotation(), -(GLfloat)TIME->DeltaTime() * 0.1f, glm::vec3(0.0f, 1.0f, 0.0f)));

	m_pTransform->UpdateTransforms();

	//Change Planet Geometry
	//**********************
	if (m_pTriangulator->Update())
	{
		//Change the actual vertex positions
		m_pTriangulator->GenerateGeometry();
		//Bind Object vertex array
		glBindVertexArray(m_VAO);

		//Send the vertex buffer again
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, m_pTriangulator->m_Positions.size() * sizeof(glm::vec3), m_pTriangulator->m_Positions.data(), GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//Done Modifying
		glBindVertexArray(0);
	}
}

void Planet::Draw()
{
	//Enable this objects shader
	glUseProgram(m_pTriangleShader->GetProgram());
	
	// Pass transformations to the shader
	glUniformMatrix4fv(m_uModel, 1, GL_FALSE, glm::value_ptr(m_pTransform->GetTransform()));
	glUniformMatrix4fv(m_uViewProj, 1, GL_FALSE, glm::value_ptr(CAMERA->GetViewProj()));
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_pDiffuse->GetHandle());
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_pHeight->GetHandle());

	//Bind Object vertex array
	glBindVertexArray(m_VAO);

	//Draw the object
	glDrawArrays(GL_TRIANGLES, 0, m_pTriangulator->m_Positions.size());

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

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_pHeight->GetHandle());

	//Bind Object vertex array
	glBindVertexArray(m_VAO);

	//Draw the object
	glDrawArrays(GL_TRIANGLES, 0, m_pTriangulator->m_Positions.size());

	//unbind vertex array
	glBindVertexArray(0);

	//draw the frustum if it is locked
	if (m_pTriangulator->IsFrustumLocked())
	{
		m_pTriangulator->GetFrustum()->SetShaderAccessors(m_uModelWire, m_uViewProjWire);
		m_pTriangulator->GetFrustum()->Draw();
	}
}

Planet::~Planet()
{
	glUseProgram(m_pTriangleShader->GetProgram());
	SafeDelete(m_pTriangleShader);
	glUseProgram(m_pWireShader->GetProgram());
	SafeDelete(m_pWireShader);
	
	SafeDelete(m_pTransform);
	SafeDelete(m_pDiffuse);

	SafeDelete(m_pTriangulator);

	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
}

int Planet::GetVertexCount()
{
	return m_pTriangulator->GetVertexCount();
}