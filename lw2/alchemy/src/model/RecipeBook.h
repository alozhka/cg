#pragma once
#include <map>
#include <vector>

class RecipeBook
{
public:
	void AddRecipe(const std::string& name1, const std::string& name2, const std::string& result)
	{
		m_recipes[{ name1, name2 }] = result;
		m_recipes[{ name2, name1 }] = result;
	}

	std::string FindRecipeResult(const std::string& name1, const std::string& name2)
	{
		return m_recipes[{ name1, name2 }];
	}

private:
	std::map<std::pair<std::string, std::string>, std::string> m_recipes{};
};
