#include "stdafx.h"
#include "Triangulator.h"

#include "Frustum.h"
#include "Planet.h"
#include "Transform.h"
#include "Camera.h"

Triangulator::Triangulator(Planet* pPlanet)
	: m_pPlanet(pPlanet)
{
	m_pFrustum = new Frustum();
}

void Triangulator::Init()
{
	//Generate icosahedron
	float ratio = (1.f + sqrt(5.f)) / 2.f;
	float scale = m_pPlanet->GetRadius() / glm::length(glm::vec2(ratio, 1.f));
	ratio *= scale;
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

	GenerateGeometry();
	m_pFrustum->Init();
}

bool Triangulator::Update()
{
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
	m_pFrustum->SetCullTransform(m_pPlanet->GetTransform()->GetTransform());
	if (!m_LockFrustum) m_pFrustum->SetToCamera(CAMERA);
	m_pFrustum->Update();

	return geometryChanged;
}

void Triangulator::Precalculate()
{
	//Calculate Look up tables
	m_TriLevelSizeLUT.clear();
	m_TriLevelSizeLUT.push_back(glm::length(m_Icosahedron[3] - m_Icosahedron[1]));
	m_TriLevelDotLUT.clear();
	m_TriLevelDotLUT.push_back(0.5f);
	float angle = acosf(m_TriLevelDotLUT[0]);
	for (int i = 1; i < m_MaxLevel; i++)
	{
		m_TriLevelSizeLUT.push_back(m_TriLevelSizeLUT[i - 1] * 0.5f);
		angle *= 0.5f;
		m_TriLevelDotLUT.push_back(sin(angle));
	}

	//height multipliers
	m_HeightMultLUT.clear();
	glm::vec3 a = m_Icosahedron[1];
	glm::vec3 b = m_Icosahedron[3];
	glm::vec3 c = m_Icosahedron[8];
	glm::vec3 center = (a + b + c) / 3.f;
	center *= m_pPlanet->GetRadius() / glm::length(center);//+maxHeight
	m_HeightMultLUT.push_back(1 / glm::dot(glm::normalize(a), glm::normalize(center)));
	for (int i = 1; i < m_MaxLevel; i++)
	{
		glm::vec3 A = b + ((c - b)*0.5f);
		glm::vec3 B = c + ((a - c)*0.5f);
		c = a + ((b - a)*0.5f);
		a = A*m_pPlanet->GetRadius() / glm::length(A);
		b = B*m_pPlanet->GetRadius() / glm::length(B);
		c *= m_pPlanet->GetRadius() / glm::length(c);
		m_HeightMultLUT.push_back(1 / glm::dot(glm::normalize(a), glm::normalize(center)));
	}

	m_AllowedScreenPerc = m_AllowedTriPx / WINDOW.Width;
}

void Triangulator::GenerateGeometry()
{
	Precalculate();
	m_Positions.clear();

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

bool Triangulator::DistanceExceeds(short level, glm::vec3 &center)
{
	//if (dist*level < m_SplitDist);
	float dist = glm::length(center - m_pFrustum->GetPositionOS());
	float sizeL = m_TriLevelSizeLUT[level];
	float screenPerc = atanf(sizeL / dist) * m_pFrustum->GetRadInvFOV();
	return screenPerc > m_AllowedScreenPerc;
}
TriNext Triangulator::SplitHeuristic(glm::vec3 &a, glm::vec3 &b, glm::vec3 &c, short level, bool frustumCull)
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
		auto intersect = m_pFrustum->ContainsTriVolume(a, b, c, m_HeightMultLUT[level]);
		//auto intersect = m_pFrustum->ContainsTriangle(a, b, c);
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

void Triangulator::RecursiveTriangle(glm::vec3 a, glm::vec3 b, glm::vec3 c, short level, bool frustumCull)
{
	TriNext next = SplitHeuristic(a, b, c, level, frustumCull);
	if (next == CULL) return;
	//check if subdivision is needed based on camera distance
	else if (next == SPLIT || next == SPLITCULL)
	{
		//find midpoints
		glm::vec3 A = b + ((c - b)*0.5f);
		glm::vec3 B = c + ((a - c)*0.5f);
		glm::vec3 C = a + ((b - a)*0.5f);
		//make the distance from center larger according to planet radius
		A *= m_pPlanet->GetRadius() / glm::length(A);
		B *= m_pPlanet->GetRadius() / glm::length(B);
		C *= m_pPlanet->GetRadius() / glm::length(C);
		//Make 4 new triangles
		short nLevel = level + 1;
		RecursiveTriangle(a, B, C, nLevel, next == SPLITCULL);
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

Triangulator::~Triangulator()
{
	SafeDelete(m_pFrustum);
}
