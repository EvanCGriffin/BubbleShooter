#include "PlayerBubble.h"

PlayerBubble* PlayerBubble::create()
{
	PlayerBubble* playerBubble = new PlayerBubble();
	playerBubble->autorelease();
	return playerBubble;
}

PlayerBubble::PlayerBubble(void)
{
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	m_obPosition = ccp(winSize.width / 2, BUBBLE_RADIUS);
	m_bActive = false;
}


PlayerBubble::~PlayerBubble(void)
{
}

void PlayerBubble::setActive(const bool& active)
{
	m_bActive = active;
}

const bool& PlayerBubble::getActive()
{
	return m_bActive;
}

void PlayerBubble::updatePosition(float dt)
{
	if(!m_bActive)
		return;

	Bubble::updatePosition(dt);

	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	// Check for collision against walls
	if(m_obPosition.x < BUBBLE_RADIUS || (m_obPosition.x > winSize.width - BUBBLE_RADIUS))
	{
		m_fVelocityX *= -1;
	}
}
