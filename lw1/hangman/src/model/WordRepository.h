#pragma once
#include <codecvt>
#include <fstream>
#include <iostream>
#include <locale>
#include <random>
#include <string>
#include <vector>

struct WordEntry
{
	std::wstring word;
	std::wstring hint;
};

class WordRepository
{
public:
	WordRepository()
		: m_rng(std::random_device{}())
	{
	}

	bool LoadFromFile(const std::string& filename)
	{
		std::wifstream file(filename);
		file.imbue(std::locale("ru_RU.UTF-8"));

		if (!file.is_open())
		{
			std::cerr << "Failed to open words file: " << filename << std::endl;
			return false;
		}

		std::wstring line;
		while (std::getline(file, line))
		{
			ProcessLine(line);
		}

		return true;
	}

	WordEntry GetRandomWord() const
	{
		if (m_words.empty())
		{
			return { L"DEFAULT", L"No words loaded" };
		}
		std::uniform_int_distribution<size_t> dist(0, m_words.size() - 1);
		return m_words[dist(m_rng)];
	}

private:
	void ProcessLine(const std::wstring& line)
	{
		if (line.empty())
		{
			return;
		}

		size_t delimiterPos = line.find(L':');
		if (delimiterPos == std::wstring::npos)
		{
			return;
		}

		std::wstring word = line.substr(0, delimiterPos);
		std::wstring hint = line.substr(delimiterPos + 1);
		if (!word.empty() && !hint.empty())
		{
			m_words.push_back({ word, hint });
		}
	}

	std::vector<WordEntry> m_words;
	mutable std::mt19937 m_rng;
};
