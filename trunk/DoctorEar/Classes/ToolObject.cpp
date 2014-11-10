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
    _isMoved = false;
    //Use for keepear tool and scissor
    _isSet = false;
    //Use for small table
    _velocityMoveSmallTable = Point(1.0f,0.0f);
    _velocityMoveOngSoi = Point(0.0f,0.0f);
    //use for drug water
    _isDropDrugWater = false;
    //Use for catched bug
    _isCatchedBug = false;
    _isDroppedBug = false;
    _ignoreDropBug = false;
    _countNumberBugCatched = 0;
    
    _isMoveClean = false;
    //
    _right = false;
    _left = false;
    _down = false;
    _up = false;
    
    if(_typeTool == 100 || _typeTool == 200){
        this->schedule(schedule_selector(Tool::updateTool));
        _startMove = false;
    }
    
    if (_typeTool == TOOL_TYPE_LAZER) {
        this->setupLazer();
    }
}

//Use for small table
void Tool::updateTool(float dt){
    if(_typeTool == 100){
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
        
        if(_toolCatchBug->_isCatchedBug){
            Rect pGetMess = _toolCatchBug->getBoundingBox();
            Rect rect =  Rect(pGetMess.origin.x + pGetMess.size.width/3,pGetMess.origin.y + pGetMess.size.height*5/6,pGetMess.size.width /3, pGetMess.size.height/6);
            if(this->getBoundingBox().intersectsRect(rect)){
                CCLOG("Drop bug");
                _countNumberBugCatched ++;
                
                char str1[100] = {0};
                std::string img;
                if(_toolCatchBug->_typeBugCatched == 1){
                    img = BUG_BLUE_BUG;
                }else{
                    img = BUG_RED_BUG;
                }
                sprintf(str1,"%s_1.png",img.c_str());
                
                fakeBug *fBug = fakeBug::createBug(str1);
                switch (_countNumberBugCatched) {
                    case 1:
                        fBug->setRotation(90);
                        fBug->setPosition(Point(arc4random()%(int)this->getContentSize().width/4 + this->getContentSize().width/2, arc4random()%(int)this->getContentSize().height/4 + this->getContentSize().height/4));
                        fBug->_savePosition = fBug->getPosition();
                        fBug->_pointFinish = Point(fBug->_savePosition.x - 100, fBug->_savePosition.y);
                        break;
                    case 2:
                        fBug->setRotation(-135);
                        fBug->setPosition(Point(arc4random()%(int)this->getContentSize().width/4 + this->getContentSize().width/4, arc4random()%(int)this->getContentSize().height/4 + this->getContentSize().height/6));
                        fBug->_savePosition = fBug->getPosition();
                        fBug->_pointFinish = Point(fBug->_savePosition.x + 50, fBug->_savePosition.y + 50);
                        break;
                    case 3:
                        fBug->setRotation(135);
                        fBug->setPosition(Point(arc4random()%(int)this->getContentSize().width/4 + this->getContentSize().width/2, arc4random()%(int)this->getContentSize().height/4 + this->getContentSize().height/6));
                        fBug->_savePosition = fBug->getPosition();
                        fBug->_pointFinish = Point(fBug->_savePosition.x - 50, fBug->_savePosition.y + 50);
                        break;
                    case 4:
                        fBug->setRotation(105);
                        fBug->setPosition(Point(arc4random()%(int)this->getContentSize().width/2 + this->getContentSize().width/2, arc4random()%(int)this->getContentSize().height/4 + this->getContentSize().height/4));
                        fBug->_savePosition = fBug->getPosition();
                        fBug->_pointFinish = Point(fBug->_savePosition.x - 60, fBug->_savePosition.y + 30);
                        break;
                    default:
                        break;
                }
                
                fBug->_typeMove = 1;
                fBug->setScale(0.7f);
                fBug->animationBug(_toolCatchBug->_typeBugCatched);
                fBug->bugMove();
                this->addChild(fBug,10);
                
                _toolCatchBug->_isDroppedBug = true;
            }
        }
    }

    if (_typeTool == 200) {
        if(_isTouch){
            if (_right && !_left) {
                _velocityMoveOngSoi = Point(2.0f,0);
            }else if(!_right && _left){
                _velocityMoveOngSoi = Point(-2.0f,0);
            }else{
                _velocityMoveOngSoi = Point(0,_velocityMoveOngSoi.y);
            }
            
            if (_up && !_down) {
                _velocityMoveOngSoi = Point(0,2.0f);
            }else if(!_up && _down){
                _velocityMoveOngSoi = Point(0,-2.0f);
            }else{
                _velocityMoveOngSoi = Point(_velocityMoveOngSoi.x,0);
            }
        }else{
            _velocityMoveOngSoi = Point(0.0f,0.0f);
            _up = false;
            _down = false;
            _right = false;
            _left = false;
        }
        
        //
        Point pointTmp = _toolOngSoi->getPosition() + _velocityMoveOngSoi;
        
        if (pointTmp.y < _toolOngSoi->getContentSize().height/6.2f){
            _down = false;
            _velocityMoveOngSoi = Point(_velocityMoveOngSoi.x,0);
        }
        else if(pointTmp.y > _visibleSize.height - _toolOngSoi->getContentSize().height/4) {
            _up = false;
            _velocityMoveOngSoi = Point(_velocityMoveOngSoi.x,0);
        }
        else if (pointTmp.x < _toolOngSoi->getContentSize().width /4.9f){
            _left = false;
            _velocityMoveOngSoi = Point(0,_velocityMoveOngSoi.y);
        }
        else if(pointTmp.x > _visibleSize.width - _toolOngSoi->getContentSize().width/4.9f){
            _right = false;
            _velocityMoveOngSoi = Point(0,_velocityMoveOngSoi.y);
        }else{
            
        }
        _toolOngSoi->setPosition(_toolOngSoi->getPosition() + _velocityMoveOngSoi);
        _circle->setPosition(_circle->getPosition() + _velocityMoveOngSoi);
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
    (this->getParent())->addChild(_muiTen,15);
    _muiTen->setVisible(false);
    
    if(_typeTool == TOOL_TYPE_WATER_DRUG){
        _muiTen->setRotation(-45.0f);
        _muiTen->setScale(2.0f);
        _muiTen->setPosition(this->getPositionX() + 110,this->getPositionY() - 20);

        //Set drop drug
        _dropDrug = Sprite::create(TOOL_DRUG_WATER_DROP);
        (this->getParent())->addChild(_dropDrug,15);
        _dropDrug->setScale(2.0f);
        _dropDrug->setVisible(false);
    }else if(_typeTool == TOOL_TYPE_INJECTION){
        _muiTen->setRotation(180.0f);
        _muiTen->setScale(1.2f);
        _muiTen->setPosition(this->getPositionX() + 100,this->getPositionY() + 60);
        _muiTen->_savePosition = _muiTen->getPosition();
    }
}

void Tool::showMuiTen(){
    if(_typeTool == TOOL_TYPE_INJECTION){
        _muiTen->stopAllActions();
        _muiTen->setPosition(this->getPositionX() + 80,this->getPositionY());
        _muiTen->_savePosition = _muiTen->getPosition();
        _muiTen->setMuiTenMove();
    }else{
        _muiTen->setPosition(this->getPositionX() + 110,this->getPositionY() - 40);
    }
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
        case TOOL_TYPE_ONG_SOI:
            _noteHelp = Help::createHelp(HELP_NOTE_13, TOOL_TYPE_ONG_SOI);
            _noteHelp->setPosition(Point(_visibleSize.width*0.16f,_visibleSize.height*0.7f));
            break;
        case TOOL_TYPE_LAZER:
            _noteHelp = Help::createHelp(HELP_NOTE_17, TOOL_TYPE_LAZER);
            _noteHelp->setPosition(Point(_visibleSize.width*0.88f,_visibleSize.height*0.85f));
            break;
        case TOOL_TYPE_CATCH_BUG_ADVANCE:
            _noteHelp = Help::createHelp(HELP_NOTE_14, TOOL_TYPE_CATCH_BUG_ADVANCE);
            _noteHelp->setPosition(Point(_visibleSize.width*0.88f,_visibleSize.height*0.85f));
            break;
        case TOOL_TYPE_TAM_BONG_ADVANCE:
            _noteHelp = Help::createHelp(HELP_NOTE_15, TOOL_TYPE_TAM_BONG_ADVANCE);
            _noteHelp->setPosition(Point(_visibleSize.width*0.88f,_visibleSize.height*0.85f));
            break;
        case TOOL_TYPE_GEL:
            _noteHelp = Help::createHelp(HELP_NOTE_18, TOOL_TYPE_GEL);
            _noteHelp->setPosition(Point(_visibleSize.width*0.88f,_visibleSize.height*0.85f));
            break;
        case TOOL_TYPE_GET_WATER_ADVANCE:
            _noteHelp = Help::createHelp(HELP_NOTE_16, TOOL_TYPE_GET_WATER_ADVANCE);
            _noteHelp->setPosition(Point(_visibleSize.width*0.88f,_visibleSize.height*0.85f));
            break;
        default:
            break;
    }
    
    _noteHelp->setScale(2.0f);
    (this->getParent())->addChild(_noteHelp,17);
    _noteHelp->setVisible(false);
}

