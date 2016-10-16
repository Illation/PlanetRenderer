#pragma once
#include <glm\gtc\quaternion.hpp>
class Transform
{
public:
	Transform();
	~Transform();


	void UpdateTransforms();

	void SetPosition(glm::vec3 pos) { m_Position = pos; }
	void SetPosition(float x, float y, float z) { m_Position = glm::vec3(x, y, z); }
	void Translate(glm::vec3 translation) { m_Position += translation; }
	void SetRotation(glm::quat rot) { m_Rotation = rot; }
	void SetRotation(glm::vec3 euler) { m_Rotation = glm::quat(euler); }
	void Rotate(glm::quat rot) { m_Rotation *= rot; }
	void Rotate(glm::vec3 euler) { m_Rotation *= glm::quat(euler); }
	void SetScale(glm::vec3 scale) { m_Scale = scale; }
	void Scale(glm::vec3 scale) { m_Scale *= scale; }

	const glm::mat4& GetTransform() const { return m_Tranform; }
	const glm::vec3& GetPosition() const { return m_Position; }
	const glm::quat& GetRotation() const { return m_Rotation; }
	const glm::vec3& GetScale() const { return m_Scale; }

	const glm::vec3& GetForward() const { return m_Forward; }
	const glm::vec3& GetUp() const { return m_Up; }
	const glm::vec3& GetRight() const { return m_Right; }

private:
	glm::mat4 m_Tranform;

	glm::vec3 m_Position;
	glm::quat m_Rotation;
	glm::vec3 m_Scale;

	glm::vec3 m_Forward;
	glm::vec3 m_Right;
	glm::vec3 m_Up;
};

