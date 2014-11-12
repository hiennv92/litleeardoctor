//
//  GamePlay.cpp
//  DoctorEar
//  Created by Trinh Van Duong on 10/30/14.
//

#include "GamePlay.h"
#include "Define.h"
#include "AppDelegate.h"
#include "MainMenu.h"


USING_NS_CC;

Scene* GamePlay::createScene(){
    auto scene = Scene::create();
    auto layer = GamePlay::create();
    scene->addChild(layer);
    return scene;
}

bool GamePlay::init(){
    if( !Layer::init()){
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    //Add background
    if(AppDelegate::_typePatient %2 == 0){
        _background = Sprite::create(GAME_PLAY_BG1);
    }else{
        _background = Sprite::create(GAME_PLAY_BG2);
    }
    
    _background->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
    _background->setScale(visibleSize.width/_background->getContentSize().width, visibleSize.height/_background->getContentSize().height);
    this->addChild(_background,0);
    
    //The secondbackground
    _earHoleScale = Sprite::create(EARHOLE_SCALE);
    _earHoleScale->setPosition(Vec2(visibleSize.width/2,visibleSize.height/2));
    _earHoleScale->setVisible(false);
    _earHoleScale->setScale(visibleSize.width/_earHoleScale->getContentSize().width, visibleSize.height/_earHoleScale->getContentSize().height);
    this->addChild(_earHoleScale,7);
    
    _darkBackground = Sprite::create("all/blackFont2.png");
    _darkBackground->setPosition(Vec2(visibleSize.width/2,visibleSize.height/2));
    _darkBackground->setVisible(false);
    _darkBackground->setScale(visibleSize.width/_darkBackground->getContentSize().width, visibleSize.height/_darkBackground->getContentSize().height);
    this->addChild(_darkBackground,16);
    _darkBackground->setOpacity(100);
    
    spriteCircle = Sprite::create(TOOL_VONG_TRON);
    spriteCircle->setPosition(_background->getPosition());
    spriteCircle->setVisible(false);
    spriteCircle->setScale(2.0f);
    this->addChild(spriteCircle,16);
    
    _backgroundBlackFont = Sprite::create(BLACK_BACKGROUND);
    _backgroundBlackFont->setPosition(Vec2(visibleSize.width/2,visibleSize.height/2));
    _backgroundBlackFont->setVisible(false);
    _backgroundBlackFont->setScale(2.0f);
    this->addChild(_backgroundBlackFont,15);
    
    _spriteTable = Sprite::create(GAME_PLAY_TABLE);
    _spriteTable->setPosition(visibleSize.width/2,0);
    _spriteTable->setScale(visibleSize.width/_spriteTable->getContentSize().width,1.5);
    this->addChild(_spriteTable,14);
    
    //Init patient
    switch (AppDelegate::_typePatient) {
        case 1:
            _patient = Patient::createPatient(PATIENT_BODY1, AppDelegate::_typePatient);
            break;
        case 2:
            _patient = Patient::createPatient(PATIENT_BODY2, AppDelegate::_typePatient);
            break;
        case 3:
            _patient = Patient::createPatient(PATIENT_BODY3, AppDelegate::_typePatient);
            break;
        case 4:
            _patient = Patient::createPatient(PATIENT_BODY4, AppDelegate::_typePatient);
            break;
        case 5:
            _patient = Patient::createPatient(PATIENT_BODY5, AppDelegate::_typePatient);
            break;
        case 6:
            _patient = Patient::createPatient(PATIENT_BODY6, AppDelegate::_typePatient);
            break;
        default:
            break;
    }
    _patient->setPosition(_background->getContentSize().width/2,_background->getContentSize().height/2);
    _background->addChild(_patient,1);
    _patient->createSmallEye();
    _patient->createEarHole();
    
    //Add touch
    listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    
    listener->onTouchBegan = CC_CALLBACK_2(GamePlay::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(GamePlay::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(GamePlay::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    
    //Add Tool keep ear
    _keepEar = Tool::createTool(TOOL_KEEP_EAR_3, 1);
    _keepEar->setPosition(Point(visibleSize.width*0,visibleSize.height*0.3f));
    _keepEar->setScale(1.3f);
    _keepEar->_patient = _patient;
    _keepEar->_savePositionOriginal = _keepEar->getPosition();
    this->addChild(_keepEar,15);
    _keepEar->setHandUp();
    _keepEar->setUpNoteHelp();
    
    //Add Ong soi
    _ongSoi = Tool::createTool(TOOL_ONG_SOI, TOOL_TYPE_ONG_SOI);
    _ongSoi->setPosition(Point(visibleSize.width*1.1,visibleSize.height*0.4f));
    _ongSoi->setScale(2.0f);
    _ongSoi->_patient = _patient;
    _ongSoi->_savePositionOriginal = _ongSoi->getPosition();
    this->addChild(_ongSoi,15);
    _ongSoi->setUpNoteHelp();
    _ongSoi->setVisible(false);
    
    //Add Joystick
    _joystickBase = Tool::createTool(JOYSTICK_BASE,300);
    _joystickButton =Tool::createTool(JOYSTICK_BUTTON,200);
    _joystickButton->setVisible(false);
    _joystickBase->setVisible(false);
    _joystickBase->setPosition(visibleSize.width * 0.9f, visibleSize.height * 0.85f); // set the initial position
    _joystickButton->setPosition(visibleSize.width * 0.9f, visibleSize.height * 0.85f); // set the initial position
    _joystickButton->_savePositionOriginal = _joystickButton->getPosition();
    _joystickButton->setScale(2.0f);
    _joystickBase->setScale(2.0f);
    _joystickButton->_toolOngSoi = _backgroundBlackFont;
    _joystickButton->_circle = spriteCircle;
    _joystickButton->_circle = spriteCircle;
    this->addChild(_joystickBase,16);
    this->addChild(_joystickButton,16);
    
    //Add messes and bugs
    this->addTools();
    this->addMessesAndBugs();
    
    //Add button
    //Add buttons
    _btnNextTools   = MenuItemImage::create(GAME_PLAY_BTN_NEXT_TOOLS_NORMAL,GAME_PLAY_BTN_NEXT_TOOLS_SELECTED, CC_CALLBACK_1(GamePlay::nextToolsSelected, this));
    _btnBackTools   = MenuItemImage::create(GAME_PLAY_BTN_BACK_TOOLS_NORMAL,GAME_PLAY_BTN_BACK_TOOLS_SELECTED, CC_CALLBACK_1(GamePlay::backToolsSelected, this));
    _stopAdvanceLevelButton = MenuItemImage::create(GAME_PLAY_BTN_STOP_ADVANCE_LEVEL,GAME_PLAY_BTN_STOP_ADVANCE_LEVEL_SELECTED, CC_CALLBACK_1(GamePlay::stopAdvanceLevel,this));
    _drawButton = MenuItemImage::create(GAME_PLAY_BTN_DRAW,GAME_PLAY_BTN_DRAW_SELECTED, CC_CALLBACK_1(GamePlay::drawImage,this));
    _homeButton = MenuItemImage::create(GAME_PLAY_BTN_BACK_HOME,GAME_PLAY_BTN_BACK_HOME_SELECTED, CC_CALLBACK_1(GamePlay::backHome,this));
    _saveButton = MenuItemImage::create(GAME_PLAY_BTN_SAVE,GAME_PLAY_BTN_SAVE_SELECTED, CC_CALLBACK_1(GamePlay::saveImage,this));
    _mailButton = MenuItemImage::create(GAME_PLAY_BTN_MAIL,GAME_PLAY_BTN_MAIL_SELECTED, CC_CALLBACK_1(GamePlay::email,this));
//    _faceButton = MenuItemImage::create(GAME_PLAY_BTN_FB,GAME_PLAY_BTN_FB_SELECTED, CC_CALLBACK_1(GamePlay::facebook,this));
    
    _btnNextTools->setPosition(visibleSize.width*0.9f,visibleSize.height*0.85f);
    _btnBackTools->setPosition(visibleSize.width*0.1f,visibleSize.height*0.85f);
    _stopAdvanceLevelButton->setPosition(_btnBackTools->getPosition());
    _drawButton->setPosition(visibleSize.width*0.15f,visibleSize.height*0.8f);
    _drawButton->setVisible(false);
    _homeButton->setPosition(visibleSize.width*0.17f,visibleSize.height*0.07f);
    _homeButton->setVisible(false);
    _saveButton->setPosition(visibleSize.width*0.39f,visibleSize.height*0.07f);
    _saveButton->setVisible(false);
    _mailButton->setPosition(visibleSize.width*0.61f,visibleSize.height*0.07f);
    _mailButton->setVisible(false);
//    _faceButton->setPosition(visibleSize.width*0.83f,visibleSize.height*0.07f);
//    _faceButton->setVisible(false);
    
    _btnNextTools->setScale(2.0f);
    _btnBackTools->setScale(2.0f);
    _drawButton->setScale(1.8f);
    _homeButton->setScale(1.8f);
    _saveButton->setScale(1.8f);
    _mailButton->setScale(1.8f);
//    _faceButton->setScale(1.8f);
    _stopAdvanceLevelButton->setScale(2.0f);

    _btnBackTools->setVisible(false);
    _btnNextTools->setVisible(false);
    _stopAdvanceLevelButton->setVisible(false);
    _pageTools = 1;

    //Menu
    auto menu = Menu::create(_btnNextTools,_btnBackTools,_stopAdvanceLevelButton,_drawButton,_homeButton,_saveButton,_mailButton, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu,18);
    
    //Draw
    setTouchMode(Touch::DispatchMode::ALL_AT_ONCE);
    setTouchEnabled( true );

    _isStartDraw = false;
    return true;
}

#pragma mark - TOUCH HANDLE

bool GamePlay::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event){
    cocos2d::Vec2 p = touch->getLocation();
    if (!_isStartDraw) {
        _patient->setPositionSmallEye(p);
        
        if(!_keepEar->_isSet){
            cocos2d::Rect rect = _keepEar->getBoundingBox();
            if(rect.containsPoint(p)){
                if(_keepEar->_handHelp->isVisible()){
                    _keepEar->_handHelp->stopAllActions();
                    _keepEar->_handHelp->setVisible(false);
                }
                _keepEar->_noteHelp->showHelp(0.0f);
                _keepEar->_isTouch = true;
                return true;
            }
        }
        
        if(!_ongSoi->_isSet){
            if (_ongSoi->isVisible()) {
                cocos2d::Rect rect = _ongSoi->getBoundingBox();
                if(rect.containsPoint(p)){
                    _ongSoi->_noteHelp->showHelp(0.0f);
                    _ongSoi->_isTouch = true;
                    _btnBackTools->setVisible(false);
                    _btnNextTools->setVisible(false);
                    return true;
                }
            }
        }
        
        if(_scissor){
            if(_scissor->isVisible()){
                cocos2d::Rect rectScissor = _scissor->getBoundingBox();
                if(rectScissor.containsPoint(p)){
                    _btnBackTools->setVisible(false);
                    _btnNextTools->setVisible(false);
                    
                    _scissor->_isTouch = true;
                    _scissor->_noteHelp->showHelp(0.0f);
                    _patient->setMouthScare();
                    _patient->setEyeBrowScare();
                    
                    _longTai1->_tool = _scissor;
                    _longTai2->_tool = _scissor;
                    _longTai3->_tool = _scissor;
                    _longTai4->_tool = _scissor;
                    _longTai5->_tool = _scissor;
                    _longTai6->_tool = _scissor;
                    
                    return true;
                }
            }
        }
        
        if(_shakeEar){
            if (_shakeEar->isVisible()) {
                cocos2d::Rect rectScissor = _shakeEar->getBoundingBox();
                if(rectScissor.containsPoint(p)){
                    _btnBackTools->setVisible(false);
                    _btnNextTools->setVisible(false);
                    
                    _shakeEar->_isTouch = true;
                    _shakeEar->_noteHelp->showHelp(0.0f);
                    _shakeEar->setShakeEarAnimation();
                    _patient->setMouthScare();
                    _patient->setEyeBrowScare();
                    
                    _longTai1->_tool = _shakeEar;
                    _longTai2->_tool = _shakeEar;
                    _longTai3->_tool = _shakeEar;
                    _longTai4->_tool = _shakeEar;
                    _longTai5->_tool = _shakeEar;
                    _longTai6->_tool = _shakeEar;
                    
                    return true;
                }
                
            }
        }
        
        if(_getMess){
            if (_getWater->isVisible()) {
                cocos2d::Rect rectScissor = _getMess->getBoundingBox();
                if(rectScissor.containsPoint(p)){
                    _btnBackTools->setVisible(false);
                    _btnNextTools->setVisible(false);
                    
                    _getMess->_isTouch = true;
                    _getMess->_noteHelp->showHelp(0.0f);
                    _patient->setMouthScare();
                    _patient->setEyeBrowScare();
                    
                    return true;
                }
            }
        }
        
        if(_catchBug){
            if (_catchBug->isVisible()) {
                cocos2d::Rect rectScissor = _catchBug->getBoundingBox();
                if(rectScissor.containsPoint(p)){
                    _btnBackTools->setVisible(false);
                    _btnNextTools->setVisible(false);
                    
                    _catchBug->_isTouch = true;
                    _catchBug->_noteHelp->showHelp(0.0f);
                    _patient->setMouthScare();
                    _patient->setEyeBrowScare();
                    
                    _bug1->_tool = _catchBug;
                    _bug2->_tool = _catchBug;
                    _bug3->_tool = _catchBug;
                    _bug4->_tool = _catchBug;
                    
                    //Call small table
                    _smallTable->_startMove = true;
                    _smallTable->_isTouch = true;
                    return true;
                }
            }
        }
        
        if(_getWater){
            if (_getWater->isVisible()) {
                cocos2d::Rect rectScissor = _getWater->getBoundingBox();
                if(rectScissor.containsPoint(p)){
                    _btnBackTools->setVisible(false);
                    _btnNextTools->setVisible(false);
                    
                    _getWater->_isTouch = true;
                    _getWater->_noteHelp->showHelp(0.0f);
                    _patient->setMouthScare();
                    _patient->setEyeBrowScare();
                    
                    return true;
                }
            }
        }
        
        if(_desiccate){
            if (_desiccate->isVisible()) {
                cocos2d::Rect rectScissor = _desiccate->getBoundingBox();
                if(rectScissor.containsPoint(p)){
                    _btnBackTools->setVisible(false);
                    _btnNextTools->setVisible(false);
                    
                    _desiccate->_isTouch = true;
                    _desiccate->_noteHelp->showHelp(0.0f);
                    _patient->setMouthSmile();
                    _patient->setEyeBrowSmile();
                    
                    return true;
                }
            }
        }
        
        if(_sprayChemicals){
            if (_sprayChemicals->isVisible()) {
                cocos2d::Rect rectScissor = _sprayChemicals->getBoundingBox();
                if(rectScissor.containsPoint(p)){
                    _btnBackTools->setVisible(false);
                    _btnNextTools->setVisible(false);
                    
                    _sprayChemicals->_isTouch = true;
                    _sprayChemicals->_noteHelp->showHelp(0.0f);
                    _patient->setMouthSmile();
                    _patient->setEyeBrowSmile();
                    
                    _bug1->_tool = _sprayChemicals;
                    _bug2->_tool = _sprayChemicals;
                    _bug3->_tool = _sprayChemicals;
                    _bug4->_tool = _sprayChemicals;
                    
                    return true;
                }
            }
        }
        
        if(_tamPon){
            if (_tamPon->isVisible()) {
                cocos2d::Rect rectScissor = _tamPon->getBoundingBox();
                if(rectScissor.containsPoint(p)){
                    _btnBackTools->setVisible(false);
                    _btnNextTools->setVisible(false);
                    
                    _tamPon->_isTouch = true;
                    _tamPon->_noteHelp->showHelp(0.0f);
                    _patient->setMouthSmile();
                    _patient->setEyeBrowSmile();
                    
                    return true;
                }
            }
        }
        
        if(_drugWater){
            if (_drugWater->isVisible()) {
                cocos2d::Rect rectScissor = _drugWater->getBoundingBox();
                if(rectScissor.containsPoint(p)){
                    if(!_drugWater->_isSet){
                        _btnBackTools->setVisible(false);
                        _btnNextTools->setVisible(false);
                        
                        _drugWater->_isTouch = true;
                        _drugWater->_noteHelp->showHelp(0.0f);
                        _patient->setMouthSmile();
                        _patient->setEyeBrowSmile();
                        _drugWater->setRotation(65.0f);
                        //                    _drugWater->_isMoved = true;
                        _drugWater->setPosition(_drugWater->getPositionX(),_drugWater->getPositionY()+ 30);
                        return true;
                    }
                    else{
                        
                        CCLOG("Nho thuoc");
                        _drugWater->_isDropDrugWater = true;
                        _drugWater->dropDrug();
                    }
                }
            }
        }
        
        if(_injection){
            if (_injection->isVisible()) {
                cocos2d::Rect rectScissor = _injection->getBoundingBox();
                if(rectScissor.containsPoint(p)){
                    if(!_injection->_isSet){
                        _btnBackTools->setVisible(false);
                        _btnNextTools->setVisible(false);
                        
                        _injection->_isTouch = true;
                        _injection->_noteHelp->showHelp(0.0f);
                        _patient->setMouthScare();
                        _patient->setEyeBrowScare();
                        
                        return true;
                    }
                    else if(_injection->_isSet && _injection->_isMoved){
                        CCLOG("Hut mu");
                        _injection->_isDropDrugWater = true;
                        _injection->setInjectionFull();
                    }
                }
            }
        }
        
        if(_joystickButton){
            if (_joystickButton->isVisible()) {
                cocos2d::Rect rectScissor = _joystickButton->getBoundingBox();
                if(rectScissor.containsPoint(p)){
                    _joystickButton->_isTouch = true;
                    CCLOG("TOUCH JOYSTICK");
                    return true;
                }
            }
        }
        
        if(_lazer){
            if (_lazer->isVisible()) {
                cocos2d::Rect rectScissor = _lazer->getBoundingBox();
                if(rectScissor.containsPoint(p)){
                    if(UserDefault::getInstance()->getBoolForKey(SOUND_ON_OFF)){
                        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(SOUND_LAZER, true);
                    }
                    
                    _lazer->_isTouch = true;
                    _lazer->_noteHelp->showHelp(0.0f);
                    _lazer->_lazer->setVisible(true);
                    return true;
                }
            }
        }
        
        if(_catchBugAdvance){
            if (_catchBugAdvance->isVisible()) {
                cocos2d::Rect rectScissor = _catchBugAdvance->getBoundingBox();
                if(rectScissor.containsPoint(p)){
                    _catchBugAdvance->_isTouch = true;
                    _catchBugAdvance->_noteHelp->showHelp(0.0f);
                    return true;
                }
            }
        }
        
        if(_tamponAdvance){
            if (_tamponAdvance->isVisible()) {
                cocos2d::Rect rectScissor = _tamponAdvance->getBoundingBox();
                if(rectScissor.containsPoint(p)){
                    _tamponAdvance->_isTouch = true;
                    _tamponAdvance->_noteHelp->showHelp(0.0f);
                    
                    _dirtyWater1->_tool = _tamponAdvance;
                    _dirtyWater2->_tool = _tamponAdvance;
                    _dirtyWater3->_tool = _tamponAdvance;
                    _dirtyWater4->_tool = _tamponAdvance;
                    
                    return true;
                }
            }
        }
        
        if(_bottleGel){
            if (_bottleGel->isVisible()) {
                cocos2d::Rect rectScissor = _bottleGel->getBoundingBox();
                if(rectScissor.containsPoint(p)){
                    _bottleGel->_isTouch = true;
                    _bottleGel->_noteHelp->showHelp(0.0f);
                    return true;
                }
            }
        }
        
        if(_getWaterAdvance){
            if (_getWaterAdvance->isVisible()) {
                cocos2d::Rect rectScissor = _getWaterAdvance->getBoundingBox();
                if(rectScissor.containsPoint(p)){
                    _getWaterAdvance->_isTouch = true;
                    _getWaterAdvance->_noteHelp->showHelp(0.0f);
                    
                    _dirtyWater1->_tool = _getWaterAdvance;
                    _dirtyWater2->_tool = _getWaterAdvance;
                    _dirtyWater3->_tool = _getWaterAdvance;
                    _dirtyWater4->_tool = _getWaterAdvance;
                    
                    return true;
                }
            }
        }
    }
    return true;
}

void GamePlay::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event){
    if (!_isStartDraw){
        cocos2d::Vec2 p = touch->getLocation();
        cocos2d::Vec2 locationInNode = this ->convertToNodeSpace (touch->getLocation());
        
        //Set smalleye move
        _patient->setPositionSmallEye(p);
        
        //Check keepEar
        if(!_keepEar->_isSet){
            Rect pKeepEar = _keepEar->getBoundingBox();
            Rect rect =  Rect(pKeepEar.origin.x + pKeepEar.size.width*3/4,pKeepEar.origin.y,pKeepEar.size.width/5, pKeepEar.size.height*2/3);
            
            if(_keepEar->_isTouch)
            {
                _keepEar->setTouchDotPosition (_keepEar-> getPosition () + touch-> getDelta ());
                if (rect.containsPoint(_patient->_earHole->getPosition())) {
                    CCLOG("Correct");
                    _keepEar->runAction(MoveTo::create(0.4f, Point(_keepEar->_visibleSize.width*0.15f,_keepEar->_visibleSize.height*0.61f)));
                    _keepEar->_isTouch = false;
                    _keepEar->_isSet = true;
                    
                    //Setup handHorizontal
                    _keepEar->_handHelp->setVisible(true);
                    _keepEar->_handHelp->_typeHand = 2;
                    _keepEar->_handHelp->setPosition(Point(_keepEar->_visibleSize.width*0.25f,_keepEar->_visibleSize.height*0.7f));
                    _keepEar->_handHelp->_savePosition = _keepEar->_handHelp->getPosition();
                    _keepEar->_handHelp->moveHandHorizontal();
                    
                    //Add slider bar
                    this->addSliderBar();
                }
                return;
            }
        }
        
        if(!_ongSoi->_isSet){
            Rect pKeepEar = _ongSoi->getBoundingBox();
            Rect rect =  Rect(pKeepEar.origin.x,pKeepEar.origin.y,pKeepEar.size.width/6, pKeepEar.size.height/6);
            
            if(_ongSoi->_isTouch)
            {
                _ongSoi->setTouchDotPosition (_ongSoi-> getPosition () + touch-> getDelta ());
                if (_patient->_earHole->getBoundingBox().containsPoint(Point(rect.origin.x + rect.size.width/2 , rect.origin.y + rect.size.height/2))) {
                    _ongSoi->runAction(MoveTo::create(0.4f,Point(_ongSoi->_visibleSize.width*0.8f,_ongSoi->_visibleSize.height*0.74f)));
                    _ongSoi->_isTouch = false;
                    _ongSoi->_isSet = true;
                    
                    auto action = CallFunc::create(CC_CALLBACK_0(GamePlay::setupAdvanceLevel,this));
                    this->runAction(Sequence::create(DelayTime::create(1.0f),action, NULL));
                }
                return;
            }
        }
        
        //Move scissor
        if(_scissor){
            if(_scissor->_isTouch){
                _scissor->setTouchDotPosition (_scissor-> getPosition () + touch-> getDelta ());
                return;
            }
        }
        
        //Move shake ear
        if(_shakeEar){
            if(_shakeEar->_isTouch){
                _shakeEar->setTouchDotPosition (_shakeEar-> getPosition () + touch-> getDelta ());
                return;
            }
        }
        
        //Move get mess
        if(_getMess){
            if(_getMess->_isTouch){
                _getMess->setTouchDotPosition (_getMess-> getPosition () + touch-> getDelta ());
                return;
            }
        }
        
        //Move catch bug
        if(_catchBug){
            if(_catchBug->_isTouch){
                _catchBug->setTouchDotPosition (_catchBug-> getPosition () + touch-> getDelta ());
                return;
            }
        }
        
        //Move get water
        if(_getWater){
            if(_getWater->_isTouch){
                _getWater->setTouchDotPosition (_getWater-> getPosition () + touch-> getDelta ());
                return;
            }
        }
        
        //Move desiccate
        if(_desiccate){
            if(_desiccate->_isTouch){
                _desiccate->setTouchDotPosition (_desiccate-> getPosition () + touch-> getDelta ());
                return;
            }
        }
        
        //Move spray chemical
        if(_sprayChemicals){
            if(_sprayChemicals->_isTouch){
                _sprayChemicals->setTouchDotPosition (_sprayChemicals-> getPosition () + touch-> getDelta ());
                return;
            }
        }
        
        //Move tam bong
        if(_tamPon){
            if(_tamPon->_isTouch){
                _tamPon->setTouchDotPosition (_tamPon-> getPosition () + touch-> getDelta ());
                return;
            }
        }
        
        //Move water drug
        if(_drugWater){
            if(_drugWater->_isTouch && !_drugWater->_isSet){
                _drugWater->setTouchDotPosition (_drugWater-> getPosition () + touch-> getDelta ());
                return;
            }
        }
        
        //Move injection
        if(_injection){
            if(_injection->_isTouch && !_injection->_isSet){
                _injection->setTouchDotPosition (_injection-> getPosition () + touch-> getDelta ());
                return;
            }
        }
        
        if(_joystickButton){
            if(_joystickButton->_isTouch){
                _joystickButton->setTouchDotPosition(_joystickButton->getPosition() + touch->getDelta());
                return;
            }
        }
        
        if(_lazer){
            if(_lazer->_isTouch){
                _lazer->setTouchDotPosition (_lazer-> getPosition () + touch-> getDelta ());
                return;
            }
        }
        
        if(_catchBugAdvance){
            if(_catchBugAdvance->_isTouch){
                _catchBugAdvance->setTouchDotPosition (_catchBugAdvance-> getPosition () + touch->getDelta ());
                return;
            }
        }
        
        if(_tamponAdvance){
            if(_tamponAdvance->_isTouch){
                _tamponAdvance->_isMoveClean = true;
                _tamponAdvance->setTouchDotPosition (_tamponAdvance-> getPosition () + touch->getDelta ());
                return;
            }
        }
        
        if(_bottleGel){
            if(_bottleGel->_isTouch){
                _bottleGel->setTouchDotPosition (_bottleGel-> getPosition () + touch-> getDelta ());
                return;
            }
        }
        
        if(_getWaterAdvance){
            if(_getWaterAdvance->_isTouch){
                _getWaterAdvance->setTouchDotPosition (_getWaterAdvance-> getPosition () + touch->getDelta ());
                return;
            }
        }
    }
}

void GamePlay::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event){
    if(_isStartDraw){
        //    CCLOG("TOUCH ENDED");
        _patient->_isSetCloseEye = true;
    }
}

void GamePlay::onTouchesBegan(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *unused_event) {
    if(_isStartDraw && !_isShowBoard && !usingBrush){
        std::vector<Touch *>::const_iterator it = touches.begin();
        Touch* touch;
        Point tap;
        
        for (int i =0; i<touches.size(); i++) {
            touch = (Touch*)(*it);
            if(touch){
                tap = touch->getLocation();
                canvas->begin();
                stempBrush->setPosition(tap);
                stempBrush->visit();
                canvas->end();
                Director::getInstance()->getRenderer()->render();
            }
            it++;
        }
    }
}

void GamePlay::onTouchesMoved(const std::vector<Touch *> &touches, cocos2d::Event *unused_event)
{
    if(_isStartDraw && !_isShowBoard && usingBrush){
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
        
        
        if (isEraser) {
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
                }
            }
            
            for (int i = 0; i < int(distance); i++)
            {
                float delta = float(i) / distance;
                Point pos(start.x + (diffX * delta), start.y + (diffY * delta));
                if (isEraser) {
                    shadowCanvas->begin();
                    eraser->setPosition(pos);
                    eraser->visit();
                    shadowCanvas->end();
                    Director::getInstance()->getRenderer()->render();
                }
            }
            return;
        }
        
        if (brushIndexTab == 2) {
            for (int i = 0; i < distance; i++) {
                float delta = float(i) / distance;
                Point pos(start.x + (diffX * delta), start.y + (diffY * delta));

                shadowCanvas->begin();
                shadow->setPosition(pos);
                shadow->setOpacity(2);
                shadow->visit();
                
                shadowBase->setPosition(pos);
                shadowBase->visit();
                
                shadowCanvas->end();
                Director::getInstance()->getRenderer()->render();
            }
        }
        else if(brushIndexTab == 3){
            for (int i = 0; i < distance; i++) {
                float delta = float(i) / distance;
                Point pos(start.x + (diffX * delta), start.y + (diffY * delta));
                
                shadowCanvas->begin();
                shadowBase->setPosition(pos);
                shadowBase->visit();
                
                shadowCanvas->end();
                Director::getInstance()->getRenderer()->render();
            }
        }
        
        int next = 1;
        if (brushIndexTab == 4) {
            next = int(brush->getContentSize().width * brush->getScale()) / 2;
        }
        
        for (int i = 0; i < int(distance); i+= next)
        {
            float delta = float(i) / distance;
            Point pos(start.x + (diffX * delta), start.y + (diffY * delta));

            if (brushIndexTab == 5) {
                canvas->begin();
                brush->setPosition(pos);
                brush->visit();
                canvas->end();
                Director::getInstance()->getRenderer()->render();
            }
            else{
                canvas->begin();
                brush->setPosition(pos);
                brush->visit();
                canvas->end();
                Director::getInstance()->getRenderer()->render();
            }
        }
    }
}

