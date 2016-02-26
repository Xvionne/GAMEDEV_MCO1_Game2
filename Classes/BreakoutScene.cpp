#include "BreakoutScene.h"
#include "GameOverScene.h"

USING_NS_CC;

Scene* Breakout::createScene() {
    auto scene = Scene::createWithPhysics();
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

    auto layer = Breakout::create();
	layer->setPhysicsWorld(scene->getPhysicsWorld());

    scene->addChild(layer);

    return scene;
}

bool Breakout::init() {
    if (!Layer::init()) {
        return false;
    }

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
	auto edgeBody = PhysicsBody::createEdgeBox(visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 3);
	edgeBody->getShape(0)->setRestitution(1.0f);
	edgeBody->getShape(0)->setFriction(0.0f);
	edgeBody->getShape(0)->setDensity(1.0f);
	edgeBody->setContactTestBitmask(0x000001);

	auto edgeNode = Sprite::create();
	edgeNode->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	edgeNode->setPhysicsBody(edgeBody);

	edgeNode->setTag(0);

	this->addChild(edgeNode);

	backgroundSprite = Sprite::create("BackgroundSprite.jpg");
	backgroundSprite->setAnchorPoint(Vec2(0, 0));

	this->addChild(backgroundSprite);

	ballSprite = Sprite::create("BallSprite.png");
	ballSprite->setPosition(Point(visibleSize.width / 2 + origin.x, 130));

	auto ballBody = PhysicsBody::createCircle(ballSprite->getContentSize().width / 2, PhysicsMaterial(0, 1, 0));
	ballBody->getShape(0)->setRestitution(1.0f);
	ballBody->getShape(0)->setFriction(0.0f);
	ballBody->getShape(0)->setDensity(1.0f);
	ballBody->setGravityEnable(false);
	ballBody->setCollisionBitmask(1);
	ballBody->setContactTestBitmask(0x000001);
	ballBody->setVelocity(Vec2(300, 200));

	ballSprite->setTag(1);
	ballSprite->setPhysicsBody(ballBody);

	this->addChild(ballSprite);

	platformSprite = Sprite::create("PlatformSprite.png");
	platformSprite->setPosition(Point(visibleSize.width / 2 + origin.x, 80));

	auto platformBody = PhysicsBody::createBox(platformSprite->getContentSize(), PhysicsMaterial(0, 1, 0));
	platformBody->getShape(0)->setRestitution(1.0f);
	platformBody->getShape(0)->setFriction(0.0f);
	platformBody->getShape(0)->setDensity(10.0f);
	platformBody->setGravityEnable(false);
	platformBody->setDynamic(false);
	platformBody->setContactTestBitmask(0x000001);

	platformSprite->setTag(2);
	platformSprite->setPhysicsBody(platformBody);

	this->addChild(platformSprite);
	
	int counter = 3;
	int y = 750;

	for (int row = 0; row < 6; row++) {
		int x = 70;

		for (int column = 0; column < 20; column++) {
			auto blockSprite = Sprite::create("BlockSpriteBlack.png");
			
			auto blockBody = PhysicsBody::createBox(blockSprite->getContentSize(), PhysicsMaterial(0, 1, 0));
			blockBody->getShape(0)->setDensity(10.0f);
			blockBody->getShape(0)->setFriction(0.0f);
			blockBody->getShape(0)->setRestitution(1.f);
			blockBody->setDynamic(false);
			blockBody->setCollisionBitmask(2);
			blockBody->setContactTestBitmask(0x000001);

			blockSprite->setPhysicsBody(blockBody);
			blockSprite->setTag(counter);
			blockSprite->setPosition(x, y);
			counter++;

			x += blockSprite->getContentSize().width + 5;

			this->addChild(blockSprite);
		}

		y -= 70;
	}

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(Breakout::onTouchBegan, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(Breakout::onTouchMoved, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(Breakout::onTouchEnded, this);
	touchListener->onTouchCancelled = CC_CALLBACK_2(Breakout::onTouchCancelled, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(Breakout::onContactBegin, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

	this->scheduleUpdate();

    return true;
}

void Breakout::update(float delta) {
	bool isWon = true;

	Vector<PhysicsBody*> bodies = gameWorld->getAllBodies();

	for each(PhysicsBody* body in bodies) {
		if (body->getNode()->getTag() > 2) {
			isWon = false;
		}
	}

	if (isWon) {
		auto gameOverScene = GameOverScene::create();
		gameOverScene->getLayer()->getLabel()->setString("GAME OVER!");
		Director::getInstance()->replaceScene(gameOverScene);
	}
}

bool Breakout::onTouchBegan(Touch* touch, Event* event) {
	return true;
}

void Breakout::onTouchEnded(Touch* touch, Event* event) {}

void Breakout::onTouchMoved(Touch* touch, Event* event) {
	Point touchLocation = this->convertToWorldSpace(this->convertTouchToNodeSpace(touch));
	platformSprite->setPositionX(touchLocation.x);
}

void Breakout::onTouchCancelled(Touch* touch, Event* event) {}

bool Breakout::onContactBegin(cocos2d::PhysicsContact &contact) {
	auto A = (Sprite*)contact.getShapeA()->getBody()->getNode();
	auto B = (Sprite*)contact.getShapeB()->getBody()->getNode();

	int tagA = A->getTag();
	int tagB = B->getTag();

	if (tagA > 2){
		this->removeChild(A, true);
	}
	if (tagB > 2) {
		this->removeChild(B, true);
	}

	if ((tagA == 0 || tagB == 0) && (ballSprite->getPositionY() <= platformSprite->getPositionY())) {
		auto gameOverScene = GameOverScene::create();
		gameOverScene->getLayer()->getLabel()->setString("GAME OVER!");
		Director::getInstance()->replaceScene(gameOverScene);
	}

	return true;
}
