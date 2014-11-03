//
//  Patient.cpp
//  DoctorEar
//
//  Created by Trinh Van Duong on 10/30/14.
//
//

#include "Patient.h"
#include "Define.h"

Patient::Patient(){}

Patient::~Patient(){}

Patient* Patient::createPatient(std::string imagePatient, int typePatient){
    Patient *patient = new Patient();
    if(patient->initWithFile(imagePatient)){
        patient->autorelease();
        patient->initOptions(typePatient);
//        patient->addEvents();
        return  patient;
    }
    CC_SAFE_DELETE(patient);
    return NULL;
}

void Patient::initOptions(int typePatient){
    _typePatient = typePatient;
    switch (typePatient) {
        case 1:
            _stringPathImage = PATIENT_BOY1_PATH;
            break;
        default:
            break;
    }
 
    //Create ear
    char strEar[100] = {0};
    sprintf(strEar, "%s_ear_3.png",_stringPathImage.c_str());
    _ear = Sprite::create(strEar);
    _ear->setPosition(Point(this->getContentSize().width*0.36f,this->getContentSize().height*0.61f));
    this->addChild(_ear,1);
    
    //Create eyebrow
    char strEyeBrow[100] = {0};
    sprintf(strEyeBrow, "%s_brow_1.png",_stringPathImage.c_str());
    _eyeBrow = Sprite::create(strEyeBrow);
    _eyeBrow->setPosition(Point(this->getContentSize().width*0.66f,this->getContentSize().height*0.84f));
    this->addChild(_eyeBrow,1);
    
    //Create eye
    _isSetCloseEye = true;
    char strEye[100] = {0};
    sprintf(strEye,"%s_eye_1.png",_stringPathImage.c_str());
    _eye = Sprite::create(strEye);
    _eye->setPosition(Point(this->getContentSize().width*0.74f,this->getContentSize().height*0.73f));
    this->addChild(_eye,1);
    
    this->schedule(schedule_selector(Patient::updateCloseEye), 10.0f);
    
    //Create mouth
    char strMouth[100] = {0};
    sprintf(strMouth,"%s_mouth_1.png",_stringPathImage.c_str());
    _mouth = Sprite::create(strMouth);
    _mouth->setPosition(Point(this->getContentSize().width*0.85f,this->getContentSize().height*0.45f));
    this->addChild(_mouth,1);
    
    char strMouthScare[100] = {0};
    sprintf(strMouthScare,"%s_mouth_2.png",_stringPathImage.c_str());
    _mouthScare = Sprite::create(strMouthScare);
    _mouthScare->setPosition(Point(this->getContentSize().width*0.85f,this->getContentSize().height*0.45f));
    this->addChild(_mouthScare,1);
    _mouthScare->setVisible(false);
    
    char strMouthSmile[100] = {0};
    sprintf(strMouthSmile,"%s_mouth_2.png",_stringPathImage.c_str());
    _mouthSmile = Sprite::create(strMouthScare);
    _mouthSmile->setPosition(Point(this->getContentSize().width*0.85f,this->getContentSize().height*0.45f));
    this->addChild(_mouthSmile,1);
    _mouthSmile->setVisible(false);
}

void Patient::createSmallEye(){
    char strSmallEye[100] = {0};
    sprintf(strSmallEye, "%s_eyeb.png",_stringPathImage.c_str());
    _smallEye = Sprite::create(strSmallEye);
    Size visibleSize = Director::getInstance()->getVisibleSize();
    _pointSavePositionSmallEye = Point(visibleSize.width*0.75f,visibleSize.height*0.725f);
    //    _smallEye->setPosition(_eye->getContentSize().width*0.55f, _eye->getContentSize().height*0.5f);
    _smallEye->setPosition(_pointSavePositionSmallEye);
    _smallEye->setScale(1.9f);
    (this->getParent())->getParent()->addChild(_smallEye, 5);
}