void GamePlay::onTouchesEnded(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *unused_event) {
    
}

void GamePlay::onTouchesCancelled(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *unused_event) {
    
}

#pragma mark - SLIDER BAR

void GamePlay::addSliderBar(){
    // Add the slider
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(SOUND_MACHINE_KEEP_EAR);
    _isChangeBarSlider = false;
    
    slider = cocos2d::extension::ControlSlider::create(TOOL_SLIDER_BAR_1,TOOL_SLIDER_BAR_1_BUTTON,TOOL_SLIDER_BAR_1_BUTTON);
    slider->setTag(1);
    (slider->getProgressSprite())->setVisible(false);
    slider->setAnchorPoint(Vec2(0.5f, 1.0f));
    slider->setMinimumValue(0.0f); // Sets the min value of range
    slider->setMaximumValue(5.0f); // Sets the max value of range
    slider->setMinimumAllowedValue(1.5f);
    slider->setValue(1.5f);
    slider->setPosition(Vec2(_background->getContentSize().width*0.185f, _background->getContentSize().height*0.9f));
    slider->addTargetWithActionForControlEvents(this, cccontrol_selector(GamePlay::valueChanged), cocos2d::extension::Control::EventType::VALUE_CHANGED);
    _background->addChild(slider,15);
}

void GamePlay::valueChanged(Ref *sender, cocos2d::extension::Control::EventType controlEvent)
{
    cocos2d::extension::ControlSlider *sd = (extension::ControlSlider*)sender;
    
    if (sd->getTag() == 1) {
        if(!_isChangeBarSlider){
            _keepEar->_handHelp->stopAllActions();
            _keepEar->_handHelp->removeFromParentAndCleanup(true);
            
            auto spriteThumb = Sprite::create(TOOL_SLIDER_BAR_1_BUTTON);
            slider->setEnabled(false);
            (slider->getThumbSprite())->removeFromParentAndCleanup(true);
            
            slider->addChild(spriteThumb);
            spriteThumb->setPosition(slider->getContentSize().width*0.4f,slider->getContentSize().height*0.5f);
            auto action = MoveTo::create(0.5f, Point(slider->getContentSize().width*0.7f,spriteThumb->getPosition().y));
            spriteThumb->runAction(action);
            
            auto actionFlash = CallFunc::create( CC_CALLBACK_0(GamePlay::addFlashLight,this));
            
            _patient->_earHole->runAction(Sequence::create(ScaleTo::create(0.5f, 2.0f),NULL));
            _patient->_earHole->runAction(Sequence::create(MoveTo::create(0.5f, Point(_patient->_earHole->getPosition().x - 15,_patient->_earHole->getPosition().y + 14)), NULL));
            
            _keepEar->runAction(Sequence::create(ScaleTo::create(0.5f, 2.0f),NULL));
            _keepEar->runAction(Sequence::create(MoveTo::create(0.5f, Point(_keepEar->_visibleSize.width*0.0f,_keepEar->_visibleSize.height*0.63f)),actionFlash,NULL));
            
            slider->runAction(Sequence::create(DelayTime::create(0.6f),MoveTo::create(0.5f,Point(-slider->getPosition().x,slider->getPosition().y)), NULL));
            
            if(UserDefault::getInstance()->getBoolForKey(SOUND_ON_OFF)){
                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(SOUND_MACHINE_KEEP_EAR);
            }
            
            _isChangeBarSlider = true;
        }
    }
    else if(sd->getTag() == 2){
        _bgBoardBrushes->getChildByTag(-500)->setScale(pSlider->getValue());

        brush->setScale(pSlider->getValue());
        shadowBase->setScale(pSlider->getValue()*2.0);
        shadow->setScale(pSlider->getValue()*6.0f);
    }
}

