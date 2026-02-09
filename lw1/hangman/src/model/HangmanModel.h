#pragma once
#include "../Observer.h"
#include <algorithm>
#include <string>
#include <vector>
#include <cwctype>

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
		m_guessedLetters.assign(ALPHABET.size(), false);
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
		auto it = ALPHABET.find(letter);
		if (it == std::wstring::npos)
		{
			return false;
		}

		size_t index = it;
		if (m_guessedLetters[index])
		{
			return false;
		}

		m_guessedLetters[index] = true;

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

		if (checkWinCondition())
		{
			m_gameState = GameState::WON;
		}
		NotifyObservers();
		return true;
	}

	const std::wstring& getTargetWord() const { return m_word; }

	bool IsLetterGuessed(wchar_t c) const
	{
		auto it = ALPHABET.find(c);
		if (it != std::wstring::npos)
		{
			return m_guessedLetters[it];
		}
		return false;
	}

	int GetWrongGuesses() const { return m_wrongGuesses; }
	GameState GetGameState() const { return m_gameState; }

private:
	bool checkWinCondition() const
	{
		for (wchar_t c : m_word)
		{
			if (ALPHABET.find(c) != std::wstring::npos)
			{
				size_t index = ALPHABET.find(c);
				if (!m_guessedLetters[index])
					return false;
			}
		}
		return true;
	}

	std::wstring m_word;
	std::vector<bool> m_guessedLetters;
	int m_wrongGuesses = 0;
	GameState m_gameState = GameState::PLAYING;
	static constexpr int MAX_WRONG_GUESSES = 7;
};
