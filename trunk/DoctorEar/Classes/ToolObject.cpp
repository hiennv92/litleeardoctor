//
//  ToolObject.cpp
//  DoctorEar
//
//  Created by Trinh Van Duong on 10/31/14.
//
//

#include "ToolObject.h"
#include "Define.h"
#include "GamePlay.h"

Tool::Tool(){}

Tool::~Tool(){}

Tool* Tool::createTool(std::string imageTool, int typeTool){
    Tool *tool = new Tool();
    if(tool->initWithFile(imageTool)){
        tool->autorelease();
        tool->initOptions(typeTool);
        tool->addEvents();
        return  tool;
    }
    CC_SAFE_DELETE(tool);
    return NULL;
}

void Tool::initOptions(int typeTool){
    _visibleSize = Director::getInstance()->getVisibleSize();
    _typeTool = typeTool;
    //Use for keepear tool
    _isSet = false;
    //Use for small table
    _velocityMoveSmallTable = Point(1.0f,0.0f);
    if(_typeTool == 100){
        this->schedule(schedule_selector(Tool::updateTool));
        _startMove = false;
    }
}

void Tool::updateTool(float dt){
    if(_startMove){
        if(!_isTouch){
            _velocityMoveSmallTable = Point(-25,_velocityMoveSmallTable.y);
        }else{
            _velocityMoveSmallTable = Point(25,_velocityMoveSmallTable.y);
        }
        this->setPosition(this->getPosition() + _velocityMoveSmallTable);

        if(this->getPosition().x <= -_visibleSize.width*0.5f || this->getPosition().x >= _visibleSize.width*0.3f){
            _startMove = false;
        }
    }
}

void Tool::setHandUp(){
    _handHelp = Help::createHelp(HAND_UP_DOWN_HELP, 1);
    _handHelp->setScale(2.5f);
    _handHelp->setPosition(Point(_visibleSize.width*0.53,_visibleSize.height*0.18f));
    (this->getParent())->addChild(_handHelp,15);
    _handHelp->_savePosition = _handHelp->getPosition();
    _handHelp->_typeHand = 1;
    _handHelp->moveHandVertical();
}

void Tool::setUpNoteHelp(){
    //set note help
    switch (_typeTool) {
        case TOOL_1:
            _noteHelp = Help::createHelp(HELP_NOTE_1, TOOL_1);
            _noteHelp->setPosition(Point(_visibleSize.width*0.85f,_visibleSize.height*0.45f));
            break;
        case TOOL_2:{
            _noteHelp = Help::createHelp(HELP_NOTE_2, TOOL_2);
            _noteHelp->setPosition(Point(_visibleSize.width*0.16f,_visibleSize.height*0.44f));
            auto btnOnOff = MenuItemImage::create(TOOL_DENPIN_BUTTON_OFF,TOOL_DENPIN_BUTTON_OFF,CC_CALLBACK_1(Tool::turnOnFlashLight, this));
            btnOnOff->setPosition(this->getContentSize().width*0.5f,this->getContentSize().height*0.5f);
            _isTurnOnFlashLight = false;
            
            //Menu
            auto menu = Menu::create(btnOnOff,NULL);
            menu->setPosition(Vec2::ZERO);
            this->addChild(menu,15);
            
            break;
        }
        case TOOL_TYPE_SCISSOR:
            _noteHelp = Help::createHelp(HELP_NOTE_3, TOOL_TYPE_SCISSOR);
            _noteHelp->setPosition(Point(_visibleSize.width*0.16f,_visibleSize.height*0.44f));
            break;
        case TOOL_TYPE_SHAKE_EAR:
            _noteHelp = Help::createHelp(HELP_NOTE_4, TOOL_TYPE_SHAKE_EAR);
            _noteHelp->setPosition(Point(_visibleSize.width*0.16f,_visibleSize.height*0.44f));
            break;
        case TOOL_TYPE_GET_MESS:
            _noteHelp = Help::createHelp(HELP_NOTE_5, TOOL_TYPE_GET_MESS);
            _noteHelp->setPosition(Point(_visibleSize.width*0.16f,_visibleSize.height*0.44f));
            break;
        case TOOL_TYPE_CATCH_BUG:
            _noteHelp = Help::createHelp(HELP_NOTE_6, TOOL_TYPE_CATCH_BUG);
            _noteHelp->setPosition(Point(_visibleSize.width*0.16f,_visibleSize.height*0.44f));
            break;
        case TOOL_TYPE_GET_WATER:
            _noteHelp = Help::createHelp(HELP_NOTE_7, TOOL_TYPE_GET_WATER);
            _noteHelp->setPosition(Point(_visibleSize.width*0.16f,_visibleSize.height*0.44f));
            break;
            
        default:
            break;
    }
    
    _noteHelp->setScale(2.0f);
    (this->getParent())->addChild(_noteHelp,15);
    _noteHelp->setVisible(false);
}

