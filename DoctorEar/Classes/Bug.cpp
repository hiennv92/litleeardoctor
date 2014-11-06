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
    _isPlaySoundEffect = false;
}

void Bug::updateBug(float dt){

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

void Bug::bugMove(Point start, Point finish, int typeMove){
    
}

void Bug::removeBug(){
    
}

void Bug::deleteBug(){
    
}