#include "GameView.hpp"
#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>

GameView::GameView(int width, int height) : m_width(width), m_height(height) {}

void GameView::setWindowSize(int width, int height) {
    m_width = width;
    m_height = height;
}

void GameView::draw(int wrongGuesses, const std::string& displayWord, const std::vector<int>& alphabetStates) {
    // Setup 2D projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, m_width, m_height, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Clear background
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw game elements
    drawGallows();
    drawHangman(wrongGuesses);
    drawWord(displayWord);
    drawAlphabet(alphabetStates);
}

void GameView::drawLine(float x1, float y1, float x2, float y2) const {
    glBegin(GL_LINES);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glEnd();
}

void GameView::drawCircle(float cx, float cy, float r, int segments) const {
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < segments; i++) {
        float theta = 2.0f * 3.1415926f * float(i) / float(segments);
        float x = r * cosf(theta);
        float y = r * sinf(theta);
        glVertex2f(cx + x, cy + y);
    }
    glEnd();
}

void GameView::drawGallows() const {
    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(3.0f);
    
    // Base
    drawLine(50, 400, 150, 400);
    // Pole
    drawLine(100, 400, 100, 100);
    // Top
    drawLine(100, 100, 250, 100);
    // Rope
    drawLine(250, 100, 250, 150);
    
    glLineWidth(1.0f);
}

void GameView::drawHangman(int wrongGuesses) const {
    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(2.0f);

    if (wrongGuesses >= 1) { // Head
        drawCircle(250, 170, 20, 20);
    }
    if (wrongGuesses >= 2) { // Body
        drawLine(250, 190, 250, 270);
    }
    if (wrongGuesses >= 3) { // Left Arm
        drawLine(250, 210, 220, 240);
    }
    if (wrongGuesses >= 4) { // Right Arm
        drawLine(250, 210, 280, 240);
    }
    if (wrongGuesses >= 5) { // Left Leg
        drawLine(250, 270, 230, 310);
    }
    if (wrongGuesses >= 6) { // Right Leg
        drawLine(250, 270, 270, 310);
    }
    // 7 could be face or something else if needed
}

void GameView::drawWord(const std::string& word) const {
    float startX = 400;
    float startY = 300;
    float charSize = 20;
    float spacing = 30;

    for (size_t i = 0; i < word.length(); ++i) {
        drawChar(word[i], startX + i * spacing, startY, charSize, false, false);
    }
}

void GameView::drawAlphabet(const std::vector<int>& alphabetStates) const {
    float startX = 50;
    float startY = 450;
    float boxSize = 30;
    float padding = 10;
    int cols = 13; // 26 letters / 2 rows

    for (int i = 0; i < 26; ++i) {
        char c = 'A' + i;
        float x = startX + (i % cols) * (boxSize + padding);
        float y = startY + (i / cols) * (boxSize + padding);

        bool isCorrect = (alphabetStates[i] == 1);
        bool isWrong = (alphabetStates[i] == 2);

        // Draw box background/border
        if (isCorrect) glColor3f(0.8f, 1.0f, 0.8f); // Light green
        else if (isWrong) glColor3f(1.0f, 0.8f, 0.8f); // Light red
        else glColor3f(0.9f, 0.9f, 0.9f); // Gray

        glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x + boxSize, y);
        glVertex2f(x + boxSize, y + boxSize);
        glVertex2f(x, y + boxSize);
        glEnd();

        // Draw border
        glColor3f(0.0f, 0.0f, 0.0f);
        glBegin(GL_LINE_LOOP);
        glVertex2f(x, y);
        glVertex2f(x + boxSize, y);
        glVertex2f(x + boxSize, y + boxSize);
        glVertex2f(x, y + boxSize);
        glEnd();

        // Draw char centered
        drawChar(c, x + 5, y + 5, boxSize - 10, isCorrect, isWrong);
    }
}

std::optional<char> GameView::getLetterAt(double mouseX, double mouseY) const {
    float startX = 50;
    float startY = 450;
    float boxSize = 30;
    float padding = 10;
    int cols = 13;

    for (int i = 0; i < 26; ++i) {
        float x = startX + (i % cols) * (boxSize + padding);
        float y = startY + (i / cols) * (boxSize + padding);

        if (mouseX >= x && mouseX <= x + boxSize &&
            mouseY >= y && mouseY <= y + boxSize) {
            return (char)('A' + i);
        }
    }
    return std::nullopt;
}

