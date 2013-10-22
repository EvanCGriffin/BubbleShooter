#pragma once
#include "cocos2d.h"
#include "Bubble.h"

using namespace cocos2d;

class BubbleGrid : public CCNode
{
public:
	BubbleGrid(void);
	~BubbleGrid(void);

	static BubbleGrid* create(void);

	void buildGrid(void);

	void update(float dt);

	bool checkForCollision(Bubble* playerBubble);

	const CCPoint getPosition(int x, int y);
protected:
	Bubble*** m_pBubbles; // 2D array

	std::vector<Bubble*> m_pFallingBubbles;
	std::vector<Bubble*>::iterator m_FallingBubblesIter;

private:
	void placeInClosestEmptySlot(Bubble* playerBubble, int collidedX, int collidedY, int& selectedIndexX, int& selectedIndexY);
	
	void checkMatchingBubbles(int x, int y, int type, int& count);
	void checkMatchingBubblesR(int x, int y, int type, int& count); // recursive function

	void checkConnectedBubbles(void);
	void checkConnectedBubblesR(int x, int y); // recursive function

	void dropBubbles(void);
};

