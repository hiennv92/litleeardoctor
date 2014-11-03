//
//  GamePlay.cpp
//  DoctorEar
//  Created by Trinh Van Duong on 10/30/14.
//

#include "GamePlay.h"
#include "Define.h"
#include "AppDelegate.h"

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
    
    _spriteTable = Sprite::create(GAME_PLAY_TABLE);
    _spriteTable->setPosition(_background->getContentSize().width/2,0);
    _spriteTable->setScaleY(0.85f);
    _background->addChild(_spriteTable,10);
    
    //Init patient
    _patient = Patient::createPatient(PATIENT_BODY1, AppDelegate::_typePatient);
    _patient->setPosition(_background->getContentSize().width/2,_background->getContentSize().height/2);
    _background->addChild(_patient,1);
    _patient->createSmallEye();
    _patient->createEarHole();
    
    //Add touch
    auto *listener = EventListenerTouchOneByOne::create();
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
    
    //Add messes and bugs
    this->addMessesAndBugs();
    
    return true;
}

#pragma mark - TOUCH HANDLE

bool GamePlay::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event){
    cocos2d::Vec2 p = touch->getLocation();
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
    
    if(_scissor){
        cocos2d::Rect rectScissor = _scissor->getBoundingBox();
        if(rectScissor.containsPoint(p)){
            _scissor->_isTouch = true;
            _scissor->_noteHelp->showHelp(0.0f);
            _patient->setMouthScare();
            _patient->setEyeBrowScare();
            return true;
        }
    }
    
    if(_shakeEar){
        cocos2d::Rect rectScissor = _shakeEar->getBoundingBox();
        if(rectScissor.containsPoint(p)){
            _shakeEar->_isTouch = true;
            _shakeEar->_noteHelp->showHelp(0.0f);
            _patient->setMouthScare();
            _patient->setEyeBrowScare();
            
            return true;
        }
    }
    
    if(_getMess){
        cocos2d::Rect rectScissor = _getMess->getBoundingBox();
        if(rectScissor.containsPoint(p)){
            _getMess->_isTouch = true;
            _getMess->_noteHelp->showHelp(0.0f);
            _patient->setMouthScare();
            _patient->setEyeBrowScare();

            return true;
        }
    }
    
    if(_catchBug){
        cocos2d::Rect rectScissor = _catchBug->getBoundingBox();
        if(rectScissor.containsPoint(p)){
            _catchBug->_isTouch = true;
            _catchBug->_noteHelp->showHelp(0.0f);
            _patient->setMouthScare();
            _patient->setEyeBrowScare();
            
            //Call small table
            _smallTable->_startMove = true;
            _smallTable->_isTouch = true;
            return true;
        }
    }
    
    if(_getWater){
        cocos2d::Rect rectScissor = _getWater->getBoundingBox();
        if(rectScissor.containsPoint(p)){
            _getWater->_isTouch = true;
            _getWater->_noteHelp->showHelp(0.0f);
            _patient->setMouthScare();
            _patient->setEyeBrowScare();

            return true;
        }
    }
    
    return true;
}

void GamePlay::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event){
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
                _keepEar->setPosition(Point(_keepEar->_visibleSize.width*0.15f,_keepEar->_visibleSize.height*0.61f));
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
}

void GamePlay::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event){
//    CCLOG("TOUCH ENDED");
    _patient->_isSetCloseEye = true;
}

#pragma mark - SLIDER BAR

void GamePlay::addSliderBar(){
    // Add the slider
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(SOUND_MACHINE_KEEP_EAR);
    _isChangeBarSlider = false;

    slider = cocos2d::extension::ControlSlider::create(TOOL_SLIDER_BAR_1,TOOL_SLIDER_BAR_1_BUTTON,TOOL_SLIDER_BAR_1_BUTTON);
    (slider->getProgressSprite())->setVisible(false);
    slider->setAnchorPoint(Vec2(0.5f, 1.0f));
    slider->setMinimumValue(0.0f); // Sets the min value of range
    slider->setMaximumValue(5.0f); // Sets the max value of range
    slider->setMinimumAllowedValue(1.5f);
    slider->setValue(1.5f);
    slider->setPosition(Vec2(_background->getContentSize().width*0.185f, _background->getContentSize().height*0.9f));
    slider->setTag(1);
    slider->addTargetWithActionForControlEvents(this, cccontrol_selector(GamePlay::valueChanged), cocos2d::extension::Control::EventType::VALUE_CHANGED);
    _background->addChild(slider,15);
}

