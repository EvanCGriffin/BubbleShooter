#include "HelloWorldScene.h"
#include "BubbleGrid.h"
using namespace cocos2d;

CCScene* HelloWorld::scene()
{
    CCScene * scene = NULL;
    do 
    {
        // 'scene' is an autorelease object
        scene = CCScene::create();
        CC_BREAK_IF(! scene);

        // 'layer' is an autorelease object
        HelloWorld *layer = HelloWorld::create();
        CC_BREAK_IF(! layer);

        // add layer as a child to scene
        scene->addChild(layer);
    } while (0);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    bool bRet = false;
    do 
    {
        //////////////////////////////////////////////////////////////////////////
        // super init first
        //////////////////////////////////////////////////////////////////////////

       CC_BREAK_IF(! CCLayerColor::initWithColor( ccc4(255,255,255,255) ) );

        //////////////////////////////////////////////////////////////////////////
        // add your codes below...
        //////////////////////////////////////////////////////////////////////////

        // 1. Add a menu item with "X" image, which is clicked to quit the program.

        // Create a "close" menu item with close icon, it's an auto release object.
        CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
            "CloseNormal.png",
            "CloseSelected.png",
            this,
            menu_selector(HelloWorld::menuCloseCallback));
        CC_BREAK_IF(! pCloseItem);

        // Place the menu item bottom-right conner.
        pCloseItem->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width - 20, 20));

        // Create a menu with the "close" menu item, it's an auto release object.
        CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
        pMenu->setPosition(CCPointZero);
        CC_BREAK_IF(! pMenu);

        // Add the menu to HelloWorld layer as a child layer.
        this->addChild(pMenu, 1);

        // Get window size and place the label upper. 
        CCSize size = CCDirector::sharedDirector()->getWinSize();

		// Receive touch events
		this->setTouchEnabled(true);
		// schedule update function
		this->schedule(schedule_selector(HelloWorld::update));

		// Create the bubble grid and add it to the layer
		m_pBubbleGrid = BubbleGrid::create();
		CC_BREAK_IF(!m_pBubbleGrid);
		this->addChild(m_pBubbleGrid, 0);

		// Create the cannon and add it to the layer
		m_pCannon = Cannon::create();
		CC_BREAK_IF(!m_pCannon);
		this->addChild(m_pCannon, 0);

		// Create the player bubble and add it to the layer
		m_pPlayerBubble = PlayerBubble::create();
		m_pPlayerBubble->setType((BUBBLE_TYPE)((int)((CCRANDOM_0_1() * 4) + 1)));
		this->addChild(m_pPlayerBubble, 0);

        bRet = true;
    } while (0);

    return bRet;
}

void HelloWorld::update(float dt)
{
	m_pBubbleGrid->update(dt); // update falling bubbles

	if(m_pPlayerBubble != NULL && m_pPlayerBubble->getActive())
	{
		m_pPlayerBubble->updatePosition(dt);

		if(m_pBubbleGrid->checkForCollision(m_pPlayerBubble))
		{
			// If it has collided we need a new player bubble

			CCSize size = CCDirector::sharedDirector()->getWinSize();

			// Create new player bubble
			m_pPlayerBubble = PlayerBubble::create();
			m_pPlayerBubble->setType((BUBBLE_TYPE)((int)((CCRANDOM_0_1() * 4) + 1)));
			this->addChild(m_pPlayerBubble, 0);
		}
	}
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    // "close" menu item clicked
    CCDirector::sharedDirector()->end();
}

void HelloWorld::ccTouchesBegan(CCSet* touches, CCEvent* event)
{
	// Set the cannon to point the in the direction of the touch
	CCTouch* touch = (CCTouch*)(touches->anyObject());
	CCPoint location = touch->getLocationInView();
	m_pCannon->setDestination(CCDirector::sharedDirector()->convertToGL(location));

	m_pCannon->setEnabled(true);
}

void HelloWorld::ccTouchesMoved(CCSet* touches, CCEvent* event)
{
	// Update the cannon
	CCTouch* touch = (CCTouch*)(touches->anyObject());
	CCPoint location = touch->getLocationInView();
	m_pCannon->setDestination(CCDirector::sharedDirector()->convertToGL(location));
}

void HelloWorld::ccTouchesEnded(CCSet* touches, CCEvent* event)
{
	if(m_pPlayerBubble == NULL || m_pPlayerBubble->getActive())
		return;

	// Get the touch position and shoot the ball in that direction

	CCTouch* touch = (CCTouch*)(touches->anyObject());
	CCPoint location = touch->getLocationInView();
	location = CCDirector::sharedDirector()->convertToGL(location);

	m_pCannon->setEnabled(false);

	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	CCPoint direction = ccpNormalize(ccpSub(location, m_pPlayerBubble->getPosition()));

	if(direction.y <= 0) // don't shoot downwards
		return;

	m_pPlayerBubble->setVelocityX(direction.x * BUBBLE_SPEED);
	m_pPlayerBubble->setVelocityY(direction.y * BUBBLE_SPEED);
	m_pPlayerBubble->setActive(true);
}
