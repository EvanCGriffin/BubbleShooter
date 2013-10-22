#include "BubbleGrid.h"
#include "ccMacros.h"

const int GRID_WIDTH = 12;
const int GRID_HEIGHT = 16;
const int GRID_HEIGHT_START = 10;

BubbleGrid* BubbleGrid::create()
{
	BubbleGrid* bubbleGrid = new BubbleGrid();
	bubbleGrid->autorelease();
	return bubbleGrid;
}

BubbleGrid::BubbleGrid(void)
{
	buildGrid();
}


BubbleGrid::~BubbleGrid(void)
{
	// Delete each row first
	for(int i = 0; i < GRID_WIDTH; ++i)
	{
		delete [] m_pBubbles[i];
	}

	delete [] m_pBubbles;
}

void BubbleGrid::buildGrid()
{
	// Create the array with the rows
	m_pBubbles = new Bubble**[GRID_WIDTH];

	// Fill each row with the cols
	for(int i = 0; i < GRID_WIDTH; i++)
		m_pBubbles[i] = new Bubble*[GRID_HEIGHT];

	CCSize size = CCDirector::sharedDirector()->getWinSize();

	float startingPosX = BUBBLE_RADIUS;
	float startingPosY = size.height - BUBBLE_RADIUS;

	// fill the 2D array wiht bubbles
	for (int i = 0; i < GRID_WIDTH; i++)
	{
		for (int j = 0; j < GRID_HEIGHT; j++)
		{
			Bubble *bubble = Bubble::create();
			bubble->setPosition(getPosition(i, j));

			if(j < GRID_HEIGHT_START)
			{
				bubble->setType((BUBBLE_TYPE)((int)((CCRANDOM_0_1() * 4) + 1)));
				if(j % 2 != 0)
				{
					if(i == GRID_WIDTH - 1) // odd rows have 1 less bubble
						bubble->setType(EMPTY);
				}
			}
			else
			{
				bubble->setType(EMPTY);
			}

			this->addChild(bubble);
			m_pBubbles[i][j] = bubble;
		}
	}
}

void BubbleGrid::update(float dt)
{
	// update any falling bubble position
	m_FallingBubblesIter = m_pFallingBubbles.begin();

	while (m_FallingBubblesIter != m_pFallingBubbles.end())
	{
		if((*m_FallingBubblesIter)->updatePosition(dt))
		{
			// Remove item
			this->removeChild(*m_FallingBubblesIter);
			m_FallingBubblesIter = m_pFallingBubbles.erase(m_FallingBubblesIter);
		}
		else
		{
			++m_FallingBubblesIter;
		}
	}
}

bool BubbleGrid::checkForCollision(Bubble* playerBubble)
{
	bool collided = false;
	int collidedX = 0;
	int collidedY = 0;

	CCPoint playerBubblePosition = playerBubble->getPosition();

	// Breadth first Search through the array starting from the bottom left
	for(int y = GRID_HEIGHT - 1; y >= 0; y--)
	{
		if(collided)
			break;

		int maxWidth = GRID_WIDTH;
		if(y % 2 != 0) // odd rows have GRID_WIDTH -1 bubbles
		{
			maxWidth -= 1;
		}

		for(int x = 0; x < maxWidth; x++)
		{
			Bubble* bubble = m_pBubbles[x][y];

			if((y != 0 && bubble->getType() == EMPTY) || bubble->getFalling())
				continue;

			// NOT BUBBLE_RADIUS * 2 because it would be two accurate - allows better gameplay
			if(ccpDistance(bubble->getPosition(), playerBubblePosition) < (BUBBLE_RADIUS * 1.5f)) 
			{
				collidedX = x;
				collidedY = y;
				collided = true;
				break;
			}
		}
	}

	if(collided)
	{
		int selectedIndexX = 0;
		int selectedIndexY = 0;

		// Find a place to put the bubble
		// selectedIndexX & selectedIndexY hold the selected position
		placeInClosestEmptySlot(playerBubble, collidedX, collidedY, selectedIndexX, selectedIndexY);

		int count = 0;
		// check for matching bubbles
		checkMatchingBubbles(selectedIndexX, selectedIndexY, playerBubble->getType(), count);

		// If we found 3 or more matches
		if(count > 2)
		{
			// Mark all connected bubbles starting from [0][0]
			checkConnectedBubbles();

			// Drop any bubbles that are not marked
			dropBubbles();
		}

		return true;
	}

	return false;
}


