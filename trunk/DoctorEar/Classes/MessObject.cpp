//
//  MessObject.cpp
//  DoctorEar
//
//  Created by Trinh Van Duong on 10/31/14.
//
//

#include "MessObject.h"
#include "Define.h"

MessObject::MessObject(){}

MessObject::~MessObject(){}

MessObject* MessObject::createMess(std::string imageMess, int typeMess){
    MessObject *mess = new MessObject();
    if(mess->initWithFile(imageMess)){
        mess->autorelease();
        mess->initOptions(typeMess);
        return  mess;
    }
    CC_SAFE_DELETE(mess);
    return NULL;
}

void MessObject::initOptions(int typeMess){
    _visibleSize = Director::getInstance()->getVisibleSize();
    _typeMess = typeMess;
    _isRemove = false;
    this->schedule(schedule_selector(MessObject::updateMess));
    if(_typeMess == MESS_TYPE_DICH_TAI){
        _stateMess = 6;
    }else if(_typeMess == MESS_TYPE_MANG_TAI){
        _stateMess = 6;
    }else if(_typeMess == MESS_TYPE_NUOC_BAN){
        _stateMess = 4;
    }else if(_typeMess == MESS_TYPE_MUN){
        _stateMess = 3;
    }
    _isCheckingMess = true;
    _isPlaySoundEffect = false;
}

void MessObject::updateMess(float dt){
    if(_tool){
        if(_tool->_isTouch && !_isRemove){
            if(_typeMess == MESS_TYPE_RAY_TAI){
                Rect pGetMess = _tool->getBoundingBox();
                Rect rect =  Rect(pGetMess.origin.x,pGetMess.origin.y + pGetMess.origin.y*2.5/6,pGetMess.size.width, pGetMess.size.height/6);
                if(rect.intersectsRect(this->getBoundingBox())){
                    CCLOG("GET MESSSSSS");
                    this->_isRemove = true;
                    removeMess();
                }
            }
            
            if (_typeMess == MESS_TYPE_DICH_TAI && _isCheckingMess){
                Rect pGetMess = _tool->getBoundingBox();
                Rect rect =  Rect(pGetMess.origin.x,pGetMess.origin.y + pGetMess.size.height*18/20,pGetMess.size.width/6, pGetMess.size.height/20);
                
                if(this->getBoundingBox().containsPoint(Point(rect.origin.x + rect.size.width/2, rect.origin.y + rect.size.height/2))){
                    if(!_isPlaySoundEffect){
                        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(SOUND_GET_DIRTY_WATER);
                        _isPlaySoundEffect = true;
                    }
                    CCLOG("GET Dirty water");
                    removeMess();
                }else{
                    _isPlaySoundEffect = false;
                    CocosDenshion::SimpleAudioEngine::getInstance()->stopAllEffects();
                }
            }
            
            if (_typeMess == MESS_TYPE_LONG_TAI && !_tool->_isSet){
                Rect pGetMess = _tool->getBoundingBox();
                Rect rect =  Rect(pGetMess.origin.x + pGetMess.size.width*2/5,pGetMess.origin.y + pGetMess.size.height*4/5,pGetMess.size.width/5, pGetMess.size.height/5);
                
                if(rect.intersectsRect(this->getBoundingBox())){
                    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(SOUND_SCISSOR);
                    _tool->setScissorCutAnimation();
                    
                    CCLOG("remove fur");
                    removeMess();
                }
            }
            
            if(_typeMess == MESS_TYPE_MANG_TAI && _isCheckingMess){
                Rect pGetMess = _tool->getBoundingBox();
                Rect rect =  Rect(pGetMess.origin.x + pGetMess.size.width*2/3 ,pGetMess.origin.y + pGetMess.size.height*18/20,pGetMess.size.width/3, pGetMess.size.height/20);
                
                if(this->getBoundingBox().containsPoint(Point(rect.origin.x + rect.size.width/2, rect.origin.y + rect.size.height/2))){
                    if(!_isPlaySoundEffect){
                        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(SOUND_DECSICATE, true);
                        _isPlaySoundEffect = true;
                    }
                    CCLOG("GET Mang tai");
                    removeMess();
                }else{
                    _isPlaySoundEffect = false;
                    CocosDenshion::SimpleAudioEngine::getInstance()->stopAllEffects();
                }
            }
            //Dich tai
            if(_typeMess == MESS_TYPE_NUOC_BAN && _isCheckingMess){
                Rect pGetMess = _tool->getBoundingBox();
                Rect rect =  Rect(pGetMess.origin.x,pGetMess.origin.y + pGetMess.size.height*7/8,pGetMess.size.width, pGetMess.size.height/8);
                
                if(this->getBoundingBox().containsPoint(Point(rect.origin.x + rect.size.width/2, rect.origin.y + rect.size.height/2))){
                    CCLOG("hut nuoc ban");
                    removeMess();
                }
            }
            
            
            if(_typeMess == MESS_TYPE_MUN && !_tool->_isSet && _isCheckingMess){
                Rect pGetMess = _tool->getBoundingBox();
                Rect rect =  Rect(pGetMess.origin.x,pGetMess.origin.y,pGetMess.size.width, pGetMess.size.height/6);
                if(this->getBoundingBox().intersectsRect(rect)){
                    CCLOG("Set tay mun");
                    _isCheckingMess = false;
                    _tool->runAction(MoveTo::create(0.3f, Point(this->getPositionX() + 90,this->getPositionY() + 100)));
                    _tool->_isSet = true;                    

                    auto action = CallFunc::create(CC_CALLBACK_0(Tool::showMuiTen,_tool));
                    _tool->runAction(Sequence::create(DelayTime::create(0.4f),action, NULL));
                }
            }else if(_typeMess == MESS_TYPE_MUN && _tool->_isSet && !_isCheckingMess){
                if(_tool->_isDropDrugWater){
                    removeMess();
                    _tool->_isDropDrugWater = false;
                }
            }
        }
    }
}

