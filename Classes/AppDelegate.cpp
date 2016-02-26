#include "AppDelegate.h"
#include "BreakoutScene.h"
#include "GameOverScene.h"

USING_NS_CC;

AppDelegate::AppDelegate() {}

AppDelegate::~AppDelegate() {}

bool AppDelegate::applicationDidFinishLaunching() {
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    
	if(!glview) {
		glview = GLViewImpl::create("Breakout");
		glview->setFrameSize(1000, 800);
		director->setOpenGLView(glview);
    }

    auto scene = Breakout::createScene();
	director->runWithScene(scene);

    return true;
}

void AppDelegate::applicationDidEnterBackground() {}

void AppDelegate::applicationWillEnterForeground() {}