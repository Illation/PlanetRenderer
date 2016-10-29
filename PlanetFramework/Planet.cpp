#include "stdafx.h"
#include "Planet.h"

#include <limits>

#include "Shader.h"
#include "Transform.h"
#include "Camera.h"
#include "Frustum.h"
#include "Texture.h"

Planet::Planet()
{
	m_pTransform = new Transform();

	m_pFrustum = new Frustum();
	
	m_pTriangleShader = new Shader("./Shaders/triangle.glsl");
	m_pWireShader = new Shader("./Shaders/wire.glsl");

	m_pDiffuse = new Texture("./Textures/moon8k.jpg");
}

void Planet::Init()
{
	//Generate icosahedron
	float ratio = (1.f + sqrt(5.f)) / 2.f;
	float scale = m_Radius / glm::length(glm::vec2(ratio, 1.f));
	ratio *= scale;

	m_pTransform->SetRotation(glm::rotate(m_pTransform->GetRotation(), glm::radians(270.f), glm::vec3(0.0f, 1.0f, 0.0f)));

	//X plane
	m_Icosahedron.push_back(glm::vec3(ratio, 0, -scale));		//rf 0
	m_Icosahedron.push_back(glm::vec3(-ratio, 0, -scale));		//lf 1
	m_Icosahedron.push_back(glm::vec3(ratio, 0, scale));		//rb 2
	m_Icosahedron.push_back(glm::vec3(-ratio, 0, scale));		//lb 3
	//Y plane													 
	m_Icosahedron.push_back(glm::vec3(0, -scale, ratio));		//db 4
	m_Icosahedron.push_back(glm::vec3(0, -scale, -ratio));		//df 5
	m_Icosahedron.push_back(glm::vec3(0, scale, ratio));		//ub 6
	m_Icosahedron.push_back(glm::vec3(0, scale, -ratio));		//uf 7
	//Z plane													 
	m_Icosahedron.push_back(glm::vec3(-scale, ratio, 0));		//lu 8
	m_Icosahedron.push_back(glm::vec3(-scale, -ratio, 0));		//ld 9
	m_Icosahedron.push_back(glm::vec3(scale, ratio, 0));		//ru 10
	m_Icosahedron.push_back(glm::vec3(scale, -ratio, 0));		//rd 11

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
	glBufferData(GL_ARRAY_BUFFER, m_Positions.size() * sizeof(glm::vec3), m_Positions.data(), GL_DYNAMIC_DRAW);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(GLuint), m_Indices.data(), GL_STATIC_DRAW);
	
	//input layout
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);



	m_pFrustum->Init();
}

void Planet::Update()
{
	m_pTransform->SetPosition(0, 0, 0);
	if (INPUT->IsKeyboardKeyPressed('r'))m_Rotate = !m_Rotate;
	if(m_Rotate)m_pTransform->SetRotation(glm::rotate(m_pTransform->GetRotation(), -(GLfloat)TIME->DeltaTime() * 0.1f, glm::vec3(0.0f, 1.0f, 0.0f)));

	m_pTransform->UpdateTransforms();
	//Change Planet
	//***************

	//Set Max subd level
	bool geometryChanged = true;
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
	if (CAMERA->HasMoved())geometryChanged = true;

	//Frustum update
	if (INPUT->IsKeyboardKeyPressed(SDL_SCANCODE_SPACE))m_LockFrustum = !m_LockFrustum;
	m_pFrustum->SetCullTransform(m_pTransform->GetTransform());
	if (!m_LockFrustum) m_pFrustum->SetToCamera(CAMERA);
	m_pFrustum->Update();

	if (geometryChanged)
	{
		//Change the actual vertex positions
		GenerateGeometry();
		//Bind Object vertex array
		glBindVertexArray(m_VAO);

		//Send the vertex buffer again
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, m_Positions.size() * sizeof(glm::vec3), m_Positions.data(), GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//Done Modifying
		glBindVertexArray(0);
	}
	//std::cout << "FPS: " << TIME->FPS() << " - Vertices: " << m_Positions.size() <<std::endl;
	//std::cout << std::endl << std::endl << std::endl;
}