#pragma mark - FLASH LIGHT
void GamePlay::addFlashLight(){
    _keepEar->_noteHelp->removeFromParentAndCleanup(true);
    slider->removeFromParentAndCleanup(true);
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    _flashLight = Tool::createTool(TOOL_DENPIN, 2);
    _flashLight->setPosition(Point(visibleSize.width*1.3f,visibleSize.height*0.38f));
    _flashLight->_patient = _patient;
    _flashLight->setScale(2.0f);
    this->addChild(_flashLight,13);
    _flashLight->setUpNoteHelp();
    _flashLight->runAction(MoveTo::create(1.0f, Point(visibleSize.width*0.62f,visibleSize.height*0.37f)));
    _flashLight->_noteHelp->showHelp(1.1f);
}

#pragma mark - INIT TOOLS
void GamePlay::addTools(){
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    _scissor = Tool::createTool(TOOL_SCISSOR_CLOSE, TOOL_TYPE_SCISSOR);
    this->addChild(_scissor,15);
    _scissor->setPosition(Point(visibleSize.width*0.12f, -visibleSize.height*0.13f));
    _scissor->setScale(2.0f);
    _scissor->_patient = _patient;
    _scissor->_savePositionOriginal = Point(visibleSize.width*0.12f, visibleSize.height*0.13f);
    _scissor->setUpNoteHelp();

    _shakeEar = Tool::createTool(TOOL_SHAKE_EAR, TOOL_TYPE_SHAKE_EAR);
    this->addChild(_shakeEar,15);
    _shakeEar->setPosition(Point(visibleSize.width*0.33f, -visibleSize.height*0.13f));
    _shakeEar->cocos2d::Node::setScale(2.0f);
    _shakeEar->_savePositionOriginal = Point(visibleSize.width*0.33f, visibleSize.height*0.13f);
    _shakeEar->_patient = _patient;
    _shakeEar->setUpNoteHelp();
    
    _getMess = Tool::createTool(TOOL_GET_MESS, TOOL_TYPE_GET_MESS);
    this->addChild(_getMess,15);
    _getMess->setPosition(Point(visibleSize.width*0.5f, -visibleSize.height*0.13f));
    _getMess->cocos2d::Node::setScale(2.0f);
    _getMess->_savePositionOriginal = Point(visibleSize.width*0.5f, visibleSize.height*0.13f);
    _getMess->_patient = _patient;
    _getMess->setUpNoteHelp();
    
    _catchBug = Tool::createTool(TOOL_CATCH_BUG, TOOL_TYPE_CATCH_BUG);
    this->addChild(_catchBug,15);
    _catchBug->setPosition(Point(visibleSize.width*0.66f, -visibleSize.height*0.13f));
    _catchBug->cocos2d::Node::setScale(2.0f);
    _catchBug->_savePositionOriginal = Point(visibleSize.width*0.66f, visibleSize.height*0.13f);
    _catchBug->_patient = _patient;
    _catchBug->setUpNoteHelp();

    _smallTable = Tool::createTool(TOOL_SMALL_TABLE, 100);
    this->addChild(_smallTable,14);
    _smallTable->setScale(1.1f,1.6f);
    _smallTable->_toolCatchBug = _catchBug;
    _smallTable->setPosition(-visibleSize.width*0.5f, visibleSize.height*0.3f);
    
    _getWater = Tool::createTool(TOOL_GET_WATER, TOOL_TYPE_GET_WATER);
    this->addChild(_getWater,15);
    _getWater->setPosition(Point(visibleSize.width*0.91f, -visibleSize.height*0.5f));
    _getWater->cocos2d::Node::setScale(2.0f);
    _getWater->_savePositionOriginal = Point(visibleSize.width*0.91f, -visibleSize.height*0.3f);
    _getWater->_patient = _patient;
    _getWater->setUpNoteHelp();
    
    //Page tools 2
    float widthTmp = visibleSize.width*1.2f;
    
    _desiccate = Tool::createTool(TOOL_DESICATE, TOOL_TYPE_DESICCATE);
    this->addChild(_desiccate,15);
    _desiccate->setPosition(Point(visibleSize.width*0.05f + widthTmp, -visibleSize.height*0.5f));
    _scissor->setScale(2.0f);
    _desiccate->cocos2d::Node::setScale(2.0f);
    _desiccate->_savePositionOriginal = Point(visibleSize.width*0.05f, -visibleSize.height*0.27f);
    _desiccate->_patient = _patient;
    _desiccate->setUpNoteHelp();
    
    _sprayChemicals = Tool::createTool(TOOL_SPRAY_CHEMICALS_NORMAL, TOOL_TYPE_SPRAY_CHEMICALs);
    this->addChild(_sprayChemicals,15);
    _sprayChemicals->setPosition(Point(visibleSize.width*0.33f + widthTmp, -visibleSize.height*0.14f));
    _sprayChemicals->cocos2d::Node::setScale(2.0f);
    _sprayChemicals->_savePositionOriginal = Point(visibleSize.width*0.33f, visibleSize.height*0.14f);
    _sprayChemicals->_patient = _patient;
    _sprayChemicals->setUpNoteHelp();
    
    _tamPon = Tool::createTool(TOOL_TAM_BONG, TOOL_TYPE_TAM_BONG);
    this->addChild(_tamPon,15);
    _tamPon->setPosition(Point(visibleSize.width*0.51f + widthTmp, -visibleSize.height*0.13f));
    _tamPon->cocos2d::Node::setScale(2.0f);
    _tamPon->_savePositionOriginal = Point(visibleSize.width*0.51f, visibleSize.height*0.13f);
    _tamPon->_patient = _patient;
    _tamPon->setUpNoteHelp();
    
    _drugWater = Tool::createTool(TOOL_DRUG_WATER_TOOL, TOOL_TYPE_WATER_DRUG);
    this->addChild(_drugWater,15);
    _drugWater->setPosition(Point(visibleSize.width*0.68f + widthTmp, -visibleSize.height*0.13f));
    _drugWater->cocos2d::Node::setScale(2.0f);
    _drugWater->_savePositionOriginal = Point(visibleSize.width*0.69f, visibleSize.height*0.13f);
    _drugWater->_patient = _patient;
    _drugWater->setMuiTen();
    _drugWater->setUpNoteHelp();
    _drugWater->_muiTen->setMuiTenScale();

    _injection = Tool::createTool(TOOL_INJECTION_NORMAL, TOOL_TYPE_INJECTION);
    this->addChild(_injection,15);
    _injection->setPosition(Point(visibleSize.width*0.91f + widthTmp, -visibleSize.height*0.13f));
    _injection->cocos2d::Node::setScale(2.0f);
    _injection->_savePositionOriginal = Point(visibleSize.width*0.91f, visibleSize.height*0.13f);
    _injection->_patient = _patient;
    _injection->setUpNoteHelp();
    _injection->setMuiTen();
    
    //TOOLS ADVANCE
    _lazer = Tool::createTool(TOOL_LAZER, TOOL_TYPE_LAZER);
    this->addChild(_lazer,15);
    _lazer->setPosition(Point(visibleSize.width*0.05f, -visibleSize.height*0.05f));
    _lazer->setScale(2.0f);
    _lazer->_patient = _patient;
    _lazer->_savePositionOriginal = _lazer->getPosition();
    _lazer->setUpNoteHelp();
    
    _catchBugAdvance = Tool::createTool(TOOL_CATCH_BUG_ADVANCE, TOOL_TYPE_CATCH_BUG_ADVANCE);
    this->addChild(_catchBugAdvance,15);
    _catchBugAdvance->setPosition(Point(visibleSize.width*0.33f, -visibleSize.height*0.2f));
    _catchBugAdvance->setScale(2.0f);
    _catchBugAdvance->_patient = _patient;
    _catchBugAdvance->_savePositionOriginal = _catchBugAdvance->getPosition();
    _catchBugAdvance->setUpNoteHelp();
    
    _tamponAdvance = Tool::createTool(TOOL_TAM_BONG, TOOL_TYPE_TAM_BONG_ADVANCE);
    this->addChild(_tamponAdvance,15);
    _tamponAdvance->setPosition(Point(visibleSize.width*0.52f, visibleSize.height*0.13f));
    _tamponAdvance->setScale(2.0f);
    _tamponAdvance->_patient = _patient;
    _tamponAdvance->_savePositionOriginal = _tamponAdvance->getPosition();
    _tamponAdvance->setUpNoteHelp();

    _bottleGel = Tool::createTool(TOOL_BOTTLE_GEL, TOOL_TYPE_GEL);
    this->addChild(_bottleGel,15);
    _bottleGel->setPosition(Point(visibleSize.width*0.7f, visibleSize.height*0.13f));
    _bottleGel->setScale(2.0f);
    _bottleGel->_patient = _patient;
    _bottleGel->_savePositionOriginal = _bottleGel->getPosition();
    _bottleGel->setUpNoteHelp();
    
    _getWaterAdvance = Tool::createTool(TOOL_GET_WATER, TOOL_TYPE_GET_WATER_ADVANCE);
    this->addChild(_getWaterAdvance,15);
    _getWaterAdvance->setPosition(Point(visibleSize.width*0.92f, -visibleSize.height*0.20f));
    _getWaterAdvance->setScale(2.0f);
    _getWaterAdvance->_patient = _patient;
    _getWaterAdvance->_savePositionOriginal = _getWaterAdvance->getPosition();
    _getWaterAdvance->setUpNoteHelp();
    ///
    
    _scissor->setVisible(false);
    _getWater->setVisible(false);
    _smallTable->setVisible(false);
    _getMess->setVisible(false);
    _catchBug->setVisible(false);
    _shakeEar->setVisible(false);
    
    _desiccate->setVisible(false);
    _sprayChemicals->setVisible(false);
    _tamPon->setVisible(false);
    _drugWater->setVisible(false);
    _injection->setVisible(false);
    
    _lazer->setVisible(false);
    _bottleGel->setVisible(false);
    _catchBugAdvance->setVisible(false);
    _tamponAdvance->setVisible(false);
    _getWaterAdvance->setVisible(false);
}

void GamePlay::showTools(){
    float fTime = 0.3f;
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    _scissor->setVisible(true);
    _getWater->setVisible(true);
    _smallTable->setVisible(true);
    _getMess->setVisible(true);
    _catchBug->setVisible(true);
    _shakeEar->setVisible(true);

    
    _desiccate->setVisible(true);
    _sprayChemicals->setVisible(true);
    _tamPon->setVisible(true);
    _drugWater->setVisible(true);
    _injection->setVisible(true);
    
    _scissor->runAction(MoveTo::create(fTime, Point(visibleSize.width*0.12f, visibleSize.height*0.13f)));
    _shakeEar->runAction(MoveTo::create(fTime, Point(visibleSize.width*0.33f, visibleSize.height*0.13f)));
    _getMess->runAction(MoveTo::create(fTime, Point(visibleSize.width*0.5f, visibleSize.height*0.13f)));
    _catchBug->runAction(MoveTo::create(fTime, Point(visibleSize.width*0.66f, visibleSize.height*0.13f)));
    _getWater->runAction(MoveTo::create(fTime, Point(visibleSize.width*0.91f, -visibleSize.height*0.3f)));
    
    //Page 2;
    float widthTmp = visibleSize.width*1.2f;

    _desiccate->runAction(MoveTo::create(fTime, Point(visibleSize.width*0.05f + widthTmp, - visibleSize.height*0.27f)));
    _sprayChemicals->runAction(MoveTo::create(fTime, Point(visibleSize.width*0.33f +widthTmp, visibleSize.height*0.14f)));
    _tamPon->runAction(MoveTo::create(fTime, Point(visibleSize.width*0.51f + widthTmp, visibleSize.height*0.13f)));
    _drugWater->runAction(MoveTo::create(fTime, Point(visibleSize.width*0.69f + widthTmp, visibleSize.height*0.13f)));
    _injection->runAction(MoveTo::create(fTime, Point(visibleSize.width*0.91f + widthTmp, -visibleSize.height*0.13f)));
    
    _pageTools = 1;
    _btnNextTools->setVisible(true);
}

