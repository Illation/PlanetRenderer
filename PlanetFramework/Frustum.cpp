#include "stdafx.h"
#include "Frustum.h"

#include "Transform.h"
#include "Camera.h"

Frustum::Frustum()
{
}

void Frustum::Init()
{
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, m_Corners.size() * sizeof(glm::vec3), m_Corners.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

//create transforms to prevent transforming every triangle into world space
void Frustum::SetCullTransform(glm::mat4 objectWorld)
{
	m_CullWorld = objectWorld;
	m_CullInverse = glm::inverse(objectWorld);
}

void Frustum::SetToCamera(Camera* pCamera)
{
	m_pTransform = pCamera->GetTransform();
	m_NearPlane = pCamera->GetNearPlane();
	m_FarPlane = pCamera->GetFarPlane();
	m_FOV = pCamera->GetFOV();
}

void Frustum::Update()
{
	//get camera transform vectors
	glm::vec3 up = m_pTransform->GetUp();
	glm::vec3 right = m_pTransform->GetRight();

	//calculate generalized relative width and aspect ratio
	float normHalfWidth = tan(glm::radians(m_FOV));
	float aspectRatio = (float)WINDOW.Width / (float)WINDOW.Height;

	//calculate width and height for near and far plane
	float nearHW = normHalfWidth*m_NearPlane;
	float nearHH = nearHW / aspectRatio;
	float farHW = normHalfWidth*m_FarPlane*0.5f;
	float farHH = farHW / aspectRatio;

	//calculate near and far plane centers
	auto nCenter = m_pTransform->GetPosition()
		+ m_pTransform->GetForward()*m_NearPlane;
	auto fCenter = m_pTransform->GetPosition()
		+ m_pTransform->GetForward()*m_FarPlane*0.5f;

	//construct corners of the near plane in the culled objects world space
	auto na = glm::vec3(m_CullInverse*glm::vec4(nCenter + up*nearHH - right*nearHW, 0));
	auto nb = glm::vec3(m_CullInverse*glm::vec4(nCenter + up*nearHH + right*nearHW, 0));
	auto nc = glm::vec3(m_CullInverse*glm::vec4(nCenter - up*nearHH - right*nearHW, 0));
	auto nd = glm::vec3(m_CullInverse*glm::vec4(nCenter - up*nearHH + right*nearHW, 0));
	//construct corners of the far plane
	auto fa = glm::vec3(m_CullInverse*glm::vec4(fCenter + up*farHH - right*farHW, 0));
	auto fb = glm::vec3(m_CullInverse*glm::vec4(fCenter + up*farHH + right*farHW, 0));
	auto fc = glm::vec3(m_CullInverse*glm::vec4(fCenter - up*farHH - right*farHW, 0));
	auto fd = glm::vec3(m_CullInverse*glm::vec4(fCenter - up*farHH + right*farHW, 0));

	//construct planes
	m_Planes.clear();
	//winding in an outside perspective so the cross product creates normals pointing inward
	m_Planes.push_back(Plane(na, nb, nc));//Near
	m_Planes.push_back(Plane(fb, fa, fd));//Far Maybe skip this step? most polys further away should already be low res
	m_Planes.push_back(Plane(fa, na, fc));//Left
	m_Planes.push_back(Plane(nb, fb, nd));//Right
	m_Planes.push_back(Plane(fa, fb, na));//Top
	m_Planes.push_back(Plane(nc, nd, fc));//Bottom

	//update the list of corners for debug drawing
	m_Corners.clear();
	m_Corners.push_back(na); m_Corners.push_back(nb);
	m_Corners.push_back(nd); m_Corners.push_back(nb);
	m_Corners.push_back(nd); m_Corners.push_back(nc);
	m_Corners.push_back(na); m_Corners.push_back(nc);

	m_Corners.push_back(fa); m_Corners.push_back(fb);
	m_Corners.push_back(fd); m_Corners.push_back(fb);
	m_Corners.push_back(fd); m_Corners.push_back(fc);
	m_Corners.push_back(fa); m_Corners.push_back(fc);

	m_Corners.push_back(na); m_Corners.push_back(fa);
	m_Corners.push_back(nb); m_Corners.push_back(fb);
	m_Corners.push_back(nc); m_Corners.push_back(fc);
	m_Corners.push_back(nd); m_Corners.push_back(fd);
}

bool Frustum::Contains(glm::vec3 p)
{
	for (auto plane : m_Planes)
	{
		if (glm::dot(plane.n, p - plane.d) < 0)return false;
	}
	return true;
}

//this method will treat triangles as intersecting even though they may be outside
//but it is faster then performing a proper intersection test with every plane
//and it does not reject triangles that are inside but with all corners outside
VolumeTri Frustum::ContainsTriangle(glm::vec3 &a, glm::vec3 &b, glm::vec3 &c)
{
	VolumeTri ret = VolumeTri::CONTAINS;
	for (auto plane : m_Planes)
	{
		char rejects = 0;
		if (glm::dot(plane.n, a - plane.d) < 0)rejects++;
		if (glm::dot(plane.n, b - plane.d) < 0)rejects++;
		if (glm::dot(plane.n, c - plane.d) < 0)rejects++;
		// if all three are outside a plane the triangle is outside the frustrum
		if (rejects >= 3)return VolumeTri::OUTSIDE;
		// if at least one is outside the triangle intersects at least one plane
		else if (rejects > 0)ret = VolumeTri::INTERSECT;
	}
	return ret;
}

void Frustum::Draw()
{
	//Rebind the vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, m_Corners.size() * sizeof(glm::vec3), m_Corners.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Pass transformations to the shader
	glUniformMatrix4fv(m_uModelWire, 1, GL_FALSE, glm::value_ptr(m_CullWorld));
	glUniformMatrix4fv(m_uViewProjWire, 1, GL_FALSE, glm::value_ptr(CAMERA->GetViewProj()));

	//Bind Object vertex array
	glBindVertexArray(m_VAO);

	//Draw the object
	glDrawArrays(GL_LINES, 0, m_Corners.size());

	//unbind vertex array
	glBindVertexArray(0);
}


Frustum::~Frustum()
{
}
