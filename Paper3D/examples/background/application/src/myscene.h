// myscene.h
// "Background" example. 
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#ifndef MYSCENE_H
#define MYSCENE_H

#include <Paper3D/pscene.h>

class PDrawable;
class PBackground;
class PContext;

class MyScene : public PScene
{
public:
    MyScene(PContext *context);
    ~MyScene();

    virtual void update();

    void rotate(pfloat32 offset);

    void switchBackgroundFillMode();

private:
    PDrawable *m_cube;
    PBackground *m_background;
    pfloat32 m_backgroundMovement;
};

#endif
