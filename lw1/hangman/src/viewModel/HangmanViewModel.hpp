#pragma once
#include "../model/HangmanModel.h"
#include <map>
#include <string>
#include <vector>

enum class LetterStatus
{
	NotGuessed = 0,
	Correct = 1,
	Incorrect = 2
};

class HangmanViewModel
{
public:
	explicit HangmanViewModel(HangmanModel& model)
		: m_model(model)
	{
	}

	void AddObserver(IObserver* observer)
	{
		m_model.AddObserver(observer);
	}

	void RemoveObserver(IObserver* observer)
	{
		m_model.RemoveObserver(observer);
	}

	void ProcessInput(uint32_t letter)
	{
		if (m_model.GetGameState() == HangmanModel::GameState::Playing)
		{
			m_model.GuessLetter(static_cast<wchar_t>(letter));
		}
	}

	void RestartGame()
	{
		m_model.StartNewGame();
	}

	std::wstring GetHint() const
	{
		return m_model.GetHint();
	}

	std::wstring GetDisplayWord() const
	{
		std::wstring display;
		const std::wstring& word = m_model.GetTargetWord();
		for (wchar_t c : word)
		{
			if (HangmanModel::ALPHABET.find(c) != std::wstring::npos)
			{
				if (m_model.IsLetterGuessed(c))
				{
					display += c;
				}
				else
				{
					display += L'_';
				}
			}
			else
			{
				display += c;
			}
		}
		return display;
	}

	std::vector<std::pair<wchar_t, LetterStatus>> GetAlphabetStates() const
	{
		std::vector<std::pair<wchar_t, LetterStatus>> states;
		const std::wstring& word = m_model.GetTargetWord();

		for (size_t i = 0; i < HangmanModel::ALPHABET.size(); ++i)
		{
			wchar_t letter = HangmanModel::ALPHABET[i];
			if (!m_model.IsLetterGuessed(letter))
			{
				states.emplace_back(letter, LetterStatus::NotGuessed);
			}
			else
			{
				if (word.find(letter) != std::wstring::npos)
				{
					states.emplace_back(letter, LetterStatus::Correct);
				}
				else
				{
					states.emplace_back(letter, LetterStatus::Incorrect);
				}
			}
		}
		return states;
	}

	size_t GetWrongGuesses() const
	{
		return m_model.GetWrongGuesses();
	}

	bool IsGameOver() const
	{
		return m_model.GetGameState() != HangmanModel::GameState::Playing;
	}

	bool IsGameWon() const
	{
		return m_model.GetGameState() == HangmanModel::GameState::Won;
	}

private:
	HangmanModel& m_model;
};
