#ifndef __ZG_EVENT_H__
#define __ZG_EVENT_H__

//
//  \brief General event structure
//
typedef union ZG_Event
{
    uint32_t type;                         //< Event type, shared with all events
    ZG_CommonEvent common;                 // < Common event data
    ZG_DisplayEvent display;               // < Display event data
    ZG_WindowEvent window;                 // < Window event data
    ZG_KeyboardEvent key;                  // < Keyboard event data
    ZG_TextEditingEvent edit;              // < Text editing event data
    ZG_TextEditingExtEvent editExt;        // < Extended text editing event data
    ZG_TextInputEvent text;                // < Text input event data
    ZG_MouseMotionEvent motion;            // < Mouse motion event data
    ZG_MouseButtonEvent button;            // < Mouse button event data
    ZG_MouseWheelEvent wheel;              // < Mouse wheel event data
    ZG_JoyAxisEvent jaxis;                 // < Joystick axis event data
    ZG_JoyBallEvent jball;                 // < Joystick ball event data
    ZG_JoyHatEvent jhat;                   // < Joystick hat event data
    ZG_JoyButtonEvent jbutton;             // < Joystick button event data
    ZG_JoyDeviceEvent jdevice;             // < Joystick device change event data
    ZG_JoyBatteryEvent jbattery;           // < Joystick battery event data
    ZG_ControllerAxisEvent caxis;          // < Game Controller axis event data
    ZG_ControllerButtonEvent cbutton;      // < Game Controller button event data
    ZG_ControllerDeviceEvent cdevice;      // < Game Controller device event data
    SDL_ControllerTouchpadEvent ctouchpad; // < Game Controller touchpad event data
    SDL_ControllerSensorEvent csensor;     // < Game Controller sensor event data
    SDL_AudioDeviceEvent adevice;          // < Audio device event data
    SDL_SensorEvent sensor;                // < Sensor event data
    SDL_QuitEvent quit;                    // < Quit request event data
    SDL_UserEvent user;                    // < Custom event data
    SDL_SysWMEvent syswm;                  // < System dependent window event data
    SDL_TouchFingerEvent tfinger;          // < Touch finger event data
    SDL_MultiGestureEvent mgesture;        // < Gesture event data
    SDL_DollarGestureEvent dgesture;       // < Gesture event data
    SDL_DropEvent drop;                    // < Drag and drop event data

    // This is necessary for ABI compatibility between Visual C++ and GCC.
    // Visual C++ will respect the push pack pragma and use 52 bytes (size of
    // SDL_TextEditingEvent, the largest structure for 32-bit and 64-bit
    // architectures) for this union, and GCC will use the alignment of the
    // largest datatype within the union, which is 8 bytes on 64-bit
    // architectures.
    //
    //   So... we'll add padding to force the size to be 56 bytes for both.
    //
    //   On architectures where pointers are 16 bytes, this needs rounding up to
    //   the next multiple of 16, 64, and on architectures where pointers are
    //   even larger the size of SDL_UserEvent will dominate as being 3 pointers.

    Uint8 padding[sizeof(void *) <= 8 ? 56 : sizeof(void *) == 16 ? 64 : 3 * sizeof(void *)];
} ZG_Event;

#endif
