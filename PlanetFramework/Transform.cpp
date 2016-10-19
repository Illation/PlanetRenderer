#include "stdafx.h"
#include "Transform.h"

#include <glm/gtx/transform.hpp>
#include <glm\gtx\quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/euler_angles.hpp>

Transform::Transform()
{
	m_Tranform = glm::mat4(1.0f);

	m_Position = glm::vec3(0.f, 0.f, 0.f);
	m_Rotation = glm::quat();
	m_Scale = glm::vec3(1.f, 1.f, 1.f);


	m_Forward = glm::vec3(0.f, 0.f, 1.f);
	m_Right = glm::vec3(1.f, 0.f, 0.f);
	m_Up = glm::vec3(0.f, 1.f, 0.f);
}

void Transform::UpdateTransforms()
{
	m_Tranform = glm::translate(m_Position)*glm::toMat4(m_Rotation)*glm::scale(m_Scale);

	m_Forward = glm::normalize(m_Rotation*glm::vec3(0, 0, 1));
	m_Right = glm::normalize(m_Rotation*glm::vec3(1, 0, 0));
	m_Up = glm::normalize(glm::cross(m_Forward, m_Right));
}

Transform::~Transform()
{
}
