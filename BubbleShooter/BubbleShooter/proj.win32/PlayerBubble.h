#pragma once
#include "bubble.h"
class PlayerBubble : public Bubble
{
public:
	PlayerBubble(void);
	~PlayerBubble(void);

	static PlayerBubble* create();

	void updatePosition(float dt);

	void setActive(const bool& active);
	const bool& getActive(void);

protected:
	bool m_bActive;
};

