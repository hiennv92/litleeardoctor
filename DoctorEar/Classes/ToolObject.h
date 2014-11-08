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

enum JoystickEnum
{
    DEFAULT,
    D_UP,
    D_DOWN,
    D_LEFT,
    D_RIGHT,
    D_LEFT_UP,
    D_LEFT_DOWN,
    D_RIGHT_UP,
    D_RIGHT_DOWN
};

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
    void setTouchAvailable();
    void setupLazer();
    void setMoveCleanFalse();
    
    int _typeTool, _typeBugCatched, _countNumberBugCatched;
    bool _isTouch,_isSet,_isTurnOnFlashLight, _isCatchedBug, _isDroppedBug, _ignoreDropBug, _isMoved;
    bool _isMoveClean;//Use for tampon advance;
    bool _startMove;//Use for small table
    bool _isDropDrugWater;//Use for drug water
    bool _up, _down, _right, _left;
    Patient *_patient;
    Size _visibleSize;
    Point _savePositionOriginal;
    Point _velocityMoveSmallTable;
    Point _velocityMoveOngSoi;
    
    Help *_handHelp, *_noteHelp, *_muiTen;
    Sprite *_dropDrug;
    Sprite *_toolOngSoi, *_circle, *_lazer;
    Tool *_toolCatchBug;
    
    //Touchs
    void addEvents();
    void touchEvent(Touch* touch);
    void setTouchDotPosition (Vec2 vec);
    void changeAngle ();
    
    JoystickEnum _direction;
};

#endif /* defined(__DoctorEar__ToolObject__) */
