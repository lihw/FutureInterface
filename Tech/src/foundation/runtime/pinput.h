// pinput.h
// The window events.
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com


#ifndef PINPUT_H
#define PINPUT_H

#include <PFoundation/pnew.h>
#include <PFoundation/passert.h>
#include <PFoundation/ptime.h>

#include <math.h>

#define P_GESTURE_PINCH_SIMULATION_ENABLED  (1)

class PDevice;
class PGesturePinchSimulator;
enum PInputEventTypeEnum 
{
    P_INPUT_EVENT_KEY,
    P_INPUT_EVENT_TOUCH,
    P_INPUT_EVENT_UNKNOWN,
};

enum PInputPointingDeviceStateEnum
{
    P_POINTING_DEVICE_STATE_DOWN, ///< Pressed. 
    P_POINTING_DEVICE_STATE_MOVE, ///< Pointer moved. Not all platforms support this, for example many touch screens. 
    P_POINTING_DEVICE_STATE_DRAG, ///< Pointer dragged. 
    P_POINTING_DEVICE_STATE_UP    ///< Released. 
};

enum PInputCursorStateEnum
{
    P_CURSOR_STATE_DOWN,       //< finger down. 
    P_CURSOR_STATE_STATIONARY, //< finger still. 
    P_CURSOR_STATE_MOVE,       //< finger move. 
    P_CURSOR_STATE_UP          //< finger up. 
};

enum PInputTouchStateEnum
{
    P_TOUCH_STATE_BEGIN,    ///< First finger down. 
    P_TOUCH_STATE_CONTINUE, ///< Gesture or touch event in progress. 
    P_TOUCH_STATE_END       ///< Last finger up. 
};

enum PInputKeyDeviceStateEnum
{
    P_KEY_DEVICE_STATE_DOWN     = 0x01,   ///< Key down
    P_KEY_DEVICE_STATE_UP       = 0x02,   ///< Key up
    P_KEY_DEVICE_STATE_REPEAT   = 0x04,   ///< The key is pressed
    P_KEY_DEVICE_STATE_CTRL     = 0x08,   ///< The ctrl is pressed down
    P_KEY_DEVICE_STATE_ALT      = 0x10,   ///< Ditto
    P_KEY_DEVICE_STATE_SHIFT    = 0x20,   ///< Ditto
};

/// Platform independent key scancode 
enum PInputKey
{
    // Unknown keys are used for unmapped keys. 
    P_KEY_UNKNOWN = 0,

    // Keys used for numeric input 
    P_KEY_0,
    P_KEY_1,
    P_KEY_2,
    P_KEY_3,
    P_KEY_4,
    P_KEY_5,
    P_KEY_6,
    P_KEY_7,
    P_KEY_8,
    P_KEY_9,
    P_KEY_COMMA,
    P_KEY_PERIOD,
    P_KEY_SEMICOLON,
    P_KEY_DECIMAL_SEPARATOR,      ///< allows for locale specific separator key mapping */
    P_KEY_THOUSANDS_SEPARATOR,    ///< allows for locale specific separator key mapping */
    P_KEY_STAR,
    P_KEY_POUND,

    // Keys used for text input 
    P_KEY_A,
    P_KEY_B,
    P_KEY_C,
    P_KEY_D,
    P_KEY_E,
    P_KEY_F,
    P_KEY_G,
    P_KEY_H,
    P_KEY_I,
    P_KEY_J,
    P_KEY_K,
    P_KEY_L,
    P_KEY_M,
    P_KEY_N,
    P_KEY_O,
    P_KEY_P,
    P_KEY_Q,
    P_KEY_R,
    P_KEY_S,
    P_KEY_T,
    P_KEY_U,
    P_KEY_V,
    P_KEY_W,
    P_KEY_X,
    P_KEY_Y,
    P_KEY_Z,

    P_KEY_SPACE,
    P_KEY_BACKSPACE,
    P_KEY_ENTER,
    P_KEY_TAB,
    P_KEY_ESC,
    
