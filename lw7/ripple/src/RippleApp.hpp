#pragma once

#include "RippleEffect.hpp"

#include <graphics/windows/GraphicsApplication.hpp>
#include <graphics/windows/TimeProvider.hpp>

#include <glm/glm.hpp>

class RippleApp final : public GraphicsApplication
{
public:
	RippleApp(int width, int height, const std::string& title)
		: GraphicsApplication(width, height, title)
	{
	}

protected:
	void OnDraw(const glm::mat4&) override
	{
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		float dt = m_time.GetDeltaTime();

		if (m_transitioning)
		{
			m_transitionTime += dt;
			if (m_transitionTime >= CLICK_COOLDOWN)
			{
				m_transitionTime = 0;
				m_transitioning = false;
				m_flipped = !m_flipped;
			}
		}

		m_effect.Draw(m_rippleOrigin, m_transitionTime, m_flipped);
	}

	void OnMouseButton(int button, int action, glm::vec2 p) override
	{
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && !m_transitioning)
		{
			m_rippleOrigin = glm::vec2(p.x * 0.5 + 0.5, p.y * 0.5 + 0.5);
			m_transitionTime = 0;
			m_transitioning = true;
		}
	}

private:
	static constexpr float CLICK_COOLDOWN = 2.5;

	RippleEffect m_effect;
	TimeProvider m_time;

	glm::vec2 m_rippleOrigin{};
	float m_transitionTime = 0;
	bool m_transitioning = false;
	bool m_flipped = false;
};
