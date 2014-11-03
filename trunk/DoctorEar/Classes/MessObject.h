//
//  MessObject.h
//  DoctorEar
//
//  Created by Trinh Van Duong on 10/31/14.
//
//

#ifndef __DoctorEar__MessObject__
#define __DoctorEar__MessObject__

#include <stdio.h>
#include "cocos2d.h"
#include "Patient.h"
#include "ToolObject.h"

USING_NS_CC;
class MessObject:public Sprite{
public:
    MessObject();
    ~MessObject();
    
    static MessObject* createMess(std::string imageMess, int typeMess);
    void updateMess(float dt);
    void initOptions(int typeMess);
    void removeMess();
    
    int _typeMess;
    bool _isRemove;
    Size _visibleSize;
    Tool *_tool;
};

#endif /* defined(__DoctorEar__MessObject__) */