void GamePlay::addToolsPaint(){
    Size visibleSize = Director::getInstance()->getVisibleSize();
    //Init draw tool
    spriteBarDraw = Sprite::create(DRAW_BAR);
    spriteBarDraw->setScaleY(2.0f);
    spriteBarDraw->setPosition(visibleSize.width/2, visibleSize.height - spriteBarDraw->getContentSize().height);
    spriteBarDraw->setScaleX(visibleSize.width/spriteBarDraw->getContentSize().width);
    this->addChild(spriteBarDraw,18);
    
    btnStopDraw = MenuItemImage::create("slice27_27.png","slice23_23.png","slice23_23.png",CC_CALLBACK_1(GamePlay::stopDraw,this));
    btnStopDraw->setPosition(spriteBarDraw->getContentSize().width*0.1,spriteBarDraw->getContentSize().height*0.5f);
    auto spriteErrow = Sprite::create("slice48_48.png");
    spriteBarDraw->addChild(spriteErrow,19);
    spriteErrow->setScale(0.5f);
    spriteErrow->setPosition(btnStopDraw->getPosition());
    
    btnOpenTabBrush = MenuItemImage::create("slice14_14.png","slice14_14.png","slice28_28.png",CC_CALLBACK_1(GamePlay::chooseBrush,this));
    btnOpenTabBrush->setPosition(spriteBarDraw->getContentSize().width*0.26,spriteBarDraw->getContentSize().height*0.5f);
    spriteBrush = Sprite::create("slice13_13.png");
    spriteBrushWhite = Sprite::create("slice18_18.png");
    spriteBrushWhite->setColor(Color3B::BLACK);
    spriteBrush->setPosition(btnOpenTabBrush->getPosition());
    spriteBrushWhite->setPosition(btnOpenTabBrush->getPosition());
    spriteBarDraw->addChild(spriteBrushWhite,19);
    spriteBarDraw->addChild(spriteBrush,19);
    
    btnOpenTabStamp = MenuItemImage::create("slice28_28.png","slice14_14.png","slice28_28.png",CC_CALLBACK_1(GamePlay::chooseStamp,this));
    btnOpenTabStamp->setPosition(spriteBarDraw->getContentSize().width*0.42,spriteBarDraw->getContentSize().height*0.5f);
    spriteStamp = Sprite::create("stamp/slice24_24.png");
    spriteBarDraw->addChild(spriteStamp,19);
    spriteStamp->setScale(0.5f);
    spriteStamp->setPosition(btnOpenTabStamp->getPosition());
    
    btnChooseEraser = MenuItemImage::create("slice28_28.png","slice14_14.png","slice28_28.png",CC_CALLBACK_1(GamePlay::chooseEraser,this));
    btnChooseEraser->setPosition(spriteBarDraw->getContentSize().width*0.58,spriteBarDraw->getContentSize().height*0.5f);
    auto spriteEraser = Sprite::create("common/slice16_16.png");
    spriteBarDraw->addChild(spriteEraser,19);
    spriteEraser->setScale(0.7f);
    spriteEraser->setTag(30);
    spriteEraser->setPosition(btnChooseEraser->getPosition());
    
    auto menuItem = Menu::create(btnStopDraw,btnOpenTabBrush,btnOpenTabStamp,btnChooseEraser, NULL);
    menuItem->setPosition(0,0);
    spriteBarDraw->addChild(menuItem,18);
    spriteBarDraw->setVisible(false);
    
    tabIndex = 0;
    
    //Table brushes
    _bgBoardBrushes = Sprite::create(DRAW_UI_BG_3);
    this->addChild(_bgBoardBrushes, 20);
    _bgBoardBrushes->setScale(2.0f);
    _bgBoardBrushes->setPosition(visibleSize.width/2, visibleSize.height/2);
    
    //Starts
    auto sprite1 = Sprite::create("common/slice15_15.png");
    _bgBoardBrushes->addChild(sprite1,19);
    sprite1->setPosition(_bgBoardBrushes->getContentSize().width *0.2, _bgBoardBrushes->getContentSize().height*0.95);
    auto sprite2 = Sprite::create("common/slice22_22.png");
    _bgBoardBrushes->addChild(sprite2,19);
    sprite2->setPosition(_bgBoardBrushes->getContentSize().width *0.4, _bgBoardBrushes->getContentSize().height);
    auto sprite3 = Sprite::create("common/slice27_27.png");
    _bgBoardBrushes->addChild(sprite3,19);
    sprite3->setPosition(_bgBoardBrushes->getContentSize().width *0.8, _bgBoardBrushes->getContentSize().height*0.9);
    
    _menuBrushesType = Menu::create();
    _menuBrushesColor = Menu::create();
    
    for(int i = 0; i < 5;i++){
        if(i == 0){
            //41_41
            auto item = MenuItemImage::create("slice23_23.png","slice23_23.png",CC_CALLBACK_1(GamePlay::chooseTypeBrush,this));
            item->setPosition(_bgBoardBrushes->getContentSize().width*0.15, _bgBoardBrushes->getContentSize().height*0.83);
            item->setTag(i);
            _menuBrushesType->addChild(item);
            
            _iconBrushType1 = Sprite::create("slice13_13.png");
            _iconBrushWhiteType1 = Sprite::create("slice18_18.png");
            _iconBrushWhiteType1->setColor(Color3B::BLACK);
            _iconBrushType1->setPosition(item->getPosition());
            _iconBrushWhiteType1->setPosition(item->getPosition());
            _bgBoardBrushes->addChild(_iconBrushWhiteType1,15);
            _bgBoardBrushes->addChild(_iconBrushType1,15);
            _iconBrushWhiteType1->setScale(1.0f);
            _iconBrushType1->setScale(1.0f);
        }else if(i == 4){
            auto item = MenuItemImage::create("slice05_05.png","slice11_11.png",CC_CALLBACK_1(GamePlay::chooseTypeBrush,this));
            item->setPosition(_bgBoardBrushes->getContentSize().width*(0.15 + i*0.175), _bgBoardBrushes->getContentSize().height*0.83);
            item->setTag(i);
            _menuBrushesType->addChild(item);
            
            _iconBrushRainBow = Sprite::create("slice16_16.png");
            _iconBrushRainBow->setPosition(item->getPosition());
            _iconBrushRainBow->setScale(0.5f);
            _bgBoardBrushes->addChild(_iconBrushRainBow,15);
        }else{
            auto item = MenuItemImage::create("slice43_43.png","slice14_14.png",CC_CALLBACK_1(GamePlay::chooseTypeBrush,this));
            item->setPosition(_bgBoardBrushes->getContentSize().width*(0.15 + i*0.175), _bgBoardBrushes->getContentSize().height*0.83);
            item->setTag(i);
            _menuBrushesType->addChild(item);
            
            if (i == 1) {
                _iconBrushType2 = Sprite::create("slice29_29.png");
                _iconBrushWhiteType2 = Sprite::create("slice47_47.png");
                _iconBrushWhiteType2->setColor(Color3B::BLACK);
                _iconBrushType2->setPosition(item->getPosition());
                _iconBrushWhiteType2->setPosition(item->getPosition());
                _bgBoardBrushes->addChild(_iconBrushWhiteType2,15);
                _bgBoardBrushes->addChild(_iconBrushType2,15);
                _iconBrushWhiteType2->setScale(0.5f);
                _iconBrushType2->setScale(0.5f);
            }else if (i == 2){
                _iconBrushType3 = Sprite::create("slice42_42.png");
                _iconBrushWhiteType3 = Sprite::create("slice45_45.png");
                _iconBrushWhiteType3->setColor(Color3B::BLACK);
                _iconBrushType3->setPosition(item->getPosition());
                _iconBrushWhiteType3->setPosition(item->getPosition());
                _bgBoardBrushes->addChild(_iconBrushWhiteType3,15);
                _bgBoardBrushes->addChild(_iconBrushType3,15);
                _iconBrushWhiteType3->setScale(0.5f);
                _iconBrushType3->setScale(0.5f);
            }else if (i == 3){
                _iconBrushType4 = Sprite::create("slice20_20.png");
                _iconBrushWhiteType4 = Sprite::create("slice15_15.png");
                _iconBrushWhiteType4->setColor(Color3B::BLACK);
                _iconBrushType4->setPosition(item->getPosition());
                _iconBrushWhiteType4->setPosition(item->getPosition());
                _bgBoardBrushes->addChild(_iconBrushWhiteType4,15);
                _bgBoardBrushes->addChild(_iconBrushType4,15);
                _iconBrushWhiteType4->setScale(0.5f);
                _iconBrushType4->setScale(0.5f);
            }
        }
    }
    
    auto itemClose = MenuItemImage::create("common/slice17_17.png","common/slice17_17.png",CC_CALLBACK_1(GamePlay::closeBoard,this));
    itemClose->setPosition(_bgBoardBrushes->getContentSize().width*0.95f, _bgBoardBrushes->getContentSize().height*0.95f);
    itemClose->setTag(-1);

    _menuBrushesType->addChild(itemClose);
    _menuBrushesType->setPosition(0,0);
    _bgBoardBrushes->addChild(_menuBrushesType);
    
    auto imgRainBowBrush = Sprite::create("slice01_01.png");
    imgRainBowBrush->setTag(-200);
    imgRainBowBrush->setPosition(_bgBoardBrushes->getContentSize().width/2, _bgBoardBrushes->getContentSize().height/2);
    _bgBoardBrushes->addChild(imgRainBowBrush,15);
    imgRainBowBrush->setVisible(false);
    
    for(int i = 0; i < 24; i++){
        auto spriteBrush = Sprite::create("slice31_31.png");
        spriteBrush->setTag(-100);
        _bgBoardBrushes->addChild(spriteBrush,10);
        
        auto spriteBrush2 = Sprite::create("slice04_04.png");
        spriteBrush2->setVisible(false);
        _bgBoardBrushes->addChild(spriteBrush2,10);
        spriteBrush2->setTag(-101 - i);
        
        if (i <= 5) {
            if ( i == 0) {
                auto item = MenuItemImage::create("slice38_38.png","slice38_38.png",CC_CALLBACK_1(GamePlay::chooseBrushPath,this));
                item->setPosition(_bgBoardBrushes->getContentSize().width*(0.155 + 0.14*i), _bgBoardBrushes->getContentSize().height*0.69);
                item->setTag(i);
                _menuBrushesColor->addChild(item);
                
                spriteBrush2->setPosition(item->getPosition());
                spriteBrush2->setColor(Color3B::WHITE);
                
                spriteBrush->setPosition(item->getPosition());
                spriteBrush->setColor(Color3B::BLACK);
            }else{
                auto item = MenuItemImage::create("slice40_40.png","slice40_40.png",CC_CALLBACK_1(GamePlay::chooseBrushPath,this));
                item->setPosition(_bgBoardBrushes->getContentSize().width*(0.155 + 0.14*i), _bgBoardBrushes->getContentSize().height*0.69);
                item->setTag(i);
                _menuBrushesColor->addChild(item);
                
                spriteBrush2->setPosition(item->getPosition());
                spriteBrush->setPosition(item->getPosition());
                
                if (i == 1) {
                    spriteBrush2->setColor(Color3B::BLACK);
                    spriteBrush->setColor(Color3B::WHITE);
                }
                else if(i == 2){
                    spriteBrush2->setColor(Color3B::WHITE);
                    spriteBrush->setColor(Color3B::GRAY);
                }else if(i == 3){
                    spriteBrush2->setColor(Color3B::YELLOW);
                    spriteBrush->setColor(Color3B(220, 20, 60));
                }else if(i == 4){
                    spriteBrush2->setColor(Color3B::YELLOW);
                    spriteBrush->setColor(Color3B(139, 0, 0));
                }else if(i == 5){
                    spriteBrush2->setColor(Color3B::WHITE);
                    spriteBrush->setColor(Color3B(219, 112, 147));
                }
            }
        }
        else if(i> 5 && i<= 11){
            auto item = MenuItemImage::create("slice40_40.png","slice40_40.png",CC_CALLBACK_1(GamePlay::chooseBrushPath,this));
            item->setPosition(_bgBoardBrushes->getContentSize().width*(0.155 + 0.14*(i%6)), _bgBoardBrushes->getContentSize().height*0.56);
            item->setTag(i);
            _menuBrushesColor->addChild(item);
            
            spriteBrush2->setPosition(item->getPosition());
            spriteBrush->setPosition(item->getPosition());
            
            if(i == 6){
                spriteBrush2->setColor(Color3B::YELLOW);
                spriteBrush->setColor(Color3B(0, 109, 255));
            }else if(i == 7){
                spriteBrush2->setColor(Color3B(0, 109, 255));
                spriteBrush->setColor(Color3B::BLUE);
            }else if(i == 8){
                spriteBrush2->setColor(Color3B::GREEN);
                spriteBrush->setColor(Color3B(106, 90, 205));
            }else if(i == 9){
                spriteBrush2->setColor(Color3B::YELLOW);
                spriteBrush->setColor(Color3B(205, 0, 205));
            }else if(i == 10){
                spriteBrush2->setColor(Color3B::YELLOW);
                spriteBrush->setColor(Color3B(205, 20, 147));
            }else if(i == 11){
                spriteBrush2->setColor(Color3B::WHITE);
                spriteBrush->setColor(Color3B(255, 105, 180));
            }
        }
        else if(i> 11 && i<= 17){
            auto item = MenuItemImage::create("slice40_40.png","slice40_40.png",CC_CALLBACK_1(GamePlay::chooseBrushPath,this));
            item->setPosition(_bgBoardBrushes->getContentSize().width*(0.155 + 0.14*(i%12)), _bgBoardBrushes->getContentSize().height*0.43);
            item->setTag(i);
            _menuBrushesColor->addChild(item);

            spriteBrush2->setPosition(item->getPosition());
            spriteBrush->setPosition(item->getPosition());
            
            if(i == 12){
                spriteBrush2->setColor(Color3B::BLUE);
                spriteBrush->setColor(Color3B(0, 205, 205));
            }else if(i == 13){
                spriteBrush2->setColor(Color3B::BLUE);
                spriteBrush->setColor(Color3B(175, 238, 238));
            }else if(i == 14){
                spriteBrush2->setColor(Color3B::RED);
                spriteBrush->setColor(Color3B(0,250,154));
            }else if(i == 15){
                spriteBrush2->setColor(Color3B::YELLOW);
                spriteBrush->setColor(Color3B(0,255,127));
            }else if(i == 16){
                spriteBrush2->setColor(Color3B::WHITE);
                spriteBrush->setColor(Color3B(0,255,0));
            }else if(i == 17){
                spriteBrush2->setColor(Color3B::MAGENTA);
                spriteBrush->setColor(Color3B(0,128,0));
            }
        }
        else{
            auto item = MenuItemImage::create("slice40_40.png","slice40_40.png",CC_CALLBACK_1(GamePlay::chooseBrushPath,this));
            item->setPosition(_bgBoardBrushes->getContentSize().width*(0.155 + 0.14*(i%18)), _bgBoardBrushes->getContentSize().height*0.3);
            item->setTag(i);
            _menuBrushesColor->addChild(item);

            spriteBrush2->setPosition(item->getPosition());
            spriteBrush->setPosition(item->getPosition());
            
            if(i == 18){
                spriteBrush2->setColor(Color3B::ORANGE);
                spriteBrush->setColor(Color3B(255,0,0));
            }
            else if(i == 19){
                spriteBrush2->setColor(Color3B::YELLOW);
                spriteBrush->setColor(Color3B(255,69,0));
            }
            else if(i == 20){
                spriteBrush2->setColor(Color3B::RED);
                spriteBrush->setColor(Color3B(255,140,0));
            }else if(i == 21){
                spriteBrush2->setColor(Color3B::BLUE);
                spriteBrush->setColor(Color3B(255,165,0));
            }else if(i == 22){
                spriteBrush2->setColor(Color3B::BLUE);
                spriteBrush->setColor(Color3B(255,215,0));
            }else if(i == 23){
                spriteBrush2->setColor(Color3B(205,0,205));
                spriteBrush->setColor(Color3B(255,255,0));
            }
        }
    }
    
    _menuBrushesColor->setPosition(0,0);
    _bgBoardBrushes->addChild(_menuBrushesColor);
    
    //Sliceder
    auto jsp1 = Sprite::create(TOOL_SLIDER_BAR_1);
    Size sizeJ = jsp1->getContentSize();
    sizeJ.width *= 1.7f;
    jsp1->setContentSize(sizeJ);
    
    auto jsp2 = Sprite::create("slice39_39.png");
    auto jsp3 = Sprite::create("slice39_39.png");

    pSlider = cocos2d::extension::ControlSlider::create(jsp1,jsp2,jsp3);
    pSlider->setTag(2);
    (pSlider->getBackgroundSprite())->setVisible(false);
    (pSlider->getProgressSprite())->setVisible(false);
    
    pSlider->setMinimumValue(0.5f); // Sets the min value of range
    pSlider->setMaximumValue(2.5); // Sets the max value of range
    pSlider->setValue(1.0f);
    pSlider->setPosition(Vec2(_bgBoardBrushes->getContentSize().width*0.41f, _bgBoardBrushes->getContentSize().height*0.17f));
    pSlider->addTargetWithActionForControlEvents(this, cccontrol_selector(GamePlay::valueChanged), cocos2d::extension::Control::EventType::VALUE_CHANGED);

    auto jsp4 = Sprite::create("DrawResources/brush/pen_brush_1.png");
    jsp4->setTag(-500);
    jsp4->setPosition(_bgBoardBrushes->getContentSize().width*0.85f,pSlider->getPositionY());
    
    _bgBoardBrushes->addChild(pSlider,15);
    _bgBoardBrushes->addChild(jsp4,15);
    _bgBoardBrushes->setVisible(false);
    _isShowBoard = false;
    
    //Tool draw
    // ask director the window size
    Size size = Director::getInstance()->getWinSize();
    // create a canvas to draw on
    canvas = RenderTexture::create(size.width, size.height,
                                   Texture2D::PixelFormat::RGBA8888);
    // position on the center of the screen
    canvas->setPosition(Point(size.width / 2, size.height / 2));
    // used later do not release
    canvas->retain();
    // set target as child and z order to 1
    this->addChild(canvas, 17);
    
    shadowCanvas = RenderTexture::create(size.width, size.height, Texture2D::PixelFormat::RGBA8888);
    shadowCanvas->setPosition(size.width / 2, size.height / 2);
    shadowCanvas->retain();
    this->addChild(shadowCanvas, 16);
    
    shadow = Sprite::create("DrawResources/brush/pen_brush_1.png");
    shadow->setColor(Color3B::RED);
    shadow->setScale(6.0f);
    shadow->retain();

    shadowBase = Sprite::create("DrawResources/brush/pen_brush_1.png");
    shadowBase->setColor(Color3B::RED);
    shadowBase->setScale(2.0f);
    shadowBase->retain();
    
    // init the brush tip
    brush = Sprite::create("DrawResources/brush/pen_brush_1.png");
    // Change color of brush by this line.
    brush->setColor(Color3B::BLACK);
    // RGB color
    // used later do not release
    brush->retain();
    
    // init the brush tip
    stempBrush = Sprite::create("stamp/slice24_24.png");
    stempBrush->setColor(Color3B::WHITE);
    stempBrush->retain();
    
    eraser = Sprite::create("common/slice16_16.png");
    eraser->retain();
    eraser->setScale(1.5f);
    BlendFunc f = {GL_ZERO,GL_ONE_MINUS_SRC_ALPHA };
    eraser->setBlendFunc(f);
    
    brushIndexTab = 1;
    isEraser = false;
    _isStartDraw = false;
}

