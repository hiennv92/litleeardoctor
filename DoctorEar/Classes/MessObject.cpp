//
//  MessObject.cpp
//  DoctorEar
//
//  Created by Trinh Van Duong on 10/31/14.
//
//

#include "MessObject.h"
#include "Define.h"

MessObject::MessObject(){}

MessObject::~MessObject(){}

MessObject* MessObject::createMess(std::string imageMess, int typeMess){
    MessObject *mess = new MessObject();
    if(mess->initWithFile(imageMess)){
        mess->autorelease();
        mess->initOptions(typeMess);
        return  mess;
    }
    CC_SAFE_DELETE(mess);
    return NULL;
}

void MessObject::initOptions(int typeMess){
    _visibleSize = Director::getInstance()->getVisibleSize();
    _typeMess = typeMess;
}

void MessObject::removeMess(){
    
}