void Tool::setupLazer(){
    _lazer = Sprite::create(TOOL_TIA_LAZER);
    this->addChild(_lazer,15);
    _lazer->setPosition(Point(this->getContentSize().width*0.65f,this->getContentSize().height*1.1f));
    _lazer->setVisible(false);
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
    auto action = CallFunc::create(CC_CALLBACK_0(Tool::setTouchAvailable,this));
    if(_isTouch && _typeTool != 2 && _typeTool != 200){
        _isTouch = false;
        _isMoveClean = false;
        _noteHelp->setVisible(false);
        _noteHelp->setScale(2.0f);
        _patient->setMouthNormal();
        _patient->setEyeBrowNormal();

        if(_typeTool == TOOL_TYPE_SCISSOR){
            this->stopAllActions();
            this->setScissorClose();
        }
        
        if(_typeTool == TOOL_TYPE_LAZER){
            _lazer->setVisible(false);
        }
        
        if(_typeTool == TOOL_TYPE_SHAKE_EAR){
            this->stopAllActions();
        }
        
        if(_typeTool != TOOL_TYPE_WATER_DRUG && _typeTool != TOOL_TYPE_INJECTION && _isMoved){
            if(_typeTool == TOOL_TYPE_CATCH_BUG_ADVANCE && _isCatchedBug){
                auto actionMove1 = MoveTo::create(0.7f, Point(_savePositionOriginal.x,_savePositionOriginal.y - this->getContentSize().height/2));
                this->runAction(Sequence::create(actionMove1,action,actionMove, NULL));
            }else{
                this->runAction(Sequence::create(actionMove,action, NULL));
            }
        }
        
        if(_typeTool == TOOL_TYPE_CATCH_BUG){
            //Small table
            ((GamePlay*)(this->getParent()))->_smallTable->_startMove = true;
            ((GamePlay*)(this->getParent()))->_smallTable->_isTouch = false;
            if(_isCatchedBug){
                this->_ignoreDropBug = true;
            }
        }
        
        if(_typeTool >= TOOL_TYPE_SCISSOR && _typeTool != TOOL_TYPE_WATER_DRUG && _typeTool < TOOL_TYPE_LAZER){
            if(((GamePlay*)(this->getParent()))->_pageTools == 1){
                ((GamePlay*)(this->getParent()))->_btnNextTools->setVisible(true);
            }else if(((GamePlay*)(this->getParent()))->_pageTools == 2){
                ((GamePlay*)(this->getParent()))->_btnBackTools->setVisible(true);
                ((GamePlay*)(this->getParent()))->_btnNextTools->setVisible(true);
            }
            else if(((GamePlay*)(this->getParent()))->_pageTools == 3){
                ((GamePlay*)(this->getParent()))->_btnBackTools->setVisible(false);
                ((GamePlay*)(this->getParent()))->_btnNextTools->setVisible(true);
            }
        }
        
        if((_typeTool == TOOL_TYPE_WATER_DRUG || _typeTool == TOOL_TYPE_INJECTION)  && !_isSet){
            this->runAction(RotateTo::create(0.7f, 0.0f));
            if (_isMoved) {
                this->runAction(Sequence::create(actionMove,action, NULL));
            }

            if(((GamePlay*)(this->getParent()))->_pageTools == 1){
                ((GamePlay*)(this->getParent()))->_btnNextTools->setVisible(true);
            }else if(((GamePlay*)(this->getParent()))->_pageTools == 2){
                ((GamePlay*)(this->getParent()))->_btnBackTools->setVisible(true);
                ((GamePlay*)(this->getParent()))->_btnNextTools->setVisible(true);
            }
            else if(((GamePlay*)(this->getParent()))->_pageTools == 3){
                ((GamePlay*)(this->getParent()))->_btnBackTools->setVisible(false);
                ((GamePlay*)(this->getParent()))->_btnNextTools->setVisible(true);
            }
        }else if((_typeTool == TOOL_TYPE_WATER_DRUG || _typeTool == TOOL_TYPE_INJECTION) && _isSet){
            _isTouch = true;
            _noteHelp->setVisible(true);
        }
    }else if(_isTouch && _typeTool == 200){
        _isTouch = false;
        this->runAction(Sequence::create(MoveTo::create(0.2f, _savePositionOriginal),action, NULL));
    }
    
    CocosDenshion::SimpleAudioEngine::getInstance()->stopAllEffects();
}