void GamePlay::addStempBoard(){
      Size visibleSize = Director::getInstance()->getVisibleSize();
    //Table stamp
    _bgBoardStemp = Sprite::create(DRAW_UI_BG_2);
    this->addChild(_bgBoardStemp, 20);
    _bgBoardStemp->setScale(2.0f);
    _bgBoardStemp->setPosition(visibleSize.width/2, visibleSize.height/2);
    _bgBoardStemp->setVisible(false);
    
    //Starts
    auto sprite1 = Sprite::create("common/slice15_15.png");
    _bgBoardStemp->addChild(sprite1,19);
    sprite1->setPosition(_bgBoardStemp->getContentSize().width *0.2, _bgBoardStemp->getContentSize().height*0.95);
    auto sprite2 = Sprite::create("common/slice22_22.png");
    _bgBoardStemp->addChild(sprite2,19);
    sprite2->setPosition(_bgBoardStemp->getContentSize().width *0.4, _bgBoardStemp->getContentSize().height);
    auto sprite3 = Sprite::create("common/slice27_27.png");
    _bgBoardStemp->addChild(sprite3,19);
    sprite3->setPosition(_bgBoardStemp->getContentSize().width *0.8, _bgBoardStemp->getContentSize().height*0.9);
    
    _menuStemps = Menu::create();
    
    auto itemClose = MenuItemImage::create("common/slice17_17.png","common/slice17_17.png",CC_CALLBACK_1(GamePlay::closeBoard,this));
    itemClose->setPosition(_bgBoardStemp->getContentSize().width*0.95f, _bgBoardStemp->getContentSize().height*0.95f);
    itemClose->setTag(-25);
    _menuStemps->addChild(itemClose);
    
    _menuStemps->setPosition(0,0);
    _bgBoardStemp->addChild(_menuStemps);
    
    for(int i = 0; i < 24; i++){
        char str[100] ={0};
        if(i<=8)
            sprintf(str, "stamp/slice0%d_0%d.png",i+1,i+1);
        else{
            sprintf(str, "stamp/slice%d_%d.png",i+1,i+1);
        }
        
        auto spriteBrush = Sprite::create(str);
        spriteBrush->setTag(-i);
        spriteBrush->setScale(0.5f);
        _bgBoardStemp->addChild(spriteBrush,10);
        
        //37 - 21
        if (i <= 3) {
            if ( i == 0) {
                auto item = MenuItemImage::create("slice21_21.png","slice21_21.png",CC_CALLBACK_1(GamePlay::chooseStampsDraw,this));
                item->setPosition(_bgBoardStemp->getContentSize().width*0.2, _bgBoardStemp->getContentSize().height*0.82);
                item->setTag(i);
                _menuStemps->addChild(item);
                
                spriteBrush->setPosition(item->getPosition());
                spriteBrush->setColor(Color3B::BLACK);
            }else{
                auto item = MenuItemImage::create("slice37_37.png","slice37_37.png",CC_CALLBACK_1(GamePlay::chooseStampsDraw,this));
                item->setPosition(_bgBoardStemp->getContentSize().width*(0.2 + 0.2*i), _bgBoardStemp->getContentSize().height*0.82);
                item->setTag(i);
                _menuStemps->addChild(item);
                
                spriteBrush->setPosition(item->getPosition());
            }
        }
        else if(i> 3 && i<= 7){
            auto item = MenuItemImage::create("slice37_37.png","slice37_37.png",CC_CALLBACK_1(GamePlay::chooseStampsDraw,this));
            item->setPosition(_bgBoardStemp->getContentSize().width*(0.2 + 0.2*(i%4)), _bgBoardStemp->getContentSize().height*0.7);
            item->setTag(i);
            _menuStemps->addChild(item);
            
            spriteBrush->setPosition(item->getPosition());
            
        }else if(i> 7 && i<= 11){
            auto item = MenuItemImage::create("slice37_37.png","slice37_37.png",CC_CALLBACK_1(GamePlay::chooseStampsDraw,this));
            item->setPosition(_bgBoardStemp->getContentSize().width*(0.2 + 0.2*(i%8)), _bgBoardStemp->getContentSize().height*0.58);
            item->setTag(i);
            _menuStemps->addChild(item);
            
            spriteBrush->setPosition(item->getPosition());
            
        }else if(i > 11 && i <= 15){
            auto item = MenuItemImage::create("slice37_37.png","slice37_37.png",CC_CALLBACK_1(GamePlay::chooseStampsDraw,this));
            item->setPosition(_bgBoardStemp->getContentSize().width*(0.2 + 0.2*(i%12)), _bgBoardStemp->getContentSize().height*0.46);
            item->setTag(i);
            _menuStemps->addChild(item);
            
            spriteBrush->setPosition(item->getPosition());
        }else if(i > 15 && i <= 19){
            auto item = MenuItemImage::create("slice37_37.png","slice37_37.png",CC_CALLBACK_1(GamePlay::chooseStampsDraw,this));
            item->setPosition(_bgBoardStemp->getContentSize().width*(0.2 + 0.2*(i%16)), _bgBoardStemp->getContentSize().height*0.34);
            item->setTag(i);
            _menuStemps->addChild(item);
            
            spriteBrush->setPosition(item->getPosition());
        }else if(i > 19 && i <= 23){
            auto item = MenuItemImage::create("slice37_37.png","slice37_37.png",CC_CALLBACK_1(GamePlay::chooseStampsDraw,this));
            item->setPosition(_bgBoardStemp->getContentSize().width*(0.2 + 0.2*(i%20)), _bgBoardStemp->getContentSize().height*0.22);
            item->setTag(i);
            _menuStemps->addChild(item);
            
            spriteBrush->setPosition(item->getPosition());
        }
    }
    
    _menuStemps->setPosition(0,0);
    _bgBoardStemp->setVisible(false);
    _isShowBoard = false;
}

#pragma mark - mess and bugs
void GamePlay::addMessesAndBugs(){
    Size visibleSize = Director::getInstance()->getVisibleSize();

//    if (_patient->_typePatient == 1) {
    char str1[100] = {0};
    sprintf(str1, "%s_1.png",BUG_BLUE_BUG);
    
    char str2[100] = {0};
    sprintf(str2, "%s_1.png",BUG_RED_BUG);
    
    _bug1 = Bug::createBug(str1);
    _bug1->setScale(0.2f);
    _bug1->setRotation(90);
    _bug1->setPosition(visibleSize.width*0.46f, visibleSize.height* 0.65f);
    _bug1->animationBug(1);
    _bug1->_typeMove = 2;
    _bug1->_typeBug = 1;
    _bug1->_savePosition = _bug1->getPosition();
    _bug1->_pointFinish = Point(_bug1->_savePosition.x - 60, _bug1->_savePosition.y);
    _bug1->bugMove();
    _bug1->_tool = _catchBug;
    this->addChild(_bug1,13);
    
    _bug2 = Bug::createBug(str1);
    _bug2->setRotation(-135);
    _bug2->setPosition(visibleSize.width*0.20f, visibleSize.height* 0.56f);
    _bug2->animationBug(1);
    _bug2->_typeMove = 1;
    _bug2->_typeBug = 1;
    _bug2->_savePosition = _bug2->getPosition();
    _bug2->_pointFinish = Point(_bug2->_savePosition.x + 60, _bug2->_savePosition.y + 60);
    _bug2->bugMove();
    _bug2->_tool = _catchBug;
    this->addChild(_bug2,13);
    
    _bug3 = Bug::createBug(str2);
    _bug3->setRotation(135.0f);
    _bug3->setPosition(visibleSize.width*0.45f, visibleSize.height* 0.45f);
    _bug3->animationBug(2);
    _bug3->_typeMove = 1;
    _bug3->_typeBug = 2;
    _bug3->_savePosition = _bug3->getPosition();
    _bug3->_pointFinish = Point(_bug3->_savePosition.x - 60, _bug3->_savePosition.y + 60);
    _bug3->bugMove();
    _bug3->_tool = _catchBug;
    this->addChild(_bug3,13);
    
    _bug4 = Bug::createBug(str2);
    _bug4->setScale(0.2f);
    _bug4->setRotation(105.0f);
    _bug4->setPosition(visibleSize.width*0.45f, visibleSize.height* 0.58f);
    _bug4->animationBug(2);
    _bug4->_typeMove = 2;
    _bug4->_typeBug = 2;
    _bug4->_savePosition = _bug4->getPosition();
    _bug4->_pointFinish = Point(_bug4->_savePosition.x - 60, _bug2->_savePosition.y + 30);
    _bug4->bugMove();
    _bug4->_tool = _catchBug;
    this->addChild(_bug4,13);

    
    //Add nuoc ban
    _nuocTai1 = MessObject::createMess(MESS_NUOC_BAN_1, MESS_TYPE_NUOC_BAN);
    _nuocTai1->_tool = _tamPon;//Tam bong hut nuoc ban
    _nuocTai1->setScale(2.0f);
    _nuocTai1->setPosition(Point(visibleSize.width*0.28,visibleSize.height*0.57f));
    this->addChild(_nuocTai1,13);
    
    //Add mun
    _munTai1 = MessObject::createMess(MESS_MUN_1, MESS_TYPE_MUN);
    _munTai2 = MessObject::createMess(MESS_MUN_2, MESS_TYPE_MUN);
    _munTai3 = MessObject::createMess(MESS_MUN_2, MESS_TYPE_MUN);
    _munTai1->_tool = _drugWater;
    _munTai2->_tool = _drugWater;
    _munTai3->_tool = _drugWater;
    _munTai3->setScale(2.0f);
    _munTai2->setScale(2.0f);
    _munTai1->setScale(2.0f);
    _munTai3->setPosition(Point(visibleSize.width*0.25f, visibleSize.height*0.73f));
    _munTai2->setPosition(Point(visibleSize.width*0.2f, visibleSize.height*0.6f));
    _munTai1->setPosition(Point(visibleSize.width*0.35f, visibleSize.height*0.50f));
    this->addChild(_munTai3,13);
    this->addChild(_munTai2,13);
    this->addChild(_munTai1,13);
    
    //Add Long tai
    _longTai1 = MessObject::createMess(MESS_LONG_TAI_2, MESS_TYPE_LONG_TAI);
    _longTai2 = MessObject::createMess(MESS_LONG_TAI_1, MESS_TYPE_LONG_TAI);
    _longTai1->setScale(2.0f);
    _longTai2->setScale(2.0f);
    _longTai1->setPosition(visibleSize.width*0.3f, visibleSize.height*0.51f);
    _longTai2->setPosition(visibleSize.width*0.31f, visibleSize.height*0.51f);
    _longTai1->_tool = _scissor;
    _longTai2->_tool = _scissor;
    this->addChild(_longTai1,13);
    this->addChild(_longTai2,13);
    
    _longTai3 = MessObject::createMess(MESS_LONG_TAI_3, MESS_TYPE_LONG_TAI);
    _longTai3->setScale(1.5f);
    _longTai3->setPosition(visibleSize.width*0.32f,visibleSize.height*0.51f);
    _longTai3->_tool = _scissor;
    this->addChild(_longTai3,13);
    
    _longTai4 = MessObject::createMess(MESS_LONG_TAI_2, MESS_TYPE_LONG_TAI);
    _longTai4->setScale(1.6f);
    _longTai4->setPosition(visibleSize.width*0.36f,visibleSize.height*0.50f);
    _longTai4->_tool = _scissor;
    this->addChild(_longTai4,13);
    
    _longTai5 = MessObject::createMess(MESS_LONG_TAI_1, MESS_TYPE_LONG_TAI);
    _longTai5->setScale(2.0f);
    _longTai5->setPosition(visibleSize.width*0.36f,visibleSize.height*0.50f);
    _longTai5->_tool = _scissor;
    this->addChild(_longTai5,13);
    
    _longTai6 = MessObject::createMess(MESS_LONG_TAI_1, MESS_TYPE_LONG_TAI);
    _longTai6->setScale(2.0f);
    _longTai6->setPosition(visibleSize.width*0.37f,visibleSize.height*0.49f);
    _longTai6->_tool = _scissor;
    this->addChild(_longTai6,13);
    
    //Add ray tai
    _rayTai1 = MessObject::createMess(MESS_RAY_TAI_1, MESS_TYPE_RAY_TAI);
    _rayTai2 = MessObject::createMess(MESS_RAY_TAI_2, MESS_TYPE_RAY_TAI);
    _rayTai3 = MessObject::createMess(MESS_RAY_TAI_3, MESS_TYPE_RAY_TAI);
    _rayTai1->setScale(2.0f);
    _rayTai2->setScale(2.0f);
    _rayTai3->setScale(2.0f);
    _rayTai3->_tool = _getMess;
    _rayTai2->_tool = _getMess;
    _rayTai1->_tool = _getMess;
    _rayTai1->setPosition(visibleSize.width*0.46f, visibleSize.height*0.66f);
    _rayTai2->setPosition(visibleSize.width*0.35f, visibleSize.height*0.62f);
    _rayTai3->setPosition(visibleSize.width*0.40f, visibleSize.height*0.60f);
    this->addChild(_rayTai3,13);
    this->addChild(_rayTai2,13);
    this->addChild(_rayTai1,13);
    
    //Add mu tai
    _muTai1 = MessObject::createMess(MESS_MU_TAI_NHO_1, MESS_TYPE_MU_TAI);
    _muTai2 = MessObject::createMess(MESS_MU_TAI_NHO_2, MESS_TYPE_MU_TAI);
    _muTai1->setScale(2.0f);
    _muTai2->setScale(2.0f);
    _muTai1->_tool = _injection;
    _muTai2->_tool = _injection;
    _muTai1->setPosition(visibleSize.width*0.36f, visibleSize.height*0.66f);
    _muTai2->setPosition(visibleSize.width*0.46f, visibleSize.height*0.62f);
    this->addChild(_muTai1,13);
    this->addChild(_muTai2,13);
    
    //Add Mang tai
    _mangTai = MessObject::createMess(MESS_MANG_TAI_1, MESS_TYPE_MANG_TAI);
    _mangTai->setScale(2.0f);
    _mangTai->setPosition(Point(visibleSize.width*0.4f,visibleSize.height*0.6f));
    _mangTai->_tool = _desiccate;
    this->addChild(_mangTai,13);
    
    //Add Dich tai
    _dichTai = MessObject::createMess(MESS_DICH_TAI_FULL, MESS_TYPE_DICH_TAI);
    _dichTai->setScale(2.0f);
    _dichTai->setPosition(Point(visibleSize.width*0.415f,visibleSize.height*0.555f));
    _dichTai->_tool = _getWater;
    this->addChild(_dichTai,13);
    //    }
    
    _dichTai->setVisible(false);
    _mangTai->setVisible(false);
    _muTai1->setVisible(false);
    _muTai2->setVisible(false);
    _rayTai1->setVisible(false);
    _rayTai2->setVisible(false);
    _rayTai3->setVisible(false);
    _longTai6->setVisible(false);
    _longTai5->setVisible(false);
    _longTai4->setVisible(false);
    _longTai3->setVisible(false);
    _longTai1->setVisible(false);
    _longTai2->setVisible(false);
    _bug3->setVisible(false);
    _bug1->setVisible(false);
    _bug2->setVisible(false);
    _bug4->setVisible(false);
}

