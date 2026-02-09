#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <string>
#include "GameView.hpp"

// Global game state for testing
int wrongGuesses = 0;
std::string displayWord = "H_NGM_N";
std::vector<int> alphabetStates(26, 0); // 0: unknown, 1: correct, 2: incorrect

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        
        GameView* view = static_cast<GameView*>(glfwGetWindowUserPointer(window));
        if (view) {
            auto letter = view->getLetterAt(xpos, ypos);
            if (letter) {
                std::cout << "Clicked: " << *letter << std::endl;
                // Simple interaction for testing
                int index = *letter - 'A';
                if (alphabetStates[index] == 0) {
                    // Toggle between correct/incorrect for demo
                    alphabetStates[index] = (index % 2 == 0) ? 1 : 2;
                    if (alphabetStates[index] == 2) {
                        wrongGuesses = (wrongGuesses + 1) % 8;
                    }
                }
            }
        }
    }
}

int main()
{
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(800, 600, "Hangman Game", NULL, NULL);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    
    // Initialize view
    GameView view(800, 600);
    glfwSetWindowUserPointer(window, &view);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {
        // Handle resize
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        view.setWindowSize(width, height);

        // Render
        view.draw(wrongGuesses, displayWord, alphabetStates);

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
