//
//  Joystick.h
//  Samurai
//
//  Created by Trinh Van Duong on 10/10/14.
//
//

#ifndef __Samurai__Joystick__
#define __Samurai__Joystick__

#include "cocos2d.h"
#include <stdio.h>

USING_NS_CC;

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

class Joystick: public Layer
{
    public :
    /* start * joystick device */
    void OnRun ();
    /** Clear data */
    void onDisable ();
    /* death * Set the radius, the radius of the joystick control that is beyond failure */
    void setDieRadius(float radius );
    /** Set invalid region radius (if the invalid region, will be reset) */
    void setFailRadius(float radius);
    /** whether to display the chassis and contacts */
    void setVisibleJoystick(bool visible);
    /** location is free transform the rocker, coordinates that every time you press the mouse on the screen will be the rocker's coordinates, the coordinates will not change when the joystick is moved until the next press the mouse */
    void setAutoPosition(bool value);
    /** callback function pointers */
    std::function<void(JoystickEnum)>onDirection;
    /* create * a static function (need to pass the chassis and the contact picture path) */
    static Joystick * create (std::string chassisPath, std::string dotPath);
    /* initialize rocker * (requires incoming chassis and contacts picture path) */
    void initWithJoystick (std::string chassisPath, std::string dotPath);
    protected :
    /** effective area radius */
    float _radius;
    /** Failure region radius */
    float _failradius;
    /* out * if the effective area */
    bool isMoveOut;
    /** whether there is a valid area radius */
    bool isDieRadius;
    /* if * Free Transform joystick device coordinates */
    bool isAutoPosition;
    /** direction */
    JoystickEnum _direction;
    /** Chassis */
    Sprite * _chassis;
    /** contact */
    Sprite * _touchDot;
    EventListenerTouchOneByOne * listener;
    
    bool onTouchBegan (Touch * Touch, Event * Event );
    void onTouchMoved (Touch * Touch, Event * Event );
    void onTouchEnded (Touch * Touch, Event * Event );
    /**
    1, set the contact, and determine whether the invalid region (if the invalid region, will be reset)
    2, sending the angle change (if not invalid region) */
    void setTouchDotPosition (Vec2 vec1, Vec2 vec2);
    /**
    1, eight direction calculation joystick control
    2, sending the angle changes, the curvature change callback function */
    void changeAngle (Vec2 position);
    /* function * callbacks registered listeners */
    void callDirectionFun ();
    /** Reset (reset only when the curvature is not DEFAULT) */
    void resetState ();
};

#endif