void GamePlay::addMessesAndBugsAdvance(){
    Size visibleSize = Director::getInstance()->getVisibleSize();

    char str1[100] = {0};
    sprintf(str1, "%s_1.png",BUG_BLUE_BUG);
    
    char str2[100] = {0};
    sprintf(str2, "%s_1.png",BUG_RED_BUG);
    
    int n = arc4random()%2;
    
    for (int i = 0; i< 4; i++) {
        n = arc4random()%2;
        switch (i) {
            case 0:
                if (n == 0) {
                    _bigBug1 = Bug::createBug(str1);
                }else{
                    _bigBug1 = Bug::createBug(str2);
                }
                _bigBug1->_typeBug = n + 1;
                _bigBug1->animationBug(n+1);
                break;
            case 1:
                if (n == 0) {
                    _bigBug2 = Bug::createBug(str1);
                }else{
                    _bigBug2 = Bug::createBug(str2);
                }
                _bigBug2->_typeBug = n + 1;
                _bigBug2->animationBug(n+1);
                break;
            case 2:
                if (n == 0) {
                    _bigBug3 = Bug::createBug(str1);
                }else{
                    _bigBug3 = Bug::createBug(str2);
                }
                _bigBug3->_typeBug = n + 1;
                _bigBug3->animationBug(n+1);
                break;
            case 3:
                if (n == 0) {
                    _bigBug4 = Bug::createBug(str1);
                }else{
                    _bigBug4 = Bug::createBug(str2);
                }
                _bigBug4->_typeBug = n + 1;
                _bigBug4->animationBug(n+1);
                break;
            default:
                break;
        }
    }
  
    int tag = -20;

    _bigBug1->_tool = _catchBugAdvance;
    _bigBug1->setScale(2.0f);
    _bigBug1->setRotation(-90);
    _bigBug1->setTag(tag);
    auto array1 = PointArray::create(20);
    array1->addControlPoint(Vec2(visibleSize.width*0.25f, visibleSize.height*0.25));
    array1->addControlPoint(Vec2(visibleSize.width*0.45f, visibleSize.height*0.2f));
    array1->addControlPoint(Vec2(visibleSize.width*0.6,visibleSize.height*0.25));
    array1->addControlPoint(Vec2(visibleSize.width*0.7, visibleSize.height*0.45f));
    array1->addControlPoint(Vec2(visibleSize.width*0.6,visibleSize.height*0.65));
    array1->addControlPoint(Vec2(visibleSize.width*0.45, visibleSize.height*0.72f));
    array1->addControlPoint(Vec2(visibleSize.width*0.25f, visibleSize.height*0.65));
    array1->addControlPoint(Vec2(visibleSize.width*0.15f, visibleSize.height*0.4f));
    array1->addControlPoint(Vec2(visibleSize.width*0.25f, visibleSize.height*0.25));
    _bigBug1->setPosition(Vec2(50,50));
    _bigBug1->_savePosition = _bigBug1->getPosition();
    _bigBug1->_arrayPointMove = array1;
    _bigBug1->_originalRotate = -90;
    _bigBug1->_deltaRotate = -300;
    _bigBug1->bugMoveInCircle();
    array1->retain();
    this->addChild(_bigBug1,13);
    
    _bigBug2->_tool = _catchBugAdvance;
    _bigBug2->setRotation(-235);
    _bigBug2->setScale(2.0f);
    _bigBug2->setTag(tag);
    auto array2 = PointArray::create(20);
    array2->addControlPoint(Vec2(visibleSize.width*0.25f, visibleSize.height*0.25));
    array2->addControlPoint(Vec2(visibleSize.width*0.15f, visibleSize.height*0.4f));
    array2->addControlPoint(Vec2(visibleSize.width*0.25f, visibleSize.height*0.65));
    array2->addControlPoint(Vec2(visibleSize.width*0.45, visibleSize.height*0.72f));
    array2->addControlPoint(Vec2(visibleSize.width*0.6,visibleSize.height*0.65));
    array2->addControlPoint(Vec2(visibleSize.width*0.7, visibleSize.height*0.45f));
    array2->addControlPoint(Vec2(visibleSize.width*0.6,visibleSize.height*0.25));
    array2->addControlPoint(Vec2(visibleSize.width*0.45f, visibleSize.height*0.2f));
    array2->addControlPoint(Vec2(visibleSize.width*0.25f, visibleSize.height*0.25));
    _bigBug2->setPosition(Vec2(50,50));
    _bigBug2->_savePosition = _bigBug2->getPosition();
    _bigBug2->_arrayPointMove = array2;
    _bigBug2->_originalRotate = -225;
    _bigBug2->_deltaRotate  = 320;
    _bigBug2->bugMoveInCircle();
    array2->retain();
    this->addChild(_bigBug2,13);
    
    _bigBug3->setPosition(visibleSize.width*0.45f, visibleSize.height* 0.75f);
    _bigBug3->_tool = _catchBugAdvance;
    _bigBug3->setRotation(-225.0f);
    _bigBug3->setScale(2.0f);
    _bigBug3->setTag(tag);
    _bigBug2->_tool = _catchBugAdvance;
    _bigBug2->setScale(2.0f);
    _bigBug2->setTag(tag);
    
    auto array3 = PointArray::create(20);
    array3->addControlPoint(Vec2(visibleSize.width*0.25f, visibleSize.height*0.25));
    array3->addControlPoint(Vec2(visibleSize.width*0.15f, visibleSize.height*0.4f));
    array3->addControlPoint(Vec2(visibleSize.width*0.25f, visibleSize.height*0.6));
    array3->addControlPoint(Vec2(visibleSize.width*0.35, visibleSize.height*0.65f));
    array3->addControlPoint(Vec2(visibleSize.width*0.45,visibleSize.height*0.55));
    array3->addControlPoint(Vec2(visibleSize.width*0.55, visibleSize.height*0.45f));
    array3->addControlPoint(Vec2(visibleSize.width*0.45,visibleSize.height*0.25));
    array3->addControlPoint(Vec2(visibleSize.width*0.35f, visibleSize.height*0.2f));
    array3->addControlPoint(Vec2(visibleSize.width*0.25f, visibleSize.height*0.25));
    _bigBug3->setPosition(Vec2(100,100));
    _bigBug3->_savePosition = _bigBug3->getPosition();
    _bigBug3->_arrayPointMove = array3;
    _bigBug3->_originalRotate = -225;
    _bigBug3->_deltaRotate  = 320;
    _bigBug3->bugMoveInCircle();
    array3->retain();
    this->addChild(_bigBug3,13);
    
    _bigBug4->_tool = _catchBugAdvance;
    _bigBug4->setScale(2.0f);
    _bigBug4->setTag(tag);
    _bigBug4->setRotation(-90.0f);
    auto array4 = PointArray::create(20);
    array4->addControlPoint(Vec2(visibleSize.width*0.25f, visibleSize.height*0.25));
    array4->addControlPoint(Vec2(visibleSize.width*0.35f, visibleSize.height*0.2f));
    array4->addControlPoint(Vec2(visibleSize.width*0.45,visibleSize.height*0.25));
    array4->addControlPoint(Vec2(visibleSize.width*0.67, visibleSize.height*0.45f));
    array4->addControlPoint(Vec2(visibleSize.width*0.55,visibleSize.height*0.65));
    array4->addControlPoint(Vec2(visibleSize.width*0.45, visibleSize.height*0.72f));
    array4->addControlPoint(Vec2(visibleSize.width*0.25f, visibleSize.height*0.65));
    array4->addControlPoint(Vec2(visibleSize.width*0.15f, visibleSize.height*0.4f));
    array4->addControlPoint(Vec2(visibleSize.width*0.25f, visibleSize.height*0.25));
    _bigBug4->setPosition(Vec2(100,0));
    _bigBug4->_savePosition = _bigBug4->getPosition();
    _bigBug4->_arrayPointMove = array1;
    _bigBug4->_originalRotate = -90;
    _bigBug4->_deltaRotate = -300;
    _bigBug4->bugMoveInCircle();
    array4->retain();
    this->addChild(_bigBug4,13);
    
    //Add mu tai
    n = arc4random()%3;
    for (int i = 0; i<4; i++) {
        n = arc4random()%3;
        switch (i) {
            case 0:
                if (n == 0) {
                    _muTaiAd1 = MessObject::createMess(MESS_MU_TAI_TO_1, MESS_TYPE_MU_TAI);
                }else if(n == 1){
                    _muTaiAd1 = MessObject::createMess(MESS_MU_TAI_TO_2, MESS_TYPE_MU_TAI);
                }else{
                    _muTaiAd1 = MessObject::createMess(MESS_MU_TAI_TO_3, MESS_TYPE_MU_TAI);
                }
                break;
            case 1:
                if (n == 0) {
                    _muTaiAd2 = MessObject::createMess(MESS_MU_TAI_TO_1, MESS_TYPE_MU_TAI);
                }else if(n == 1){
                    _muTaiAd2 = MessObject::createMess(MESS_MU_TAI_TO_2, MESS_TYPE_MU_TAI);
                }else{
                    _muTaiAd2 = MessObject::createMess(MESS_MU_TAI_TO_3, MESS_TYPE_MU_TAI);
                }
                break;
            case 2:
                if (n == 0) {
                    _muTaiAd3 = MessObject::createMess(MESS_MU_TAI_TO_1, MESS_TYPE_MU_TAI);
                }else if(n == 1){
                    _muTaiAd3 = MessObject::createMess(MESS_MU_TAI_TO_2, MESS_TYPE_MU_TAI);
                }else{
                    _muTaiAd3 = MessObject::createMess(MESS_MU_TAI_TO_3, MESS_TYPE_MU_TAI);
                }
                break;
            case 3:
                if (n == 0) {
                    _muTaiAd4 = MessObject::createMess(MESS_MU_TAI_TO_1, MESS_TYPE_MU_TAI);
                }else if(n == 1){
                    _muTaiAd4 = MessObject::createMess(MESS_MU_TAI_TO_2, MESS_TYPE_MU_TAI);
                }else{
                    _muTaiAd4 = MessObject::createMess(MESS_MU_TAI_TO_3, MESS_TYPE_MU_TAI);
                }                break;
            default:
                break;
        }
    }
    
    _muTaiAd1->setScale(2.0f);
    _muTaiAd2->setScale(2.0f);
    _muTaiAd3->setScale(2.0f);
    _muTaiAd4->setScale(2.0f);
    _muTaiAd1->setTag(tag);
    _muTaiAd2->setTag(tag);
    _muTaiAd3->setTag(tag);
    _muTaiAd4->setTag(tag);

    _muTaiAd1->_tool = _lazer;
    _muTaiAd2->_tool = _lazer;
    _muTaiAd3->_tool = _lazer;
    _muTaiAd4->_tool = _lazer;
    
    _muTaiAd1->setPosition(visibleSize.width*0.25f, visibleSize.height*0.7f);
    _muTaiAd2->setPosition(visibleSize.width*0.75f, visibleSize.height*0.75f);
    _muTaiAd3->setPosition(visibleSize.width*0.3f, visibleSize.height*0.5f);
    _muTaiAd4->setPosition(visibleSize.width*0.65f, visibleSize.height*0.3f);

    this->addChild(_muTaiAd1,13);
    this->addChild(_muTaiAd2,13);
    this->addChild(_muTaiAd3,13);
    this->addChild(_muTaiAd4,13);
    
    //Add dirty water
    _dirtyWater1 = MessObject::createMess(MESS_NAM_TAI_1, MESS_TYPE_NAM_MOC);
    _dirtyWater2 = MessObject::createMess(MESS_NAM_TAI_2, MESS_TYPE_NAM_MOC);
    _dirtyWater3 = MessObject::createMess(MESS_NAM_TAI_1, MESS_TYPE_NAM_MOC);
    _dirtyWater4 = MessObject::createMess(MESS_NAM_TAI_2, MESS_TYPE_NAM_MOC);
    
    _dirtyWater1->setPosition(visibleSize.width*0.3f, visibleSize.height*0.68f);
    _dirtyWater2->setPosition(visibleSize.width*0.7f, visibleSize.height*0.7f);
    _dirtyWater3->setPosition(visibleSize.width*0.32f, visibleSize.height*0.4f);
    _dirtyWater4->setPosition(visibleSize.width*0.6f, visibleSize.height*0.25f);
    
    _dirtyWater1->setScale(2.0f);
    _dirtyWater2->setScale(2.0f);
    _dirtyWater3->setScale(2.0f);
    _dirtyWater4->setScale(2.0f);
    
    _dirtyWater1->_tool = _getWaterAdvance;
    _dirtyWater2->_tool = _getWaterAdvance;
    _dirtyWater3->_tool = _getWaterAdvance;
    _dirtyWater4->_tool = _getWaterAdvance;

    _dirtyWater1->setTag(tag);
    _dirtyWater2->setTag(tag);
    _dirtyWater3->setTag(tag);
    _dirtyWater4->setTag(tag);
    
    this->addChild(_dirtyWater1,13);
    this->addChild(_dirtyWater2,13);
    this->addChild(_dirtyWater3,13);
    this->addChild(_dirtyWater4,13);
}

void GamePlay::showMessesAndBugs(){
//    switch (AppDelegate::_typePatient) {
//        case 1:
            _dichTai->setVisible(true);
            _mangTai->setVisible(true);
            _rayTai1->setVisible(true);
            _rayTai2->setVisible(true);
            _rayTai3->setVisible(true);
            _muTai1->setVisible(true);
            _muTai2->setVisible(true);
            _longTai1->setVisible(true);
            _longTai2->setVisible(true);
            _longTai3->setVisible(true);
            _longTai4->setVisible(true);
            _longTai5->setVisible(true);
            _longTai6->setVisible(true);
    
    _bug1->setVisible(true);
    _bug2->setVisible(true);
    _bug3->setVisible(true);
    _bug4->setVisible(true);
//            break;
    
//        default:
//            break;
//    }
}

#pragma mark - BUTTONS SELECTED
void GamePlay::backToolsSelected(Ref *pSender){
    if(UserDefault::getInstance()->getBoolForKey(SOUND_ON_OFF)){
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(SOUND_MAIN_BUTTON);
    }

    if(_pageTools == 2){
        _pageTools = 1;
        _btnBackTools->setVisible(false);
        
        float fTime = 0.3f;
        Size visibleSize = Director::getInstance()->getVisibleSize();
        
        _scissor->setVisible(true);
        _getWater->setVisible(true);
        _smallTable->setVisible(true);
        _getMess->setVisible(true);
        _catchBug->setVisible(true);
        _shakeEar->setVisible(true);
        
        
        _desiccate->setVisible(true);
        _sprayChemicals->setVisible(true);
        _tamPon->setVisible(true);
        _drugWater->setVisible(true);
        _injection->setVisible(true);
        
        _scissor->runAction(MoveTo::create(fTime, Point(visibleSize.width*0.12f, visibleSize.height*0.13f)));
        _shakeEar->runAction(MoveTo::create(fTime, Point(visibleSize.width*0.33f, visibleSize.height*0.13f)));
        _getMess->runAction(MoveTo::create(fTime, Point(visibleSize.width*0.5f, visibleSize.height*0.13f)));
        _catchBug->runAction(MoveTo::create(fTime, Point(visibleSize.width*0.66f, visibleSize.height*0.13f)));
        _getWater->runAction(MoveTo::create(fTime, Point(visibleSize.width*0.91f, -visibleSize.height*0.3f)));
        
        //Page 2;
        float widthTmp = visibleSize.width*1.2f;
        
        _desiccate->runAction(MoveTo::create(fTime, Point(visibleSize.width*0.05f + widthTmp, -visibleSize.height*0.27f)));
        _sprayChemicals->runAction(MoveTo::create(fTime, Point(visibleSize.width*0.33f +widthTmp, visibleSize.height*0.14f)));
        _tamPon->runAction(MoveTo::create(fTime, Point(visibleSize.width*0.51f + widthTmp, visibleSize.height*0.13f)));
        _drugWater->runAction(MoveTo::create(fTime, Point(visibleSize.width*0.69f + widthTmp, visibleSize.height*0.13f)));
        _injection->runAction(MoveTo::create(fTime, Point(visibleSize.width*0.91f + widthTmp, -visibleSize.height*0.13f)));
    }
}

