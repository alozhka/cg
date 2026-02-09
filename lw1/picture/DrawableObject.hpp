#pragma once
#include <SFML/Graphics.hpp>

class DrawableObject : public sf::Drawable, public sf::Transformable
{
public:
    virtual ~DrawableObject() = default;

    virtual sf::FloatRect getGlobalBounds() const = 0;

    bool handleEvent(const sf::Event& event, const sf::RenderWindow& window)
    {
        if (const auto* mousePressed = event.getIf<sf::Event::MouseButtonPressed>())
        {
            HandleMousePressed(window, mousePressed);
        }
        else if (const auto* mouseReleased = event.getIf<sf::Event::MouseButtonReleased>())
        {
            HandleMouseReleased(mouseReleased);
        }
        else if (const auto* mouseMoved = event.getIf<sf::Event::MouseMoved>())
        {
            HandleMouseMoved(window, mouseMoved);
        }
        return false;
    }

private:
    void HandleMousePressed(
        const sf::RenderWindow& window,
        const sf::Event::MouseButtonPressed* mousePressed
    )
    {
        if (mousePressed->button == sf::Mouse::Button::Left)
        {
            sf::Vector2f mousePos = window.mapPixelToCoords(mousePressed->position);
            if (getGlobalBounds().contains(mousePos))
            {
                m_isDragging = true;
                m_lastMousePos = mousePos;
            }
        }
    }

    void HandleMouseMoved(
        const sf::RenderWindow& window,
        const sf::Event::MouseMoved* mouseMoved
    )
    {
        if (m_isDragging)
        {
            sf::Vector2f currMousePos = window.mapPixelToCoords(mouseMoved->position);
            sf::Vector2f delta = currMousePos - m_lastMousePos;
            move(delta);
            m_lastMousePos = currMousePos;
        }
    }

    void HandleMouseReleased(const sf::Event::MouseButtonReleased* mouseReleased)
    {
        if (mouseReleased->button == sf::Mouse::Button::Left)
        {
            m_isDragging = false;
        }
    }

    bool m_isDragging = false;
    sf::Vector2f m_lastMousePos;
};
