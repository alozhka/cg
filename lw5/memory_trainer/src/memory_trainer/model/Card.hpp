#pragma once
#include <string>

enum class CardState
{
	Hidden = 0,
	FlippingUp = 1,
	Visible = 2,
	FlippingDown = 3
};

class Card
{
public:
	Card(const std::string& name, size_t row, size_t col)
		: m_name(name)
		, m_row(row)
		, m_col(col)
	{
	}

	void Flip()
	{
		if (m_state == CardState::Hidden)
		{
			m_state = CardState::FlippingUp;
		}
	}

	void Update(float dt)
	{
		if (m_state == CardState::Hidden || m_state == CardState::Visible)
		{
			return;
		}

		float deltaAngle = FLIP_SPEED * dt;

		if (m_state == CardState::FlippingUp)
		{
			m_angle += deltaAngle;
			if (m_angle > 180)
			{
				m_state = CardState::Visible;
				m_angle = 180;
			}
		}

		if (m_state == CardState::FlippingDown)
		{
			m_angle -= deltaAngle;
			if (m_angle < 0)
			{
				m_state = CardState::Hidden;
				m_angle = 0;
			}
		}
	}

	[[nodiscard]] const std::string& GetName() const { return m_name; }
	[[nodiscard]] size_t GetRow() const { return m_row; }
	[[nodiscard]] size_t GetCol() const { return m_col; }
	[[nodiscard]] CardState GetState() const { return m_state; }
	[[nodiscard]] float GetAngle() const { return m_angle; }

private:
	static constexpr float FLIP_SPEED = 180;

	std::string m_name;
	size_t m_row, m_col;
	CardState m_state = CardState::Hidden;
	float m_angle = 0;
};