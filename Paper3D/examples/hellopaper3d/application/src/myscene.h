// myscene.h
// Android live wallpaper powered by Future Interface Paper3D technology
//
// Copyright 2012 - 2024 Future Interface. All rights reserved.
//
// Hongwei Li lihw81@gmail.com
//

#ifndef MYSCENE_H
#define MYSCENE_H

#include <Paper3D/pscene.h>

#include <PFoundation/pvector2.h>

class PContext;
class PDrawable;

class MyScene : public PScene
{
public:
    MyScene(PContext *context);
    ~MyScene();

    virtual void update();

private:
    enum { NUM_LOGOS = 32 };
    PDrawable *m_logos[NUM_LOGOS];
    PVector2   m_velocities[NUM_LOGOS];
};

#endif
