#pragma once

#include "Card.hpp"
#include <algorithm>
#include <memory>
#include <random>
#include <vector>

class MemoryTrainer;
using MemoryTrainerPtr = std::shared_ptr<MemoryTrainer>;

class MemoryTrainer
{
public:
	MemoryTrainer()
	{
		Reset();
	}

	void Reset()
	{
		RecreateCards();
	}

	void Update(float dt)
	{
		for (Card& card : m_availableCards)
		{
			card.Update(dt);
		}
	}

	void TryFlip(size_t row, size_t col)
	{
		auto card = std::ranges::find_if(m_availableCards, [row, col](const Card& card) {
			return card.GetRow() == row && card.GetCol() == col;
		});

		if (card == m_availableCards.end())
		{
			return;
		}

		card->Flip();
	}

	[[nodiscard]] const std::vector<Card>& ListCards() const
	{
		return m_availableCards;
	}

	[[nodiscard]] std::vector<Card>& ListCards()
	{
		return m_availableCards;
	}

	[[nodiscard]] size_t GetRows() const { return MAX_ROW; }
	[[nodiscard]] size_t GetCols() const { return MAX_COL; }

private:
	void RecreateCards()
	{
		std::vector<std::string> names = CARD_NAMES;
		for (std::string& name : CARD_NAMES)
		{
			names.push_back(name);
		}
		Shuffle(names);

		m_availableCards.clear();
		for (size_t i = 0; i < names.size(); ++i)
		{
			size_t row = i / MAX_COL;
			size_t col = i % MAX_COL;
			m_availableCards.emplace_back(names[i], row, col);
		}
	}

	static void Shuffle(std::vector<std::string>& v)
	{
		std::random_device rd;
		std::mt19937 generator(rd());
		std::ranges::shuffle(v, generator);
	}

	static inline std::vector<std::string> CARD_NAMES = {
		"chika",
		"cupcake",
		"freddie",
		"glamrock_helpy",
		"nightmare_freddie",
		"nightmare_puppet",
		"springtrap"
	};
	static constexpr size_t MAX_ROW = 2, MAX_COL = 7;

	std::vector<Card> m_availableCards;
};