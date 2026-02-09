#pragma once
#include "DrawableObject.hpp"
#include <SFML/System/Angle.hpp>

class EricCartman : public DrawableObject
{
public:
    EricCartman()
    {
        initializeShapes();
    }

    sf::FloatRect getGlobalBounds() const override
    {
        return getTransform().transformRect(
            sf::FloatRect({-90.f, -100.f}, {180.f, 270.f})
        );
    }

private:
    void initializeShapes()
    {
        initBody();
        initLegs();
        initHead();
        initHat();
        initFace();
        initHands();
        initCoatDetails();
    }

    void initBody()
    {
        sf::Color coatRed(220, 20, 20);
        // --- Body (The Fat) ---
        // He is basically a wide red blob.
        m_stomach.setRadius(70.f);
        m_stomach.setScale(sf::Vector2f(1.1f, 0.9f));
        m_stomach.setFillColor(coatRed);
        m_stomach.setPosition(sf::Vector2f(-77.f, 20.f));
    }

    void initLegs()
    {
        sf::Color pantsBrown(100, 50, 0);
        sf::Color shoeBlack(20, 20, 20);

        // --- Pants ---
        m_pants.setSize(sf::Vector2f(120.f, 30.f));
        m_pants.setFillColor(pantsBrown);
        m_pants.setPosition(sf::Vector2f(-60.f, 130.f));

        // --- Shoes ---
        m_leftShoe.setSize(sf::Vector2f(60.f, 20.f));
        m_leftShoe.setFillColor(shoeBlack);
        m_leftShoe.setPosition(sf::Vector2f(-80.f, 150.f));

        m_rightShoe.setSize(sf::Vector2f(60.f, 20.f));
        m_rightShoe.setFillColor(shoeBlack);
        m_rightShoe.setPosition(sf::Vector2f(20.f, 150.f));
    }

    void initHead()
    {
        sf::Color skinColor(255, 220, 177);
        // --- Head ---
        m_head.setRadius(60.f);
        m_head.setScale(sf::Vector2f(1.1f, 0.95f));
        m_head.setFillColor(skinColor);
        m_head.setPosition(sf::Vector2f(-66.f, -60.f));
    }

    void initHat()
    {
        sf::Color hatBlue(0, 180, 200); // Turquoise
        sf::Color hatYellow(255, 255, 0);

        // --- Hat ---
        // Blue part (half circle-ish)
        // Adjusted to not cover eyes
        m_hatTop.setRadius(60.f);
        m_hatTop.setScale(sf::Vector2f(1.15f, 0.6f)); // Squashed circle
        m_hatTop.setFillColor(hatBlue);
        m_hatTop.setPosition(sf::Vector2f(-69.f, -90.f));

        m_hatBrim.setSize(sf::Vector2f(138.f, 15.f));
        m_hatBrim.setFillColor(hatYellow);
        m_hatBrim.setPosition(sf::Vector2f(-69.f, -25.f));

        m_hatPomPom.setRadius(15.f);
        m_hatPomPom.setScale(sf::Vector2f(1.2f, 0.8f));
        m_hatPomPom.setFillColor(hatYellow);
        m_hatPomPom.setPosition(sf::Vector2f(-18.f, -100.f));
    }

    void initFace()
    {
        m_leftEye.setRadius(18.f);
        m_leftEye.setScale(sf::Vector2f(0.8f, 1.0f));
        m_leftEye.setFillColor(sf::Color::White);
        m_leftEye.setPosition(sf::Vector2f(-40.f, -10.f));

        m_rightEye.setRadius(18.f);
        m_rightEye.setScale(sf::Vector2f(0.8f, 1.0f));
        m_rightEye.setFillColor(sf::Color::White);
        m_rightEye.setPosition(sf::Vector2f(4.f, -10.f));

        m_leftPupil.setRadius(2.f);
        m_leftPupil.setFillColor(sf::Color::Black);
        m_leftPupil.setPosition(sf::Vector2f(-24.f, 10.f));

        m_rightPupil.setRadius(2.f);
        m_rightPupil.setFillColor(sf::Color::Black);
        m_rightPupil.setPosition(sf::Vector2f(20.f, 10.f));

        m_mouth.setSize(sf::Vector2f(20.f, 2.f));
        m_mouth.setFillColor(sf::Color::Black);
        m_mouth.setPosition(sf::Vector2f(-10.f, 35.f));
        m_mouth.setRotation(sf::degrees(10.f));
    }

    void initHands()
    {
        sf::Color hatYellow(255, 255, 0);

        m_leftHand.setRadius(15.f);
        m_leftHand.setFillColor(hatYellow);
        m_leftHand.setPosition(sf::Vector2f(-90.f, 60.f));

        m_rightHand.setRadius(15.f);
        m_rightHand.setFillColor(hatYellow);
        m_rightHand.setPosition(sf::Vector2f(60.f, 60.f));
    }

    void initCoatDetails()
    {
        m_coatSplit.setSize(sf::Vector2f(2.f, 110.f));
        m_coatSplit.setFillColor(sf::Color(180, 0, 0)); // Darker red
        m_coatSplit.setPosition(sf::Vector2f(-1.f, 20.f));

        for (int i = 0; i < 3; ++i)
        {
            m_buttons[i].setRadius(3.f);
            m_buttons[i].setFillColor(sf::Color::Black);
            m_buttons[i].setPosition(sf::Vector2f(-3.f, 40.f + i * 25.f));
        }
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override
    {
        states.transform *= getTransform();

        // Shoes
        target.draw(m_leftShoe, states);
        target.draw(m_rightShoe, states);
        // Pants
        target.draw(m_pants, states);
        // Body
        target.draw(m_stomach, states);
        target.draw(m_coatSplit, states);
        for (const auto& btn : m_buttons)
        {
            target.draw(btn, states);
        }
        // Head
        target.draw(m_head, states);
        // Face
        target.draw(m_leftEye, states);
        target.draw(m_rightEye, states);
        target.draw(m_leftPupil, states);
        target.draw(m_rightPupil, states);
        target.draw(m_mouth, states);
        // Hat
        target.draw(m_hatTop, states);
        target.draw(m_hatBrim, states);
        target.draw(m_hatPomPom, states);
        // Hands (in front of body)
        target.draw(m_leftHand, states);
        target.draw(m_rightHand, states);
    }

    bool m_isDragging = false;
    sf::Vector2f m_lastMousePos;

    // Body parts
    sf::CircleShape m_head;
    sf::RectangleShape m_body;
    sf::CircleShape m_stomach;

    // Hat
    sf::CircleShape m_hatPomPom;
    sf::CircleShape m_hatTop;
    sf::RectangleShape m_hatBrim;

    // Face
    sf::CircleShape m_leftEye;
    sf::CircleShape m_rightEye;
    sf::CircleShape m_leftPupil;
    sf::CircleShape m_rightPupil;
    sf::RectangleShape m_mouth;

    // Limbs
    sf::CircleShape m_leftHand;
    sf::CircleShape m_rightHand;
    sf::RectangleShape m_pants;
    sf::RectangleShape m_leftShoe;
    sf::RectangleShape m_rightShoe;

    // Coat details
    sf::CircleShape m_buttons[3];
    sf::RectangleShape m_coatSplit; // Vertical line
};
