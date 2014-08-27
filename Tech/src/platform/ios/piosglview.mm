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
<<<<<<< HEAD
#include <PFoundation/pglerror.h>
=======
#include <PFoundation/pinput.h>
#include <PFoundation/pdevice.h>
>>>>>>> d72c8f90d18e1467a18d14472d2db3af6688a4ff


@implementation PIOSGLView

+ (Class)layerClass {
    return [CAEAGLLayer class];
}

- (void)render:(CADisplayLink*)displayLink {
    
<<<<<<< HEAD
    //_currentRotation += displayLink.duration * 90;
    
    pGlErrorCheckError();
    
=======
<<<<<<< HEAD
=======
    //_currentRotation += displayLink.duration * 90;
    //NSLog(@"render once");
    
>>>>>>> bf7878be62d5a65af0edfcb8e0f104897df1388a
>>>>>>> d72c8f90d18e1467a18d14472d2db3af6688a4ff
    if (_pcontext != P_NULL &&
        (_pcontext->state() == P_CONTEXT_STATE_RUNNING ||
         _pcontext->state() == P_CONTEXT_STATE_PAUSED))
    {
        if (!_pcontext->update())
        {
            _pcontext->setState(P_CONTEXT_STATE_ERROR);

            NSLog(@"Context update error.");
        }
    }
    else
    {
        NSLog(@"Context is invalid or its state is wrong.");
        // TODO: Render an error message onto the screen.
    }
    
<<<<<<< HEAD
    // FIXME: restore the renderbuffer binding after present?
=======
    
>>>>>>> bf7878be62d5a65af0edfcb8e0f104897df1388a
    glBindRenderbuffer(GL_RENDERBUFFER, _colorRenderBuffer);
    [_context presentRenderbuffer:GL_RENDERBUFFER];
}

- (id)initWithFrame:(CGRect)frame
        TechContext:(PContext *)pcontext
{
    _pcontext = pcontext;
<<<<<<< HEAD
    
    self = [super initWithFrame:frame];
    if (self)
    {
        // Scale the viewport to real resolution.
        float contentScale = 1.0f;
        if ([[UIScreen mainScreen] respondsToSelector:@selector(displayLinkWithTarget:selector:)])
        {
            contentScale = [[UIScreen mainScreen] scale];
        }
        self.contentScaleFactor = contentScale;
        
=======

    self = [super initWithFrame:frame];
    //[self setContentScaleFactor:2.0f];
    if (self)
    {
>>>>>>> bf7878be62d5a65af0edfcb8e0f104897df1388a
        // Setup the layer
        _eaglLayer = (CAEAGLLayer*) self.layer;
        _eaglLayer.opaque = YES;
        _eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                        kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat, nil];
        _eaglLayer.contentsScale = contentScale;
    
        // Setup the GL context.
        EAGLRenderingAPI api = kEAGLRenderingAPIOpenGLES2;
        _context = [[EAGLContext alloc] initWithAPI:api];
        if (!_context)
        {
<<<<<<< HEAD
            NSLog(@"Failed to initialize OpenGLES 2.0 context.");
=======
            NSLog(@"Failed to initialize OpenGLES 2.0 context");
>>>>>>> bf7878be62d5a65af0edfcb8e0f104897df1388a
            return nil;
        }

        if (![EAGLContext setCurrentContext:_context])
        {
<<<<<<< HEAD
            NSLog(@"Failed to set current OpenGL context.");
=======
            NSLog(@"Failed to set current OpenGL context");
>>>>>>> bf7878be62d5a65af0edfcb8e0f104897df1388a
            return nil;
        }
        
<<<<<<< HEAD
        int screenWidth = self.frame.size.width;
        int screenHeight = self.frame.size.height;
    
=======
>>>>>>> d72c8f90d18e1467a18d14472d2db3af6688a4ff
        // Setup the color and depth buffer.
<<<<<<< HEAD
        glGenRenderbuffers(1, &_colorRenderBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, _colorRenderBuffer);
        [_context renderbufferStorage:GL_RENDERBUFFER fromDrawable:_eaglLayer];
        
        GLint renderbufferWidth, renderbufferHeight;
        glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &renderbufferWidth);
        glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &renderbufferHeight);
        
        glGenRenderbuffers(1, &_depthRenderBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, _depthRenderBuffer);
<<<<<<< HEAD
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, screenWidth, screenHeight);
=======
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, renderbufferWidth, renderbufferHeight);
        
=======
        glGenRenderbuffers(1, &_depthRenderBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, _depthRenderBuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, screenWidth, screenHeight);
        
        glGenRenderbuffers(1, &_colorRenderBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, _colorRenderBuffer);
        [_context renderbufferStorage:GL_RENDERBUFFER fromDrawable:_eaglLayer];
>>>>>>> d72c8f90d18e1467a18d14472d2db3af6688a4ff
    
>>>>>>> bf7878be62d5a65af0edfcb8e0f104897df1388a
        // Setup the default framebuffer object.
        glGenFramebuffers(1, &_framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, 
            GL_RENDERBUFFER, _colorRenderBuffer);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, 
            GL_RENDERBUFFER, _depthRenderBuffer);
        
        // Initialize the Tech context.
