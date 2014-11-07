//
//  MenuChoosePatients.cpp
//  DoctorEar
//
//  Created by Trinh Van Duong on 10/30/14.
//
//

#include "MenuChoosePatients.h"
#include "Define.h"
#include "MainMenu.h"
#include "AppDelegate.h"
#include "GamePlay.h"

USING_NS_CC;

Scene* MenuChoosePatients::createScene(){
    auto scene = Scene::create();
    auto layer = MenuChoosePatients::create();
    scene->addChild(layer);
    return scene;
}

bool MenuChoosePatients::init(){
    if( !Layer::init()){
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    //Add background
    auto background = Sprite::create(MENU_PATIENTS_BG);
    background->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
    background->setScale(visibleSize.width/background->getContentSize().width, visibleSize.height/background->getContentSize().height);
    this->addChild(background,0);
    
    auto imagePlay = Sprite::create(MENU_PATIENTS_TABLE);
    imagePlay->setPosition(background->getContentSize().width/2,background->getContentSize().height*0.5f);
    background->addChild(imagePlay,5);
    
    auto imageTitle = Sprite::create(MENU_PATIENTS_TITLE);
    imageTitle->setPosition(background->getContentSize().width/2,background->getContentSize().height*0.9f);
    background->addChild(imageTitle,15);
    
    //Add buttons
    auto btnPlayGame1    = MenuItemImage::create(MENU_PATIENTS_BTN_PAT_1_1,MENU_PATIENTS_BTN_PAT_1_2, CC_CALLBACK_1(MenuChoosePatients::playGameSelected, this));
    auto btnPlayGame2    = MenuItemImage::create(MENU_PATIENTS_BTN_PAT_2_1,MENU_PATIENTS_BTN_PAT_2_2, CC_CALLBACK_1(MenuChoosePatients::playGameSelected, this));
    auto btnPlayGame3    = MenuItemImage::create(MENU_PATIENTS_BTN_PAT_3_1,MENU_PATIENTS_BTN_PAT_3_2, CC_CALLBACK_1(MenuChoosePatients::playGameSelected, this));
    auto btnPlayGame4    = MenuItemImage::create(MENU_PATIENTS_BTN_PAT_4_1,MENU_PATIENTS_BTN_PAT_4_2, CC_CALLBACK_1(MenuChoosePatients::playGameSelected, this));
    auto btnPlayGame5    = MenuItemImage::create(MENU_PATIENTS_BTN_PAT_5_1,MENU_PATIENTS_BTN_PAT_5_2, CC_CALLBACK_1(MenuChoosePatients::playGameSelected, this));
    auto btnPlayGame6    = MenuItemImage::create(MENU_PATIENTS_BTN_PAT_6_1,MENU_PATIENTS_BTN_PAT_6_2, CC_CALLBACK_1(MenuChoosePatients::playGameSelected, this));

    btnPlayGame1->setTag(1);
    btnPlayGame2->setTag(2);
    btnPlayGame3->setTag(3);
    btnPlayGame4->setTag(4);
    btnPlayGame5->setTag(5);
    btnPlayGame6->setTag(6);

    auto btnBackMenu = MenuItemImage::create(MENU_PATIENTS_BTN_HOME_1,MENU_PATIENTS_BTN_HOME_2,CC_CALLBACK_1(MenuChoosePatients::returnHome, this));
    
    btnPlayGame1->setPosition(background->getContentSize().width*0.3,background->getContentSize().height*0.68f);
    btnPlayGame2->setPosition(background->getContentSize().width*0.7,background->getContentSize().height*0.68f);
    btnPlayGame3->setPosition(background->getContentSize().width*0.3,background->getContentSize().height*0.42f);
    btnPlayGame4->setPosition(background->getContentSize().width*0.7,background->getContentSize().height*0.42f);
    btnPlayGame5->setPosition(background->getContentSize().width*0.3,background->getContentSize().height*0.16f);
    btnPlayGame6->setPosition(background->getContentSize().width*0.7,background->getContentSize().height*0.16f);

    
    btnBackMenu->setPosition(-background->getContentSize().width*0.3f, background->getContentSize().height*0.93);
    auto moveBtn = MoveTo::create(2.0f, Point(background->getContentSize().width*0.1f,background->getContentSize().height*0.93f));
    btnBackMenu->runAction(moveBtn);
    
    //Menu
    auto menu = Menu::create(btnBackMenu,btnPlayGame1,btnPlayGame2,btnPlayGame3,btnPlayGame4,btnPlayGame5,btnPlayGame6,NULL);
    menu->setPosition(Vec2::ZERO);
    background->addChild(menu,15);
    
    return true;
}


void MenuChoosePatients::playGameSelected(Ref* pSender){
    if(UserDefault::getInstance()->getBoolForKey(SOUND_ON_OFF)){
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(SOUND_SUB_BUTTON);
    }
    CCLOG("PATIENT: %d",((MenuItemImage*)pSender)->getTag());
    AppDelegate::_typePatient = ((MenuItemImage*)pSender)->getTag();
    
    auto newScene = GamePlay::createScene();
    auto trans    = TransitionSplitCols::create(.5f, newScene);
    Director::getInstance()->replaceScene(trans);
}

void MenuChoosePatients::returnHome(Ref* pSender){
    if(UserDefault::getInstance()->getBoolForKey(SOUND_ON_OFF)){
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(SOUND_MAIN_BUTTON);
    }
    
    auto newScene = MainMenu::createScene();
    auto trans    = TransitionSplitRows::create(.5f, newScene);
    Director::getInstance()->replaceScene(trans);
}
