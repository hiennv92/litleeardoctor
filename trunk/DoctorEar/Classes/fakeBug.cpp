//
//  fakeBug.cpp
//  DoctorEar
//
//  Created by Trinh Van Duong on 11/7/14.
//
//

#include "fakeBug.h"
#include "Define.h"

fakeBug::fakeBug(){}

fakeBug::~fakeBug(){}

fakeBug* fakeBug::createBug(std::string imageBug){
    fakeBug *bug = new fakeBug();
    if(bug->initWithFile(imageBug)){
        bug->autorelease();
        bug->initOptions();
        return  bug;
    }
    CC_SAFE_DELETE(bug);
    return NULL;
}

void fakeBug::initOptions(){
    _isTurnAround = false;
}

void fakeBug::animationBug(int typeBug){
    Vector<SpriteFrame*> animFrames(3);
    char str1[100] = {0};
    char str2[100] = {0};
    char str3[100] = {0};
    
    std::string img;
    if(typeBug == 1){
        img = BUG_BLUE_BUG;
    }else{
        img = BUG_RED_BUG;
    }
    
    sprintf(str1,"%s_1.png",img.c_str());
    auto frame1 = SpriteFrame::create(str1,Rect(0,0,this->getContentSize().width,this->getContentSize().height)); //we assume that the sprites' dimentions are 40*40 rectangles.
    animFrames.pushBack(frame1);
    
    sprintf(str2,"%s_2.png",img.c_str());
    auto frame2 = SpriteFrame::create(str2,Rect(0,0,this->getContentSize().width,this->getContentSize().height)); //we assume that the sprites' dimentions are 40*40 rectangles.
    animFrames.pushBack(frame2);
    
    sprintf(str3,"%s_3.png",img.c_str());
    auto frame3 = SpriteFrame::create(str2,Rect(0,0,this->getContentSize().width,this->getContentSize().height)); //we assume that the sprites' dimentions are 40*40 rectangles.
    animFrames.pushBack(frame3);
    
    auto animation = Animation::createWithSpriteFrames(animFrames, 0.05f);
    auto animate = Animate::create(animation);
    
    RepeatForever *repeat = RepeatForever::create(animate);
    this->runAction(repeat);
}

void fakeBug::bugMove(){
    if (!_isTurnAround) {
        if (_typeMove == 2) {
            auto action = CallFunc::create(CC_CALLBACK_0(fakeBug::bugMoveTurnAround,this));
            this->runAction(Sequence::create(MoveTo::create(1.0f, _pointFinish),action, NULL));
            this->runAction(Sequence::create(ScaleTo::create(1.0f, 1.0f), NULL));
        }else{
            auto action = CallFunc::create(CC_CALLBACK_0(fakeBug::bugMoveTurnAround,this));
            this->runAction(Sequence::create(MoveTo::create(1.0f, _pointFinish),action, NULL));
        }
        
        _isTurnAround = true;
    }else{
        if (_typeMove == 2) {
            auto action = CallFunc::create(CC_CALLBACK_0(fakeBug::bugMoveTurnAround,this));
            this->runAction(Sequence::create(RotateTo::create(0.5f,this->getRotation()-180),MoveTo::create(1.0f, _pointFinish),action, NULL));
            this->runAction(Sequence::create(ScaleTo::create(1.5f, 1.0f), NULL));
        }else{
            auto action = CallFunc::create(CC_CALLBACK_0(fakeBug::bugMoveTurnAround,this));
            this->runAction(Sequence::create(RotateTo::create(0.5f,this->getRotation()-180),MoveTo::create(1.0f, _pointFinish),action, NULL));
        }
    }
}

void fakeBug::bugMoveTurnAround(){
    if (_typeMove == 2) {
        auto action = CallFunc::create(CC_CALLBACK_0(fakeBug::bugMove,this));
        this->runAction(Sequence::create(RotateTo::create(0.5f,this->getRotation()+ 180),MoveTo::create(1.5f,_savePosition),action, NULL));
        
        this->runAction(Sequence::create(ScaleTo::create(2.0f, 0.2f), NULL));
    }else{
        auto action = CallFunc::create(CC_CALLBACK_0(fakeBug::bugMove,this));
        this->runAction(Sequence::create(RotateTo::create(0.5f,this->getRotation()+ 180),MoveTo::create(1.5f,_savePosition),action, NULL));
    }
}