void GamePlay::valueChanged(Ref *sender, cocos2d::extension::Control::EventType controlEvent)
{
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
        
//        if(UserDefault::getInstance()->getBoolForKey(SOUND_ON_OFF)){
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(SOUND_MACHINE_KEEP_EAR);
//        }
        
        _isChangeBarSlider = true;
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
    float fTime = 0.3f;
    
    _scissor = Tool::createTool(TOOL_SCISSOR_CLOSE, TOOL_TYPE_SCISSOR);
    this->addChild(_scissor,15);
    _scissor->setPosition(Point(visibleSize.width*0.12f, -visibleSize.height*0.13f));
    _scissor->setScale(2.0f);
    _scissor->_patient = _patient;
    _scissor->_savePositionOriginal = Point(visibleSize.width*0.12f, visibleSize.height*0.13f);
    _scissor->runAction(MoveTo::create(fTime, Point(visibleSize.width*0.12f, visibleSize.height*0.13f)));
    _scissor->setUpNoteHelp();

    _shakeEar = Tool::createTool(TOOL_SHAKE_EAR, TOOL_TYPE_SHAKE_EAR);
    this->addChild(_shakeEar,15);
    _shakeEar->setPosition(Point(visibleSize.width*0.33f, -visibleSize.height*0.13f));
    _shakeEar->cocos2d::Node::setScale(2.0f);
    _shakeEar->_savePositionOriginal = Point(visibleSize.width*0.33f, visibleSize.height*0.13f);
    _shakeEar->runAction(MoveTo::create(fTime, Point(visibleSize.width*0.33f, visibleSize.height*0.13f)));
    _shakeEar->_patient = _patient;
    _shakeEar->setUpNoteHelp();
    
    _getMess = Tool::createTool(TOOL_GET_MESS, TOOL_TYPE_GET_MESS);
    this->addChild(_getMess,15);
    _getMess->setPosition(Point(visibleSize.width*0.5f, -visibleSize.height*0.13f));
    _getMess->cocos2d::Node::setScale(2.0f);
    _getMess->_savePositionOriginal = Point(visibleSize.width*0.5f, visibleSize.height*0.13f);
    _getMess->runAction(MoveTo::create(fTime, Point(visibleSize.width*0.5f, visibleSize.height*0.13f)));
    _getMess->_patient = _patient;
    _getMess->setUpNoteHelp();
    
    _catchBug = Tool::createTool(TOOL_CATCH_BUG, TOOL_TYPE_CATCH_BUG);
    this->addChild(_catchBug,15);
    _catchBug->setPosition(Point(visibleSize.width*0.66f, -visibleSize.height*0.13f));
    _catchBug->cocos2d::Node::setScale(2.0f);
    _catchBug->_savePositionOriginal = Point(visibleSize.width*0.66f, visibleSize.height*0.13f);
    _catchBug->runAction(MoveTo::create(fTime, Point(visibleSize.width*0.66f, visibleSize.height*0.13f)));
    _catchBug->_patient = _patient;
    _catchBug->setUpNoteHelp();

    _smallTable = Tool::createTool(TOOL_SMALL_TABLE, 100);
    this->addChild(_smallTable,14);
    _smallTable->setScale(1.1f,1.6f);
    _smallTable->setPosition(-visibleSize.width*0.5f, visibleSize.height*0.3f);
    
    _getWater = Tool::createTool(TOOL_GET_WATER, TOOL_TYPE_GET_WATER);
    this->addChild(_getWater,15);
    _getWater->setPosition(Point(visibleSize.width*0.91f, -visibleSize.height*0.5f));
    _getWater->cocos2d::Node::setScale(2.0f);
    _getWater->_savePositionOriginal = Point(visibleSize.width*0.91f, -visibleSize.height*0.3f);
    _getWater->runAction(MoveTo::create(fTime, Point(visibleSize.width*0.91f, -visibleSize.height*0.3f)));
    _getWater->_patient = _patient;
    _getWater->setUpNoteHelp();
}

