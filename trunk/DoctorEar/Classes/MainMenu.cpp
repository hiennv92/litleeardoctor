//
//  MainMenu.cpp
//  DoctorEar
//
//  Created by Trinh Van Duong on 10/30/14.
//
//

#include "MainMenu.h"
#include "Define.h"
#include "MenuChoosePatients.h"
#include "AppDelegate.h"

USING_NS_CC;

Scene* MainMenu::createScene(){
    auto scene = Scene::create();
    auto layer = MainMenu::create();
    scene->addChild(layer);
    return scene;
}

bool MainMenu::init(){
    if( !Layer::init()){
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    //Add background
    auto background = Sprite::create(MAIN_BG);
    background->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
    background->setScale(visibleSize.width/background->getContentSize().width, visibleSize.height/background->getContentSize().height);
    this->addChild(background,0);
    
    auto imagePlay = Sprite::create(MAIN_IMAGE);
    imagePlay->setPosition(background->getContentSize().width/2,background->getContentSize().height*0.4f);
    background->addChild(imagePlay,5);
    
    auto imageTitle = Sprite::create(MAIN_TITLE);
    imageTitle->setPosition(background->getContentSize().width/2,background->getContentSize().height*0.75f);
    background->addChild(imageTitle,15);
    
    //Add buttons
    auto btnPlayGame    = MenuItemImage::create(MAIN_START_BUTTON_1,MAIN_START_BUTTON_2, CC_CALLBACK_1(MainMenu::playGameSelected, this));


    if(UserDefault::getInstance()->getBoolForKey(SOUND_ON_OFF)){
        if(!CocosDenshion::SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying()){
            CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic(SOUND_GAME);
            CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic(SOUND_MAIN_BUTTON);
            
            CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(SOUND_GAME,true);
        }

        _btnSoundOnOff = MenuItemImage::create(MAIN_SOUND_ON_1,MAIN_SOUND_ON_2,CC_CALLBACK_1(MainMenu::soundOnOff, this));
        _soundOnOff = true;
    }else{
        CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic(SOUND_GAME);
        CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic(SOUND_MAIN_BUTTON);
        
        _btnSoundOnOff = MenuItemImage::create(MAIN_SOUND_OFF,MAIN_SOUND_OFF,CC_CALLBACK_1(MainMenu::soundOnOff, this));
        _soundOnOff = false;
    }
    
    btnPlayGame->setPosition(background->getContentSize().width*0.50f,background->getContentSize().height*0.30f);
    _btnSoundOnOff->setPosition(background->getContentSize().width*0.9f, background->getContentSize().height*0.93);
    
    //Menu
    auto menu = Menu::create(btnPlayGame,_btnSoundOnOff, NULL);
    menu->setPosition(Vec2::ZERO);
    background->addChild(menu,15);
    return true;
}


void MainMenu::playGameSelected(Ref* pSender){
    if(UserDefault::getInstance()->getBoolForKey(SOUND_ON_OFF)){
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(SOUND_MAIN_BUTTON);
    }

    auto newScene = MenuChoosePatients::createScene();
    auto trans    = TransitionSplitCols::create(.5f, newScene);
    Director::getInstance()->replaceScene(trans);
}

void MainMenu::soundOnOff(Ref* pSender){
    if(_soundOnOff){
        CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
        CocosDenshion::SimpleAudioEngine::getInstance()->stopAllEffects();
        UserDefault::getInstance()->setBoolForKey(SOUND_ON_OFF, false);
        UserDefault::getInstance()->flush();

        auto normalImg = Sprite::create(MAIN_SOUND_OFF);
        auto selectImg = Sprite::create(MAIN_SOUND_OFF);
        
        ((MenuItemImage*)(pSender))->setNormalSpriteFrame(SpriteFrame::create(MAIN_SOUND_OFF, Rect(0, 0, normalImg->getContentSize().width, normalImg->getContentSize().height)));
        ((MenuItemImage*)(pSender))->setSelectedSpriteFrame(SpriteFrame::create(MAIN_SOUND_OFF, Rect(0, 0, selectImg->getContentSize().width, selectImg->getContentSize().height)));
        
        _soundOnOff = false;
    }else{
        UserDefault::getInstance()->setBoolForKey(SOUND_ON_OFF, true);
        UserDefault::getInstance()->flush();
        CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(SOUND_GAME,true);
    
        auto normalImg = Sprite::create(MAIN_SOUND_ON_1);
        auto selectImg = Sprite::create(MAIN_SOUND_ON_2);
        
        ((MenuItemImage*)(pSender))->setNormalSpriteFrame(SpriteFrame::create(MAIN_SOUND_ON_1, Rect(0, 0, normalImg->getContentSize().width, normalImg->getContentSize().height)));
        ((MenuItemImage*)(pSender))->setSelectedSpriteFrame(SpriteFrame::create(MAIN_SOUND_ON_2, Rect(0, 0, selectImg->getContentSize().width, selectImg->getContentSize().height)));

        _soundOnOff = true;
    }
}
