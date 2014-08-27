// myscene.cpp
// "LoadScene" example
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com

#include "myscene.h"

#include <Paper3D/paper3d.h>

#include <PFoundation/pfoundation.h>


MyScene::MyScene(PContext *context)
    : PScene("my-scene", context)
{
    PResourceManager *resMgr = context->module<PResourceManager>("resource-manager");
<<<<<<< HEAD
    (void *)resMgr;
    
    //if (!load("scene.psc"))
    //{
    //    PASSERT(!"Failed to load scene.psc");
    //    return ;
    //}
    
    pGlErrorCheckError();
=======
    
    if (!load("scene.psc"))
    {
        PASSERT(!"Failed to load scene.psc");
        return ;
    }
>>>>>>> d72c8f90d18e1467a18d14472d2db3af6688a4ff
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
