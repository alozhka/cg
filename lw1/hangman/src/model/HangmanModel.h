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

	explicit HangmanModel(WordRepository& wordRepository)
		: m_wordRepository(wordRepository)
	{
		StartNewGame();
	}

	void StartNewGame()
	{
		WordEntry entry = m_wordRepository.GetRandomWord();
		m_word = entry.word;
		m_hint = entry.hint;

		std::ranges::transform(m_word, m_word.begin(), towupper);
		m_lettersToGuess = UniqueLettersAmount(m_word);
		m_guessedLetters.clear();
		m_wrongGuesses = 0;
		m_gameState = GameState::PLAYING;
		NotifyObservers();
	}

	void GuessLetter(wchar_t letter)
	{
		if (m_gameState != GameState::PLAYING)
		{
			return;
		}

		letter = std::towupper(letter);
		if (m_guessedLetters.contains(letter))
		{
			return;
		}

		m_guessedLetters.insert(letter);

		if (m_word.find(letter) == std::wstring::npos)
		{
			ProcessCorrectLetter();
		}
		else
		{
			ProcessIncorrectLetter();
		}
		NotifyObservers();
	}

	const std::wstring& GetTargetWord() const
	{
		return m_word;
	}

	const std::wstring& GetHint() const
	{
		return m_hint;
	}

	bool IsLetterGuessed(wchar_t c) const
	{
		return m_guessedLetters.contains(c);
	}

	size_t GetWrongGuesses() const
	{
		return m_wrongGuesses;
	}

	GameState GetGameState() const
	{
		return m_gameState;
	}

private:
	void ProcessCorrectLetter()
	{
		m_wrongGuesses++;
		if (m_wrongGuesses >= MAX_WRONG_GUESSES)
		{
			m_gameState = GameState::LOST;
		}
	}

	void ProcessIncorrectLetter()
	{
		m_lettersToGuess--;
		if (m_lettersToGuess <= 0)
		{
			m_gameState = GameState::WON;
		}
	}

	static size_t UniqueLettersAmount(const std::wstring& word)
	{
		std::unordered_set<wchar_t> uniqueChars;
		for (wchar_t c : word)
		{
			if (ALPHABET.find(c) != std::wstring::npos)
			{
				uniqueChars.insert(c);
			}
		}
		return uniqueChars.size();
	}

	std::wstring m_word;
	std::wstring m_hint;
	std::unordered_set<wchar_t> m_guessedLetters;
	size_t m_wrongGuesses = 0;
	size_t m_lettersToGuess = 0;
	GameState m_gameState = GameState::PLAYING;
	static constexpr size_t MAX_WRONG_GUESSES = 7;

	WordRepository& m_wordRepository;
};
