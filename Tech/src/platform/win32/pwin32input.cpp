// pwin32input.cpp 
// Input events handling under win32
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#include <PFoundation/pinput.h>

#define P_INPUT_NATIVE_KEY_TRANSLATION_TABLE_SIZE     225

// win32 specific translation table 
static const enum PInputKey s_pInputKeyTranslationTable[P_INPUT_NATIVE_KEY_TRANSLATION_TABLE_SIZE] = 
{
    P_KEY_UNKNOWN, // 0x00
    P_KEY_UNKNOWN,
    P_KEY_UNKNOWN,
    P_KEY_UNKNOWN,
    P_KEY_UNKNOWN,

    P_KEY_UNKNOWN,
    P_KEY_UNKNOWN,
    P_KEY_UNKNOWN,  
    P_KEY_BACKSPACE,      
    P_KEY_TAB,

    P_KEY_UNKNOWN,   // 0x0a
    P_KEY_UNKNOWN,
    P_KEY_UNKNOWN,
    P_KEY_ENTER,          
    P_KEY_UNKNOWN,

    P_KEY_UNKNOWN,
    P_KEY_SHIFT,   
    P_KEY_CONTROL, 
    P_KEY_ALT,
    P_KEY_PAUSE,

    P_KEY_CAPS_LOCK,   // 0x14
    P_KEY_UNKNOWN,
    P_KEY_UNKNOWN,
    P_KEY_UNKNOWN,
    P_KEY_UNKNOWN,

    P_KEY_UNKNOWN,
    P_KEY_UNKNOWN,
    P_KEY_ESC,
    P_KEY_UNKNOWN,
    P_KEY_UNKNOWN,

    P_KEY_UNKNOWN,  // 0x1e
    P_KEY_UNKNOWN,
    P_KEY_SPACE,          
    P_KEY_PAGE_UP,
    P_KEY_PAGE_DOWN,

    P_KEY_END,
    P_KEY_HOME,
    P_KEY_ARROW_LEFT,
    P_KEY_ARROW_UP,
    P_KEY_ARROW_RIGHT,

    P_KEY_ARROW_DOWN, // 0x28
    P_KEY_UNKNOWN,
    P_KEY_UNKNOWN,
    P_KEY_UNKNOWN,
    P_KEY_PRINT_SCREEN,

    P_KEY_INSERT,
    P_KEY_DELETE,
    P_KEY_UNKNOWN,
    P_KEY_0,  
    P_KEY_1,

    P_KEY_2, // 0x32
    P_KEY_3,
    P_KEY_4,
    P_KEY_5,
    P_KEY_6,

    P_KEY_7,
    P_KEY_8,
    P_KEY_9,  
    P_KEY_UNKNOWN,
    P_KEY_UNKNOWN,

    P_KEY_UNKNOWN, // 0x3c
    P_KEY_UNKNOWN,
    P_KEY_UNKNOWN,
    P_KEY_UNKNOWN,
    P_KEY_UNKNOWN,

    P_KEY_A,
    P_KEY_B,
    P_KEY_C,
    P_KEY_D,
    P_KEY_E,

    P_KEY_F, // 0x46
    P_KEY_G,
    P_KEY_H,
    P_KEY_I,
    P_KEY_J,

    P_KEY_K,
    P_KEY_L,
    P_KEY_M,
    P_KEY_N,
    P_KEY_O,

    P_KEY_P, // 0x50
    P_KEY_Q,
    P_KEY_R,
    P_KEY_S,
    P_KEY_T,

    P_KEY_U,
    P_KEY_V,
    P_KEY_W,
    P_KEY_X,
    P_KEY_Y,

    P_KEY_Z,    // 0x5a
    P_KEY_META,
    P_KEY_UNKNOWN,
    P_KEY_MENU,
    P_KEY_UNKNOWN,

    P_KEY_UNKNOWN,
    P_KEY_UNKNOWN,
    P_KEY_UNKNOWN,
    P_KEY_UNKNOWN,
    P_KEY_UNKNOWN, 

    P_KEY_UNKNOWN, // 0x64
    P_KEY_UNKNOWN,
    P_KEY_UNKNOWN,
    P_KEY_UNKNOWN,
    P_KEY_UNKNOWN,

    P_KEY_UNKNOWN,
    P_KEY_UNKNOWN,
    P_KEY_UNKNOWN,
    P_KEY_UNKNOWN,
    P_KEY_UNKNOWN, 

    P_KEY_UNKNOWN, // 0x6e
    P_KEY_UNKNOWN,
    P_KEY_F1,
    P_KEY_F2,
    P_KEY_F3,

    P_KEY_F4,
    P_KEY_F5,
    P_KEY_F6,
    P_KEY_F7,
    P_KEY_F8,

    P_KEY_F9, // 0x78
    P_KEY_F10,
    P_KEY_F11,
    P_KEY_F12,
    P_KEY_UNKNOWN, 

    P_KEY_UNKNOWN, 
    P_KEY_UNKNOWN, 
    P_KEY_UNKNOWN, 
    P_KEY_UNKNOWN, 
    P_KEY_UNKNOWN, 

    P_KEY_UNKNOWN, // 0x82
    P_KEY_UNKNOWN, 
    P_KEY_UNKNOWN, 
    P_KEY_UNKNOWN, 
    P_KEY_UNKNOWN, 

    P_KEY_UNKNOWN, 
    P_KEY_UNKNOWN, 
    P_KEY_UNKNOWN, 
    P_KEY_UNKNOWN, 
    P_KEY_UNKNOWN, 

    P_KEY_UNKNOWN, // 0x8c
    P_KEY_UNKNOWN, 
    P_KEY_UNKNOWN, 
    P_KEY_UNKNOWN, 
    P_KEY_NUM_LOCK, 

    P_KEY_SCROLL_LOCK, 
    P_KEY_UNKNOWN, 
    P_KEY_UNKNOWN, 
    P_KEY_UNKNOWN, 
    P_KEY_UNKNOWN, 

    P_KEY_UNKNOWN, // 0x96
    P_KEY_UNKNOWN, 
    P_KEY_UNKNOWN, 
    P_KEY_UNKNOWN, 
    P_KEY_UNKNOWN, 

    P_KEY_UNKNOWN, 
    P_KEY_UNKNOWN, 
    P_KEY_UNKNOWN, 
    P_KEY_UNKNOWN, 
    P_KEY_UNKNOWN, 

    P_KEY_UNKNOWN, // 0xa0
    P_KEY_UNKNOWN, 
    P_KEY_UNKNOWN, 
    P_KEY_UNKNOWN, 
    P_KEY_UNKNOWN, 

    P_KEY_UNKNOWN, 
    P_KEY_UNKNOWN, 
    P_KEY_UNKNOWN, 
    P_KEY_UNKNOWN, 
    P_KEY_UNKNOWN, 

    P_KEY_UNKNOWN, // 0xaa
    P_KEY_UNKNOWN, 
    P_KEY_UNKNOWN, 
    P_KEY_UNKNOWN, 
    P_KEY_UNKNOWN, 

    P_KEY_UNKNOWN, 
    P_KEY_UNKNOWN, 
    P_KEY_UNKNOWN, 
    P_KEY_UNKNOWN, 
    P_KEY_UNKNOWN, 

    P_KEY_UNKNOWN, // 0xb4
    P_KEY_UNKNOWN, 
    P_KEY_UNKNOWN, 
    P_KEY_UNKNOWN, 
    P_KEY_UNKNOWN, 

    P_KEY_UNKNOWN, 
    P_KEY_SEMICOLON, 
    P_KEY_EQUAL, 
    P_KEY_COMMA, 
    P_KEY_MINUS,

    P_KEY_PERIOD, // 0xbe
    P_KEY_DIVIDE,
    P_KEY_QUOTELEFT,
    P_KEY_UNKNOWN,
    P_KEY_UNKNOWN,

    P_KEY_UNKNOWN, 
    P_KEY_UNKNOWN, 
    P_KEY_UNKNOWN, 
    P_KEY_UNKNOWN, 
    P_KEY_UNKNOWN, 
    
    P_KEY_UNKNOWN, // 0xc8
    P_KEY_UNKNOWN, 
    P_KEY_UNKNOWN, 
    P_KEY_UNKNOWN, 
    P_KEY_UNKNOWN, 
    
    P_KEY_UNKNOWN, 
    P_KEY_UNKNOWN, 
    P_KEY_UNKNOWN, 
    P_KEY_UNKNOWN, 
    P_KEY_UNKNOWN, 
    
    P_KEY_UNKNOWN, // 0xd2
    P_KEY_UNKNOWN, 
    P_KEY_UNKNOWN, 
    P_KEY_UNKNOWN, 
    P_KEY_UNKNOWN, 
    
    P_KEY_UNKNOWN, 
    P_KEY_UNKNOWN, 
    P_KEY_UNKNOWN, 
    P_KEY_UNKNOWN, 
    P_KEY_LEFT_BRACKET, 
    
    P_KEY_BACKSLASH,  // 0xdc
    P_KEY_RIGHT_BRACKET, 
    P_KEY_QUOTE, 
    P_KEY_UNKNOWN, 
    P_KEY_UNKNOWN, 
};

              
PInputKey P_APIENTRY pInputNativeGetTranslatedKey(puint32 keyCode)
{            
    PInputKey key;

    if (keyCode >= P_INPUT_NATIVE_KEY_TRANSLATION_TABLE_SIZE)
    {
        key = P_KEY_UNKNOWN;
    }
    else
    {
        key = s_pInputKeyTranslationTable[keyCode];
    }

    return key;
}

