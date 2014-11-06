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
#include "fakeBug.h"

USING_NS_CC;
class Tool:public Sprite{
public:
    Tool();
    ~Tool();
    
    static Tool* createTool(std::string imageTool, int typeTool);
    void updateTool(float dt);
    void initOptions(int typeTool);
    void setHandUp();
    void setMuiTen();
    void showMuiTen();
    void dropDrug();
    void setUpNoteHelp();
    void turnOnFlashLight(cocos2d::Ref* pSender);
    void setShakeEarAnimation();
    void setScissorCutAnimation();
    void setScissorClose();
    void setInjectionFull();
    void setInjectionNormal();
    void setToolCatchedBug();
    void setToolCatchNormal();
    
    int _typeTool, _typeBugCatched, _countNumberBugCatched;
    bool _isTouch,_isSet,_isTurnOnFlashLight, _isCatchedBug, _isDroppedBug, _ignoreDropBug;
    bool _startMove;//Use for small table
    bool _isDropDrugWater;//Use for drug water
    Patient *_patient;
    Size _visibleSize;
    Point _savePositionOriginal;
    Point _velocityMoveSmallTable;
    
    Help *_handHelp, *_noteHelp, *_muiTen;
    Sprite *_dropDrug;
    Tool *_toolCatchBug;
    
    //Touchs
    void addEvents();
    void touchEvent(Touch* touch);
    void setTouchDotPosition (Vec2 vec);
};

#endif /* defined(__DoctorEar__ToolObject__) */
