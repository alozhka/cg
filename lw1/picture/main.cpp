#include <functional>
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <optional>
#include "EricCartman.hpp"

constexpr float GROUND_Y = 450.0f;

sf::RenderWindow CreateWindow()
{
    sf::ContextSettings settings;
    settings.antiAliasingLevel = 16;
    return sf::RenderWindow{
        sf::VideoMode(sf::Vector2u(800, 600)),
        "Eric Cartman",
        sf::Style::Default,
        sf::State::Windowed,
        settings
    };
}

int MainLoop(const std::function<void(sf::RenderWindow& window)>& onDraw)
{
    sf::RenderWindow window = CreateWindow();

    while (window.isOpen())
    {
        while (const std::optional<sf::Event> event = window.pollEvent())
        {
            if (event.has_value() && event->is<sf::Event::Closed>())
            {
                window.close();
                break;
            }
        }

        window.clear(sf::Color::White);
        onDraw(window);
        window.display();
    }

    return 0;
}

int main()
{
    EricCartman cartman;
    cartman.setPosition(sf::Vector2f(400.f, 300.f));
    cartman.setScale(sf::Vector2f(1.5f, 1.5f));

    bool isDragging = false;
    sf::Vector2f lastMousePos;
    bool wasMousePressed = false;

    auto onDraw = [&](sf::RenderWindow& window) {
        sf::Vector2f currMousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        bool isMousePressed = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);

        if (isMousePressed && !wasMousePressed)
        {
            if (cartman.getGlobalBounds().contains(currMousePos))
            {
                isDragging = true;
                lastMousePos = currMousePos;
            }
        }
        else if (!isMousePressed)
        {
            isDragging = false;
        }

        if (isDragging)
        {
            sf::Vector2f delta = currMousePos - lastMousePos;
            cartman.move(delta);
            lastMousePos = currMousePos;
        }

        wasMousePressed = isMousePressed;

        window.draw(cartman);
    };

    return MainLoop(onDraw);
}
