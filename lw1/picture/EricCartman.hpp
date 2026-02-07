#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System/Angle.hpp>

class EricCartman : public sf::Drawable, public sf::Transformable
{
public:
    EricCartman()
    {
        initializeShapes();
    }

private:
    // Body parts
    sf::CircleShape m_head;
    sf::RectangleShape m_body; // Or circle/convex for fatness
    sf::CircleShape m_stomach; // To make him round

    // Hat
    sf::CircleShape m_hatPomPom;
    sf::CircleShape m_hatTop; // Half circle
    sf::RectangleShape m_hatBrim;

    // Face
    sf::CircleShape m_leftEye;
    sf::CircleShape m_rightEye;
    sf::CircleShape m_leftPupil;
    sf::CircleShape m_rightPupil;
    sf::RectangleShape m_mouth; // Simple line for now, maybe a curve
    sf::CircleShape m_chin; // Double chin effect?

    // Limbs
    sf::CircleShape m_leftHand;
    sf::CircleShape m_rightHand;
    sf::RectangleShape m_pants;
    sf::RectangleShape m_leftShoe;
    sf::RectangleShape m_rightShoe;

    // Coat details
    sf::CircleShape m_buttons[3];
    sf::RectangleShape m_coatSplit; // Vertical line

    void initializeShapes()
    {
        // Colors
        sf::Color skinColor(255, 220, 177);
        sf::Color hatBlue(0, 180, 200); // Turquoise
        sf::Color hatYellow(255, 255, 0);
        sf::Color coatRed(220, 20, 20);
        sf::Color pantsBrown(100, 50, 0);
        sf::Color shoeBlack(20, 20, 20);

        // --- Body (The Fat) ---
        // He is basically a wide red blob.
        m_stomach.setRadius(70.f);
        m_stomach.setScale(sf::Vector2f(1.1f, 0.9f));
        m_stomach.setFillColor(coatRed);
        m_stomach.setPosition(sf::Vector2f(-77.f, 20.f));

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

        // --- Head ---
        m_head.setRadius(60.f);
        m_head.setScale(sf::Vector2f(1.1f, 0.95f));
        m_head.setFillColor(skinColor);
        m_head.setPosition(sf::Vector2f(-66.f, -60.f));

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

        // --- Face Details ---
        // Eyes
        m_leftEye.setRadius(18.f);
        m_leftEye.setScale(sf::Vector2f(0.8f, 1.0f));
        m_leftEye.setFillColor(sf::Color::White);
        m_leftEye.setPosition(sf::Vector2f(-40.f, -10.f));

        m_rightEye.setRadius(18.f);
        m_rightEye.setScale(sf::Vector2f(0.8f, 1.0f));
        m_rightEye.setFillColor(sf::Color::White);
        m_rightEye.setPosition(sf::Vector2f(4.f, -10.f));

        // Pupils
        m_leftPupil.setRadius(2.f);
        m_leftPupil.setFillColor(sf::Color::Black);
        m_leftPupil.setPosition(sf::Vector2f(-24.f, 10.f));

        m_rightPupil.setRadius(2.f);
        m_rightPupil.setFillColor(sf::Color::Black);
        m_rightPupil.setPosition(sf::Vector2f(20.f, 10.f));

        // Mouth (simple curve using a thin rectangle rotated or just a small arc)
        // For simplicity, a small downward curve or straight line. Cartman often has a frown or small mouth.
        m_mouth.setSize(sf::Vector2f(20.f, 2.f));
        m_mouth.setFillColor(sf::Color::Black);
        m_mouth.setPosition(sf::Vector2f(-10.f, 35.f));
        // Let's make it a bit curved by adding two small lines if we wanted, but a flat line is recognizable too.
        // Or actually, a small arc would be better, but let's stick to primitives.
        // A "frown" shape:
        m_mouth.setRotation(sf::degrees(10.f)); // Slight angle

        // Double chin
        m_chin.setRadius(15.f);
        m_chin.setScale(sf::Vector2f(1.5f, 0.5f));
        m_chin.setFillColor(sf::Color(0, 0, 0, 0)); // Transparent fill
        m_chin.setOutlineColor(sf::Color(200, 160, 120)); // Darker skin tone
        m_chin.setOutlineThickness(2.f);
        m_chin.setPosition(sf::Vector2f(-22.f, 35.f));
        // Only want the bottom arc... this is tricky with primitives. 
        // Let's skip the complex chin for now and focus on the main shapes.

        // --- Hands ---
        m_leftHand.setRadius(15.f);
        m_leftHand.setFillColor(hatYellow);
        m_leftHand.setPosition(sf::Vector2f(-90.f, 60.f));

        m_rightHand.setRadius(15.f);
        m_rightHand.setFillColor(hatYellow);
        m_rightHand.setPosition(sf::Vector2f(60.f, 60.f));

        // --- Coat Details ---
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

        // Draw order matters (back to front)

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
        // target.draw(m_chin, states);

        // Hat
        // We need to clip the hat top so it doesn't cover the whole face, 
        // but since we drew the head first, we can just draw the hat top 
        // but wait... the hat sits ON the head.
        // The hat shape I made is a circle. I should probably use a custom shape or just position it higher.
        // Let's adjust the hat top to be higher up.

        target.draw(m_hatTop, states);
        target.draw(m_hatBrim, states);
        target.draw(m_hatPomPom, states);

        // Hands (in front of body)
        target.draw(m_leftHand, states);
        target.draw(m_rightHand, states);
    }
};
