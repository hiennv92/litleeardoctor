//
//  fakeBug.h
//  DoctorEar
//
//  Created by Trinh Van Duong on 11/7/14.
//
//

#ifndef __DoctorEar__fakeBug__
#define __DoctorEar__fakeBug__

#include <stdio.h>
#include "cocos2d.h"

USING_NS_CC;
class fakeBug:public Sprite{
public:
    fakeBug();
    ~fakeBug();
    
    static fakeBug* createBug(std::string imageBug);
    void initOptions();
    void bugMove();
    void bugMoveTurnAround();
    void animationBug(int typeBug);
    
    int _stateBUg, _typeMove, _typeBug;
    Point _savePosition, _pointFinish;
    bool _isTurnAround;
};

#endif /* defined(__DoctorEar__fakeBug__) */
