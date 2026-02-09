#pragma once
#include "../model/HangmanModel.h"
#include <string>
#include <vector>

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
		if (m_model.GetGameState() == HangmanModel::GameState::PLAYING)
		{
			m_model.GuessLetter(static_cast<wchar_t>(letter));
		}
	}

	std::wstring GetDisplayWord() const
	{
		std::wstring display;
		const auto& word = m_model.getTargetWord();
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

	std::vector<int> GetAlphabetStates() const
	{
		std::vector<int> states(HangmanModel::ALPHABET.size(), 0);
		const auto& word = m_model.getTargetWord();
		for (size_t i = 0; i < HangmanModel::ALPHABET.size(); ++i)
		{
			wchar_t letter = HangmanModel::ALPHABET[i];
			if (m_model.IsLetterGuessed(letter))
			{
				if (word.find(letter) != std::wstring::npos)
				{
					states[i] = 1; // Correct
				}
				else
				{
					states[i] = 2; // Incorrect
				}
			}
			else
			{
				states[i] = 0; // Unknown
			}
		}
		return states;
	}

	int GetWrongGuesses() const
	{
		return m_model.GetWrongGuesses();
	}

	bool IsGameOver() const
	{
		return m_model.GetGameState() != HangmanModel::GameState::PLAYING;
	}

private:
	HangmanModel& m_model;
};