#pragma mark - TOUCH HANDLE

void Tool::addEvents(){
    auto listener = cocos2d::EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(false);
    
    listener->onTouchBegan = [&](cocos2d::Touch* touch, cocos2d::Event* event)
    {
        cocos2d::Vec2 p = touch->getLocation();
        cocos2d::Rect rect = this->getBoundingBox();
        
        if(rect.containsPoint(p))
        {
            return true; // to indicate that we have consumed it.
        }
        
        return false; // we did not consume this event, pass thru.
    };
    
    listener->onTouchEnded = [=](cocos2d::Touch* touch, cocos2d::Event* event)
    {
        Tool::touchEvent(touch);
    };
    
    cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

void Tool::touchEvent(cocos2d::Touch* touch){
    cocos2d::Vec2 p = touch->getLocation();
//    CCLOG("%f - %f",touch->getLocation().x,touch->getLocation().y);
    auto actionMove = MoveTo::create(0.7f, _savePositionOriginal);
    if(_isTouch && _typeTool != 2){
        _isTouch = false;
        _noteHelp->setVisible(false);
        _patient->setMouthNormal();
        _patient->setEyeBrowNormal();

        this->runAction(actionMove);
        
        if(_typeTool == TOOL_TYPE_CATCH_BUG){
            //Small table
            ((GamePlay*)(this->getParent()))->_smallTable->_startMove = true;
            ((GamePlay*)(this->getParent()))->_smallTable->_isTouch = false;
        }
    }
}

void Tool::setTouchDotPosition (Vec2 vec)
{
    //Keep ear
    if (_typeTool == 1){
        if(vec.x < -_visibleSize.width*0.2f){
            vec.x = -_visibleSize.width*0.2f;
        }
        if (vec.x > _visibleSize.width*0.32f) {
            vec.x = _visibleSize.width*0.32f;
        }
        if (vec.y < _visibleSize.height*0.3f) {
            vec.y = _visibleSize.height*0.3f;
        }
        if (vec.y > _visibleSize.height*0.8f ) {
            vec.y = _visibleSize.height*0.8f;
        }
        this->setPosition (vec);
    }else{
        this->setPosition (vec);
    }
}

//Flash light
void Tool::turnOnFlashLight(Ref *pSender){
    if(!_isTurnOnFlashLight){
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(SOUND_MAIN_BUTTON);
        
        auto normalImg = Sprite::create(TOOL_DENPIN_BUTTON_ON);
        
        ((MenuItemImage*)(pSender))->setNormalSpriteFrame(SpriteFrame::create(TOOL_DENPIN_BUTTON_ON, Rect(0, 0, normalImg->getContentSize().width, normalImg->getContentSize().height)));
         ((MenuItemImage*)(pSender))->setSelectedSpriteFrame(SpriteFrame::create(TOOL_DENPIN_BUTTON_ON, Rect(0, 0, normalImg->getContentSize().width, normalImg->getContentSize().height)));
        
        auto light = Sprite::create(TOOL_LIGHT_FLASH);
        light->setPosition(Point(_visibleSize.width*0.34f,_visibleSize.height*0.56f));
        light->setScale(1.7f);
        (this->getParent())->addChild(light,12);
        
        _isTurnOnFlashLight = true;
        _noteHelp->setVisible(false);
        _noteHelp->removeFromParentAndCleanup(true);
        
        ((GamePlay *)(this->getParent()))->addTools();
        ((GamePlay*)(this->getParent()))->showMessesAndBugs();
    }
}