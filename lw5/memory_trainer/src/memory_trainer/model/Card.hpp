#pragma once
#include <string>

enum class CardState
{
	Hidden = 0,
	FlippingUp = 1,
	Visible = 2,
	FlippingDown = 3,
};

class Card
{
public:
	Card(size_t id, const std::string& name, size_t row, size_t col)
		: m_id(id)
		, m_name(name)
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

	void FlipDown()
	{
		if (m_state == CardState::Visible)
		{
			m_state = CardState::FlippingDown;
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

	void SetMatched()
	{
		m_state = CardState::Hidden;
	}

	size_t GetId() const
	{
		return m_id;
	}

	const std::string& GetName() const
	{
		return m_name;
	}

	size_t GetRow() const
	{
		return m_row;
	}

	size_t GetCol() const
	{
		return m_col;
	}

	CardState GetState() const
	{
		return m_state;
	}

	float GetAngle() const
	{
		return m_angle;
	}

private:
	static constexpr float FLIP_SPEED = 240;

	size_t m_id;
	std::string m_name;
	size_t m_row, m_col;
	CardState m_state = CardState::Hidden;
	float m_angle = 0;
};