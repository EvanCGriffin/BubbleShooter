#include "Bubble.h"

Bubble* Bubble::create()
{
	Bubble* bubble = new Bubble();
	bubble->autorelease();
	return bubble;
}

Bubble::Bubble(void)
{
	setType(EMPTY);
	m_fVelocityX = 0;
	m_fVelocityY = 0;
	m_fRadius = BUBBLE_RADIUS;
	m_bChecked = false;
	m_bFalling = false;
	m_bMatched = false;
}


Bubble::~Bubble(void)
{
}

void Bubble::setType(const BUBBLE_TYPE& type)
{
	switch(type)
	{
	case EMPTY:
		m_sColor = ccc4f(0, 0, 0, 1);
		break;
	case RED:
		m_sColor = ccc4f(1.0, 0, 0, 0.5);
		break;
	case GREEN:
		m_sColor = ccc4f(0, 1.0, 0, 0.5);
		break;
	case BLUE:
		m_sColor = ccc4f(0, 0, 1.0, 0.5);
		break;
	case YELLOW:
		m_sColor = ccc4f(1.0, 1.0, 0, 0.5);
		break;
	case PURPLE:
		m_sColor = ccc4f(1.0, 0, 1.0, 0.5);
		break;
	}
	m_eType = type;
}

const BUBBLE_TYPE& Bubble::getType()
{
	return m_eType;
}

void Bubble::setRadius(const float& radius)
{
	m_fRadius = radius;
}

const float& Bubble::getRadius()
{
	return m_fRadius;
}

void Bubble::setVelocityX(const float& velocityX)
{
	m_fVelocityX = velocityX;
}

const float& Bubble::getVelocityX()
{
	return m_fVelocityX;
}

void Bubble::setVelocityY(const float& velocityY)
{
	m_fVelocityY = velocityY;
}

const float& Bubble::getVelocityY()
{
	return m_fVelocityY;
}

void Bubble::setChecked(const bool& checked)
{
	m_bChecked = checked;
}

const bool& Bubble::getChecked()
{
	return m_bChecked;
}

void Bubble::setFalling(const bool& falling)
{
	m_bFalling = falling;
}

const bool& Bubble::getFalling()
{
	return m_bFalling;
}

void Bubble::setMatched(const bool& matched)
{
	m_bMatched = matched;
}

const bool& Bubble::getMatched()
{
	return m_bMatched;
}

void Bubble::draw(void)
{
	if(m_eType != EMPTY)
	{
		ccDrawColor4F(m_sColor.r, m_sColor.g, m_sColor.b, m_sColor.a);
		ccDrawCircle(CCPointZero, m_fRadius, 0, 16, false, 1, 1);
	}
}

bool Bubble::updatePosition(float dt)
{
	if(m_eType == EMPTY)
		return false;

	if(m_bFalling)
	{
		m_fVelocityY -= 20; // Simulate gravity constant

		if(m_obPosition.y < -BUBBLE_RADIUS)
		{
			return true; // if we are off screen then we can be removed from the world
		}
	}
	setPositionX(m_obPosition.x += m_fVelocityX * dt);
	setPositionY(m_obPosition.y += m_fVelocityY * dt);

	return false;
}
