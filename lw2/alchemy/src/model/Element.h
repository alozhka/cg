#pragma once
#include <string>

class Element
{
public:
	Element(const std::string& name, const std::string& imagePath)
		: m_name(name)
		, m_imagePath(imagePath)
	{
	}

private:
	std::string m_name;
	std::string m_imagePath;
};
