// pios32glview.mm
// The OpenGL view.
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#import "piosglview.h"

#include <PFoundation/pcontext.h>

@implementation PIOSGLView

+ (Class)layerClass {
    return [CAEAGLLayer class];
}

- (void)render:(CADisplayLink*)displayLink {
    //_currentRotation += displayLink.duration * 90;
    if (_pcontext != P_NULL &&
        (_pcontext->state() == P_CONTEXT_STATE_RUNNING ||
         _pcontext->state() == P_CONTEXT_STATE_PAUSED))
    {
        if (!_pcontext->update())
        {
            _pcontext->setState(P_CONTEXT_STATE_ERROR);

            NSLog(@"update error in context.");
        }
    }
    else
    {
        NSLog(@"context is invalid or its state is wrong");
        //TODO: Tell application to quit.
    }
    
    [_context presentRenderbuffer:GL_RENDERBUFFER];
}

- (void)setupDisplayLink {
    CADisplayLink* displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(render:)];
    [displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];    
}

- (id)initWithFrame:(CGRect)frame
        TechContext:(PContext *)context
{
    _pcontext = context;

    self = [super initWithFrame:frame];
    //[self setContentScaleFactor:2.0f];
    if (self) {        
        // Setup the layer
        _eaglLayer = (CAEAGLLayer*) self.layer;
        _eaglLayer.opaque = YES;
        _eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                        kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat, nil];
    
        // Setup the GL context.
        EAGLRenderingAPI api = kEAGLRenderingAPIOpenGLES2;
        _context = [[EAGLContext alloc] initWithAPI:api];
        if (!_context) {
            NSLog(@"Failed to initialize OpenGLES 2.0 context");
            return nil;
        }

        if (![EAGLContext setCurrentContext:_context]) {
            NSLog(@"Failed to set current OpenGL context");
            return nil;
        }
    
        // Setup the color and depth buffer.
        glGenRenderbuffers(1, &_colorRenderBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, _colorRenderBuffer);        
        [_context renderbufferStorage:GL_RENDERBUFFER fromDrawable:_eaglLayer];    
    
        glGenRenderbuffers(1, &_depthRenderBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, _depthRenderBuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, self.frame.size.width, self.frame.size.height);    
    
        // Setup the default framebuffer object.
        glGenFramebuffers(1, &_framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, 
            GL_RENDERBUFFER, _colorRenderBuffer);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, 
            GL_RENDERBUFFER, _depthRenderBuffer);
        
        // Initialize the Tech context.
        if (!_pcontext->initialize(_pcontext->properties()->m_windowWidth, 
                                   _pcontext->properties()->m_windowHeight))
        {
            return nil;
        }
        else
        {
            if (!context->onInitialized())
            {
                context->setState(P_CONTEXT_STATE_ERROR);
            }
            else
            {
                PLOG_DEBUG("Starting program main loop");
                context->setState(P_CONTEXT_STATE_RUNNING);
            }

            if (context->state() == P_CONTEXT_STATE_ERROR)
            {
                return nil;
            }
        }
    }

    return self;
}

- (void)dealloc
{
#if __has_feature(objc_arc)
    _context = nil;
#else
    [_context release];
    _context = nil;
    [super dealloc];
#endif
}

@end

