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
    this->setTag(-10);
}

void Bug::updateBug(float dt){
    if(_tool){
        if(_tool->_isTouch && !_isRemove && !_tool->_isCatchedBug){
            if(_tool->_typeTool == TOOL_TYPE_SPRAY_CHEMICALs){
                Rect pGetMess = _tool->getBoundingBox();
                Rect rect =  Rect(pGetMess.origin.x + pGetMess.size.width*0.6f,pGetMess.origin.y + pGetMess.size.height*0.9f,pGetMess.size.width*0.3f, pGetMess.size.height*0.1f);
                if(rect.intersectsRect(this->getBoundingBox())){
                    ParticleSystemQuad* particle = ParticleSystemQuad::create("yaoji.plist");
                    this->getParent()->addChild(particle,15);
                    particle->setScale(2.0f);
                    particle->setPosition(this->getPosition());
                    
                    removeBug();
                }
            }else{
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
        }
        
        if(this->_isCatching && _tool->_isCatchedBug && !_isRemove){
            if(_tool->_typeTool == TOOL_TYPE_CATCH_BUG){
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
            else if(_tool->_typeTool == TOOL_TYPE_CATCH_BUG_ADVANCE){
                removeBug();
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

void Bug::bugMoveInCircle(){
    auto actionCallBack = CallFunc::create(CC_CALLBACK_0(Bug::stopBugMoveAround,this));
    auto action = CardinalSplineBy::create(100, _arrayPointMove, 0);
    auto seq = Sequence::create(action,actionCallBack,nullptr);
    auto rotate = Sequence::create(RotateBy::create(100, _deltaRotate), NULL);
    this->runAction(seq);
    this->runAction(rotate);
    
}

void Bug::stopBugMoveAround(){
    this->stopAllActions();
    this->setPosition(_savePosition);
    this->setRotation(_originalRotate);
    this->animationBug(_typeBug);
    this->bugMoveInCircle();
}

void Bug::removeBug(){
    CCLOG("REMOVED BUG");
    if (_tool->_typeTool == TOOL_TYPE_CATCH_BUG) {
        _tool->setToolCatchNormal();
        _tool->_isCatchedBug = false;
        _tool->_isDroppedBug = false;
    }else if(_tool->_typeTool == TOOL_TYPE_CATCH_BUG_ADVANCE){
//        CC_SAFE_RELEASE(_arrayPointMove);

        auto spriteTmp = Sprite::createWithSpriteFrame(this->getSpriteFrame());
        _tool->addChild(spriteTmp,15);
        spriteTmp->setPosition(_tool->getContentSize().width*0.6, _tool->getContentSize().height);
        spriteTmp->setRotation(this->getRotation());
    }else if(_tool->_typeTool == TOOL_TYPE_SPRAY_CHEMICALs){
        auto spriteTmp = Sprite::createWithSpriteFrame(this->getSpriteFrame());
        this->getParent()->addChild(spriteTmp,15);
        spriteTmp->setPosition(this->getPosition());
        spriteTmp->setScale(1.3f);
        spriteTmp->setRotation(this->getRotation());
        spriteTmp->runAction(Sequence::create(MoveTo::create(1.0f, Point(this->getPositionX(),-100)), NULL));
    }
    
    this->_isRemove = true;
    this->stopAllActions();
    this->setVisible(false);
}

void Bug::deleteBug(){
    
}