    P_KEY_CAPS_LOCK,
    P_KEY_NUM_LOCK,
    P_KEY_PRINT_SCREEN,
    P_KEY_SCROLL_LOCK,
    P_KEY_BREAK,

    P_KEY_INSERT,
    P_KEY_DELETE,
    P_KEY_HOME,
    P_KEY_END,
    P_KEY_PAGE_UP,
    P_KEY_PAGE_DOWN,

    P_KEY_EQUAL,
    P_KEY_MINUS,
    P_KEY_MULTIPLY,
    P_KEY_DIVIDE,
    P_KEY_BACKSLASH,
    P_KEY_LEFT_BRACKET,
    P_KEY_RIGHT_BRACKET,
    P_KEY_QUOTELEFT,
    P_KEY_QUOTE,

    P_KEY_ARROW_UP,
    P_KEY_ARROW_DOWN,
    P_KEY_ARROW_LEFT,
    P_KEY_ARROW_RIGHT,
    
    P_KEY_NUM_0,
    P_KEY_NUM_1,
    P_KEY_NUM_2,
    P_KEY_NUM_3,
    P_KEY_NUM_4,
    P_KEY_NUM_5,
    P_KEY_NUM_6,
    P_KEY_NUM_7,
    P_KEY_NUM_8,
    P_KEY_NUM_9,
    P_KEY_NUM_DIVIDE,
    P_KEY_NUM_MULTIPLY,
    P_KEY_NUM_PLUS,
    P_KEY_NUM_MINUS,
    P_KEY_NUM_COMMA,
    P_KEY_NUM_ENTER,
    
    P_KEY_F1,
    P_KEY_F2,
    P_KEY_F3,
    P_KEY_F4,
    P_KEY_F5,
    P_KEY_F6,
    P_KEY_F7,
    P_KEY_F8,
    P_KEY_F9,
    P_KEY_F10,
    P_KEY_F11,
    P_KEY_F12,

    P_KEY_VOLUME_UP,
    P_KEY_VOLUME_DOWN,

    P_KEY_SOFTKEY1,  
    P_KEY_SOFTKEY2,
    P_KEY_SOFTKEY3,
    P_KEY_SOFTKEY4,
    
    // Generic names for modifier keys such as shift, ctrl, alt, Fn, command, super, etc. 
    P_KEY_MODIFIER1, 
    P_KEY_MODIFIER2,
    P_KEY_MODIFIER3,
    
    // Specific modifier key names on a common IBM keyboard 
    // We don't distinguish left and right alt, ctrl and shift.
    P_KEY_ALT,
    P_KEY_CONTROL,
    P_KEY_SHIFT,
    P_KEY_META, // On Mac OS X, this corresponds to the Commands keys. On Windows keyboards, this key is mapped to the Windows key.
    P_KEY_MENU, // The context key on Windows keyboards.

    P_KEY_PAUSE,
    P_KEY_PRINT,
    
    P_KEY_COUNT
};


// Structure for multi-touch events (touch screen) from system.
class PInputEventTouch
{
public:
    // One finger in multi-touch event.
    struct TouchCursor
    {
        /// The id of this cursor.
        puint32 m_id;
        /// X coordinate for pointer. 
        pint16 m_x;                    
        /// Y coordinate for pointer. It is in the viewport coordinate. The origin is at the 
        /// left bottom corner and the Y is up.
        pint16 m_y;                    
        /// Pressure information for pointer, if supported by platform. Otherwise value unspecified.
        pfloat32 m_pressure;      
        ///< Size of the area touched if supported by platform. Otherwise value unspecified. 
        pfloat32 m_radius;             
       /// when m_state is down it is down Event Time. when m_state is up it is up Event Time. other state
       /// it is not useful.
        puint32 m_timestamp;
       ///< State of this particular cursor. 
        PInputCursorStateEnum m_state; 
    };

    TouchCursor* getCursor(puint32 index)
    {
        PASSERT(index < m_cursorCount);
        return &m_cursorData[index];
    }
    