void Patient::createEarHole(){
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    //Add Ear Hole
    char strEarHole[100] = {0};
    sprintf(strEarHole, "%s_ear_2.png",_stringPathImage.c_str());
    
    _earHole = Sprite::create(strEarHole);
    _earHole->setPosition(Point(visibleSize.width*0.38f,visibleSize.height*0.6f));
    _earHole->setScale(1.3f);
    ((this->getParent())->getParent())->addChild(_earHole,13);
    
    char strEarTmp[100] = {0};
    sprintf(strEarTmp, "%s_ear_1.png",_stringPathImage.c_str());
    auto earTmp = Sprite::create(strEarTmp);
    earTmp->setScale(2.0f);
    earTmp->setPosition(Point(visibleSize.width*0.36f,visibleSize.height*0.61f));
    ((this->getParent())->getParent())->addChild(earTmp,14);
}

void Patient::setPositionSmallEye(Point p){
    _isSetCloseEye = false;
    
    float x1 = p.x;
    float x2 = _pointSavePositionSmallEye.x;
    float y1 = p.y;
    float y2 = _pointSavePositionSmallEye.y;
    float x,y;
    float tan;
    float angle;
    
    if(p.x < _pointSavePositionSmallEye.x && p.y < _pointSavePositionSmallEye.y){
        tan = ((x2-x1)/(y2-y1));
        angle = atanf(tan);
        
        x = x2 - 4.0f*sinf(angle);
        y = y2 - 3.0f*cosf(angle);
    }else if(p.x < _pointSavePositionSmallEye.x && p.y > _pointSavePositionSmallEye.y){
        tan = ((y1-y2)/(x2-x1));
        angle = atanf(tan);
        
        x = x2 - 5.0f*cosf(angle);
        y = y2 + 8.0f*sinf(angle);
    }else if(p.x > _pointSavePositionSmallEye.x && p.y > _pointSavePositionSmallEye.y){
        tan = ((y1-y2)/(x1-x2));
        angle = atanf(tan);
        
        x = x2 + 5.0f*cosf(angle);
        y = y2 + 8.0f*sinf(angle);
        
    }else if(p.x > _pointSavePositionSmallEye.x && p.y < _pointSavePositionSmallEye.y){
        tan = ((x1-x2)/(y2-y1));
        angle = atanf(tan);
        
        x = x2 + 6.0f*sinf(angle);
        y = y2 - 3.0f*cosf(angle);
    }
    _smallEye->setPosition(Point(x,y));
}

#pragma mark - EYE ANIMATION FUNCTIONS

void Patient::closeEyeAnimation(){
    Vector<SpriteFrame*> animFrames(2);
    char str[100] ={0};
    
    for (int i = 0; i < 3; i++) {
        sprintf(str, "%s_eye_%d.png",_stringPathImage.c_str(),i+1);
        Texture2D *texture = Director::getInstance()->getTextureCache()->addImage(str);
        auto frame = SpriteFrame::create(str,Rect(0,0,texture->getContentSize().width,texture->getContentSize().height));
        animFrames.pushBack(frame);
    }
    
    auto animation = Animation::createWithSpriteFrames(animFrames, 0.15f);
    auto animate = Animate::create(animation);
    RepeatForever *repeat = RepeatForever::create(animate);
    _eye->runAction(repeat);
    
    auto action1 = CallFunc::create( CC_CALLBACK_0(Patient::setCloseEye,this));
    this->runAction(Sequence::create(DelayTime::create(0.3f),action1,NULL));
    
    auto action2 = CallFunc::create( CC_CALLBACK_0(Patient::setHiddenSmallEye,this));
    this->runAction(Sequence::create(DelayTime::create(0.15f),action2,NULL));
}

void Patient::setHiddenSmallEye(){
    _smallEye->setVisible(false);
}

void Patient::setCloseEye(){
    _eye->stopAllActions();
    
    auto action1 = CallFunc::create( CC_CALLBACK_0(Patient::setOpenEye,this));
    this->runAction(Sequence::create(DelayTime::create(0.15f),action1,NULL));
}

void Patient::setOpenEye(){
    _smallEye->setVisible(true);
    char str[100] ={0};
    sprintf(str, "%s_eye_1.png",_stringPathImage.c_str());
    
    Texture2D *texture = Director::getInstance()->getTextureCache()->addImage(str);
    _eye->setTexture(texture);
}

