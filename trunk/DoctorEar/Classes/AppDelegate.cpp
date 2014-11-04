#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include "MainMenu.h"
#include "Define.h"

USING_NS_CC;
int AppDelegate::_typePatient = 1;

typedef struct tagResource
{
    cocos2d::Size size;
    char directory[100];
}Resource;

static Resource mediumResource =  { cocos2d::Size(640, 1136),  "iphone"   };
static Resource largeResource  =  { cocos2d::Size(1536, 2048), "ipad" };
static cocos2d::Size designResolutionSize = cocos2d::Size(640, 1136);

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
    
    
    cocos2d::Size frameSize = glview->getFrameSize();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::FIXED_WIDTH);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::FIXED_WIDTH);
#endif
    
    //    if (frameSize.width > mediumResource.size.width) {
    //        cocos2d::FileUtils::getInstance()->addSearchPath(largeResource.directory);
    //        director->setContentScaleFactor(largeResource.size.width / designResolutionSize.width);
    //    }
    //    else
    {
        cocos2d::FileUtils::getInstance()->addSearchPath(mediumResource.directory);
        director->setContentScaleFactor(mediumResource.size.width / designResolutionSize.width);
    }
    
    
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
