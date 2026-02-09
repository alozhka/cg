#pragma once
#include "../Observer.h"
#include <algorithm>
#include <string>
#include <unordered_set>

class HangmanModel : public Observable
{
public:
	static const inline std::wstring ALPHABET = L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";

	enum class GameState
	{
		PLAYING,
		WON,
		LOST
	};

	explicit HangmanModel(const std::wstring& targetWord)
	{
		startNewGame(targetWord);
	}

	void startNewGame(const std::wstring& targetWord)
	{
		m_word = targetWord;
		std::ranges::transform(m_word, m_word.begin(), ::towupper);
		m_guessedLetters.clear();
		m_wrongGuesses = 0;
		m_gameState = GameState::PLAYING;
		NotifyObservers();
	}

	bool GuessLetter(wchar_t letter)
	{
		if (m_gameState != GameState::PLAYING)
		{
			return false;
		}

		letter = std::towupper(letter);

		if (m_guessedLetters.contains(letter))
		{
			return false;
		}

		m_guessedLetters.insert(letter);

		// Check if letter is in the word
		if (m_word.find(letter) == std::wstring::npos)
		{
			m_wrongGuesses++;
			if (m_wrongGuesses >= MAX_WRONG_GUESSES)
			{
				m_gameState = GameState::LOST;
			}
			NotifyObservers();
			return false;
		}

		if (CheckWinCondition())
		{
			m_gameState = GameState::WON;
		}
		NotifyObservers();
		return true;
	}

	const std::wstring& getTargetWord() const { return m_word; }

	bool IsLetterGuessed(wchar_t c) const
	{
		return m_guessedLetters.contains(c);
	}

	int GetWrongGuesses() const
	{
		return m_wrongGuesses;
	}

	GameState GetGameState() const
	{
		return m_gameState;
	}

	std::wstring GetGuessedWord()
	{
		return m_word;
	}

private:
	bool CheckWinCondition() const
	{
		for (wchar_t c : m_word)
		{
			if (!m_guessedLetters.contains(c))
			{
				return false;
			}
		}
		return true;
	}

	std::wstring m_word;
	std::unordered_set<wchar_t> m_guessedLetters;
	int m_wrongGuesses = 0;
	GameState m_gameState = GameState::PLAYING;
	static constexpr int MAX_WRONG_GUESSES = 7;
};