void GamePlay::nextToolsSelected(Ref *pSender){
    if(UserDefault::getInstance()->getBoolForKey(SOUND_ON_OFF)){
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(SOUND_MAIN_BUTTON);
    }

    if (_pageTools == 1) {
        _pageTools = 2;
        _btnBackTools->setVisible(true);
        
        //Page 2;
        Size visibleSize = Director::getInstance()->getVisibleSize();
        float widthTmp = visibleSize.width*1.2f;
        float fTime = 0.3f;

        _scissor->runAction(MoveTo::create(fTime, Point(visibleSize.width*0.12f -widthTmp, visibleSize.height*0.13f)));
        _shakeEar->runAction(MoveTo::create(fTime, Point(visibleSize.width*0.33f -widthTmp, visibleSize.height*0.13f)));
        _getMess->runAction(MoveTo::create(fTime, Point(visibleSize.width*0.5f -widthTmp, visibleSize.height*0.13f)));
        _catchBug->runAction(MoveTo::create(fTime, Point(visibleSize.width*0.66f -widthTmp, visibleSize.height*0.13f)));
        _getWater->runAction(MoveTo::create(fTime, Point(visibleSize.width*0.91f -widthTmp, -visibleSize.height*0.3f)));
        

        _desiccate->runAction(MoveTo::create(fTime, Point(visibleSize.width*0.05f, -visibleSize.height*0.27f)));
        _sprayChemicals->runAction(MoveTo::create(fTime, Point(visibleSize.width*0.33f, visibleSize.height*0.14f)));
        _tamPon->runAction(MoveTo::create(fTime, Point(visibleSize.width*0.51f, visibleSize.height*0.13f)));
        _drugWater->runAction(MoveTo::create(fTime, Point(visibleSize.width*0.69f, visibleSize.height*0.13f)));
        _injection->runAction(MoveTo::create(fTime, Point(visibleSize.width*0.91f,visibleSize.height*0.13f)));
    }
    else if (_pageTools == 2){
        _scissor->setVisible(false);
        _getWater->setVisible(false);
        _smallTable->setVisible(false);
        _getMess->setVisible(false);
        _catchBug->setVisible(false);
        _shakeEar->setVisible(false);
        
        _desiccate->setVisible(false);
        _sprayChemicals->setVisible(false);
        _tamPon->setVisible(false);
        _drugWater->setVisible(false);
        _injection->setVisible(false);
        _keepEar->setVisible(false);
        _flashLight->setVisible(false);
        _btnBackTools->setVisible(false);
        this->getChildByTag(200)->setVisible(false);

        if(_dichTai)
            _dichTai->setVisible(false);
        
        if (_mangTai) {
            _mangTai->setVisible(false);
        }
        if (_muTai1) {
            _muTai1->setVisible(false);
        }
        if (_muTai2) {
            _muTai2->setVisible(false);
        }
        if (_rayTai1) {
            _rayTai1->setVisible(false);
        }
        if (_rayTai2) {
            _rayTai2->setVisible(false);
        }
        if (_rayTai3) {
            _rayTai3->setVisible(false);
        }
        if (_longTai1) {
            _longTai1->setVisible(false);
        }
        if (_longTai2) {
            _longTai2->setVisible(false);
        }
        if (_longTai3) {
            _longTai3->setVisible(false);
        }
        if (_longTai4) {
            _longTai4->setVisible(false);
        }
        if (_longTai5) {
            _longTai5->setVisible(false);
        }
        if (_longTai6) {
            _longTai6->setVisible(false);
        }
        if (_bug1) {
            _bug1->stopAllActions();
            _bug1->setVisible(false);
        }
        if (_bug2) {
            _bug2->stopAllActions();
            _bug2->setVisible(false);
        }
        if (_bug3){
            _bug3->stopAllActions();
            _bug3->setVisible(false);
        }
        if (_bug4){
            _bug4->stopAllActions();
            _bug4->setVisible(false);
        }
        if (_nuocTai1){
            _nuocTai1->setVisible(false);
        }
        if (_munTai1){
            _munTai1->setVisible(false);
        }
        if (_munTai2){
            _munTai2->setVisible(false);
        }
        if (_munTai3){
            _munTai3->setVisible(false);
        }
    
        _pageTools ++;
        _ongSoi->setVisible(true);
        _patient->_earHole->setScale(1.3f);
        _patient->_earHole->setPosition(_patient->_pointSavePositionEarHole);
    }
    else if(_pageTools == 3){
        for (auto mess : this->getChildren()) {
            if (mess->getTag() == -10) {
                if (!((MessObject*)mess)->_isRemove) {
                    mess->setVisible(true);
                }
            }else if(mess->getTag() == -20){
                mess->setVisible(false);
            }
        }

        _keepEar->setVisible(true);
        _flashLight->setVisible(true);
        _btnNextTools->setVisible(false);
        _ongSoi->setVisible(false);

        this->getChildByTag(200)->setVisible(true);
        
//        _patient->setOpenEye();
//        _patient->stopAllActions();
//        _patient->unscheduleAllSelectors();
        
        _patient->_earHole->setScale(2.0f);
        _patient->_earHole->setPosition(Point(_patient->_earHole->getPosition().x - 15,_patient->_earHole->getPosition().y + 14));
        
        _saveButton->setVisible(true);
        _homeButton->setVisible(true);
//        _faceButton->setVisible(true);
        _mailButton->setVisible(true);
        _drawButton->setVisible(true);
        
        _eventDispatcher->removeEventListener(listener);
        this->addToolsPaint();
        this->addStempBoard();
        
        particleStar = ParticleSystemQuad::create("particle_texture.plist");
        particleStar->setPosition(_drawButton->getPosition());
        this->getParent()->addChild(particleStar,15);
        
        particleStar2 = ParticleSystemQuad::create("particle_texture.plist");
        particleStar2->setPosition(_saveButton->getPositionX()*2, _saveButton->getPositionY());
        this->getParent()->addChild(particleStar2,15);
        
        particleStar3 = ParticleSystemQuad::create("particle_texture.plist");
        this->getParent()->addChild(particleStar3,15);
        
        particleStar->setScale(0.7f);
        particleStar2->setScale(0.7f);
        particleStar3->setScale(0.7f);
    }
}

void GamePlay::stopAdvanceLevel(Ref *pSender){
    _patient->setVisible(true);
    _ongSoi->setVisible(true);
    _patient->_earHole->setVisible(true);
    _patient->_smallEye->setVisible(true);
    _ongSoi->_noteHelp->setVisible(false);
    _spriteTable->setVisible(true);
    _background->setVisible(true);
    _btnNextTools->setVisible(true);
    _ongSoi->setPosition(_ongSoi->_savePositionOriginal);
    _ongSoi->_isTouch = false;
    _ongSoi->_isSet = false;
    this->getChildByTag(201)->setVisible(true);//Ear tmp
    
    _backgroundBlackFont->setVisible(false);
    _earHoleScale->setVisible(false);
    _stopAdvanceLevelButton->setVisible(false);
    spriteCircle->setVisible(false);
    _tamponAdvance->setVisible(false);
    _getWaterAdvance->setVisible(false);
    _catchBugAdvance->setVisible(false);
    _lazer->setVisible(false);
    _bottleGel->setVisible(false);
    _joystickButton->setVisible(false);
    _joystickBase->setVisible(false);
    _darkBackground->setVisible(false);
//    this->getChildByTag(202)->setVisible(false);//Joystick
    
    for (auto mess : this->getChildren()) {
        if(mess->getTag() == -20){
            mess->setVisible(false);
        }
    }

}

void GamePlay::backHome(Ref *pSender){
    if(UserDefault::getInstance()->getBoolForKey(SOUND_ON_OFF)){
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(SOUND_SUB_BUTTON);
    }
    
    auto newScene = MainMenu::createScene();
    auto trans    = TransitionSplitRows::create(.5f, newScene);
    Director::getInstance()->replaceScene(trans);
}

void GamePlay::saveImage(Ref *pSender){
    // hide button
    _saveButton->setVisible(false);
    _homeButton->setVisible(false);
    _drawButton->setVisible(false);
    _mailButton->setVisible(false);
    
    cocos2d::Size size = cocos2d::Director::getInstance()->getWinSize();
    cocos2d::RenderTexture* texture = cocos2d::RenderTexture::create(size.width,
                                                                     size.height);
    texture->setPosition(cocos2d::Point(size.width / 2, size.height / 2));
    texture->begin();
    cocos2d::Director::getInstance()->getRunningScene()->visit();
    texture->end();
    if (texture->saveToFile("screenshot.png", Image::Format::PNG)) {
        CCLOG("save screenshot success!");
        MessageBox("Save image success!", "");
    } else {
        MessageBox("Save image failed!", "");
        CCLOG("save screenshot failed!");
    }
    _saveButton->setVisible(true);
    _homeButton->setVisible(true);
    _drawButton->setVisible(true);
    _mailButton->setVisible(true);
}

void GamePlay::drawImage(Ref *pSender){
    if(!_isStartDraw){
        _drawButton->setVisible(false);
        _homeButton->setVisible(false);
        _saveButton->setVisible(false);
        _mailButton->setVisible(false);

        _mailButton->setEnabled(false);
        _saveButton->setEnabled(false);
        _homeButton->setEnabled(false);
        _drawButton->setEnabled(false);
        particleStar->setVisible(false);
        particleStar2->setVisible(false);
        particleStar3->setVisible(false);
        //    _faceButton->setVisible(false);
        _isStartDraw = true;
        
        _patient->stopAllActions();
        _patient->unscheduleUpdate();
        _patient->unscheduleAllSelectors();
        _patient->setOpenEye();
        
        spriteBarDraw->setVisible(true);
        //    for (auto mess : this->getChildren()) {
        //        mess->setPosition(mess->getPositionX(), mess->getPositionY() - 100);
        //    }
        auto normalImg1 = Sprite::create("slice14_14.png");
        auto normalImg2 = Sprite::create("slice28_28.png");
        btnOpenTabBrush->setNormalSpriteFrame(SpriteFrame::create("slice14_14.png", Rect(0, 0, normalImg1->getContentSize().width,normalImg1->getContentSize().height)));
        btnOpenTabStamp->setNormalSpriteFrame(SpriteFrame::create("slice28_28.png", Rect(0, 0, normalImg2->getContentSize().width,normalImg2->getContentSize().height)));
        btnChooseEraser->setNormalSpriteFrame(SpriteFrame::create("slice28_28.png", Rect(0, 0, normalImg2->getContentSize().width,normalImg2->getContentSize().height)));
        
        isEraser = false;
        usingBrush = true;
        pSlider->setEnabled(false);
        
        this->schedule(schedule_selector(GamePlay::updateColorBrush), 0.02f);
    }
}

void GamePlay::updateColorBrush(float dt){
    if (brushIndexTab == 5 && usingBrush) {
        brush->setColor(Color3B(arc4random() % 255, arc4random() %255, arc4random() % 255));
    }
}


void GamePlay::chooseBrush(Ref *pSender){
    if (!_isShowBoard) {
        pSlider->setEnabled(true);
        auto normalImg1 = Sprite::create("slice14_14.png");
        auto normalImg2 = Sprite::create("slice28_28.png");
        btnOpenTabBrush->setNormalSpriteFrame(SpriteFrame::create("slice14_14.png", Rect(0, 0, normalImg1->getContentSize().width,normalImg1->getContentSize().height)));
        btnOpenTabStamp->setNormalSpriteFrame(SpriteFrame::create("slice28_28.png", Rect(0, 0, normalImg2->getContentSize().width,normalImg2->getContentSize().height)));
        btnChooseEraser->setNormalSpriteFrame(SpriteFrame::create("slice28_28.png", Rect(0, 0, normalImg2->getContentSize().width,normalImg2->getContentSize().height)));
        isEraser = false;
        _bgBoardBrushes->setVisible(true);
        _isShowBoard = true;
        usingBrush = true;
    }
}

void GamePlay::chooseStamp(Ref *pSender){
    if(!_isShowBoard){
        auto normalImg1 = Sprite::create("slice14_14.png");
        auto normalImg2 = Sprite::create("slice28_28.png");
        btnOpenTabBrush->setNormalSpriteFrame(SpriteFrame::create("slice28_28.png", Rect(0, 0, normalImg2->getContentSize().width,normalImg2->getContentSize().height)));
        btnOpenTabStamp->setNormalSpriteFrame(SpriteFrame::create("slice14_14.png", Rect(0, 0, normalImg1->getContentSize().width,normalImg1->getContentSize().height)));
        btnChooseEraser->setNormalSpriteFrame(SpriteFrame::create("slice28_28.png", Rect(0, 0, normalImg2->getContentSize().width,normalImg2->getContentSize().height)));
        
        isEraser = false;
        _isShowBoard = true;
        _bgBoardStemp->setVisible(true);
        _bgBoardBrushes->setVisible(false);
        usingBrush = false;
    }
}

void GamePlay::chooseEraser(Ref *pSender){
    if(!_isShowBoard){
        auto normalImg1 = Sprite::create("slice14_14.png");
        auto normalImg2 = Sprite::create("slice28_28.png");
        btnOpenTabBrush->setNormalSpriteFrame(SpriteFrame::create("slice28_28.png", Rect(0, 0, normalImg2->getContentSize().width,normalImg2->getContentSize().height)));
        btnOpenTabStamp->setNormalSpriteFrame(SpriteFrame::create("slice28_28.png", Rect(0, 0, normalImg2->getContentSize().width,normalImg2->getContentSize().height)));
        btnChooseEraser->setNormalSpriteFrame(SpriteFrame::create("slice14_14.png", Rect(0, 0, normalImg1->getContentSize().width,normalImg1->getContentSize().height)));
        
        usingBrush = true;
        isEraser = true;
    }
}

void GamePlay::email(Ref *pSender){
    
}

void GamePlay::closeBoard(Ref *pSender){
    pSlider->setEnabled(false);
    _bgBoardBrushes->setVisible(false);
    _bgBoardStemp->setVisible(false);
    _isShowBoard = false;
}

