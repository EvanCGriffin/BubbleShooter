#include "Cannon.h"
#include "Bubble.h"

Cannon* Cannon::create()
{
	Cannon* cannon = new Cannon();
	cannon->autorelease();
	return cannon;
}

Cannon::Cannon(void)
{
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	m_obOrigin = ccp(winSize.width / 2, BUBBLE_RADIUS);
	setDestination(ccp(winSize.width / 2, winSize.height));
	m_bEnabled = false;
}


Cannon::~Cannon(void)
{
}

void Cannon::setDestination(const CCPoint& destination)
{
	// Cannon line should have a fixed length
	CCPoint direction = ccpNormalize(ccpSub(destination, m_obOrigin));
	m_obDestination = ccpAdd(m_obOrigin, ccpMult(direction, CANNON_LINE_LENGTH));
}

void Cannon::setOrigin(const CCPoint& origin)
{
	m_obOrigin = origin;
}

void Cannon::setEnabled(bool enabled)
{
	m_bEnabled = enabled;
}

void Cannon::draw()
{
	if(m_bEnabled)
	{
		ccDrawColor4F(0, 0, 0, 1);
		ccDrawLine(m_obOrigin, m_obDestination);
	}
}
