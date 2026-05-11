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
		, m_cruiserModel(ObjLoader::Load("assets/heavy_boat/heavy_boat.obj", textureCache))
		, m_battleshipModel(ObjLoader::Load("assets/batteship/battleship.obj", textureCache))
		, m_patrolBoatModel(ObjLoader::Load("assets/patrol_boat/patrol_boat.obj", textureCache))
	{
	}

	void Draw(ShaderProgram& shader)
	{
		m_drawables.clear();
		for (const ShipDto& ship : m_viewModel.ListShips())
		{
			m_drawables.emplace_back(ModelFor(ship.type), ship.position, glm::vec3{0, 90, 0}, ScaleFor(ship.type));
		}
		for (DrawableModel& drawable : m_drawables)
		{
			drawable.Draw(shader, glm::mat4(1));
		}
	}

private:
	Model& ModelFor(ShipType type)
	{
		switch (type)
		{
		case ShipType::HeavyBoat:
			return m_cruiserModel;
		case ShipType::Battleship:
			return m_battleshipModel;
		case ShipType::PatrolBoat:
			return m_patrolBoatModel;
		}
		
	    throw std::invalid_argument("Unsupported ship type");
	}

	static glm::vec3 ScaleFor(ShipType type)
	{
		switch (type)
		{
		case ShipType::HeavyBoat:
			return glm::vec3(1);
		case ShipType::Battleship:
			return glm::vec3(1.4);
		case ShipType::PatrolBoat:
			return glm::vec3(0.65);
		}
		return glm::vec3(1);
	}

	ShipsViewModel m_viewModel;
	Model m_cruiserModel;
	Model m_battleshipModel;
	Model m_patrolBoatModel;
	std::vector<DrawableModel> m_drawables;
};