    const TouchCursor* getCursor(puint32 index) const
    {
        PASSERT(index < m_cursorCount);
        if (index < m_cursorCount)
        {
            return &m_cursorData[index];
        }
        return P_NULL;
    }

    void setCursorCount(puint32 count)
    {
        m_cursorCount = count;
    }

    puint32 getCursorCount() const
    {
        return m_cursorCount;
    }

    void setState(PInputTouchStateEnum state)
    {
        m_state = state;
    }

    PInputTouchStateEnum getState() const
    {
        return m_state;
    }
    
private:
    puint32 m_cursorCount;  // Number of cursors for this touch event. 
    TouchCursor m_cursorData[P_INPUT_MAXIMUM_TOUCH]; // Data for all the cursors. 
    PInputTouchStateEnum m_state; // The state of this touch input event.
};

// An key event from keyboard, keypad, buttons, digital joysticks ...
class PInputEventKey
{
public:
    // Queries.
    P_INLINE PInputKey getKey() const { return m_key; }
    P_INLINE puint32 getNativeScanCode() const { return m_nativeScanCode; }
    P_INLINE puint32 getState() const { return m_state; }
    P_INLINE puint32 getKeyPressState() const { return m_state & 0x3; }
    P_INLINE puint32 getTimestamp() const { return m_timestamp; }

    P_INLINE void setTranslatedScanCode(PInputKey key) { m_key = key; }
    P_INLINE void setRawScanCode(puint32 scancode) { m_nativeScanCode = scancode; }
    P_INLINE void setState(puint32 state) { m_state = state; }
    P_INLINE void setTimestamp(puint32 timestamp) { m_timestamp = timestamp; }
    
private:
    PInputKey m_key;           ///< Platform independent button key value. 
    puint32 m_nativeScanCode;  ///< Raw value from the OS.
    puint32 m_state;           ///< Button state; a union of PInputKeyDeviceStateEnum.
    puint32 m_timestamp;
};

class PInputEvent
{
public:
    PInputEvent() { m_type = P_INPUT_EVENT_UNKNOWN; }

    P_INLINE PInputEventTypeEnum getType() const { return m_type; }
    P_INLINE void setType(PInputEventTypeEnum type) { m_type = type; }
    P_INLINE PInputEventKey* getKeyEvent() { return &m_event.m_key; }
    P_INLINE PInputEventTouch* getTouchEvent() { return &m_event.m_touch; }
        
private:
    PInputEventTypeEnum m_type;

    union 
    {
        PInputEventKey m_key;
        PInputEventTouch m_touch;
    } m_event;
};

#if P_GESTURE_PINCH_SIMULATION_ENABLED == 1
class PGesturePinchSimulator
{
public:
    PGesturePinchSimulator()
    {
        m_firstX = 0;
        m_firstY = 0;

        m_secondX = 0;
        m_secondY = 0;
   
        m_state = P_POINTING_DEVICE_STATE_UP;
        m_cursorState = P_CURSOR_STATE_UP;
    }

    void createEvent(PInputEventTouch* event)
    {
        if (event != P_NULL)
        {
            if (event->getState() == P_TOUCH_STATE_END)
            {
                m_cursorState = P_CURSOR_STATE_UP;
            }
            for (int i = 0; i < 2; i++)
            {
                PInputEventTouch::TouchCursor* c = event->getCursor(i);
                c->m_id = i;
                if (i == 0)
                {
                    c->m_x = m_firstX;
                    c->m_y = m_firstY;
                }
                else
                {
                    c->m_x = m_secondX;
                    c->m_y = m_secondY;
                }
                c->m_state = m_cursorState;
                c->m_pressure = 1.0f;
                c->m_radius = 1.0f;
                c->m_timestamp = pTimeGetCurrentTimestamp();
            }
        }
    }
    
    PInputPointingDeviceStateEnum getState()
    {
        return m_state;
    }

