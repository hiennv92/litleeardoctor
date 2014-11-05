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
    _isTouch = false;
    //Use for keepear tool and scissor
    _isSet = false;
    //Use for small table
    _velocityMoveSmallTable = Point(1.0f,0.0f);
    //use for drug water
    _isDropDrugWater = false;
    
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

void Tool::setMuiTen(){
    _muiTen = Help::createHelp(HELP_MUI_TEN, 2);
    _muiTen->setPosition(this->getPositionX() + 110,this->getPositionY() - 20);
    _muiTen->setScale(2.0f);
    _muiTen->setRotation(-45.0f);
    (this->getParent())->addChild(_muiTen,15);
    _muiTen->setVisible(false);
    
    //Set drop drug
    _dropDrug = Sprite::create(TOOL_DRUG_WATER_DROP);
    (this->getParent())->addChild(_dropDrug,15);
    _dropDrug->setScale(2.0f);
    _dropDrug->setVisible(false);
}

void Tool::showMuiTen(){
    _muiTen->setPosition(this->getPositionX() + 110,this->getPositionY() - 40);
    _muiTen->setVisible(true);
}

void Tool::dropDrug(){
    _dropDrug->setPosition(this->getPositionX() - this->getContentSize().width * 2, this->getPositionY() - this->getContentSize().height / 1.9);
    _dropDrug->setVisible(true);
    _dropDrug->setOpacity(255.0f);
    
    _dropDrug->runAction(MoveTo::create(0.25f, Point(_dropDrug->getPositionX(),this->getPositionY() - this->getContentSize().height / 1.1)));
    _dropDrug->runAction(FadeTo::create(0.25f, 0));
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
        case TOOL_TYPE_DESICCATE:
            _noteHelp = Help::createHelp(HELP_NOTE_8, TOOL_TYPE_DESICCATE);
            _noteHelp->setPosition(Point(_visibleSize.width*0.16f,_visibleSize.height*0.44f));
            break;
        case TOOL_TYPE_SPRAY_CHEMICALs:
            _noteHelp = Help::createHelp(HELP_NOTE_9, TOOL_TYPE_SPRAY_CHEMICALs);
            _noteHelp->setPosition(Point(_visibleSize.width*0.16f,_visibleSize.height*0.44f));
            break;
        case TOOL_TYPE_TAM_BONG:
            _noteHelp = Help::createHelp(HELP_NOTE_10, TOOL_TYPE_TAM_BONG);
            _noteHelp->setPosition(Point(_visibleSize.width*0.16f,_visibleSize.height*0.44f));
            break;
        case TOOL_TYPE_WATER_DRUG:
            _noteHelp = Help::createHelp(HELP_NOTE_11, TOOL_TYPE_WATER_DRUG);
            _noteHelp->setPosition(Point(_visibleSize.width*0.16f,_visibleSize.height*0.44f));
            break;
        case TOOL_TYPE_INJECTION:
            _noteHelp = Help::createHelp(HELP_NOTE_12, TOOL_TYPE_INJECTION);
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
    if(_isTouch && _typeTool != 2 && _typeTool){
        _isTouch = false;
        _noteHelp->setVisible(false);
        _patient->setMouthNormal();
        _patient->setEyeBrowNormal();

        if(_typeTool != TOOL_TYPE_WATER_DRUG){
            this->runAction(actionMove);
        }
        
        if(_typeTool == TOOL_TYPE_CATCH_BUG){
            //Small table
            ((GamePlay*)(this->getParent()))->_smallTable->_startMove = true;
            ((GamePlay*)(this->getParent()))->_smallTable->_isTouch = false;
        }
        
        if(_typeTool >= TOOL_TYPE_SCISSOR && _typeTool != TOOL_TYPE_WATER_DRUG){
            if(((GamePlay*)(this->getParent()))->_pageTools == 1){
                ((GamePlay*)(this->getParent()))->_btnNextTools->setVisible(true);
            }else if(((GamePlay*)(this->getParent()))->_pageTools == 2){
                ((GamePlay*)(this->getParent()))->_btnBackTools->setVisible(true);
                ((GamePlay*)(this->getParent()))->_btnNextTools->setVisible(true);
            }
        }
        
        if(_typeTool == TOOL_TYPE_WATER_DRUG && !_isSet){
            this->runAction(RotateTo::create(0.7f, 0.0f));
            this->runAction(actionMove);
            
            if(((GamePlay*)(this->getParent()))->_pageTools == 1){
                ((GamePlay*)(this->getParent()))->_btnNextTools->setVisible(true);
            }else if(((GamePlay*)(this->getParent()))->_pageTools == 2){
                ((GamePlay*)(this->getParent()))->_btnBackTools->setVisible(true);
                ((GamePlay*)(this->getParent()))->_btnNextTools->setVisible(true);
            }
        }else if(_typeTool == TOOL_TYPE_WATER_DRUG && _isSet){
            _isTouch = true;
            _noteHelp->setVisible(true);
        }
    }
    CocosDenshion::SimpleAudioEngine::getInstance()->stopAllEffects();
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
        
        ((GamePlay *)(this->getParent()))->showTools();
        ((GamePlay*)(this->getParent()))->showMessesAndBugs();
    }
}

//Set scissor cut animation
void Tool::setScissorCutAnimation(){
    _isSet = true;

    Vector<SpriteFrame*> animFrames(2);
    char str1[100] = {0};
    char str2[100] = {0};
    
    sprintf(str1, TOOL_SCISSOR_CLOSE);
    auto frame1 = SpriteFrame::create(str1,Rect(0,0,this->getContentSize().width,this->getContentSize().height)); //we assume that the sprites' dimentions are 40*40 rectangles.
    animFrames.pushBack(frame1);
    
    sprintf(str2, TOOL_SCISSOR_OPEN);
    auto frame2 = SpriteFrame::create(str2,Rect(0,0,this->getContentSize().width,this->getContentSize().height)); //we assume that the sprites' dimentions are 40*40 rectangles.
    animFrames.pushBack(frame2);
    
    auto animation = Animation::createWithSpriteFrames(animFrames, 0.15f);
    auto animate = Animate::create(animation);
    
    RepeatForever *repeat = RepeatForever::create(animate);
    this->runAction(repeat);
    
    auto action = CallFunc::create(CC_CALLBACK_0(Tool::setScissorClose,this));
    this->runAction(Sequence::create(DelayTime::create(0.6f),action, NULL));
}

void Tool::setScissorClose(){
    _isSet = false;

    this->stopAllActions();
    char str[100] = {0};
    sprintf(str, TOOL_SCISSOR_CLOSE);
    Texture2D *texture = Director::getInstance()->getTextureCache()->addImage(str);
    this->setTexture(texture);
}
