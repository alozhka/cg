#pragma once
#include "../model/MemoryTrainer.hpp"
#include <graphics/TexturedMesh.hpp>

#include <vector>

class CardsViewModel
{
public:
	explicit CardsViewModel(const MemoryTrainerPtr& game)
		: m_game(game)
	{
	}

	std::vector<std::array<TexturedVertex, 9>> ListCardsVertexes()
	{
		std::vector<Card> cards = m_game->ListCards();

		for (const Card& card : cards)
		{
			std::vector<TexturedVertex> vertices = CARD_VERTICES;
		}

		vertices.emplace_back();
	}

private:
	static constexpr float WIDTH = 0.5, HEIGHT = 0.5, THICKNESS = 0.015;

	static inline std::vector<TexturedVertex> CARD_VERTICES = {
		{ { -WIDTH, -HEIGHT, THICKNESS }, { 0, 1, 0 }, { 0, 0 } },
		{ { -WIDTH, -HEIGHT, THICKNESS }, { 0, 1, 0 }, { 0, 0 } },
		{ { -WIDTH, -HEIGHT, THICKNESS }, { 0, 1, 0 }, { 0, 0 } },
		{ { -WIDTH, -HEIGHT, THICKNESS }, { 0, 1, 0 }, { 0, 0 } },
	};

	MemoryTrainerPtr m_game;
};