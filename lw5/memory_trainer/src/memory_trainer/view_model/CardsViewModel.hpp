#pragma once
#include "../model/MemoryTrainer.hpp"
#include <graphics/Texture.hpp>
#include <graphics/TexturedVertex.hpp>

#include <glm/vec2.hpp>

#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

struct CardPlacement
{
	glm::vec2 gridPos;
	float angle;
	std::string frontTexture;
	bool removed;
};

struct CardTextures
{
	Texture back;
	std::unordered_map<std::string, Texture> fronts;
};

class CardsViewModel
{
public:
	static constexpr float WIDTH = 0.5f;
	static constexpr float HEIGHT = 0.5f;
	static constexpr float THICKNESS = 0.035f;
	static constexpr float CELL_SPACING = 0.05f;

	static constexpr size_t FRONT_VERTEX_OFFSET = 0;
	static constexpr size_t FRONT_VERTEX_COUNT = 6;
	static constexpr size_t BACK_VERTEX_OFFSET = 6;
	static constexpr size_t BACK_VERTEX_COUNT = 6;

	explicit CardsViewModel(MemoryTrainerPtr game)
		: m_game(std::move(game))
	{
	}

	[[nodiscard]] static constexpr std::vector<TexturedVertex> GetCardVertices()
	{
		constexpr float hw = WIDTH * 0.5f;
		constexpr float hh = HEIGHT * 0.5f;
		constexpr float hx = THICKNESS * 0.5f;

		return {
			// Перед
			{ { hx, -hw, -hh }, { 1, 0, 0 }, { 1, 0 } },
			{ { hx, hw, -hh }, { 1, 0, 0 }, { 1, 1 } },
			{ { hx, hw, hh }, { 1, 0, 0 }, { 0, 1 } },
			{ { hx, -hw, -hh }, { 1, 0, 0 }, { 1, 0 } },
			{ { hx, hw, hh }, { 1, 0, 0 }, { 0, 1 } },
			{ { hx, -hw, hh }, { 1, 0, 0 }, { 0, 0 } },

			// Зад
			{ { -hx, hw, -hh }, { -1, 0, 0 }, { 1, 1 } },
			{ { -hx, -hw, -hh }, { -1, 0, 0 }, { 1, 0 } },
			{ { -hx, -hw, hh }, { -1, 0, 0 }, { 0, 0 } },
			{ { -hx, hw, -hh }, { -1, 0, 0 }, { 1, 1 } },
			{ { -hx, -hw, hh }, { -1, 0, 0 }, { 0, 0 } },
			{ { -hx, hw, hh }, { -1, 0, 0 }, { 0, 1 } },
		};
	}

	[[nodiscard]] CardTextures LoadTextures(const std::string& texturesDir) const
	{
		CardTextures textures{ Texture(texturesDir + "/ceiling.png"), {} };
		for (const Card& card : m_game->ListCards())
		{
			if (textures.fronts.contains(card.GetName()))
				continue;
			textures.fronts.emplace(
				card.GetName(),
				Texture(texturesDir + "/" + card.GetName() + ".png"));
		}
		return textures;
	}

	[[nodiscard]] std::vector<CardPlacement> GetPlacements() const
	{
		const std::vector<Card>& cards = m_game->ListCards();
		size_t rows = m_game->GetRows();
		size_t cols = m_game->GetCols();

		constexpr float stepRow = HEIGHT + CELL_SPACING;
		constexpr float stepCol = WIDTH + CELL_SPACING;
		const float originY = (stepRow * (rows - 1)) * 0.5f;
		const float originZ = (stepCol * (cols - 1)) * 0.5f;

		std::vector<CardPlacement> placements;
		for (const Card& card : cards)
		{
			placements.push_back({
				{ originY - stepRow * card.GetRow(),
					originZ - stepCol * card.GetCol() },
				card.GetAngle(),
				card.GetName(),
				false,
			});
		}
		return placements;
	}

private:
	MemoryTrainerPtr m_game;
};