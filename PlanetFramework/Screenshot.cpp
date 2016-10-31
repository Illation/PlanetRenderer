#include "stdafx.h"
#include "Screenshot.h"

#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>

#include <algorithm>

#ifdef WIN32
#include <windows.h>
int getNumber(std::string directory)
{
	std::vector<int> names;
	std::string searchPath = directory + "/*.*";
	WIN32_FIND_DATA fd;
	HANDLE hFind = ::FindFirstFile(searchPath.c_str(), &fd);
	if (hFind != INVALID_HANDLE_VALUE) 
	{
		do 
		{
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) 
			{
				auto complete = std::string(fd.cFileName);
				names.push_back(std::stoi(complete.substr(0, complete.rfind('.'))));
			}
		} while (::FindNextFile(hFind, &fd));
		::FindClose(hFind);
	}
	if (names.size() > 0)
	{
		std::sort(names.begin(), names.end());
		int base = names[0];
		if (names.size() > 1)
		{
			for (int i = 1; i < names.size(); i++)
			{
				base++;
				if (names[i] != base)return base;
			}
		}
		return base + 1;
	}
	else return 0;
}
#endif

Screenshot::Screenshot()
{
}

void Screenshot::Init(std::string directory)
{
	m_Path = directory;
}

void Screenshot::Capture()
{
	if (m_Take)
	{
#ifdef WIN32
		std::string filename = m_Path + std::to_string(getNumber(m_Path)) +".jpg";
#elif
		std::string filename = m_Path + "1.jpg";
#endif

		ILuint imageID = ilGenImage();
		ilBindImage(imageID);
		ilutGLScreen();
		ilEnable(IL_FILE_OVERWRITE);
		ilSaveImage(filename.c_str());
		ilDeleteImage(imageID);
		printf("Screenshot saved to: %s\n", filename.c_str());

		m_Take = false;
	}
}