void Tool::setTouchAvailable(){
    _isMoved = false;
    //if type tool is catch bug
    _isCatchedBug = false;
    if (_typeTool == TOOL_TYPE_CATCH_BUG_ADVANCE) {
        this->setToolCatchNormal();
        this->removeAllChildren();
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

    }
    else if(_typeTool == TOOL_TYPE_ONG_SOI){
        if(vec.x < _visibleSize.width*0.65f){
            vec.x = _visibleSize.width*0.65f;
        }
        if (vec.x > _visibleSize.width*1.1f) {
            vec.x = _visibleSize.width*1.1f;
        }
        if (vec.y < _visibleSize.height*0.3f) {
            vec.y = _visibleSize.height*0.3f;
        }
        if (vec.y > _visibleSize.height*0.8f ) {
            vec.y = _visibleSize.height*0.8f;
        }
        this->setPosition (vec);
    }
    else if(_typeTool == TOOL_TYPE_LAZER){
        if (vec.y > _visibleSize.height*0.4f ) {
            vec.y = _visibleSize.height*0.4f;
        }
        this->setPosition (vec);
    }
    else if(_typeTool == TOOL_TYPE_CATCH_BUG_ADVANCE){
        if (vec.y > _visibleSize.height*0.4f ) {
            vec.y = _visibleSize.height*0.4f;
        }
        this->setPosition (vec);
    }
    else if(_typeTool == 200) {
        float x =  fabsf(vec.x - _savePositionOriginal.x);
        float y =  fabsf(vec.y - _savePositionOriginal.y);
    
        if (sqrtf(x*x + y*y) < 10) {
            this->setPosition (vec);
        }
//        else{
//            CCLOG("OUT");
//        }
        this->changeAngle();
    }
    else{
        this->setPosition (vec);
    }
    this->_isMoved = true;

    auto action = CallFunc::create(CC_CALLBACK_0(Tool::setMoveCleanFalse,this));
    this->runAction(Sequence::create(DelayTime::create(0.2f),action, NULL));
}

