#pragma once
class Screenshot : public Singleton<Screenshot>
{
public:
	void Take() { m_Take = true; }

	Screenshot();
	~Screenshot() {}
private:
	friend class Scene;

	void Init(std::string directory);
	void Capture();

	std::string m_Path;
	bool m_Take = false;
};