void Patient::setEyeBrowScare(){
    Vector<SpriteFrame*> animFrames(2);
    char str1[100] = {0};
    char str2[100] = {0};

    sprintf(str1, "%s_brow_2.png",_stringPathImage.c_str());
    auto frame1 = SpriteFrame::create(str1,Rect(0,0,_eyeBrow->getContentSize().width,_eyeBrow->getContentSize().height)); //we assume that the sprites' dimentions are 40*40 rectangles.
    animFrames.pushBack(frame1);
    
    sprintf(str2, "%s_brow_3.png",_stringPathImage.c_str());
    auto frame2 = SpriteFrame::create(str2,Rect(0,0,_eyeBrow->getContentSize().width,_eyeBrow->getContentSize().height)); //we assume that the sprites' dimentions are 40*40 rectangles.
    animFrames.pushBack(frame2);
    
    auto animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
    auto animate = Animate::create(animation);
    
    RepeatForever *repeat = RepeatForever::create(animate);
    _eyeBrow->runAction(repeat);
}

void Patient::setEyeBrowSmile(){
    
}

void Patient::setEyeBrowNormal(){
    _eyeBrow->stopAllActions();
    char str[100] = {0};
    sprintf(str, "%s_brow_1.png",_stringPathImage.c_str());
    Texture2D *texture = Director::getInstance()->getTextureCache()->addImage(str);
    _eyeBrow->setTexture(texture);
}

#pragma mark - UPDATE FUNCTIONS

void Patient::updateCloseEye(float dt){
    if(_isSetCloseEye){
        this->closeEyeAnimation();
    }
}

void Patient::updatePatient(float dt){
    
}

void Patient::setMouthScare(){
    _mouth->setVisible(false);
    _mouthScare->setVisible(true);
    _mouthSmile->setVisible(false);
}

void Patient::setMouthSmile(){
    _mouth->setVisible(false);
    _mouthScare->setVisible(false);
    _mouthSmile->setVisible(true);
}

void Patient::setMouthNormal(){
    _mouth->setVisible(true);
    _mouthScare->setVisible(false);
    _mouthSmile->setVisible(false);
}

#pragma mark - TOUCH HANDLE

//void Patient::addEvents(){
//    auto listener = cocos2d::EventListenerTouchOneByOne::create();
//    listener->setSwallowTouches(false);
//    
//    listener->onTouchBegan = [&](cocos2d::Touch* touch, cocos2d::Event* event)
//    {
//        return true; // to indicate that we have consumed it
//    };
//    
//    
//    listener->onTouchEnded = [=](cocos2d::Touch* touch, cocos2d::Event* event)
//    {
//        Patient::touchEvent(touch);
//    };
//    
//    cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
//}
//
//void Patient::touchEvent(cocos2d::Touch* touch){
//    cocos2d::Vec2 p = touch->getLocation();
////    CCLOG("%f - %f",touch->getLocation().x,touch->getLocation().y);
//    
//    if(p.x < _pointSavePositionSmallEye.x && p.y < _pointSavePositionSmallEye.y){
//        CCLOG("PART I");
//        return;
//    }else if(p.x < _pointSavePositionSmallEye.x && p.y > _pointSavePositionSmallEye.y){
//        CCLOG("PART II");
//        return;
//    }else if(p.x > _pointSavePositionSmallEye.x && p.y > _pointSavePositionSmallEye.y){
//        CCLOG("PART III");
//        return;
//    }else if(p.x > _pointSavePositionSmallEye.x && p.y < _pointSavePositionSmallEye.y){
//        CCLOG("PART IV");
//        return;
//    }
//    
//    if(p.x == _pointSavePositionSmallEye.x && p.y < _pointSavePositionSmallEye.y){
//        CCLOG("Same 1");
//        return;
//    }else if(p.x < _pointSavePositionSmallEye.x && p.y == _pointSavePositionSmallEye.y){
//        CCLOG("Same 2");
//        return;
//    }else if(p.x == _pointSavePositionSmallEye.x && p.y > _pointSavePositionSmallEye.y){
//        CCLOG("Same 3");
//        return;
//    }else if(p.x > _pointSavePositionSmallEye.x && p.y == _pointSavePositionSmallEye.y){
//        CCLOG("Same 4");
//        return;
//    }
//}