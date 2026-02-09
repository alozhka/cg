#pragma once
#include <algorithm>
#include <string>
#include <vector>

class HangmanModel
{
public:
	enum class GameState
	{
		PLAYING,
		WON,
		LOST
	};

	explicit HangmanModel(const std::string& targetWord = "HANGMAN")
	{
		startNewGame(targetWord);
	}

	void startNewGame(const std::string& targetWord)
	{
		m_word = targetWord;
		std::ranges::transform(m_word, m_word.begin(), ::toupper);
		m_guessedLetters.assign(26, false);
		m_wrongGuesses = 0;
		m_gameState = GameState::PLAYING;
	}

	bool guessLetter(char letter)
	{
		if (m_gameState != GameState::PLAYING)
		{
			return false;
		}

		letter = static_cast<char>(std::toupper(letter));
		if (letter < 'A' || letter > 'Z')
		{
			return false;
		}

		int index = letter - 'A';
		if (m_guessedLetters[index])
		{
			return false;
		}

		m_guessedLetters[index] = true;

		if (m_word.find(letter) == std::string::npos)
		{
			m_wrongGuesses++;
			if (m_wrongGuesses >= MAX_WRONG_GUESSES)
			{
				m_gameState = GameState::LOST;
			}
			return false;
		}

		if (checkWinCondition())
		{
			m_gameState = GameState::WON;
		}
		return true;
	}

	const std::string& getTargetWord() const { return m_word; }

	bool IsLetterGuessed(char c) const
	{
		if (c >= 'A' && c <= 'Z')
		{
			return m_guessedLetters[c - 'A'];
		}
		return false;
	}

	int GetWrongGuesses() const { return m_wrongGuesses; }
	GameState GetGameState() const { return m_gameState; }

private:
	bool checkWinCondition() const
	{
		for (char c : m_word)
		{
			if (c >= 'A' && c <= 'Z')
			{
				if (!m_guessedLetters[c - 'A'])
					return false;
			}
		}
		return true;
	}

	std::string m_word;
	std::vector<bool> m_guessedLetters;
	int m_wrongGuesses = 0;
	GameState m_gameState = GameState::PLAYING;
	static constexpr int MAX_WRONG_GUESSES = 7;
};
