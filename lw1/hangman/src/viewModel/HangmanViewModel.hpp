#pragma once
#include "../model/HangmanModel.h"
#include <string>
#include <vector>

class HangmanViewModel : public IObserver
	, public IObservable
{
public:
	explicit HangmanViewModel(HangmanModel& model)
		: m_model(model)
	{
		m_model.AddObserver(this);
	}

	HangmanViewModel(const HangmanViewModel& other)
		: m_model(other.m_model)
	{
		m_model.AddObserver(this);
	}

	~HangmanViewModel() override
	{
		m_model.RemoveObserver(this);
	}

	void update() override
	{
		NotifyObservers();
	}

	void ProcessInput(char letter)
	{
		if (m_model.GetGameState() == HangmanModel::GameState::PLAYING)
		{
			m_model.GuessLetter(letter);
		}
	}

	std::string GetDisplayWord() const
	{
		std::string display;
		const auto& word = m_model.getTargetWord();
		for (char c : word)
		{
			if (c >= 'A' && c <= 'Z')
			{
				if (m_model.IsLetterGuessed(c))
				{
					display += c;
				}
				else
				{
					display += '_';
				}
			}
			else
			{
				display += c;
			}
		}
		return display;
	}

	// Presentation logic: state of each letter for coloring
	// Returns 0: unknown, 1: correct, 2: incorrect
	std::vector<int> GetAlphabetStates() const
	{
		std::vector<int> states(26, 0);
		const auto& word = m_model.getTargetWord();
		for (int i = 0; i < 26; ++i)
		{
			char letter = 'A' + i;
			if (m_model.IsLetterGuessed(letter))
			{
				if (word.find(letter) != std::string::npos)
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

	int getWrongGuesses() const
	{
		return m_model.GetWrongGuesses();
	}

	bool isGameOver() const
	{
		return m_model.GetGameState() != HangmanModel::GameState::PLAYING;
	}

private:
	HangmanModel& m_model;
};