void BubbleGrid::placeInClosestEmptySlot(Bubble* playerBubble, int collidedX, int collidedY, int& selectedIndexX, int& selectedIndexY)
{
	CCPoint playerBubblePosition = playerBubble->getPosition();
	int maxWidth = GRID_WIDTH;
	int maxWidthOdd = GRID_WIDTH - 1;

	bool odd = false;

	if(collidedY % 2 != 0) // odd rows have GRID_WIDTH -1 bubbles
	{
		odd = true;
	}

	int left = collidedX - 1;
	int right = collidedX + 1;
	int bottom = collidedY + 1;

	float smallestDistance = FLT_MAX;

	// special case - hitting top of board
	if(collidedY == 0 && m_pBubbles[collidedX][collidedY]->getType() == EMPTY)
	{
		playerBubble->setVelocityX(0);
		playerBubble->setVelocityY(0);
		playerBubble->setPosition(getPosition(collidedX, collidedY));
		m_pBubbles[collidedX][collidedY] = playerBubble;

		selectedIndexX = collidedX;
		selectedIndexY = collidedY;
	}
	else
	{
		if(odd)
		{
			if(left >= 0)// Check if there is an empty grid space to the left of the collided bubble
			{
				if(m_pBubbles[left][collidedY]->getType() == EMPTY)
				{
					float distance = ccpDistance(getPosition(left, collidedY), playerBubblePosition);

					if(distance < smallestDistance)
					{          
						smallestDistance = distance;
						selectedIndexX = left;
						selectedIndexY = collidedY;
					}
				}
			}

			if(right < maxWidth)
			{
				if(bottom < GRID_HEIGHT) // Check if there is an empty grid space to the left and below of the collided bubble
				{
					if(m_pBubbles[right][bottom]->getType() == EMPTY)
					{
						float distance = ccpDistance(getPosition(right, bottom), playerBubblePosition);

						if(distance < smallestDistance)
						{
							smallestDistance = distance;
							selectedIndexX = right;
							selectedIndexY = bottom;
						}
					}
				}
			}

			if(right < maxWidthOdd)
			{
				if(m_pBubbles[right][collidedY]->getType() == EMPTY)
				{
					float distance = ccpDistance(getPosition(right, collidedY), playerBubblePosition);

					if(distance < smallestDistance)
					{
						smallestDistance = distance;
						selectedIndexX = right;
						selectedIndexY = collidedY;
					}
				}
			}

		}
		else
		{
			if(left >= 0) // bounds checking
			{
				if(bottom < GRID_HEIGHT) // Check if there is an empty grid space to the left and below of the collided bubble
				{
					if(m_pBubbles[left][bottom]->getType() == EMPTY)
					{
						float distance = ccpDistance(getPosition(left, bottom), playerBubblePosition);

						if(distance < smallestDistance)
						{
							smallestDistance = distance;
							selectedIndexX = left;
							selectedIndexY = bottom;
						}
					}
				}

				if(m_pBubbles[left][collidedY]->getType() == EMPTY)
				{
					float distance = ccpDistance(getPosition(left, collidedY), playerBubblePosition);

					if(distance < smallestDistance)
					{          
						smallestDistance = distance;
						selectedIndexX = left;
						selectedIndexY = collidedY;
					}
				}
			}

			if(right < maxWidthOdd)
			{
				if(m_pBubbles[right][collidedY]->getType() == EMPTY)
				{
					float distance = ccpDistance(getPosition(right, collidedY), playerBubblePosition);

					if(distance < smallestDistance)
					{
						smallestDistance = distance;
						selectedIndexX = right;
						selectedIndexY = collidedY;
					}
				}
			}
		}

		if(bottom < GRID_HEIGHT && collidedX < maxWidthOdd) // Check if there is an empty grid space beneath the collided bubble
		{
			if(m_pBubbles[collidedX][bottom]->getType() == EMPTY)
			{
				float distance = ccpDistance(getPosition(collidedX, bottom), playerBubblePosition);

				if(distance < smallestDistance)
				{
					smallestDistance = distance;
					selectedIndexX = collidedX;
					selectedIndexY = bottom;
				}
			}
		}

		// Store the bubble in the empty space
		playerBubble->setVelocityX(0);
		playerBubble->setVelocityY(0);
		playerBubble->setPosition(getPosition(selectedIndexX, selectedIndexY));

		this->removeChild(m_pBubbles[selectedIndexX][selectedIndexY]);
		m_pBubbles[selectedIndexX][selectedIndexY] = playerBubble;
	}
}

