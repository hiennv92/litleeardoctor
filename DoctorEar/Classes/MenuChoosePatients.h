//
//  MenuChoosePatients.h
//  DoctorEar
//
//  Created by Trinh Van Duong on 10/30/14.
//
//

#ifndef __DoctorEar__MenuChoosePatients__
#define __DoctorEar__MenuChoosePatients__

#include <stdio.h>
#include "cocos2d.h"

class MenuChoosePatients : public cocos2d::Layer{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    void playGameSelected(cocos2d::Ref* pSender);
    void returnHome(cocos2d::Ref* pSender);

    CREATE_FUNC(MenuChoosePatients);
};

#endif /* defined(__DoctorEar__MenuChoosePatients__) */
