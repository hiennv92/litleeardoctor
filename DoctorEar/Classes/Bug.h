//
//  Bug.h
//  DoctorEar
//
//  Created by Trinh Van Duong on 11/2/14.
//
//

#ifndef __DoctorEar__Bug__
#define __DoctorEar__Bug__

#include <stdio.h>
#include "cocos2d.h"
#include "Patient.h"
#include "ToolObject.h"

USING_NS_CC;
class Bug:public Sprite{
public:
    Bug();
    ~Bug();
    
    static Bug* createBug(std::string imageBug);
    void updateBug(float dt);
    void initOptions();
    void removeBug();
    void deleteBug();
    void callCheckAgain();
    void bugMove(Point start, Point finish, int typeMove);
    void animationBug(int typeBug);

    int _stateBUg;
    bool _isRemove,_isCatching, _isCheckingBug, _isPlaySoundEffect;
    Size _visibleSize;
    Tool *_tool;
};

#endif /* defined(__DoctorEar__Bug__) */
