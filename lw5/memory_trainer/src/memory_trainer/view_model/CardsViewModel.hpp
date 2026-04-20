#pragma once
#include "../model/MemoryTrainer.hpp"

#include <graphics/Texture.hpp>
#include <graphics/TexturedVertex.hpp>
#include <optional>

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
		std::optional<glm::vec3> hit = IntersectCardPlane(rayOrigin, rayDir);
		if (!hit.has_value())
		{
			return;
		}

		auto cell = PickCell(hit.value());
		if (!cell.has_value())
		{
			return;
		}

		m_game->TryFlip(cell->first, cell->second);
	}

	[[nodiscard]] std::vector<CardPlacement> GetPlacements() const
	{
		const std::vector<Card>& cards = m_game->ListCards();
		size_t rows = m_game->GetRows();
		size_t cols = m_game->GetCols();

		float originY = ROW_STEP * (rows - 1) * 0.5f;
		float originZ = COL_STEP * (cols - 1) * 0.5f;

		std::vector<CardPlacement> placements;
		for (const Card& card : cards)
		{
			placements.push_back({
				{ originY - ROW_STEP * card.GetRow(),
					originZ - COL_STEP * card.GetCol() },
				card.GetAngle(),
				card.GetName(),
				false,
			});
		}
		return placements;
	}

private:
	std::optional<std::pair<size_t, size_t>> PickCell(const glm::vec3& hit) const
	{
		size_t rows = m_game->GetRows();
		size_t cols = m_game->GetCols();

		float originY = ROW_STEP * (rows - 1) * 0.5f;
		float originZ = COL_STEP * (cols - 1) * 0.5f;

		size_t row = std::round((originY - hit.y) / ROW_STEP);
		size_t col = std::round((originZ - hit.z) / COL_STEP);

		if (row < 0 || col < 0 || row >= rows || col >= cols)
		{
			return std::nullopt;
		}

		float centerY = originY - ROW_STEP * row;
		float centerZ = originZ - COL_STEP * col;
		if (std::abs(hit.y - centerY) > WIDTH * 0.5f || std::abs(hit.z - centerZ) > HEIGHT * 0.5f)
		{
			return std::nullopt;
		}

		return { { row, col } };
	}

	static std::optional<glm::vec3> IntersectCardPlane(const glm::vec3& rayOrigin, const glm::vec3& rayDir)
	{
		// origin.x + t * dir.x = 0
		float t = -rayOrigin.x / rayDir.x;

		if (t < 0)
		{
			return std::nullopt;
		}

		return rayOrigin + t * rayDir;
	}

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

	static constexpr float WIDTH = 0.5;
	static constexpr float HEIGHT = 0.5;
	static constexpr float THICKNESS = 0.035;
	static constexpr float CELL_SPACING = 0.05;
	static constexpr float ROW_STEP = HEIGHT + CELL_SPACING;
	static constexpr float COL_STEP = WIDTH + CELL_SPACING;

	static inline const std::vector<TexturedVertex> m_cardVertices = BuildCardVertices();

	MemoryTrainerPtr m_game;
};