void Tool::setMoveCleanFalse(){
    this->_isMoveClean = false;
}

void Tool::changeAngle()
{
//    if(this->getPositionX() >= _savePositionOriginal.x && (this->getPositionY() < _savePositionOriginal.y + 10*cosf(45) || this->getPositionY() > _savePositionOriginal.y - 10*cosf(45))){
////        CCLOG("PHAI");
//        _right = true;
//        _left = false;
//        _up = false;
//        _down = false;
//    }else if(this->getPositionX() < _savePositionOriginal.x && (this->getPositionY() < _savePositionOriginal.y + 10*cosf(45) || this->getPositionY() > _savePositionOriginal.y - 10*cosf(45))){
////        CCLOG("TRAI");
//        _right = false;
//        _left = true;
//        _up = false;
//        _down = false;
//    }
    
    float x1 = this->getPosition().x;
    float x2 = _savePositionOriginal.x;
    float y1 = this->getPosition().y;
    float y2 = _savePositionOriginal.y;
    float tan;
    float angle = 0;
    
    if(this->getPositionX() < _savePositionOriginal.x && this->getPositionY() <= _savePositionOriginal.y){

        tan = ((x2-x1)/(y2-y1));
        angle = atanf(tan);
        angle = (90 - angle*180/M_PI) - 180;
 
    }else if(this->getPositionX() < _savePositionOriginal.x && this->getPositionY() > _savePositionOriginal.y){
        tan = ((y1-y2)/(x2-x1));
        angle = atanf(tan);
        
        angle = 180 - angle*180/M_PI;
    }else if(this->getPositionX() > _savePositionOriginal.x && this->getPositionY() > _savePositionOriginal.y){
        tan = ((y1-y2)/(x1-x2));
        angle = atanf(tan);
        
        angle = angle*180/M_PI;

        
    }else if(this->getPositionX() > _savePositionOriginal.x && this->getPositionY() < _savePositionOriginal.y){
        tan = ((x1-x2)/(y2-y1));
        angle = atanf(tan);
        
        angle = angle*180/M_PI - 90;
    }

    if((angle <= 45 && angle > 0) ||(angle < 0 && angle > -45)){
        _right = true;
        _left = false;
        _up = false;
        _down = false;
    }else if(angle <= -45 && angle > -135){
        _right = false;
        _left = false;
        _up = false;
        _down = true;
    }else if((angle <= -135 && angle >= -180) || (angle <= 180 && angle > 135)){
        _right = false;
        _left = true;
        _up = false;
        _down = false;
    }else if(angle <= 135 && angle > 45){
        _right = false;
        _left = false;
        _up = true;
        _down = false;
    }
    
    if(angle == 0){
        if (x1 > x2 && y1 == y2) {
            _right = true;
            _left = false;
            _up = false;
            _down = false;
        }
        else if(x1 < x2 && y1 == y2){
            _right = false;
            _left = true;
            _up = false;
            _down = false;
        }
        else if(x1 == x2 && y1 > y2){
            _right = false;
            _left = false;
            _up = true;
            _down = false;
        }else if(x1 == x2 && y1 < y2){
            _right = false;
            _left = false;
            _up = false;
            _down = true;
        }
    }
//    CCLOG("ANGLE: %f",angle);
}


