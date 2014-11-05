//
//  Helps.cpp
//  DoctorEar
//
//  Created by Trinh Van Duong on 10/31/14.
//
//

#include "Helps.h"
#include "Define.h"

Help::Help(){}

Help::~Help(){}

Help* Help::createHelp(std::string imageHelp, int typeTool){
    Help *help = new Help();
    if(help->initWithFile(imageHelp)){
        help->autorelease();
        help->initOptions(typeTool);
        return  help;
    }
    
    CC_SAFE_DELETE(help);
    return NULL;
}

//Ngang
void Help::initOptions(int typeHelp){
    _visibleSize = Director::getInstance()->getVisibleSize();
    _typeHelp = typeHelp;
}

//Doc
void Help::moveHandHorizontal(){
    auto action1 = ScaleTo::create(0.2f, 2.1f);
    auto action2 = MoveTo::create(0.7f, Point(this->getPosition().x + 130,this->getPosition().y));
    auto action3 = CallFunc::create( CC_CALLBACK_0(Help::setNormalState,this));
    
    this->runAction(Sequence::create(action1,action2,NULL));
    this->runAction(Sequence::create(DelayTime::create(1.0f),action3,NULL));
}

void Help::moveHandVertical(){
    auto action1 = ScaleTo::create(0.2f, 2.2f);
    auto action2 = MoveTo::create(0.7f, Point(this->getPosition().x,this->getPosition().y + 300));
    auto action3 = CallFunc::create( CC_CALLBACK_0(Help::setNormalState,this));
    
    this->runAction(Sequence::create(action1,action2,NULL));
    this->runAction(Sequence::create(DelayTime::create(1.0f),action3,NULL));
}

void Help::setNormalState(){
    this->setPosition(_savePosition);
    this->setScale(2.5f);
    
    if(_typeHand == 1){
        auto action1 = CallFunc::create( CC_CALLBACK_0(Help::moveHandVertical,this));
        this->runAction(Sequence::create(DelayTime::create(0.1f),action1,NULL));
    }else{
        auto action1 = CallFunc::create( CC_CALLBACK_0(Help::moveHandHorizontal,this));
        this->runAction(Sequence::create(DelayTime::create(0.1f),action1,NULL));
    }
}

void Help::setMuiTenNormal(){
    this->setScale(2.0f);
//    this->setOpacity();
    auto action1 = CallFunc::create( CC_CALLBACK_0(Help::setMuiTenScale,this));
    this->runAction(Sequence::create(DelayTime::create(0.1f),action1,NULL));
}

void Help::setMuiTenScale(){
    auto action1 = ScaleTo::create(0.2f, 1.0f);
//    auto action2 = FadeTo::create(0.2f, 0.5f);
    auto action3 = CallFunc::create( CC_CALLBACK_0(Help::setMuiTenNormal,this));
    
    this->runAction(Sequence::create(action1,NULL));
    this->runAction(Sequence::create(DelayTime::create(0.3f),action3,NULL));
}



#pragma mark - SHOW NOTE HELP

void Help::showHelp(float delay){
    auto action1 = CallFunc::create( CC_CALLBACK_0(Help::showNoteHelp,this));
    this->runAction(Sequence::create(DelayTime::create(delay),action1,NULL));
}

void Help::showNoteHelp(){
    this->setVisible(true);
    float s = this->getScale();
    auto action1 = ScaleTo::create(0.1f, s+ 0.3f);
    auto action2 = ScaleTo::create(0.1f, s);
    this->runAction(Sequence::create(action1,action2,NULL));
}

