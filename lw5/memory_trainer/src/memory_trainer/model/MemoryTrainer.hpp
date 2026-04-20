#pragma once

#include "Card.hpp"
#include <algorithm>
#include <memory>
#include <optional>
#include <random>
#include <ranges>
#include <unordered_map>
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

	void TryFlip(size_t row, size_t col)
	{
		auto it = std::ranges::find_if(m_availableCards, [row, col](const auto& pair) {
			return pair.second.GetRow() == row && pair.second.GetCol() == col;
		});

		if (it == m_availableCards.end())
		{
			return;
		}
		Card& card = it->second;

		if (!m_firstFlippedCardId.has_value())
		{
			card.Flip();
			m_firstFlippedCardId = card.GetId();
		}
		else if (!m_secondFlippedCardId.has_value() && card.GetId() == *m_firstFlippedCardId)
		{
			card.FlipDown();
			m_firstFlippedCardId.reset();
		}
		else if (!m_secondFlippedCardId.has_value() && card.GetId() != *m_firstFlippedCardId)
		{
			card.Flip();
			m_secondFlippedCardId = card.GetId();
			m_countdownToCheckCards = 1.2;
		}
	}

	void Update(float dt)
	{
		for (auto& card : m_availableCards | std::views::values)
		{
			card.Update(dt);
		}

		if (m_secondFlippedCardId.has_value())
		{
			m_countdownToCheckCards -= dt;
			if (m_countdownToCheckCards <= 0)
			{
				CheckCards(*m_firstFlippedCardId, *m_secondFlippedCardId);
				m_firstFlippedCardId.reset();
				m_secondFlippedCardId.reset();
			}
		}
	}

	[[nodiscard]] std::vector<Card> ListCards() const
	{
		std::vector<Card> cards;
		cards.reserve(m_availableCards.size());

		for (const auto& card : m_availableCards | std::views::values)
		{
			cards.push_back(card);
		}

		return cards;
	}

	[[nodiscard]] size_t GetRows() const { return MAX_ROW; }
	[[nodiscard]] size_t GetCols() const { return MAX_COL; }

private:
	void CheckCards(size_t firstId, size_t secondId)
	{
		if (m_availableCards.at(firstId).GetName() == m_availableCards.at(secondId).GetName())
		{
			m_availableCards.erase(firstId);
			m_availableCards.erase(secondId);
		}
		else
		{
			m_availableCards.at(firstId).FlipDown();
			m_availableCards.at(secondId).FlipDown();
		}
	}

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
			m_availableCards.try_emplace(i, i, names[i], row, col);
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

	std::unordered_map<size_t, Card> m_availableCards;
	std::optional<size_t> m_firstFlippedCardId;
	std::optional<size_t> m_secondFlippedCardId;
	float m_countdownToCheckCards = 1;
};