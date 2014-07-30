// myscene.cpp
// "Network" example
//
// Copyright 2012 - 2014 Future Interface. All rights reserved.
//
// Future Interface  lihw81@gmail.com

#include "myscene.h"

#include <Paper3D/paper3d.h>

#include <PFoundation/pfoundation.h>


MyScene::MyScene(PContext *context)
    : PScene("my-scene", context)
{
    PResourceManager *resMgr = context->module<PResourceManager>("resource-manager");

    if (!load("scene.psc")) 
    {
        PASSERT(!"Failed to load scene.psc");
        return ;
    }
}

MyScene::~MyScene()
{
}
    
void MyScene::update()
{
	PVector3 r = root()->transform().rotation();
	r[2] -= 0.01f;
	root()->transform().setRotation(r);
}
