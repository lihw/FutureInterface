// pios32glview.h
// The OpenGL view.
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#ifndef PISO32GLVIEW_H
#define PISO32GLVIEW_H

#import <UIKit/UIKit.h>
#import <QuartzCore/QuartzCore.h>

#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

class PContext;

@interface PIOSGLView : UIView {
    CAEAGLLayer *_eaglLayer;
    EAGLContext *_context;
    PContext    *_pcontext;
    GLuint       _colorRenderBuffer;
    GLuint       _depthRenderBuffer;
    GLuint       _framebuffer;
}

- (id)initWithFrame:(CGRect)frame TechContext:(PContext *)pcontext;

@end

#endif //!PISO32GLVIEW_H