    void createEvent(pint32 x, pint32 y, PInputPointingDeviceStateEnum state, PInputEventTouch* event)
    {
        if (state == P_POINTING_DEVICE_STATE_DOWN )
        {
            m_firstX = x - 50;
            m_firstY = y;
            m_secondX = x + 50;
            m_secondY = y;
            m_origX = x;
            m_origY = y;

            m_cursorState = P_CURSOR_STATE_DOWN;
        }
        else if (state  == P_POINTING_DEVICE_STATE_DRAG )
        {
            m_cursorState = P_CURSOR_STATE_MOVE;
            if (x == m_origX && y == m_origY)
            {
            }
            else
            {
                pfloat32 deltaX = (pfloat32)(x - m_origX);
                pfloat32 deltaY = (pfloat32)(y - m_origY);

                pfloat32 l = sqrtf(deltaX * deltaX + deltaY * deltaY);
                pfloat32 dirx = deltaX / l;
                pfloat32 diry = deltaY / l;

                pfloat32 dx = diry * (100.0f - l);
                pfloat32 dy = -dirx * (100.0f - l);

                m_secondX = (pint32)((pfloat32)m_origX - dx);
                m_secondY = (pint32)((pfloat32)m_origY - dy);
                m_firstX = (pint32)((pfloat32)m_origX + dx);
                m_firstY = (pint32)((pfloat32)m_origY + dy);
            }
        }
        else if (state == P_POINTING_DEVICE_STATE_UP)
        {
            m_cursorState = P_CURSOR_STATE_UP;
        }

        m_state = state;

        createEvent(event);
    }

private:
    
    pint32 m_firstX;
    pint32 m_firstY;
    pint32 m_secondX;
    pint32 m_secondY;
    pint32 m_origX;
    pint32 m_origY;
    PInputCursorStateEnum m_cursorState;
    PInputPointingDeviceStateEnum m_state;
    
};
#endif

class P_DLLEXPORT PInputEventQueue
{
public:
    // Constructor.
    // \param window the parent window.
    PInputEventQueue(PDevice* window);

    // Destructor.
    ~PInputEventQueue();
        
    // Add touch event to the queue.
    // \param the number of touch points.
    // \param the state of this touch event
    // \return the touch event just added.
    PInputEventTouch* addTouchEvent(puint32 touchPoints, PInputTouchStateEnum state);

    // Set the cursor data of one touch event.
    // \param timestamp the time of the event
    void setTouchCursor(PInputEventTouch* touchEvent, pint32 index, pint32 id,
            pfloat32 x, pfloat32 y, pfloat32 pressure, pfloat32 area, pint32 state, puint32 timestamp);
    
    // Add left button of the mouse event to the queue. The event will be translated into
    // a touch event before put it into the queue. This function is only useful for PC.
    // \param x the mouse pointer x coordinate.
    // \param y the mouse pointer y coordinate in the window coordinate (the origin is at top left).
    // \param state the state of the button
    // \param isCtrlDown if the ctrl key (left and right) is down during the mouse action.
    // \param timestamp the time of the event
    void addPointerEvent(pint32 x, pint32 y, PInputPointingDeviceStateEnum state, pbool isCtrlHeld, puint32 timestamp);

    // Add keyboard event to the queue.
    // \param keyCode the raw scan code of the keyboard input.
    // \param key the translated key code.
    // \param state the state of the key button. 
    // \param timestamp the time of the event
    void addKeyEvent(puint32 nativeScanCode, PInputKey key, puint32 state, puint32 timestamp);

    // Clear the current events. 
    void clear();

    // Get the number of events in queue.
    // \return the event number.
    puint32 getNumberOfEvents() const;

    // Get the event at the index.
    // \param index the index.
    // \return the event at the index of the queue.
    PInputEvent* getEventAtIndex(puint32 index);

private:
    // Allocate a new event node.
    PInputEvent* reserveEvent();

private:
    PInputEvent* m_events; // The event queue.
    puint32      m_numberOfEvents; // The number of events in the queue.
    puint32      m_numberOfEventsCached; // The total events in the pool.
    PDevice*     m_device; // The parent device.
    
 #if P_GESTURE_PINCH_SIMULATION_ENABLED == 1
    PGesturePinchSimulator *m_pinchSimulator;
 #endif
};

#endif // PINPUT_H
