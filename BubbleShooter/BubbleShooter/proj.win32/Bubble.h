#pragma once
#include "cocos2d.h"

using namespace cocos2d;

enum BUBBLE_TYPE : int
{
	EMPTY = 0,
	RED = 1,
	GREEN = 2,
	BLUE = 3,
	YELLOW = 4,
	PURPLE = 5
};

const float BUBBLE_RADIUS = 13.3f;
const float BUBBLE_SPEED = 400.0f;

class Bubble : public CCNode
{
public:
	Bubble(void);
	~Bubble(void);

	static Bubble* create();

	void setType(const BUBBLE_TYPE& type);
	const BUBBLE_TYPE& getType(void);

	void setRadius(const float& radius);
	const float& getRadius(void);

	void setVelocityX(const float& velocityX);
	const float& getVelocityX(void);

	void setVelocityY(const float& velocityY);
	const float& getVelocityY(void);

	void setChecked(const bool& checked);
	const bool& getChecked(void);

	void setFalling(const bool& falling);
	const bool& getFalling(void);

	void setMatched(const bool& matched);
	const bool& getMatched(void);

	void draw(void);
	bool updatePosition(float dt);

protected:
	ccColor4F m_sColor;
	float m_fRadius;
	BUBBLE_TYPE m_eType;

	float m_fVelocityX;
	float m_fVelocityY;

	bool m_bChecked; // used for recursion
	bool m_bFalling;
	bool m_bMatched;
};

