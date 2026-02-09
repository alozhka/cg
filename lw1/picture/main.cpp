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

using DrawCallback = std::function<void(sf::RenderWindow& window)>;
using EventCallback = std::function<void(const sf::Event& event, sf::RenderWindow& window)>;

int MainLoop(
    const DrawCallback& onDraw,
    const EventCallback& onEvent
)
{
    sf::RenderWindow window = CreateWindow();

    while (window.isOpen())
    {
        while (const std::optional<sf::Event> event = window.pollEvent())
        {
            if (event.has_value())
            {
                if (event->is<sf::Event::Closed>())
                {
                    window.close();
                    break;
                }
                onEvent(*event, window);
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

    auto onEvent = [&](const sf::Event& event, sf::RenderWindow& window)
    {
        cartman.handleEvent(event, window);
    };

    auto onDraw = [&](sf::RenderWindow& window)
    {
        window.draw(cartman);
    };

    return MainLoop(onDraw, onEvent);
}