void Planet::GenerateGeometry()
{
	m_Positions.clear();

	//Calculate Look up tables
	m_TriLevelSizeLUT.clear();
	m_TriLevelSizeLUT.push_back(glm::length(m_Icosahedron[3] - m_Icosahedron[1]));
	m_TriLevelDotLUT.clear();
	m_TriLevelDotLUT.push_back(0.5f);
	float angle = acosf(m_TriLevelDotLUT[0]);
	for (int i = 1; i < m_MaxLevel; i++)
	{
		m_TriLevelSizeLUT.push_back(m_TriLevelSizeLUT[i-1]*0.5f);
		angle *= 0.5f;
		m_TriLevelDotLUT.push_back(sin(angle));
	}
	m_AllowedScreenPerc = m_AllowedTriPx / WINDOW.Width;

	RecursiveTriangle(m_Icosahedron[1], m_Icosahedron[3], m_Icosahedron[8], 0, true);
	RecursiveTriangle(m_Icosahedron[1], m_Icosahedron[3], m_Icosahedron[9], 0, true);
	RecursiveTriangle(m_Icosahedron[0], m_Icosahedron[2], m_Icosahedron[10], 0, true);
	RecursiveTriangle(m_Icosahedron[0], m_Icosahedron[2], m_Icosahedron[11], 0, true);

	RecursiveTriangle(m_Icosahedron[5], m_Icosahedron[7], m_Icosahedron[0], 0, true);
	RecursiveTriangle(m_Icosahedron[5], m_Icosahedron[7], m_Icosahedron[1], 0, true);
	RecursiveTriangle(m_Icosahedron[4], m_Icosahedron[6], m_Icosahedron[2], 0, true);
	RecursiveTriangle(m_Icosahedron[4], m_Icosahedron[6], m_Icosahedron[3], 0, true);

	RecursiveTriangle(m_Icosahedron[9], m_Icosahedron[11], m_Icosahedron[4], 0, true);
	RecursiveTriangle(m_Icosahedron[9], m_Icosahedron[11], m_Icosahedron[5], 0, true);
	RecursiveTriangle(m_Icosahedron[8], m_Icosahedron[10], m_Icosahedron[6], 0, true);
	RecursiveTriangle(m_Icosahedron[8], m_Icosahedron[10], m_Icosahedron[7], 0, true);

	RecursiveTriangle(m_Icosahedron[1], m_Icosahedron[7], m_Icosahedron[8], 0, true);
	RecursiveTriangle(m_Icosahedron[1], m_Icosahedron[5], m_Icosahedron[9], 0, true);
	RecursiveTriangle(m_Icosahedron[0], m_Icosahedron[7], m_Icosahedron[10], 0, true);
	RecursiveTriangle(m_Icosahedron[0], m_Icosahedron[5], m_Icosahedron[11], 0, true);

	RecursiveTriangle(m_Icosahedron[3], m_Icosahedron[6], m_Icosahedron[8], 0, true);
	RecursiveTriangle(m_Icosahedron[3], m_Icosahedron[4], m_Icosahedron[9], 0, true);
	RecursiveTriangle(m_Icosahedron[2], m_Icosahedron[6], m_Icosahedron[10], 0, true);
	RecursiveTriangle(m_Icosahedron[2], m_Icosahedron[4], m_Icosahedron[11], 0, true);
}

bool Planet::DistanceExceeds(short level, glm::vec3 &center)
{
	//if (dist*level < m_SplitDist);
	float dist = glm::length(center - m_pFrustum->GetPositionOS());
	float sizeL = m_TriLevelSizeLUT[level];
	float screenPerc = atanf(sizeL / dist) * m_pFrustum->GetRadInvFOV();
	return screenPerc > m_AllowedScreenPerc;
}
TriNext Planet::SplitHeuristic(glm::vec3 &a, glm::vec3 &b, glm::vec3 &c, short level, bool frustumCull)
{
	glm::vec3 center = (a + b + c) / 3.f;
	//Perform backface culling
	float dotNV = glm::dot(glm::normalize(center), glm::normalize(center - m_pFrustum->GetPositionOS()));
	if (dotNV >= m_TriLevelDotLUT[level])
	{
		return TriNext::CULL;
	}

	//Perform Frustum culling
	if (frustumCull)
	{
		auto intersect = m_pFrustum->ContainsTriangle(a, b, c);
		if (intersect == VolumeTri::OUTSIDE) return TriNext::CULL;
		if (intersect == VolumeTri::CONTAINS)//stop frustum culling -> all children are also inside the frustum
		{
			//check if new splits are allowed
			if (level >= m_MaxLevel)return TriNext::LEAF;
			//split according to distance
			if (DistanceExceeds(level, center))return TriNext::SPLIT;
			return TriNext::LEAF;
		}
	}
	//check if new splits are allowed
	if (level >= m_MaxLevel)return TriNext::LEAF;
	//split according to distance
	if (DistanceExceeds(level, center))return TriNext::SPLITCULL;
	return TriNext::LEAF;
}
void Planet::RecursiveTriangle(glm::vec3 a, glm::vec3 b, glm::vec3 c, short level, bool frustumCull)
{
	TriNext next = SplitHeuristic(a, b, c, level, frustumCull);
	if (next == CULL) return;
	//check if subdivision is needed based on camera distance
	else if (next == SPLIT || next==SPLITCULL)
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
		RecursiveTriangle(a, B, C, nLevel, next==SPLITCULL);
		RecursiveTriangle(A, b, C, nLevel, next == SPLITCULL);
		RecursiveTriangle(A, B, c, nLevel, next == SPLITCULL);
		RecursiveTriangle(A, B, C, nLevel, next == SPLITCULL);
	}
	else //put the triangle in the buffer
	{
		m_Positions.push_back(a);
		m_Positions.push_back(b);
		m_Positions.push_back(c);
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

	//draw the frustum if it is locked
	if (m_LockFrustum)
	{
		m_pFrustum->SetShaderAccessors(m_uModelWire, m_uViewProjWire);
		m_pFrustum->Draw();
	}
}

Planet::~Planet()
{
	glUseProgram(m_pTriangleShader->GetProgram());
	SafeDelete(m_pTriangleShader);
	glUseProgram(m_pWireShader->GetProgram());
	SafeDelete(m_pWireShader);
	SafeDelete(m_pFrustum);
	SafeDelete(m_pTransform);
	SafeDelete(m_pDiffuse);

	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
}
