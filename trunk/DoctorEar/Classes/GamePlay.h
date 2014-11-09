//
//  GamePlay.h
//  DoctorEar
//
//  Created by Trinh Van Duong on 10/30/14.
//
//

#ifndef __DoctorEar__GamePlay__
#define __DoctorEar__GamePlay__

#include <stdio.h>
#include "cocos2d.h"
#include "Patient.h"
#include "ToolObject.h"
#include "extensions/cocos-ext.h"
#include "MessObject.h"
#include "Bug.h"

class GamePlay : public cocos2d::Layer{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    
    cocos2d::Sprite *_spriteTable, *_background, *_backgroundBlackFont, *_earHoleScale,*spriteCircle, *_darkBackground;
    cocos2d::extension::ControlSlider *slider;
    cocos2d::MenuItemImage *_btnNextTools, *_btnBackTools, *_stopAdvanceLevelButton, *_drawButton, *_homeButton, *_saveButton, *_mailButton, *_faceButton;
    
    Patient *_patient;
    Tool *_keepEar, *_flashLight, *_scissor, *_getMess, *_catchBug, *_getWater, *_drugWater, *_injection, *_tamPon, *_desiccate, *_sprayChemicals, *_shakeEar, *_smallTable, *_ongSoi;

    //Tools advance
    Tool *_joystickBase, *_joystickButton, *_bottleGel, *_catchBugAdvance, *_lazer, *_getWaterAdvance, *_tamponAdvance;

    //Patien1
    MessObject *_rayTai1, *_rayTai2, *_rayTai3, *_munTai1, *_munTai2, *_munTai3,*_muTai1, *_muTai2, *_longTai1, *_longTai2, *_longTai3, *_longTai4, *_longTai5, *_longTai6;
    MessObject *_dichTai, *_nuocTai1, *_mangTai;
    Bug *_bug1, *_bug2, *_bug3, *_bug4;
    
    //Messes and Bugs advance level
    MessObject *_muTaiAd1, *_muTaiAd2, *_muTaiAd3, *_muTaiAd4, *_dirtyWater1, *_dirtyWater2, *_dirtyWater3, *_dirtyWater4;
    Bug *_bigBug1, *_bigBug2, *_bigBug3, *_bigBug4;
    
    int _tag, _pageTools;
    bool _isChangeBarSlider;
    PointArray* _array;

    CREATE_FUNC(GamePlay);

    //Touch handle
    void addEvents();
    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
    void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event);
    void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);

    void addSliderBar();
    void addFlashLight();
    void valueChanged(Ref *sender, cocos2d::extension::Control::EventType controlEvent);
    
    void addTools();
    void showTools();
    void addMessesAndBugs();
    void addMessesAndBugsAdvance();
    void showMessesAndBugs();
    void nextToolsSelected(cocos2d::Ref* pSender);
    void backToolsSelected(cocos2d::Ref* pSender);
    void stopAdvanceLevel(cocos2d::Ref* pSender);
    void backHome(cocos2d::Ref* pSender);
    void drawImage(cocos2d::Ref* pSender);
    void saveImage(cocos2d::Ref* pSender);
    void email(cocos2d::Ref* pSender);
    void facebook(cocos2d::Ref* pSender);
    
    void setupAdvanceLevel();
};

#endif /* defined(__DoctorEar__GamePlay__) */
