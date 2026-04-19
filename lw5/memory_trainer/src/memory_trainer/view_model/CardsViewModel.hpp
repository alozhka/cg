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
	static constexpr size_t BACK_VERTEX_COUNT = 30;

	explicit CardsViewModel(MemoryTrainerPtr game)
		: m_game(std::move(game))
	{
	}

	[[nodiscard]] static const std::vector<TexturedVertex>& GetCardVertices()
	{
		return m_cardVertices;
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

	void TryFlipAt(const glm::vec3& rayOrigin, const glm::vec3& rayDir) const
	{
		if (std::abs(rayDir.x) < 1e-6f)
			return;
		float t = -rayOrigin.x / rayDir.x;
		if (t < 0)
			return;
		glm::vec3 hit = rayOrigin + t * rayDir;

		size_t rows = m_game->GetRows();
		size_t cols = m_game->GetCols();
		constexpr float stepRow = HEIGHT + CELL_SPACING;
		constexpr float stepCol = WIDTH + CELL_SPACING;
		const float originY = stepRow * static_cast<float>(rows - 1) * 0.5f;
		const float originZ = stepCol * static_cast<float>(cols - 1) * 0.5f;

		int row = static_cast<int>(std::round((originY - hit.y) / stepRow));
		int col = static_cast<int>(std::round((originZ - hit.z) / stepCol));
		if (row < 0 || col < 0
			|| row >= static_cast<int>(rows)
			|| col >= static_cast<int>(cols))
			return;

		float centerY = originY - stepRow * static_cast<float>(row);
		float centerZ = originZ - stepCol * static_cast<float>(col);
		if (std::abs(hit.y - centerY) > WIDTH * 0.5f)
			return;
		if (std::abs(hit.z - centerZ) > HEIGHT * 0.5f)
			return;

		for (Card& card : m_game->ListCards())
		{
			if (card.GetRow() == static_cast<size_t>(row)
				&& card.GetCol() == static_cast<size_t>(col))
			{
				card.Flip();
				return;
			}
		}
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
	static std::vector<TexturedVertex> BuildCardVertices()
	{
		constexpr float hw = WIDTH * 0.5f;
		constexpr float hh = HEIGHT * 0.5f;
		constexpr float hx = THICKNESS * 0.5f;

		constexpr glm::vec3 frontLeftBottom{ hx, -hw, -hh };
		constexpr glm::vec3 frontRightBottom{ hx, hw, -hh };
		constexpr glm::vec3 frontRightTop{ hx, hw, hh };
		constexpr glm::vec3 frontLeftTop{ hx, -hw, hh };
		constexpr glm::vec3 backLeftBottom{ -hx, -hw, -hh };
		constexpr glm::vec3 backRightBottom{ -hx, hw, -hh };
		constexpr glm::vec3 backRightTop{ -hx, hw, hh };
		constexpr glm::vec3 backLeftTop{ -hx, -hw, hh };

		std::vector<TexturedVertex> v;
		v.reserve(36);

		AddQuad(v, frontLeftTop, frontLeftBottom, frontRightBottom, frontRightTop, { 1, 0, 0 });
		AddQuad(v, backLeftTop, backLeftBottom, backRightBottom, backRightTop, { -1, 0, 0 });
		AddQuad(v, backRightBottom, frontRightBottom, frontRightTop, backRightTop, { 0, 1, 0 });
		AddQuad(v, backLeftBottom, frontLeftBottom, frontLeftTop, backLeftTop, { 0, -1, 0 });
		AddQuad(v, backLeftTop, frontLeftTop, frontRightTop, backRightTop, { 0, 0, 1 });
		AddQuad(v, backLeftBottom, backRightBottom, frontRightBottom, frontLeftBottom, { 0, 0, -1 });

		return v;
	}

	static void AddQuad(std::vector<TexturedVertex>& out,
		const glm::vec3& p0, const glm::vec3& p1,
		const glm::vec3& p2, const glm::vec3& p3,
		const glm::vec3& normal)
	{
		out.push_back({ p0, normal, { 0, 0 } });
		out.push_back({ p1, normal, { 1, 0 } });
		out.push_back({ p2, normal, { 1, 1 } });
		out.push_back({ p0, normal, { 0, 0 } });
		out.push_back({ p2, normal, { 1, 1 } });
		out.push_back({ p3, normal, { 0, 1 } });
	}

	static inline const std::vector<TexturedVertex> m_cardVertices = BuildCardVertices();

	MemoryTrainerPtr m_game;
};