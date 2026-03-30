#pragma once

#include <fstream>
#include <glm/glm.hpp>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

struct ObjData
{
	std::vector<glm::vec3> vertices;
	std::unordered_map<std::string, std::vector<std::vector<int>>> groups;
};

class ObjLoader
{
public:
	static ObjData Load(const std::string& filepath)
	{
		std::ifstream file(filepath);
		if (!file.is_open())
		{
			throw std::runtime_error("Cannot open OBJ file: " + filepath);
		}

		ObjData data;
		std::string currentGroup = "default";
		std::string line;

		while (std::getline(file, line))
		{
			ParseLine(line, data, currentGroup);
		}

		return data;
	}

private:
	static void ParseLine(const std::string& line, ObjData& data, std::string& currentGroup)
	{
		std::istringstream ss(line);
		std::string token;
		ss >> token;

		if (token == "v")
		{
			float x, y, z;
			ss >> x >> y >> z;
			data.vertices.push_back({ x, y, z });
		}
		else if (token == "g")
		{
			ss >> currentGroup;
		}
		else if (token == "f")
		{
			std::vector<int> face;
			int idx;
			while (ss >> idx)
			{
				face.push_back(idx - 1);
			}
			data.groups[currentGroup].push_back(std::move(face));
		}
	}
};