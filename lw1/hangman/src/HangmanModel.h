#pragma once
#include <string>
#include <vector>

class HangmanModel
{
private:
    std::string word;
    std::vector<bool> guessedLetters;
};
