//
//  MainMenu.h
//  DoctorEar
//
//  Created by Trinh Van Duong on 10/30/14.
//
//

#ifndef __DoctorEar__MainMenu__
#define __DoctorEar__MainMenu__

#include <stdio.h>
#include "cocos2d.h"

class MainMenu : public cocos2d::Layer{
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    void playGameSelected(cocos2d::Ref* pSender);
    void soundOnOff(cocos2d::Ref* pSender);
    
    CREATE_FUNC(MainMenu);

private:
    cocos2d::MenuItemImage *_btnSoundOnOff;
    bool _soundOnOff;
};

#endif /* defined(__DoctorEar__MainMenu__) */
