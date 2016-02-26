#pragma once

#include "cocos2d.h"

class Breakout : public cocos2d::Layer {
public:
    static cocos2d::Scene* createScene();
    
	virtual bool init() override;
	
	void update(float) override;

	bool onContactBegin(cocos2d::PhysicsContact  &contact);

	virtual void onTouchCancelled(cocos2d::Touch*, cocos2d::Event*);
	virtual void onTouchMoved(cocos2d::Touch*, cocos2d::Event*);
	virtual void onTouchEnded(cocos2d::Touch*, cocos2d::Event*);
	virtual bool onTouchBegan(cocos2d::Touch*, cocos2d::Event*);

    CREATE_FUNC(Breakout);

private:
	cocos2d::PhysicsWorld *gameWorld;

	void setPhysicsWorld(cocos2d::PhysicsWorld *world) { gameWorld = world; };

	cocos2d::Sprite* backgroundSprite;
	cocos2d::Sprite* ballSprite;
	cocos2d::Sprite* platformSprite;
};