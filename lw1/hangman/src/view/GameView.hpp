#pragma once
#include <string>
#include <vector>
#include <optional>

class GameView {
public:
    GameView(int width, int height);

    void setWindowSize(int width, int height);

    // Draws the entire game state
    // wrongGuesses: number of wrong guesses (0-7)
    // displayWord: the word to display (with underscores for unknown letters)
    // alphabetStates: status of each letter (0: unknown, 1: correct, 2: incorrect)
    void draw(int wrongGuesses, const std::string& displayWord, const std::vector<int>& alphabetStates);

    // Returns the character clicked at the given coordinates, or nullopt if none
    std::optional<char> getLetterAt(double x, double y) const;

private:
    int m_width;
    int m_height;

    void drawGallows() const;
    void drawHangman(int wrongGuesses) const;
    void drawWord(const std::string& word) const;
    void drawAlphabet(const std::vector<int>& alphabetStates) const;
    
    // Helper to draw a single character using lines
    void drawChar(char c, float x, float y, float size, bool isCorrect, bool isWrong) const;
    
    // Helper to draw a line
    void drawLine(float x1, float y1, float x2, float y2) const;
    void drawCircle(float cx, float cy, float r, int segments) const;
};