void MessObject::removeMess(){
    if(_typeMess == MESS_TYPE_RAY_TAI){
        this->_isRemove = true;
        this->runAction(MoveTo::create(0.7f, Point(this->getPositionX(),0)));
    }
    else if(_typeMess == MESS_TYPE_DICH_TAI){
        _stateMess--;
        _isCheckingMess = false;
        
        if(_stateMess != 0){
            char str[100] = {0};
            sprintf(str, "mess/dichTai_%d.png",6 - _stateMess);
            Texture2D *texture = Director::getInstance()->getTextureCache()->addImage(str);
            this->setTexture(texture);
            
            auto action = CallFunc::create(CC_CALLBACK_0(MessObject::callCheckAgain,this));
            this->runAction(Sequence::create(DelayTime::create(.6f),action, NULL));
        }else{
            this->_isRemove = true;
            this->setVisible(false);
            CCLOG("REMOVE DIRTY WATER");
        }
        
    }
    else if(_typeMess == MESS_TYPE_LONG_TAI){
        this->_isRemove = true;
        this->runAction(Sequence::create(MoveTo::create(0.7f, Point(this->getPositionX(),0)), NULL));
    }
    else if(_typeMess == MESS_TYPE_MANG_TAI){
        _stateMess --;
        _isCheckingMess = false;
        
        if(_stateMess != 0){
            this->runAction(FadeTo::create(0.7f, _stateMess/6));

            auto action = CallFunc::create(CC_CALLBACK_0(MessObject::callCheckAgain,this));
            this->runAction(Sequence::create(DelayTime::create(0.8f),action, NULL));
        }else{
            this->_isRemove = true;
            this->setVisible(false);
            CCLOG("REMOVE MANG TAI");
            CocosDenshion::SimpleAudioEngine::getInstance()->stopAllEffects();
        }
    }
    else if(_typeMess == MESS_TYPE_NUOC_BAN){
        _stateMess --;
        _isCheckingMess = false;
        
        if(_stateMess != 0){
            this->runAction(FadeTo::create(0.7f, 255.0f - _stateMess*25));
            
            auto action = CallFunc::create(CC_CALLBACK_0(MessObject::callCheckAgain,this));
            this->runAction(Sequence::create(DelayTime::create(.8f),action, NULL));
        }else{
            this->_isRemove = true;
            this->setVisible(false);
            CCLOG("REMOVE Nuoc ban");
        }
    }
    else if(_typeMess == MESS_TYPE_MUN){
        _stateMess --;
        if(_stateMess != 0){
            CCLOG("DROP");
            if(_stateMess == 2)
                this->runAction(FadeTo::create(0.7f, 255.0f - 100));
            else
                this->runAction(FadeTo::create(0.7f, 255.0f - 150));

        }else{
            this->_isRemove = true;
            this->setVisible(false);
            CCLOG("REMOVE mun");
            _tool->_isSet = false;
//            auto actionMove = MoveTo::create(0.7f, _tool->_savePositionOriginal);
//            _tool->runAction(actionMove);
            _tool->_muiTen->setVisible(false);
//            _tool->runAction(RotateTo::create(0.7f, 0.0f));
            _tool->_isDropDrugWater = false;
        }
    }
}

void MessObject::callCheckAgain(){
    _isCheckingMess = true;
    if(_typeMess == MESS_TYPE_LONG_TAI){
        _isPlaySoundEffect = false;
    }
}