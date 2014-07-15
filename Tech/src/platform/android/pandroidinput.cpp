// pandroidinput.cpp 
// Android input.
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//
#include <PFoundation/pinput.h>

#define P_INPUT_NATIVE_KEY_TRANSLATION_TABLE_SIZE     111

static enum PInputKey pInputKeyTranslationTable[P_INPUT_NATIVE_KEY_TRANSLATION_TABLE_SIZE] = 
{
    P_KEY_UNKNOWN, /* KEYCODE_UNKNOWN         = 0*/
    P_KEY_SOFTKEY1, /* KEYCODE_SOFT_LEFT       = 1*/
    P_KEY_SOFTKEY2, /* KEYCODE_SOFT_RIGHT      = 2*/
    P_KEY_HOME, /* KEYCODE_HOME            = 3*/
    P_KEY_ESC, /* KEYCODE_BACK            = 4*/
    P_KEY_UNKNOWN, /* KEYCODE_CALL            = 5*/
    P_KEY_UNKNOWN, /* KEYCODE_ENDCALL         = 6*/
    P_KEY_0, /* KEYCODE_0               = 7*/
    P_KEY_1, /* KEYCODE_1               = 8*/
    P_KEY_2, /* KEYCODE_2               = 9*/
    P_KEY_3, /* KEYCODE_3               = 10*/
    P_KEY_4, /* KEYCODE_4               = 11*/
    P_KEY_5, /* KEYCODE_5               = 12*/
    P_KEY_6, /* KEYCODE_6               = 13*/
    P_KEY_7, /* KEYCODE_7               = 14*/
    P_KEY_8, /* KEYCODE_8               = 15*/
    P_KEY_9, /* KEYCODE_9               = 16*/
    P_KEY_STAR, /* KEYCODE_STAR            = 17*/
    P_KEY_POUND, /* KEYCODE_POUND           = 18*/
    P_KEY_ARROW_UP, /* KEYCODE_DPAD_UP         = 19*/
    P_KEY_ARROW_DOWN, /* KEYCODE_DPAD_DOWN       = 20*/
    P_KEY_ARROW_LEFT, /* KEYCODE_DPAD_LEFT       = 21*/
    P_KEY_ARROW_RIGHT, /* KEYCODE_DPAD_RIGHT      = 22*/
    P_KEY_UNKNOWN, /* KEYCODE_DPAD_CENTER     = 23*/
    P_KEY_VOLUME_UP, /* KEYCODE_VOLUME_UP       = 24*/
    P_KEY_VOLUME_DOWN, /* KEYCODE_VOLUME_DOWN     = 25*/
    P_KEY_UNKNOWN, /* KEYCODE_POWER           = 26*/
    P_KEY_UNKNOWN, /* KEYCODE_CAMERA          = 27*/
    P_KEY_BACKSPACE, /* KEYCODE_CLEAR           = 28*/
    P_KEY_A, /* KEYCODE_A               = 29*/
    P_KEY_B, /* KEYCODE_B               = 30*/
    P_KEY_C, /* KEYCODE_C               = 31*/
    P_KEY_D, /* KEYCODE_D               = 32*/
    P_KEY_E, /* KEYCODE_E               = 33*/
    P_KEY_F, /* KEYCODE_F               = 34*/
    P_KEY_G, /* KEYCODE_G               = 35*/
    P_KEY_H, /* KEYCODE_H               = 36*/
    P_KEY_I, /* KEYCODE_I               = 37*/
    P_KEY_J, /* KEYCODE_J               = 38*/
    P_KEY_K, /* KEYCODE_K               = 39*/
    P_KEY_L, /* KEYCODE_L               = 40*/
    P_KEY_M, /* KEYCODE_M               = 41*/
    P_KEY_N, /* KEYCODE_N               = 42*/
    P_KEY_O, /* KEYCODE_O               = 43*/
    P_KEY_P, /* KEYCODE_P               = 44*/
    P_KEY_Q, /* KEYCODE_Q               = 45*/
    P_KEY_R, /* KEYCODE_R               = 46*/
    P_KEY_S, /* KEYCODE_S               = 47*/
    P_KEY_T, /* KEYCODE_T               = 48*/
    P_KEY_U, /* KEYCODE_U               = 49*/
    P_KEY_V, /* KEYCODE_V               = 50*/
    P_KEY_W, /* KEYCODE_W               = 51*/
    P_KEY_X, /* KEYCODE_X               = 52*/
    P_KEY_Y, /* KEYCODE_Y               = 53*/
    P_KEY_Z, /* KEYCODE_Z               = 54*/
    P_KEY_COMMA, /* KEYCODE_COMMA           = 55*/
    P_KEY_PERIOD, /* KEYCODE_PERIOD          = 56*/
    P_KEY_ALT, /* KEYCODE_ALT_LEFT        = 57*/
    P_KEY_ALT, /* KEYCODE_ALT_RIGHT       = 58*/
    P_KEY_SHIFT, /* KEYCODE_SHIFT_LEFT      = 59*/
    P_KEY_SHIFT, /* KEYCODE_SHIFT_RIGHT     = 60*/
    P_KEY_TAB, /* KEYCODE_TAB             = 61*/
    P_KEY_SPACE, /* KEYCODE_SPACE           = 62*/
    P_KEY_UNKNOWN, /* KEYCODE_SYM             = 63*/
    P_KEY_UNKNOWN, /* KEYCODE_EXPLORER        = 64*/
    P_KEY_UNKNOWN, /* KEYCODE_ENVELOPE        = 65*/
    P_KEY_ENTER, /* KEYCODE_ENTER           = 66*/
    P_KEY_DELETE, /* KEYCODE_DEL             = 67*/
    P_KEY_UNKNOWN, /* KEYCODE_GRAVE           = 68*/
    P_KEY_MINUS, /* KEYCODE_MINUS           = 69*/
    P_KEY_EQUAL, /* KEYCODE_EQUALS          = 70*/
    P_KEY_LEFT_BRACKET, /* KEYCODE_LEFT_BRACKET    = 71*/
    P_KEY_RIGHT_BRACKET, /* KEYCODE_RIGHT_BRACKET   = 72*/
    P_KEY_UNKNOWN, /* KEYCODE_BACKSLASH       = 73*/
    P_KEY_SEMICOLON, /* KEYCODE_SEMICOLON       = 74*/
    P_KEY_UNKNOWN, /* KEYCODE_APOSTROPHE      = 75*/
    P_KEY_UNKNOWN, /* KEYCODE_SLASH           = 76*/
    P_KEY_UNKNOWN, /* KEYCODE_AT              = 77*/
    P_KEY_UNKNOWN, /* KEYCODE_NUM             = 78*/
    P_KEY_UNKNOWN, /* KEYCODE_HEADSETHOOK     = 79*/
    P_KEY_UNKNOWN, /* KEYCODE_FOCUS           = 80;*/
    P_KEY_NUM_PLUS, /* KEYCODE_PLUS            = 81*/
    P_KEY_MENU, /* KEYCODE_MENU            = 82*/
    P_KEY_UNKNOWN, /* KEYCODE_NOTIFICATION    = 83*/
    P_KEY_UNKNOWN, /* KEYCODE_SEARCH          = 84*/
    P_KEY_UNKNOWN, /* KEYCODE_MEDIA_PLAY_PAUSE= 85*/
    P_KEY_UNKNOWN, /* KEYCODE_MEDIA_STOP      = 86*/
    P_KEY_UNKNOWN, /* KEYCODE_MEDIA_NEXT      = 87*/
    P_KEY_UNKNOWN, /* KEYCODE_MEDIA_PREVIOUS  = 88*/
    P_KEY_UNKNOWN, /* KEYCODE_MEDIA_REWIND    = 89*/
    P_KEY_UNKNOWN, /* KEYCODE_MEDIA_FAST_FORWARD = 90*/
    P_KEY_UNKNOWN, /* KEYCODE_MUTE            = 91*/
    P_KEY_PAGE_UP, /* KEYCODE_PAGE_UP         = 92*/
    P_KEY_PAGE_DOWN, /* KEYCODE_PAGE_DOWN       = 93*/
    P_KEY_UNKNOWN, /* KEYCODE_PICTSYMBOLS     = 94;*/
    P_KEY_UNKNOWN, /* KEYCODE_SWITCH_CHARSET  = 95;*/
    P_KEY_UNKNOWN, /* KEYCODE_BUTTON_A = 96 */
    P_KEY_UNKNOWN, /* KEYCODE_BUTTON_B = 97 */
    P_KEY_UNKNOWN, /* KEYCODE_BUTTON_C = 98 */
    P_KEY_UNKNOWN, /* KEYCODE_BUTTON_X = 99 */
    P_KEY_UNKNOWN, /* KEYCODE_BUTTON_Y = 100 */
    P_KEY_UNKNOWN, /* KEYCODE_BUTTON_Z = 101 */
    P_KEY_UNKNOWN, /* KEYCODE_BUTTON_L1 = 102 */
    P_KEY_UNKNOWN, /* KEYCODE_BUTTON_R1 = 103 */
    P_KEY_UNKNOWN, /* KEYCODE_BUTTON_L2 = 104 */
    P_KEY_UNKNOWN, /* KEYCODE_BUTTON_R2 = 105 */
    P_KEY_UNKNOWN, /* KEYCODE_BUTTON_THUMBL = 106 */
    P_KEY_UNKNOWN, /* KEYCODE_BUTTON_THUMBR = 107 */
    P_KEY_UNKNOWN, /* KEYCODE_BUTTON_START = 108 */
    P_KEY_UNKNOWN, /* KEYCODE_BUTTON_SELECT = 109 */
    P_KEY_UNKNOWN/* KEYCODE_BUTTON_MODE = 110 */
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
        key = pInputKeyTranslationTable[keyCode];
    }

    return key;
}
