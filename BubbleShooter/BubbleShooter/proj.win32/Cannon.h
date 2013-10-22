#pragma once
#include "cocos2d.h"

using namespace cocos2d;

const float CANNON_LINE_LENGTH = 200.0f;

class Cannon : public CCNode
{
public:
	Cannon(void);
	~Cannon(void);

	static Cannon* create();

	void draw(void);

	void setDestination(const CCPoint& destination);
	void setOrigin(const CCPoint& origin);
	void setEnabled(bool enabled);

private:
	CCPoint m_obOrigin;
	CCPoint m_obDestination;
	bool m_bEnabled;
};