// Simple vector font implementation
void GameView::drawChar(char c, float x, float y, float size, bool isCorrect, bool isWrong) const {
    if (isCorrect) glColor3f(0.0f, 0.5f, 0.0f);
    else if (isWrong) glColor3f(0.8f, 0.0f, 0.0f);
    else glColor3f(0.0f, 0.0f, 0.0f);

    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(size / 10.0f, size / 10.0f, 1.0f);

    glBegin(GL_LINES);
    switch (c) {
        case 'A': glVertex2f(0, 10); glVertex2f(5, 0); glVertex2f(5, 0); glVertex2f(10, 10); glVertex2f(2, 6); glVertex2f(8, 6); break;
        case 'B': glVertex2f(0, 0); glVertex2f(0, 10); glVertex2f(0, 0); glVertex2f(8, 0); glVertex2f(8, 0); glVertex2f(8, 5); glVertex2f(8, 5); glVertex2f(0, 5); glVertex2f(0, 5); glVertex2f(8, 5); glVertex2f(8, 5); glVertex2f(8, 10); glVertex2f(8, 10); glVertex2f(0, 10); break;
        case 'C': glVertex2f(10, 0); glVertex2f(0, 0); glVertex2f(0, 0); glVertex2f(0, 10); glVertex2f(0, 10); glVertex2f(10, 10); break;
        case 'D': glVertex2f(0, 0); glVertex2f(0, 10); glVertex2f(0, 0); glVertex2f(8, 2); glVertex2f(8, 2); glVertex2f(8, 8); glVertex2f(8, 8); glVertex2f(0, 10); break;
        case 'E': glVertex2f(10, 0); glVertex2f(0, 0); glVertex2f(0, 0); glVertex2f(0, 10); glVertex2f(0, 10); glVertex2f(10, 10); glVertex2f(0, 5); glVertex2f(8, 5); break;
        case 'F': glVertex2f(0, 0); glVertex2f(0, 10); glVertex2f(0, 0); glVertex2f(10, 0); glVertex2f(0, 5); glVertex2f(8, 5); break;
        case 'G': glVertex2f(10, 0); glVertex2f(0, 0); glVertex2f(0, 0); glVertex2f(0, 10); glVertex2f(0, 10); glVertex2f(10, 10); glVertex2f(10, 10); glVertex2f(10, 5); glVertex2f(10, 5); glVertex2f(5, 5); break;
        case 'H': glVertex2f(0, 0); glVertex2f(0, 10); glVertex2f(10, 0); glVertex2f(10, 10); glVertex2f(0, 5); glVertex2f(10, 5); break;
        case 'I': glVertex2f(0, 0); glVertex2f(10, 0); glVertex2f(5, 0); glVertex2f(5, 10); glVertex2f(0, 10); glVertex2f(10, 10); break;
        case 'J': glVertex2f(10, 0); glVertex2f(10, 10); glVertex2f(10, 10); glVertex2f(0, 10); glVertex2f(0, 10); glVertex2f(0, 5); break;
        case 'K': glVertex2f(0, 0); glVertex2f(0, 10); glVertex2f(0, 5); glVertex2f(10, 0); glVertex2f(0, 5); glVertex2f(10, 10); break;
        case 'L': glVertex2f(0, 0); glVertex2f(0, 10); glVertex2f(0, 10); glVertex2f(10, 10); break;
        case 'M': glVertex2f(0, 10); glVertex2f(0, 0); glVertex2f(0, 0); glVertex2f(5, 5); glVertex2f(5, 5); glVertex2f(10, 0); glVertex2f(10, 0); glVertex2f(10, 10); break;
        case 'N': glVertex2f(0, 10); glVertex2f(0, 0); glVertex2f(0, 0); glVertex2f(10, 10); glVertex2f(10, 10); glVertex2f(10, 0); break;
        case 'O': glVertex2f(0, 0); glVertex2f(10, 0); glVertex2f(10, 0); glVertex2f(10, 10); glVertex2f(10, 10); glVertex2f(0, 10); glVertex2f(0, 10); glVertex2f(0, 0); break;
        case 'P': glVertex2f(0, 0); glVertex2f(0, 10); glVertex2f(0, 0); glVertex2f(10, 0); glVertex2f(10, 0); glVertex2f(10, 5); glVertex2f(10, 5); glVertex2f(0, 5); break;
        case 'Q': glVertex2f(0, 0); glVertex2f(10, 0); glVertex2f(10, 0); glVertex2f(10, 10); glVertex2f(10, 10); glVertex2f(0, 10); glVertex2f(0, 10); glVertex2f(0, 0); glVertex2f(5, 5); glVertex2f(10, 10); break;
        case 'R': glVertex2f(0, 0); glVertex2f(0, 10); glVertex2f(0, 0); glVertex2f(10, 0); glVertex2f(10, 0); glVertex2f(10, 5); glVertex2f(10, 5); glVertex2f(0, 5); glVertex2f(0, 5); glVertex2f(10, 10); break;
        case 'S': glVertex2f(10, 0); glVertex2f(0, 0); glVertex2f(0, 0); glVertex2f(0, 5); glVertex2f(0, 5); glVertex2f(10, 5); glVertex2f(10, 5); glVertex2f(10, 10); glVertex2f(10, 10); glVertex2f(0, 10); break;
        case 'T': glVertex2f(0, 0); glVertex2f(10, 0); glVertex2f(5, 0); glVertex2f(5, 10); break;
        case 'U': glVertex2f(0, 0); glVertex2f(0, 10); glVertex2f(0, 10); glVertex2f(10, 10); glVertex2f(10, 10); glVertex2f(10, 0); break;
        case 'V': glVertex2f(0, 0); glVertex2f(5, 10); glVertex2f(5, 10); glVertex2f(10, 0); break;
        case 'W': glVertex2f(0, 0); glVertex2f(2, 10); glVertex2f(2, 10); glVertex2f(5, 5); glVertex2f(5, 5); glVertex2f(8, 10); glVertex2f(8, 10); glVertex2f(10, 0); break;
        case 'X': glVertex2f(0, 0); glVertex2f(10, 10); glVertex2f(10, 0); glVertex2f(0, 10); break;
        case 'Y': glVertex2f(0, 0); glVertex2f(5, 5); glVertex2f(10, 0); glVertex2f(5, 5); glVertex2f(5, 5); glVertex2f(5, 10); break;
        case 'Z': glVertex2f(0, 0); glVertex2f(10, 0); glVertex2f(10, 0); glVertex2f(0, 10); glVertex2f(0, 10); glVertex2f(10, 10); break;
        case '_': glVertex2f(0, 10); glVertex2f(10, 10); break;
        default: break;
    }
    glEnd();

    glPopMatrix();
}