// Helper function that returns pixel co-ords for any index in the grid
const CCPoint BubbleGrid::getPosition(int x, int y)
{
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	float posX = BUBBLE_RADIUS;

	if(y % 2 != 0)
		posX += BUBBLE_RADIUS; // Shift every second row to the right a bit

	float posY = size.height - BUBBLE_RADIUS;

	posX += (x * (BUBBLE_RADIUS * 2));
	posY -= (y * (BUBBLE_RADIUS * 2));

	return ccp(posX, posY);
}

void BubbleGrid::checkMatchingBubbles(int selectedIndexX, int selectedIndexY, int type, int& count)
{
	// Reset status
	for (int i = 0; i < GRID_WIDTH; i++)
	{
		for (int j = 0; j < GRID_HEIGHT; j++)
		{
			m_pBubbles[i][j]->setChecked(false);
			m_pBubbles[i][j]->setMatched(false);
		}
	}

	// Start recursively checking any matching neighbour bubbles
	checkMatchingBubblesR(selectedIndexX, selectedIndexY, type, count);
}

void BubbleGrid::checkMatchingBubblesR(int x, int y, int type, int& count)
{
	if(m_pBubbles[x][y]->getChecked()) // already checked
		return;

	m_pBubbles[x][y]->setChecked(true);

	if(m_pBubbles[x][y]->getType() == type) // bubble match
	{
		count++;

		m_pBubbles[x][y]->setMatched(true);

		int maxWidth = GRID_WIDTH;
		int maxWidthOdd = GRID_WIDTH - 1;

		bool odd = false;

		if(y % 2 != 0) // odd rows have GRID_WIDTH -1 bubbles
		{
			odd = true;
		}

		int left = x-1;
		int right = x+1;
		int top = y-1;
		int bottom = y+1;

		if(odd) // if we are in an odd row
		{
			if(left >= 0) // bounds checking
			{
				checkMatchingBubblesR(left, y, type, count); // check left
			}

			if(right < maxWidth)
			{
				if(top >= 0)
					checkMatchingBubblesR(right, top, type, count); // check top right

				if(bottom < GRID_HEIGHT)
					checkMatchingBubblesR(right, bottom, type, count); // check bottom right
			}

			if(right < maxWidthOdd)
			{
				checkMatchingBubblesR(right, y, type, count); // check right
			}
		}
		else
		{
			if(left >= 0) // bounds checking
			{
				if(top >= 0 ) // bounds checking
					checkMatchingBubblesR(left, top, type, count); // check top left

				if(bottom < GRID_HEIGHT)
					checkMatchingBubblesR(left, bottom, type, count); // check bottom left

				checkMatchingBubblesR(left, y, type, count); // check left
			}

			if(right < maxWidth)
			{
				checkMatchingBubblesR(right, y, type, count); // check right
			}
		}

		if(top >= 0) // bounds checking
		{
			checkMatchingBubblesR(x, top, type, count); // check middle top
		}

		if(bottom < GRID_HEIGHT) // bounds checking
		{
			checkMatchingBubblesR(x, bottom, type, count); // check middle bottom
		}
	}
}

