#pragma once

#include "../viewModel/AlchemyViewModel.h"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>

#include <algorithm>

class PopupMessageView
{
public:
	PopupMessageView(sf::RenderWindow& window, const sf::Font& font, AlchemyViewModel& viewModel)
		: m_window(window)
		, m_font(font)
		, m_viewModel(viewModel)
	{
	}

	void Draw()
	{
		std::wstring message = m_viewModel.GetMessage();
		if (message.empty())
		{
			return;
		}

		float alpha = CalculateAlpha(m_viewModel.GetMessageAge());
		if (alpha <= 0.f)
		{
			return;
		}

		sf::Text text = CreateText(message, alpha);
		sf::RectangleShape background = CreateBackground(text, alpha);

		m_window.draw(background);
		m_window.draw(text);
	}

private:
	static constexpr unsigned int FontSize = 16;
	static constexpr float PaddingX = 20.f;
	static constexpr float PaddingY = 12.f;
	static constexpr float BottomMargin = 30.f;
	static constexpr float FadeStart = 1.4f;
	static constexpr float FadeEnd = 2.0f;
	static constexpr float BackgroundAlphaFactor = 0.85f;

	sf::Text CreateText(const std::wstring& message, float alpha) const
	{
		sf::Text text(m_font, message, FontSize);
		text.setFillColor({ 255, 255, 255, ToAlphaByte(alpha) });
		return text;
	}

	sf::RectangleShape CreateBackground(sf::Text& text, float alpha) const
	{
		auto textBounds = text.getLocalBounds();
		sf::Vector2f boxSize = {
			PaddingX * 2.f + textBounds.size.x,
			PaddingY * 2.f + textBounds.size.y,
		};

		auto windowSize = m_window.getSize();
		sf::Vector2f boxPosition = {
			(static_cast<float>(windowSize.x) - boxSize.x) / 2.f,
			static_cast<float>(windowSize.y) - boxSize.y - BottomMargin,
		};

		text.setPosition({
			boxPosition.x + PaddingX,
			boxPosition.y + PaddingY - 2.f,
		});

		sf::RectangleShape background(boxSize);
		background.setPosition(boxPosition);
		background.setFillColor({ 40, 40, 40, ToAlphaByte(alpha * BackgroundAlphaFactor) });
		background.setOutlineColor({ 80, 80, 80, ToAlphaByte(alpha) });
		background.setOutlineThickness(1.f);
		return background;
	}

	static float CalculateAlpha(float age)
	{
		if (age < FadeStart)
		{
			return 1.f;
		}
		if (age >= FadeEnd)
		{
			return 0.f;
		}
		return 1.f - (age - FadeStart) / (FadeEnd - FadeStart);
	}

	static std::uint8_t ToAlphaByte(float normalized)
	{
		return static_cast<std::uint8_t>(std::clamp(normalized, 0.f, 1.f) * 255.f);
	}

	sf::RenderWindow& m_window;
	const sf::Font& m_font;
	AlchemyViewModel& m_viewModel;
};
