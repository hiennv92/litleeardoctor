//
//  Bug.cpp
//  DoctorEar
//
//  Created by Trinh Van Duong on 11/2/14.
//
//

#include "Bug.h"
#include "Define.h"

Bug::Bug(){}

Bug::~Bug(){}

Bug* Bug::createBug(std::string imageBug){
    Bug *bug = new Bug();
    if(bug->initWithFile(imageBug)){
        bug->autorelease();
        bug->initOptions();
        return  bug;
    }
    CC_SAFE_DELETE(bug);
    return NULL;
}

void Bug::initOptions(){
    _visibleSize = Director::getInstance()->getVisibleSize();
    _isRemove = false;
    this->schedule(schedule_selector(Bug::updateBug));
    
    _isCheckingBug = true;
    _isTurnAround = false;
    _isCatching = false;
    _isPlaySoundEffect = false;
}

void Bug::updateBug(float dt){
    if(_tool){
        if(_tool->_isTouch && !_isRemove && !_tool->_isCatchedBug){
            Rect pGetMess = _tool->getBoundingBox();
            Rect rect =  Rect(pGetMess.origin.x + pGetMess.size.width/3,pGetMess.origin.y + pGetMess.size.height*5/6,pGetMess.size.width /3, pGetMess.size.height/6);
            if(rect.intersectsRect(this->getBoundingBox())){
                CCLOG("Catch a bug");
                this->setVisible(false);
                this->_isCatching = true;
                _tool->_typeBugCatched = _typeBug;
                _tool->setToolCatchedBug();
            }
        }

        if(this->_isCatching && _tool->_isCatchedBug && !_isRemove){
            if(_tool->_isDroppedBug){
                removeBug();
            }
            
            if(_tool->_ignoreDropBug){
                CCLOG("Release bug");
                _tool->_ignoreDropBug = false;
                _tool->setToolCatchNormal();
                
                this->setVisible(true);
                this->_isCatching = false;
            }
        }
    }
}

void Bug::animationBug(int typeBug){
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

void Bug::bugMove(){
    if (!_isTurnAround) {
        if (_typeMove == 2) {
            auto action = CallFunc::create(CC_CALLBACK_0(Bug::bugMoveTurnAround,this));
            this->runAction(Sequence::create(MoveTo::create(1.0f, _pointFinish),action, NULL));
            this->runAction(Sequence::create(ScaleTo::create(1.0f, 1.0f), NULL));
        }else{
            auto action = CallFunc::create(CC_CALLBACK_0(Bug::bugMoveTurnAround,this));
            this->runAction(Sequence::create(MoveTo::create(1.0f, _pointFinish),action, NULL));
        }

        _isTurnAround = true;
    }else{
        if (_typeMove == 2) {
            auto action = CallFunc::create(CC_CALLBACK_0(Bug::bugMoveTurnAround,this));
            this->runAction(Sequence::create(RotateTo::create(0.5f,this->getRotation()-180),MoveTo::create(1.0f, _pointFinish),action, NULL));
            this->runAction(Sequence::create(ScaleTo::create(1.5f, 1.0f), NULL));
        }else{
            auto action = CallFunc::create(CC_CALLBACK_0(Bug::bugMoveTurnAround,this));
            this->runAction(Sequence::create(RotateTo::create(0.5f,this->getRotation()-180),MoveTo::create(1.0f, _pointFinish),action, NULL));
        }
    }
}

void Bug::bugMoveTurnAround(){
    if (_typeMove == 2) {
        auto action = CallFunc::create(CC_CALLBACK_0(Bug::bugMove,this));
        this->runAction(Sequence::create(RotateTo::create(0.5f,this->getRotation()+ 180),MoveTo::create(1.5f,_savePosition),action, NULL));
        
        this->runAction(Sequence::create(ScaleTo::create(2.0f, 0.2f), NULL));
    }else{
        auto action = CallFunc::create(CC_CALLBACK_0(Bug::bugMove,this));
        this->runAction(Sequence::create(RotateTo::create(0.5f,this->getRotation()+ 180),MoveTo::create(1.5f,_savePosition),action, NULL));
    }
}

void Bug::removeBug(){
    CCLOG("REMOVED BUG");
    _tool->_isDroppedBug = false;
    _tool->_isCatchedBug = false;
    _tool->setToolCatchNormal();
    this->_isRemove = true;
    this->stopAllActions();
    this->setVisible(false);
}

void Bug::deleteBug(){
    
}