<<<<<<< HEAD
        if (!_pcontext->initialize(screenWidth, screenHeight))
=======
        if (!_pcontext->initialize(renderbufferWidth, renderbufferHeight))
>>>>>>> d72c8f90d18e1467a18d14472d2db3af6688a4ff
        {
            return nil;
        }
        else
        {
<<<<<<< HEAD
            if (!context->onInitialized())
            {
                context->setState(P_CONTEXT_STATE_ERROR);
                return nil;
            }
        }

        // Setup the diplay link.
        CADisplayLink* displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(render:)];
        [displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
        
        pGlErrorCheckError();
=======
            if (!_pcontext->onInitialized())
            {
                pcontext->setState(P_CONTEXT_STATE_ERROR);
                return nil;
            }
        }
        // Setup the diplay link.
        CADisplayLink* displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(render:)];
        [displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
>>>>>>> d72c8f90d18e1467a18d14472d2db3af6688a4ff
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

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    if (_pcontext->state() == P_CONTEXT_STATE_RUNNING ||
        _pcontext->state() == P_CONTEXT_STATE_PAUSED)
    {
        PInputEventQueue* inputQueue = _pcontext->device()->getInputEventQueue();
        
        int count = [touches count];
        NSArray *touchArray = [touches allObjects];
        
        PInputEventTouch *eventTouch = inputQueue->addTouchEvent(count, P_TOUCH_STATE_BEGIN);
        
        for (int i = 0; i < count; ++i)
        {
            UITouch *touch = [touchArray objectAtIndex:i];
            
            CGPoint point = [touch locationInView:self];
            
            UITouchPhase phase = [touch phase];
            
            PInputCursorStateEnum cursorStates[] =
            {
                P_CURSOR_STATE_DOWN,        // UITouchPhaseBegin
                P_CURSOR_STATE_STATIONARY,  // UITouchPhaseStationary
                P_CURSOR_STATE_MOVE,        // UITouchPhaseMoved
                P_CURSOR_STATE_UP,          // UITouchPhaseEnded
                P_CURSOR_STATE_UP,          // UITouchPhaseCanceled,
            };
            
            inputQueue->setTouchCursor(eventTouch,
                                       i,
                                       i,
                                       point.x,
                                       point.y,
                                       0,
                                       0,
                                       cursorStates[phase],
                                       (int)[touch timestamp]);
        }
    }
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    if (_pcontext->state() == P_CONTEXT_STATE_RUNNING ||
        _pcontext->state() == P_CONTEXT_STATE_PAUSED)
    {
        PInputEventQueue* inputQueue = _pcontext->device()->getInputEventQueue();

        int count = [touches count];
        NSArray *touchArray = [touches allObjects];
        
        PInputEventTouch *eventTouch = inputQueue->addTouchEvent(count, P_TOUCH_STATE_CONTINUE);

        for (int i = 0; i < count; ++i)
        {
            UITouch *touch = [touchArray objectAtIndex:i];
            
            CGPoint point = [touch locationInView:self];
            
            UITouchPhase phase = [touch phase];

            PInputCursorStateEnum cursorStates[] =
            {
                P_CURSOR_STATE_DOWN,        // UITouchPhaseBegin
                P_CURSOR_STATE_STATIONARY,  // UITouchPhaseStationary
                P_CURSOR_STATE_MOVE,        // UITouchPhaseMoved
                P_CURSOR_STATE_UP,          // UITouchPhaseEnded
                P_CURSOR_STATE_UP,          // UITouchPhaseCanceled,
            };
            
            inputQueue->setTouchCursor(eventTouch,
                                       i,
                                       i,
                                       point.x,
                                       point.y,
                                       0,
                                       0,
                                       cursorStates[phase],
                                       (int)[touch timestamp]);
        }
    }
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    if (_pcontext->state() == P_CONTEXT_STATE_RUNNING ||
        _pcontext->state() == P_CONTEXT_STATE_PAUSED)
    {
        PInputEventQueue* inputQueue = _pcontext->device()->getInputEventQueue();
        
        int count = [touches count];
        NSArray *touchArray = [touches allObjects];
        
        PInputEventTouch *eventTouch = inputQueue->addTouchEvent(count, P_TOUCH_STATE_CONTINUE);
        
        for (int i = 0; i < count; ++i)
        {
            UITouch *touch = [touchArray objectAtIndex:i];
            
            CGPoint point = [touch locationInView:self];
            
            UITouchPhase phase = [touch phase];
            
            PInputCursorStateEnum cursorStates[] =
            {
                P_CURSOR_STATE_DOWN,        // UITouchPhaseBegin
                P_CURSOR_STATE_STATIONARY,  // UITouchPhaseStationary
                P_CURSOR_STATE_MOVE,        // UITouchPhaseMoved
                P_CURSOR_STATE_UP,          // UITouchPhaseEnded
                P_CURSOR_STATE_UP,          // UITouchPhaseCanceled,
            };
            
            inputQueue->setTouchCursor(eventTouch,
                                       i,
                                       i,
                                       point.x,
                                       point.y,
                                       0,
                                       0,
                                       cursorStates[phase],
                                       (int)[touch timestamp]);
        }
    }
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
    NSLog(@"touch canceled.");
}


@end

