//
//  ToolObject.h
//  DoctorEar
//
//  Created by Trinh Van Duong on 10/31/14.
//
//

#ifndef __DoctorEar__ToolObject__
#define __DoctorEar__ToolObject__

#include <stdio.h>
#include "cocos2d.h"
#include "Patient.h"
#include "Helps.h"

USING_NS_CC;
class Tool:public Sprite{
public:
    Tool();
    ~Tool();
    
    static Tool* createTool(std::string imageTool, int typeTool);
    void updateTool(float dt);
    void initOptions(int typeTool);
    void setHandUp();
    void setUpNoteHelp();
    void turnOnFlashLight(cocos2d::Ref* pSender);
    void setScissorCutAnimation();
    void setScissorClose();
    
    int _typeTool;
    bool _isTouch,_isSet,_isTurnOnFlashLight;
    bool _startMove;//Use for small table
    Patient *_patient;
    Size _visibleSize;
    Point _savePositionOriginal;
    Point _velocityMoveSmallTable;
    
    Help *_handHelp, *_noteHelp;
    
    //Touchs
    void addEvents();
    void touchEvent(Touch* touch);
    void setTouchDotPosition (Vec2 vec);
};

#endif /* defined(__DoctorEar__ToolObject__) */
