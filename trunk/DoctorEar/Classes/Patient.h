//
//  Patient.h
//  DoctorEar
//
//  Created by Trinh Van Duong on 10/30/14.
//
//

#ifndef __DoctorEar__Patient__
#define __DoctorEar__Patient__

#include <stdio.h>
#include "cocos2d.h"
USING_NS_CC;

class Patient:public Sprite{
public:
    Patient();
    ~Patient();
    
    static Patient* createPatient(std::string imagePatient, int typePatient);
    void updatePatient(float dt);
    void initOptions(int typePatient);

    //Eye
    void closeEyeAnimation();
    void updateCloseEye(float dt);
    void setOpenEye();
    void setHiddenSmallEye();
    void setCloseEye();
    void createSmallEye();
    void createEarHole();
    void setPositionSmallEye(Point p);
    void setEyeBrowScare();
    void setEyeBrowSmile();
    void setEyeBrowNormal();

    //Mouth
    void setMouthScare();
    void setMouthSmile();
    void setMouthNormal();
    
    //Touchs
//    void addEvents();
//    void touchEvent(Touch* touch);
    
    Sprite *_eye,*_smallEye, *_mouth,*_mouthScare, *_mouthSmile, *_ear, *_eyeBrow, *_earHole;
    RepeatForever *_repeat;
    std::string _stringPathImage;
    int _typePatient;
    bool _isSetCloseEye;
    Point _pointSavePositionSmallEye;
};

#endif /* defined(__DoctorEar__Patient__) */