//Flash light
void Tool::turnOnFlashLight(Ref *pSender){
    if(!_isTurnOnFlashLight){
        if(UserDefault::getInstance()->getBoolForKey(SOUND_ON_OFF)){
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(SOUND_MAIN_BUTTON);
        }
        
        auto normalImg = Sprite::create(TOOL_DENPIN_BUTTON_ON);
        
        ((MenuItemImage*)(pSender))->setNormalSpriteFrame(SpriteFrame::create(TOOL_DENPIN_BUTTON_ON, Rect(0, 0, normalImg->getContentSize().width, normalImg->getContentSize().height)));
         ((MenuItemImage*)(pSender))->setSelectedSpriteFrame(SpriteFrame::create(TOOL_DENPIN_BUTTON_ON, Rect(0, 0, normalImg->getContentSize().width, normalImg->getContentSize().height)));
        
        auto light = Sprite::create(TOOL_LIGHT_FLASH);
        light->setPosition(Point(_visibleSize.width*0.34f,_visibleSize.height*0.56f));
        light->setScale(1.7f);
        light->setTag(200);
        (this->getParent())->addChild(light,12);
        
        _isTurnOnFlashLight = true;
        _noteHelp->setVisible(false);
        _noteHelp->removeFromParentAndCleanup(true);
        
        ((GamePlay *)(this->getParent()))->showTools();
        ((GamePlay*)(this->getParent()))->showMessesAndBugs();
    }
}