#pragma mark - mess and bugs
void GamePlay::addMessesAndBugs(){
    Size visibleSize = Director::getInstance()->getVisibleSize();

    if (_patient->_typePatient == 1) {
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
        _longTai1->setVisible(false);
        _longTai2->setVisible(false);
        _longTai1->_tool = _scissor;
        _longTai2->_tool = _scissor;
        this->addChild(_longTai1,13);
        this->addChild(_longTai2,13);

        _longTai3 = MessObject::createMess(MESS_LONG_TAI_3, MESS_TYPE_LONG_TAI);
        _longTai3->setScale(1.5f);
        _longTai3->setPosition(visibleSize.width*0.32f,visibleSize.height*0.51f);
        _longTai3->setVisible(false);
        _longTai3->_tool = _scissor;
        this->addChild(_longTai3,13);
        
        _longTai4 = MessObject::createMess(MESS_LONG_TAI_2, MESS_TYPE_LONG_TAI);
        _longTai4->setScale(1.6f);
        _longTai4->setPosition(visibleSize.width*0.36f,visibleSize.height*0.50f);
        _longTai4->setVisible(false);
        _longTai4->_tool = _scissor;
        this->addChild(_longTai4,13);
        
        _longTai5 = MessObject::createMess(MESS_LONG_TAI_1, MESS_TYPE_LONG_TAI);
        _longTai5->setScale(2.0f);
        _longTai5->setPosition(visibleSize.width*0.36f,visibleSize.height*0.50f);
        _longTai5->setVisible(false);
        _longTai5->_tool = _scissor;
        this->addChild(_longTai5,13);
        
        _longTai6 = MessObject::createMess(MESS_LONG_TAI_1, MESS_TYPE_LONG_TAI);
        _longTai6->setScale(2.0f);
        _longTai6->setPosition(visibleSize.width*0.37f,visibleSize.height*0.49f);
        _longTai6->setVisible(false);
        _longTai6->_tool = _scissor;
        this->addChild(_longTai6,13);
        
        //Add ray tai
        _rayTai1 = MessObject::createMess(MESS_RAY_TAI_1, MESS_TYPE_RAY_TAI);
        _rayTai2 = MessObject::createMess(MESS_RAY_TAI_2, MESS_TYPE_RAY_TAI);
        _rayTai3 = MessObject::createMess(MESS_RAY_TAI_3, MESS_TYPE_RAY_TAI);
        _rayTai1->setScale(2.0f);
        _rayTai2->setScale(2.0f);
        _rayTai3->setScale(2.0f);
        _rayTai1->setVisible(false);
        _rayTai2->setVisible(false);
        _rayTai3->setVisible(false);
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
        _muTai1->setVisible(false);
        _muTai2->setVisible(false);
        _muTai1->setPosition(visibleSize.width*0.36f, visibleSize.height*0.66f);
        _muTai2->setPosition(visibleSize.width*0.46f, visibleSize.height*0.62f);
        this->addChild(_muTai1,13);
        this->addChild(_muTai2,13);
        
        //Add Mang tai
        _mangTai = MessObject::createMess(MESS_MANG_TAI_1, MESS_TYPE_MANG_TAI);
        _mangTai->setScale(2.0f);
        _mangTai->setPosition(Point(visibleSize.width*0.4f,visibleSize.height*0.6f));
        _mangTai->setVisible(false);
        _mangTai->_tool = _desiccate;
        this->addChild(_mangTai,13);
        
        //Add Dich tai
        _dichTai = MessObject::createMess(MESS_DICH_TAI_FULL, MESS_TYPE_DICH_TAI);
        _dichTai->setScale(2.0f);
        _dichTai->setPosition(Point(visibleSize.width*0.415f,visibleSize.height*0.555f));
        _dichTai->setVisible(false);
        _dichTai->_tool = _getWater;
        this->addChild(_dichTai,13);
    }
}

void GamePlay::showMessesAndBugs(){
    switch (AppDelegate::_typePatient) {
        case 1:
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
            break;
            
        default:
            break;
    }
}
