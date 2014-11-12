#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class HelloWorld : public cocos2d::Layer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    ~HelloWorld();
    virtual bool init();  

    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::Scene* createScene();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);

    // implement the "static node()" method manually
    CREATE_FUNC(HelloWorld);

    // touch move event to handle draw input
    virtual void onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *unused_event);
    virtual void onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *unused_event);
    virtual void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *unused_event);
    virtual void onTouchesCancelled(const std::vector<cocos2d::Touch*>&touches, cocos2d::Event *unused_event);

private:
    // canvas and brush tip
    cocos2d::RenderTexture* canvas;
    cocos2d::RenderTexture* shadowCanvas;
    cocos2d::Sprite* shadow;
    cocos2d::Sprite* brush;
    cocos2d::Sprite*  eraser;
    
    bool isEraser;
    int brushIndex;
};

#endif // __HELLOWORLD_SCENE_H__