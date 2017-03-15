#include "stdafx.h"
#include "SpriteFont.h"

#include "BinaryReader.h"
#include "Texture.h"

#ifndef PLATFORM_Win
	#include <cstring>
#endif

SpriteFont::SpriteFont() :
	m_pTexture(nullptr),
	m_CharacterCount(0),
	m_CharacterSpacing(1),
	m_FontSize(0),
	m_FontName(""),
	m_TextureWidth(0),
	m_TextureHeight(0),
	m_TextCache(std::vector<TextCache>()),
	m_BufferStart(0),
	m_BufferSize(0),
	m_IsAddedToRenderer(false)
{
	for (int i = 0; i < CHAR_COUNT; ++i)
	{
#ifdef PLATFORM_Win
		ZeroMemory((void*)&m_CharTable[i], sizeof(FontMetric));
#else
		std::memset((void*)&m_CharTable[i], 0, sizeof(FontMetric));
#endif
		m_CharTable[i].IsValid = false;
	}
}


SpriteFont::~SpriteFont()
{
	SafeDelete(m_pTexture);
}

bool SpriteFont::IsCharValid(const wchar_t& character)
{
	if (character >= MIN_CHAR_ID && character <= MAX_CHAR_ID)
		return true;

	return false;
}

void SpriteFont::Load(std::string filename)
{
	auto pBinReader = new BinaryReader(); //Prevent memory leaks
	pBinReader->Open(filename);

	if (!pBinReader->Exists())
	{
		delete pBinReader;
		std::cout << "[ERROR] " << "SpriteFont::Load > Failed to read the assetFile!\nPath: " << filename << std::endl;

		return;
	}
	bool valid = false;
	if (pBinReader->Read<char>() == 'B')
	{
		if (pBinReader->Read<char>() == 'M')
		{
			if (pBinReader->Read<char>() == 'F')
			{
				valid = true;
			}
		}
	}
	if (!valid) { std::cout << "[ERROR] " << "Font file header invalid!" << std::endl; return; }
	if (pBinReader->Read<char>() < 3)
	{
		std::cout << "[ERROR] " << "Font version invalid!" << std::endl; return;
	}

	//**********
	// BLOCK 0 *
	//**********
	pBinReader->Read<char>();
	auto Block0Size = pBinReader->Read<int>();
	int pos = pBinReader->GetBufferPosition();
	m_FontSize = pBinReader->Read<short>();
	pBinReader->SetBufferPosition(pos + 14);
	std::string fn;
	char cur = pBinReader->Read<char>();
	while (cur != '\0')
	{
		fn += cur;
		cur = pBinReader->Read<char>();
	}
	m_FontName = fn;
	pBinReader->SetBufferPosition(pos + Block0Size);
	//**********
	// BLOCK 1 *
	//**********
	pBinReader->Read<char>();
	auto Block1Size = pBinReader->Read<int>();
	pos = pBinReader->GetBufferPosition();
	pBinReader->SetBufferPosition(pos + 4);
	m_TextureWidth = pBinReader->Read<unsigned short>();
	m_TextureHeight = pBinReader->Read<unsigned short>();
	auto pagecount = pBinReader->Read<unsigned short>();
	if (pagecount > 1)std::cout << "[ERROR] " <<
		"SpriteFont::Load > SpriteFont(.fnt): Only one texture per font allowed" << std::endl;
	pBinReader->SetBufferPosition(pos + Block1Size);
	//**********
	// BLOCK 2 *
	//**********
	pBinReader->Read<char>();
	auto Block2Size = pBinReader->Read<int>();
	pos = pBinReader->GetBufferPosition();
	std::string pn;
	cur = pBinReader->Read<char>();
	while (cur != '\0')
	{
		pn += cur;
		cur = pBinReader->Read<char>();
	}
	if (pn.size() == 0)std::cout << "[ERROR] " <<
		"SpriteFont::Load > SpriteFont(.fnt): Invalid Font Sprite [Empty]" << std::endl;
	auto filepath = filename.substr(0, filename.rfind('/') + 1);
	m_pTexture = new Texture(filepath + pn);
	m_pTexture->Load();
	pBinReader->SetBufferPosition(pos + Block2Size);
	//**********
	// BLOCK 3 *
	//**********
	pBinReader->Read<char>();
	auto Block3Size = pBinReader->Read<int>();
	pos = pBinReader->GetBufferPosition();
	auto numChars = Block3Size / 20;
	m_CharacterCount = numChars;
	for (int i = 0; i < numChars; i++)
	{
		auto posChar = pBinReader->GetBufferPosition();
		auto charId = (wchar_t)(pBinReader->Read<unsigned int>());
		if (!(IsCharValid(charId)))
		{
			std::cout << "[WARNING] " <<
				"SpriteFont::Load > SpriteFont(.fnt): Invalid Character" << std::endl;
			pBinReader->SetBufferPosition(posChar + 20);
		}
		else
		{
			auto metric = &GetMetric(charId);
			metric->IsValid = true;
			metric->Character = charId;
			auto xPos = pBinReader->Read<unsigned short>();
			auto yPos = pBinReader->Read<unsigned short>();
			metric->Width = pBinReader->Read<unsigned short>();
			metric->Height = pBinReader->Read<unsigned short>();
			metric->OffsetX = pBinReader->Read<short>();
			metric->OffsetY = pBinReader->Read<short>();
			metric->AdvanceX = pBinReader->Read<short>();
			metric->Page = pBinReader->Read<unsigned char>();
			auto chan = pBinReader->Read<unsigned char>();
			switch (chan)
			{
			case 1: metric->Channel = 2; break;
			case 2: metric->Channel = 1; break;
			case 4: metric->Channel = 0; break;
			case 8: metric->Channel = 3; break;
			default: metric->Channel = 4; break;
			}
			metric->TexCoord = glm::vec2((float)xPos / (float)m_TextureWidth
				, (float)yPos / (float)m_TextureHeight);
			pBinReader->SetBufferPosition(posChar + 20);
		}
	}
	delete pBinReader;
#pragma warning(default:4702)  
}