//Set shake ear animation
void Tool::setShakeEarAnimation(){
    Vector<SpriteFrame*> animFrames(4);
    char str1[100] = {0};
    char str2[100] = {0};
    char str3[100] = {0};
    char str4[100] = {0};
    
    sprintf(str1, "tools/timao0.png");
    auto frame1 = SpriteFrame::create(str1,Rect(0,0,this->getContentSize().width,this->getContentSize().height)); //we assume that the sprites' dimentions are 40*40 rectangles.
    animFrames.pushBack(frame1);
    
    sprintf(str2, "tools/timao1.png");
    auto frame2 = SpriteFrame::create(str2,Rect(0,0,this->getContentSize().width,this->getContentSize().height)); //we assume that the sprites' dimentions are 40*40 rectangles.
    animFrames.pushBack(frame2);
    
    sprintf(str3, "tools/timao2.png");
    auto frame3 = SpriteFrame::create(str1,Rect(0,0,this->getContentSize().width,this->getContentSize().height)); //we assume that the sprites' dimentions are 40*40 rectangles.
    animFrames.pushBack(frame3);
    
    sprintf(str4, "tools/timao3.png");
    auto frame4 = SpriteFrame::create(str1,Rect(0,0,this->getContentSize().width,this->getContentSize().height)); //we assume that the sprites' dimentions are 40*40 rectangles.
    animFrames.pushBack(frame4);
    
    auto animation = Animation::createWithSpriteFrames(animFrames, 0.05f);
    auto animate = Animate::create(animation);
    
    RepeatForever *repeat = RepeatForever::create(animate);
    this->runAction(repeat);
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

void Tool::setInjectionFull(){
    char str[100] = {0};
    sprintf(str, TOOL_INJECTION_USED);
    Texture2D *texture = Director::getInstance()->getTextureCache()->addImage(str);
    this->setTexture(texture);
    this->_muiTen->setVisible(false);
}

void Tool::setInjectionNormal(){
    char str[100] = {0};
    sprintf(str, TOOL_INJECTION_NORMAL);
    Texture2D *texture = Director::getInstance()->getTextureCache()->addImage(str);
    this->setTexture(texture);
    this->_muiTen->setVisible(false);
}

void Tool::setToolCatchedBug(){
    _isCatchedBug = true;
    char str[100] = {0};
    if(_typeTool == TOOL_TYPE_CATCH_BUG){
        if (_typeBugCatched == 1) {
            sprintf(str, TOOL_CATCH_BUG_2);
        }else{
            sprintf(str, TOOL_CATCH_BUG_1);
        }

    }else if(_typeTool == TOOL_TYPE_CATCH_BUG_ADVANCE){
        sprintf(str, TOOL_CATCH_BUG_ADVANCE_DONE);
    }

    Texture2D *texture = Director::getInstance()->getTextureCache()->addImage(str);
    this->setTexture(texture);
}

void Tool::setToolCatchNormal(){
    char str[100] = {0};
    if(_typeTool == TOOL_TYPE_CATCH_BUG){
        sprintf(str, TOOL_CATCH_BUG);
    }else if(_typeTool == TOOL_TYPE_CATCH_BUG_ADVANCE){
        sprintf(str, TOOL_CATCH_BUG_ADVANCE);
    }

    Texture2D *texture = Director::getInstance()->getTextureCache()->addImage(str);
    this->setTexture(texture);
}