void BubbleGrid::checkConnectedBubbles()
{
	// Reset checked status
	for (int i = 0; i < GRID_WIDTH; i++)
	{
		for (int j = 0; j < GRID_HEIGHT; j++)
		{
			if(m_pBubbles[i][j]->getMatched())
			{
				m_pBubbles[i][j]->setType(EMPTY);
				m_pBubbles[i][j]->setMatched(false);
			}

			m_pBubbles[i][j]->setChecked(false);
		}
	}

	// Start recursively checking any connected bubbles starting from [0][0]
	checkConnectedBubblesR(0,0);
}

void BubbleGrid::checkConnectedBubblesR(int x, int y)
{
	if(m_pBubbles[x][y]->getChecked()) // already checked
		return;

	m_pBubbles[x][y]->setChecked(true);

	if(m_pBubbles[x][y]->getType() == EMPTY && y != 0) 
		return; // this bubble is empty so don't check any neighbours

	int maxWidth = GRID_WIDTH;
	int maxWidthOdd = GRID_WIDTH - 1;

	bool odd = false;

	if(y % 2 != 0) // odd rows have GRID_WIDTH -1 bubbles
	{
		odd = true;
	}

	int left = x-1;
	int right = x+1;
	int top = y-1;
	int bottom = y+1;


	if(m_pBubbles[x][y]->getType() == EMPTY && y == 0)
	{
		if(odd)
		{
			// If we hit an empty bubble at the top just skip to the right
			if(right < maxWidthOdd) // bounds checking
			{
				checkConnectedBubblesR(right, y);
			}
		}
		else
		{
			// If we hit an empty bubble at the top just skip to the right
			if(right < maxWidth) // bounds checking
			{
				checkConnectedBubblesR(right, y);
			}
		}
		return;
	}


	if(odd)
	{
		if(left >= 0) // bounds checking
		{
			checkConnectedBubblesR(left, y); // check left
		}

		if(right < maxWidth)
		{
			if(top >= 0)
				checkConnectedBubblesR(right, top); // check top right

			if(bottom < GRID_HEIGHT)
				checkConnectedBubblesR(right, bottom); // check bottom right
		}

		if(right < maxWidthOdd)
		{
			checkConnectedBubblesR(right, y); // check right
		}
	}
	else
	{
		if(left >= 0)
		{
			if(top >= 0 ) // bounds checking
				checkConnectedBubblesR(left, top); // check top left

			if(bottom < GRID_HEIGHT)
				checkConnectedBubblesR(left, bottom); // check bottom left

			checkConnectedBubblesR(left, y); // check left
		}

		if(right < maxWidth)
		{
			checkConnectedBubblesR(right, y); // check right
		}
	}

	if(top >= 0) // bounds checking
	{
		checkConnectedBubblesR(x, top); // check middle top
	}

	if(bottom < GRID_HEIGHT) // bounds checking
	{
		checkConnectedBubblesR(x, bottom); // check middle bottom
	}
}

void BubbleGrid::dropBubbles()
{
	// Find any bubbles not connected and set them to fall
	for (int i = 0; i < GRID_WIDTH; i++)
	{
		for (int j = 0; j < GRID_HEIGHT; j++)
		{
			if(m_pBubbles[i][j]->getChecked() == false)
			{
				if(m_pBubbles[i][j]->getType() != EMPTY)
				{
					// Store the falling bubble in the vector for updating
					Bubble *fallingBubble = m_pBubbles[i][j];
					fallingBubble->setFalling(true);
					m_pFallingBubbles.push_back(fallingBubble);

					// create a new bubble in its place
					Bubble *bubble = Bubble::create();
					bubble->setPosition(getPosition(i, j));
					this->addChild(bubble);
					m_pBubbles[i][j] = bubble;
				}
			}
		}
	}
}