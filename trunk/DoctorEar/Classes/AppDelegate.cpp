#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include "MainMenu.h"
#include "Define.h"

USING_NS_CC;
int AppDelegate::_typePatient = 1;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLView::create("My Game");
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    UserDefault *defaults = UserDefault::getInstance();
    if(defaults->getIntegerForKey("firstrun") == 0){
        CCLOG("FIRST RUN");
        defaults->setIntegerForKey("highScore",0);
        defaults->setBoolForKey(SOUND_ON_OFF, true);
        defaults->setIntegerForKey("firstrun", 1);
    }

    defaults->flush();
    
    FileUtils::getInstance()->addSearchPath("images");
    
    // create a scene. it's an autorelease object
//    auto scene = HelloWorld::createScene();
    auto scene = MainMenu::createScene();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