void GamePlay::chooseTypeBrush(Ref *pSender){
    for (auto child : _menuBrushesColor->getChildren()) {
        MenuItemImage *childItem = (MenuItemImage*)child;
        childItem->setVisible(true);
    }
    
    _bgBoardBrushes->getChildByTag(-200)->setVisible(false);
    
    MenuItemImage *item = (MenuItemImage*)pSender;
    auto normalImg1 = Sprite::create("slice41_41.png");
    auto normalImg2 = Sprite::create("slice05_05.png");
    auto normalImg3 = Sprite::create("slice43_43.png");
    auto normalImg4 = Sprite::create("slice23_23.png");
    auto normalImg5 = Sprite::create("slice11_11.png");
    auto normalImg6 = Sprite::create("slice14_14.png");
    
    brushIndexTab = item->getTag()+1;

    _iconBrushWhiteType1->setScale(0.5f);
    _iconBrushType1->setScale(0.5f);
    _iconBrushWhiteType2->setScale(0.5f);
    _iconBrushType2->setScale(0.5f);
    _iconBrushWhiteType2->setScale(0.5f);
    _iconBrushType2->setScale(0.5f);
    _iconBrushWhiteType3->setScale(0.5f);
    _iconBrushType3->setScale(0.5f);
    _iconBrushWhiteType4->setScale(0.5f);
    _iconBrushType4->setScale(0.5f);
    _iconBrushRainBow->setScale(0.5f);
    
    if(item->getTag() == 0){
        auto itemSprite = Sprite::create("slice31_31.png");
        
        for (auto c : _bgBoardBrushes->getChildren()) {
            if (c->getTag() == -100) {
                ((Sprite*)c)->setSpriteFrame(SpriteFrame::create("slice31_31.png",Rect(0,0,itemSprite->getContentSize().width,itemSprite->getContentSize().height)));
                c->setScale(1.0f);
                c->setVisible(true);
            }else if(c->getTag() <= -101 && c->getTag() >= -124){
                c->setVisible(false);
            }
        }
        
        auto brushSprite = Sprite::create("DrawResources/brush/pen_brush_1.png");
        brush->setSpriteFrame(SpriteFrame::create("DrawResources/brush/pen_brush_1.png",Rect(0,0,brushSprite->getContentSize().width,brushSprite->getContentSize().height)));
    
        _iconBrushWhiteType1->setScale(1.0f);
        _iconBrushType1->setScale(1.0f);
    }
    else if(item->getTag() == 1){
        auto itemSprite = Sprite::create("slice33_33.png");
        
        for (auto c : _bgBoardBrushes->getChildren()) {
            if (c->getTag() == -100) {
                ((Sprite*)c)->setSpriteFrame(SpriteFrame::create("slice33_33.png",Rect(0,0,itemSprite->getContentSize().width,itemSprite->getContentSize().height)));
                c->setScale(0.7f);
                c->setVisible(true);
            }else if(c->getTag() <= -101 && c->getTag() >= -124){
                c->setVisible(false);
            }
        }
        
        auto brushSprite = Sprite::create("DrawResources/brush/pen_brush_1.png");
        brush->setSpriteFrame(SpriteFrame::create("DrawResources/brush/pen_brush_1.png",Rect(0,0,brushSprite->getContentSize().width,brushSprite->getContentSize().height)));
        
        _iconBrushWhiteType2->setScale(1.0f);
        _iconBrushType2->setScale(1.0f);
    }
    else if(item->getTag() == 2){
        auto itemSprite = Sprite::create("slice36_36.png");//04
        for (auto c : _bgBoardBrushes->getChildren()) {
            if (c->getTag() == -100) {
                ((Sprite*)c)->setSpriteFrame(SpriteFrame::create("slice36_36.png",Rect(0,0,itemSprite->getContentSize().width,itemSprite->getContentSize().height)));
                c->setScale(1.0f);
                c->setVisible(true);
            }else if(c->getTag() <= -101 && c->getTag() >= -124){
                c->setVisible(true);
            }
        }
        
        auto brushSprite = Sprite::create("DrawResources/brush/pen_brush_1.png");
        brush->setSpriteFrame(SpriteFrame::create("DrawResources/brush/pen_brush_1.png",Rect(0,0,brushSprite->getContentSize().width,brushSprite->getContentSize().height)));
        
        _iconBrushWhiteType3->setScale(1.0f);
        _iconBrushType3->setScale(1.0f);
    }
    else if(item->getTag() == 3){
        auto itemSprite = Sprite::create("slice06_06.png");
        for (auto c : _bgBoardBrushes->getChildren()) {
            if (c->getTag() == -100) {
                ((Sprite*)c)->setSpriteFrame(SpriteFrame::create("slice06_06.png",Rect(0,0,itemSprite->getContentSize().width,itemSprite->getContentSize().height)));
                c->setScale(1.0f);
                c->setVisible(true);
            }else if(c->getTag() <= -101 && c->getTag() >= -124){
                c->setVisible(false);
            }
        }
        
        auto brushSprite = Sprite::create("DrawResources/brush/pen_brush_4.png");
        brush->setSpriteFrame(SpriteFrame::create("DrawResources/brush/pen_brush_4.png",Rect(0,0,brushSprite->getContentSize().width,brushSprite->getContentSize().height)));
        
        _iconBrushWhiteType4->setScale(1.0f);
        _iconBrushType4->setScale(1.0f);
    }
    else if(item->getTag() == 4){
        for (auto c : _bgBoardBrushes->getChildren()) {
            if (c->getTag() == -100) {
                c->setScale(1.0f);
                c->setVisible(false);
            }else if(c->getTag() <= -101 && c->getTag() >= -124){
                c->setVisible(false);
            }
        }
        
        for (auto child : _menuBrushesColor->getChildren()) {
            MenuItemImage *childItem = (MenuItemImage*)child;
            childItem->setVisible(false);
        }
        
        _bgBoardBrushes->getChildByTag(-200)->setVisible(true);
        
        _iconBrushRainBow->setScale(1.0f);
    }
    
    if (brushIndexTab == 1) {
        auto sp1 = Sprite::create("slice13_13.png");
        auto sp2 = Sprite::create("slice18_18.png");
        spriteBrush->setSpriteFrame(SpriteFrame::create("slice13_13.png", Rect(0, 0, sp1->getContentSize().width,sp1->getContentSize().height)));
        spriteBrushWhite->setSpriteFrame(SpriteFrame::create("slice18_18.png", Rect(0, 0, sp2->getContentSize().width,sp2->getContentSize().height)));
        
    }else if (brushIndexTab == 2){
        auto sp1 = Sprite::create("slice29_29.png");
        auto sp2 = Sprite::create("slice47_47.png");
        spriteBrush->setSpriteFrame(SpriteFrame::create("slice29_29.png", Rect(0, 0, sp1->getContentSize().width,sp1->getContentSize().height)));
        spriteBrushWhite->setSpriteFrame(SpriteFrame::create("slice47_47.png", Rect(0, 0, sp2->getContentSize().width,sp2->getContentSize().height)));
    }else if(brushIndexTab == 3){
        auto sp1 = Sprite::create("slice42_42.png");
        auto sp2 = Sprite::create("slice45_45.png");
        spriteBrush->setSpriteFrame(SpriteFrame::create("slice42_42.png", Rect(0, 0, sp1->getContentSize().width,sp1->getContentSize().height)));
        spriteBrushWhite->setSpriteFrame(SpriteFrame::create("slice45_45.png", Rect(0, 0, sp2->getContentSize().width,sp2->getContentSize().height)));
    }else if(brushIndexTab == 4){
        auto sp1 = Sprite::create("slice20_20.png");
        auto sp2 = Sprite::create("slice15_15.png");
        spriteBrush->setSpriteFrame(SpriteFrame::create("slice20_20.png", Rect(0, 0, sp1->getContentSize().width,sp1->getContentSize().height)));
        spriteBrushWhite->setSpriteFrame(SpriteFrame::create("slice15_15.png", Rect(0, 0, sp2->getContentSize().width,sp2->getContentSize().height)));
    }else if(brushIndexTab == 5){
        auto sp1 = Sprite::create("slice16_16.png");
        auto sp2 = Sprite::create("slice16_16.png");
        spriteBrush->setSpriteFrame(SpriteFrame::create("slice16_16.png", Rect(0, 0, sp1->getContentSize().width,sp1->getContentSize().height)));
        spriteBrushWhite->setSpriteFrame(SpriteFrame::create("slice16_16.png", Rect(0, 0, sp2->getContentSize().width,sp2->getContentSize().height)));
    }

    
    for (auto child : _menuBrushesType->getChildren()) {
        MenuItemImage *childItem = (MenuItemImage*)child;
        if(child->getTag() == 0){
            if (child->getTag() != item->getTag()) {
                childItem->setNormalSpriteFrame(SpriteFrame::create("slice41_41.png", Rect(0, 0, normalImg1->getContentSize().width,normalImg1->getContentSize().height)));
            }else{
                childItem->setNormalSpriteFrame(SpriteFrame::create("slice23_23.png", Rect(0, 0, normalImg4->getContentSize().width,normalImg4->getContentSize().height)));
                
            }
        }else if(child->getTag() == 4){
            if (child->getTag() != item->getTag()) {
                childItem->setNormalSpriteFrame(SpriteFrame::create("slice05_05.png", Rect(0, 0, normalImg2->getContentSize().width,normalImg2->getContentSize().height)));
            }else{
                childItem->setNormalSpriteFrame(SpriteFrame::create("slice11_11.png", Rect(0, 0, normalImg5->getContentSize().width,normalImg5->getContentSize().height)));
            }
        }else if(child->getTag() > 0){
            if (child->getTag() != item->getTag()) {
                childItem->setNormalSpriteFrame(SpriteFrame::create("slice43_43.png", Rect(0, 0, normalImg3->getContentSize().width,normalImg3->getContentSize().height)));
            }else{
                childItem->setNormalSpriteFrame(SpriteFrame::create("slice14_14.png", Rect(0, 0, normalImg6->getContentSize().width,normalImg6->getContentSize().height)));
            }
        }
    }
}

void GamePlay::chooseBrushPath(Ref *pSender){
    MenuItemImage *item = (MenuItemImage*)pSender;

    auto normalImg1 = Sprite::create("slice38_38.png");
    auto normalImg2 = Sprite::create("slice40_40.png");

    item->setNormalSpriteFrame(SpriteFrame::create("slice38_38.png", Rect(0, 0, normalImg1->getContentSize().width,normalImg1->getContentSize().height)));

    for (auto child : _menuBrushesColor->getChildren()) {
        MenuItemImage *childItem = (MenuItemImage*)child;
        if (childItem->getTag() != item->getTag()) {
            childItem->setNormalSpriteFrame(SpriteFrame::create("slice40_40.png", Rect(0, 0, normalImg2->getContentSize().width,normalImg2->getContentSize().height)));
        }
    }
    
    switch (item->getTag()) {
        case 0:
            brush->setColor(Color3B::BLACK);
            break;
        case 1:
            brush->setColor(Color3B::WHITE);
            break;
        case 2:
            brush->setColor(Color3B::GRAY);
            break;
        case 3:
            brush->setColor(Color3B(220, 20, 60));
            break;
        case 4:
            brush->setColor(Color3B(139, 0, 0));
            break;
        case 5:
            brush->setColor(Color3B(219, 112, 147));
            break;
        case 6:
            brush->setColor(Color3B(0, 109, 255));
            break;
        case 7:
            brush->setColor(Color3B::BLUE);
            break;
        case 8:
            brush->setColor(Color3B(106, 90, 205));
            break;
        case 9:
            brush->setColor(Color3B(205, 0, 205));
            break;
        case 10:
            brush->setColor(Color3B(205, 20, 147));
            break;
        case 11:
            brush->setColor(Color3B(255, 105, 180));
            break;
        case 12:
            brush->setColor(Color3B(0, 205, 205));
            break;
        case 13:
            brush->setColor(Color3B(175, 238, 238));
            break;
        case 14:
            brush->setColor(Color3B(0,250,154));
            break;
        case 15:
            brush->setColor(Color3B(0,255,127));
            break;
        case 16:
            brush->setColor(Color3B(0,255,0));
            break;
        case 17:
            brush->setColor(Color3B(0,128,0));
            break;
        case 18:
            brush->setColor(Color3B(255,0,0));
            break;
        case 19:
            brush->setColor(Color3B(255,69,0));
            break;
        case 20:
            brush->setColor(Color3B(255,140,0));
            break;
        case 21:
            brush->setColor(Color3B(255,165,0));
            break;
        case 22:
            brush->setColor(Color3B(255,215,0));
            break;
        case 23:
            brush->setColor(Color3B(255,255,0));
            break;
        default:
            break;
    }
    
    _iconBrushWhiteType1->setColor(brush->getColor());
    _iconBrushWhiteType2->setColor(brush->getColor());
    _iconBrushWhiteType3->setColor(brush->getColor());
    _iconBrushWhiteType4->setColor(brush->getColor());
    spriteBrushWhite->setColor(brush->getColor());
    
    if (brushIndexTab == 2) {
        shadow->setColor(brush->getColor());
        shadowBase->setColor(brush->getColor());
        brush->setColor(Color3B::WHITE);
    }else if (brushIndexTab == 3){
        shadowBase->setColor(_bgBoardBrushes->getChildByTag(-101 - item->getTag())->getColor());
    }
    
    _isShowBoard = false;
    _bgBoardBrushes->setVisible(false);
}

void GamePlay::chooseStampsDraw(Ref *pSender){
    _isShowBoard = false;
    _bgBoardStemp->setVisible(false);
    
    MenuItemImage *item = (MenuItemImage*)pSender;
    
    auto normalImg1 = Sprite::create("slice21_21.png");
    auto normalImg2 = Sprite::create("slice37_37.png");
    
    item->setNormalSpriteFrame(SpriteFrame::create("slice21_21.png", Rect(0, 0, normalImg1->getContentSize().width,normalImg1->getContentSize().height)));
    
    for (auto child : _menuStemps->getChildren()) {
        MenuItemImage *childItem = (MenuItemImage*)child;
        if (childItem->getTag() != item->getTag() && childItem->getTag() >= 0) {
            childItem->setNormalSpriteFrame(SpriteFrame::create("slice37_37.png", Rect(0, 0, normalImg2->getContentSize().width,normalImg2->getContentSize().height)));
        }
    }
    
    char str[100] ={0};
    if(item->getTag()<=8)
        sprintf(str, "stamp/slice0%d_0%d.png",item->getTag()+1,item->getTag()+1);
    else{
        sprintf(str, "stamp/slice%d_%d.png",item->getTag()+1,item->getTag()+1);
    }

    auto spriteBrush = Sprite::create(str);

    stempBrush->setColor(Color3B::WHITE);
    stempBrush->setSpriteFrame(SpriteFrame::create(str,Rect(0,0,spriteBrush->getContentSize().width,spriteBrush->getContentSize().height)));
    
    spriteStamp->setSpriteFrame(SpriteFrame::create(str,Rect(0,0,spriteBrush->getContentSize().width,spriteBrush->getContentSize().height)));
}

//void GamePlay::facebook(Ref *pSender){
//
//}

void GamePlay::stopDraw(Ref *pSender){
    if(!_isShowBoard){
        _drawButton->setVisible(true);
        _homeButton->setVisible(true);
        _saveButton->setVisible(true);
        _mailButton->setVisible(true);
        
        _mailButton->setEnabled(true);
        _saveButton->setEnabled(true);
        _homeButton->setEnabled(true);
        _drawButton->setEnabled(true);
        particleStar->setVisible(true);
        particleStar2->setVisible(true);
        particleStar3->setVisible(true);
        
        //    _faceButton->setVisible(true);
        _isStartDraw = false;
        spriteBarDraw->setVisible(false);
        this->unschedule(schedule_selector(GamePlay::updateColorBrush));
    }
}

void GamePlay::setupAdvanceLevel(){
    _patient->setVisible(false);
    _ongSoi->setVisible(false);
    _patient->_earHole->setVisible(false);
    _patient->_smallEye->setVisible(false);
    _ongSoi->_noteHelp->setVisible(false);
    _spriteTable->setVisible(false);
    _background->setVisible(false);
    this->getChildByTag(201)->setVisible(false);//Ear tmp
    
    _backgroundBlackFont->setVisible(true);
    spriteCircle->setVisible(true);
    _earHoleScale->setVisible(true);
    _stopAdvanceLevelButton->setVisible(true);
    _joystickBase->setVisible(true);
    _joystickButton->setVisible(true);
//    this->getChildByTag(202)->setVisible(true);//Joystick

    //Show tool
    _tamponAdvance->setVisible(true);
    _getWaterAdvance->setVisible(true);
    _lazer->setVisible(true);
    _catchBugAdvance->setVisible(true);
    _bottleGel->setVisible(true);
    _darkBackground->setVisible(true);
    
    this->addMessesAndBugsAdvance();
}
