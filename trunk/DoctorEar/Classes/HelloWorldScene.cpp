#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    Scene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

HelloWorld::~HelloWorld() {
    // remember to release all retain ref
    brush->release();
    canvas->release();
    eraser->release();
    shadow->release();
    shadowCanvas->release();
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    MenuItemImage *pCloseItem = MenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        this,
                                        menu_selector(HelloWorld::menuCloseCallback) );
    pCloseItem->setPosition( Point(Director::getInstance()->getWinSize().width - 20, 20) );
    
    MenuItemLabel* pEraser = MenuItemLabel::create(Label::createWithSystemFont("Eraser", "System", 30), [&](Ref* sender){
        MenuItemLabel* labelMenu = dynamic_cast<MenuItemLabel*>(sender);
        Label* label = dynamic_cast<Label*>(labelMenu->getLabel());
        
        this->isEraser =!this->isEraser;
        if (this->isEraser) {
            label->setString("Brush");
        }
        else {
            label->setString("Eraser");
        }
    });
    pEraser->setPosition(getContentSize().width - 100, 30);
    
    MenuItemLabel* pBrush1 = MenuItemLabel::create(Label::createWithSystemFont("Brush 1", "System", 30), [&](Ref* sender){
        brush->setTexture("DrawResources/brush/pen_brush_1.png");
        shadow->setTexture("DrawResources/brush/pen_brush_1.png");
        brushIndex = 1;
    });
    pBrush1->setPosition(getContentSize().width - 220, 30);
    
    MenuItemLabel* pBrush2 = MenuItemLabel::create(Label::createWithSystemFont("Brush 2", "System", 30), [&](Ref* sender){
        brush->setTexture("DrawResources/brush/pen_brush_2.png");
        shadow->setTexture("DrawResources/brush/pen_brush_2.png");
        brushIndex = 2;
    });
    pBrush2->setPosition(getContentSize().width - 340, 30);
    
    MenuItemLabel* pBrush3 = MenuItemLabel::create(Label::createWithSystemFont("Brush 3", "System", 30), [&](Ref* sender){
        brush->setTexture("DrawResources/brush/pen_brush_3.png");
        shadow->setTexture("DrawResources/brush/pen_brush_3.png");
        brushIndex = 3;
    });
    pBrush3->setPosition(getContentSize().width - 460, 30);
    
    MenuItemLabel* pBrush4 = MenuItemLabel::create(Label::createWithSystemFont("Brush 4", "System", 30), [&](Ref* sender){
        brush->setTexture("pen_brush_4.png");
        shadow->setTexture("pen_brush_4.png");
        brushIndex = 4;
    });
    pBrush4->setPosition(getContentSize().width - 580, 30);

    // create menu, it's an autorelease object
    Menu* pMenu = Menu::create(pCloseItem, pEraser, pBrush1, pBrush2, pBrush3, pBrush4, NULL);
    pMenu->setPosition( Point::ZERO );
    this->addChild(pMenu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    CCLabelTTF* pLabel = CCLabelTTF::create("Finger Paint Demo", 
                                            "Thonburi", 
                                            34);

    // ask director the window size
    Size size = Director::getInstance()->getWinSize();

    // position the label on the center of the screen
    pLabel->setPosition( Point(size.width / 2, size.height - 20) );

    // add the label as a child to this layer
    this->addChild(pLabel, 1);

    /////////////////////////////
    // finger paint implementation
    
    // enable touch events
    setTouchMode(Touch::DispatchMode::ALL_AT_ONCE);
    setTouchEnabled( true );
    
    // create a canvas to draw on
    canvas = RenderTexture::create(size.width, size.height,
                                   Texture2D::PixelFormat::RGBA8888);
    
    // position on the center of the screen
    canvas->setPosition(Point(size.width / 2, size.height / 2));
    
    // used later do not release
    canvas->retain();
    
    // set target as child and z order to 1
    this->addChild(canvas, 15);
    
    shadowCanvas = RenderTexture::create(size.width, size.height, Texture2D::PixelFormat::RGBA8888);
    shadowCanvas->setPosition(size.width / 2, size.height / 2);
    shadowCanvas->retain();
    this->addChild(shadowCanvas, 1);
    
    shadow = Sprite::create("DrawResources/brush/pen_brush_1.png");
    shadow->retain();
    shadow->setColor(Color3B::RED);

    // init the brush tip
    brush = Sprite::create("DrawResources/brush/pen_brush_1.png");
    // Change color of brush by this line.
    brush->setColor(Color3B::YELLOW);
    // RGB color
    brush->setColor(Color3B(0, 0, 0));
    // used later do not release
    brush->retain();
    
//    eraser = Sprite::create("brush.png");
//    eraser->retain();
//    eraser->setScale(3);
//    BlendFunc f = {GL_ZERO,GL_ONE_MINUS_SRC_ALPHA };
//    eraser->setBlendFunc(f);
    
    brushIndex = 1;

    return true;
}

void HelloWorld::onTouchesBegan(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *unused_event) {
    
}

void HelloWorld::onTouchesMoved(const std::vector<Touch *> &touches, cocos2d::Event *unused_event)
{
    Touch* touch = touches.at(0);
    
    // get start & end location
    Point start = touch->getLocationInView();
    Point end = touch->getPreviousLocationInView();
    
    // get corrected location
    start = Director::getInstance()->convertToGL(start);
    end = Director::getInstance()->convertToGL(end);

    // draw line on the canvas

    float distance = end.getDistance(start);
    float diffX = end.x - start.x;
    float diffY = end.y - start.y;
    
    if (brushIndex == 3 || brushIndex == 2) {
        for (int i = 0; i < distance; i++) {
            shadowCanvas->begin();
            float delta = float(i) / distance;
            Point pos(start.x + (diffX * delta), start.y + (diffY * delta));
            shadow->setPosition(pos);
            shadow->setScale(1.5);
            shadow->setOpacity(140);
            shadow->visit();
            shadowCanvas->end();
            Director::getInstance()->getRenderer()->render();
        }
        
    }
    
    for (int i = 0; i < int(distance); i++)
    {
        
        float delta = float(i) / distance;
        Point pos(start.x + (diffX * delta), start.y + (diffY * delta));
        
        if (isEraser) {
            canvas->begin();
            eraser->setPosition(pos);
            eraser->visit();
            canvas->end();
            Director::getInstance()->getRenderer()->render();
            
            shadowCanvas->begin();
            eraser->setPosition(pos);
            eraser->visit();
            shadowCanvas->end();
            Director::getInstance()->getRenderer()->render();
        }
        else {
            canvas->begin();
            if (brushIndex == 4) {
                brush->setColor(Color3B(arc4random() % 255, arc4random() %255, arc4random() % 255));
            }
            else {
                brush->setColor(Color3B::YELLOW);
            }
            
            brush->setPosition(pos);
            brush->visit();
            canvas->end();
            Director::getInstance()->getRenderer()->render();
        }
    }
}

void HelloWorld::onTouchesEnded(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *unused_event) {
    
}

void HelloWorld::onTouchesCancelled(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *unused_event) {
    
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
