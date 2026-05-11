#pragma once
#include "../view_model/ShipsViewModel.hpp"

#include <graphics/DrawableModel.hpp>
#include <graphics/Model.hpp>
#include <graphics/ObjLoader.hpp>
#include <graphics/shaders/ShaderProgram.hpp>
#include <graphics/textures/TextureCache.hpp>

#include <glm/glm.hpp>
#include <vector>

class ShipsView
{
public:
	ShipsView(const ShipsViewModel& viewModel, TextureCache& textureCache)
		: m_viewModel(viewModel)
		, m_cruiserModel(ObjLoader::Load("assets/cruiser/24RPMGBXX36TDDT35PAGXDXJI.obj", textureCache))
	{
	}

	void Draw(ShaderProgram& shader)
	{
		m_drawables.clear();
		for (const ShipDto& ship : m_viewModel.ListShips())
		{
			m_drawables.emplace_back(m_cruiserModel, ship.position, glm::vec3{0, 90,0}, ScaleFor(ship.type));
		}
		for (DrawableModel& drawable : m_drawables)
		{
			drawable.Draw(shader, glm::mat4(1));
		}
	}

private:
	static glm::vec3 ScaleFor(ShipType type)
	{
		switch (type)
		{
		case ShipType::Cruiser:
			return glm::vec3(1);
		case ShipType::Battleship:
			return glm::vec3(1.4);
		case ShipType::Frigate:
			return glm::vec3(0.65);
		}
		return glm::vec3(1);
	}

	ShipsViewModel m_viewModel;
	Model m_cruiserModel;
	std::vector<DrawableModel> m_drawables;
};