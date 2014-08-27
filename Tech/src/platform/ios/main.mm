// main.cpp 
// Main entry and main loop in iOS.
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#import <UIKit/UIKit.h>

#include <PFoundation/penvironment.h>
#include <PFoundation/pactivity.h>
#include <PFoundation/pnew.h>
#include <PFoundation/pcontext.h>

#import "piosglview.h"

@interface PAppDelegate : UIResponder <UIApplicationDelegate> 
{
    UIWindow   *_mainWindow;
    PIOSGLView *_mainView;
    PActivity  *_activity;
}

@property (nonatomic, retain) IBOutlet UIWindow *mainWindow;
@property (nonatomic, retain) IBOutlet PIOSGLView *mainView;
@end

P_EXTERN void pMain(int argc, char* argv[]);

@implementation PAppDelegate
@synthesize mainView   = _mainView;
@synthesize mainWindow = _mainWindow;

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    pEnvironmentInitialize(P_NULL);

    // TODO: Find a way to convert launchOptions to argc and argv.
    _activity = PNEW(PActivity(0, P_NULL));
    if (!_activity->initialize())
    {
        return NO;
    }
    
    // Create a Tech context here.
    pMain(0, P_NULL);
    
    PContext *context = _activity->findContext((puint32)0);
    context->setState(P_CONTEXT_STATE_UNINITIALIZED);
    
    // Create the window and view.
    CGRect screenBounds = [[UIScreen mainScreen] bounds];
    
    float contentScale = 1.0f;
    if ([[UIScreen mainScreen] respondsToSelector:@selector(displayLinkWithTarget:selector:)])
    {
        contentScale = [[UIScreen mainScreen] scale];
    }
    
#if __has_feature(objc_arc)
    self.mainWindow = [[UIWindow alloc] initWithFrame:screenBounds];
    self.mainView = [[PIOSGLView alloc] initWithFrame:screenBounds TechContext:context];
#else
    self.mainWindow = [[UIWindow alloc] initWithFrame:screenBounds] autorelease];
    self.mainView = [[[PIOSGLView alloc] initWithFrame:screenBounds TechContext:context]
                   autorelease];
#endif
    
    [self.mainWindow addSubview:_mainView];
    
    [self.mainWindow makeKeyAndVisible];
        
    return YES;
}

- (void)applicationWillResignActive:(UIApplication *)application
{
    /*
     Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
     Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
     */
}

- (void)applicationDidEnterBackground:(UIApplication *)application
{
    /*
     Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later. 
     If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
     */
    PContext *context = _activity->findContext((puint32)0);
    context->pause();
}

- (void)applicationWillEnterForeground:(UIApplication *)application
{
    /*
     Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
     */
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
    /*
     Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
     */
    PContext *context = _activity->findContext((puint32)0);
    if (context->state() == P_CONTEXT_STATE_UNINITIALIZED)
    {
        PLOG_DEBUG("Starting program main loop.");
        context->setState(P_CONTEXT_STATE_RUNNING);
    }
    else
    {
        context->resume();
    }
}

- (void)applicationWillTerminate:(UIApplication *)application
{
    /*
     Called when the application is about to terminate.
     Save data if appropriate.
     See also applicationDidEnterBackground:.
     */
    PContext *context = _activity->findContext((puint32)0);
    if (context->state() != P_CONTEXT_STATE_UNINITIALIZED)
    {
    	context->onDestroy();
        PDELETE(context);
    }

    _activity->uninitialize();
    PDELETE(_activity);
    pEnvironmentUninitialize();
}

- (void)dealloc
{
#if __has_feature(objc_arc)
    _mainView = nil;
    _mainWindow = nil;
#else
    [_mainView release];
    [_mainWindow release];
    [super dealloc];
#endif
}

@end

int main(int argc, char* argv[])
{
    @autoreleasepool
    {
        int retVal = UIApplicationMain(argc, argv, nil, @"PAppDelegate");
        return retVal;
    }
}
