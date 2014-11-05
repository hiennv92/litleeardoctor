//
//  Helps.h
//  DoctorEar
//
//  Created by Trinh Van Duong on 10/31/14.
//
//

#ifndef __DoctorEar__Helps__
#define __DoctorEar__Helps__

#include <stdio.h>
#include "cocos2d.h"
#include "Patient.h"

USING_NS_CC;
class Help:public Sprite{
public:
    Help();
    ~Help();
    
    static Help* createHelp(std::string imageHelp, int typeHelp);
    void initOptions(int typeTool);
    void showHelp(float delay);
    void showNoteHelp();
    void moveHandVertical();
    void moveHandHorizontal();
    void setNormalState();
    void setMuiTenNormal();
    void setMuiTenScale();
    void setMuiTenMove();
    void setMuiTenMoveNormal();
    
    int _typeHelp;
    int _typeHand;
    Size _visibleSize;
    Point _savePosition;
};


#endif /* defined(__DoctorEar__Helps__) */
