#ifndef __ZG_EVENT_H__
#define __ZG_EVENT_H__
/*
 * The types of events that can be delivered.
 */
typedef enum
{
    ZG_FIRSTEVENT     = 0,     /**< Unused (do not remove) */

    /* Application events */
    ZG_QUIT           = 0x100, /**< User-requested quit */

    /* These application events have special meaning on iOS, see README-ios.md for details */
    ZG_APP_TERMINATING,        /**< The application is being terminated by the OS
                                     Called on iOS in applicationWillTerminate()
                                     Called on Android in onDestroy()
                                */
    ZG_APP_LOWMEMORY,          /**< The application is low on memory, free memory if possible.
                                     Called on iOS in applicationDidReceiveMemoryWarning()
                                     Called on Android in onLowMemory()
                                */
    ZG_APP_WILLENTERBACKGROUND, /**< The application is about to enter the background
                                     Called on iOS in applicationWillResignActive()
                                     Called on Android in onPause()
                                */
    ZG_APP_DIDENTERBACKGROUND, /**< The application did enter the background and may not get CPU for some time
                                     Called on iOS in applicationDidEnterBackground()
                                     Called on Android in onPause()
                                */
    ZG_APP_WILLENTERFOREGROUND, /**< The application is about to enter the foreground
                                     Called on iOS in applicationWillEnterForeground()
                                     Called on Android in onResume()
                                */
    ZG_APP_DIDENTERFOREGROUND, /**< The application is now interactive
                                     Called on iOS in applicationDidBecomeActive()
                                     Called on Android in onResume()
                                */

    ZG_LOCALECHANGED,  /**< The user's locale preferences have changed. */

    /* Display events */
    ZG_DISPLAYEVENT   = 0x150,  /**< Display state change */

    /* Window events */
    ZG_WINDOWEVENT    = 0x200, /**< Window state change */
    ZG_SYSWMEVENT,             /**< System specific event */

    /* Keyboard events */
    ZG_KEYDOWN        = 0x300, /**< Key pressed */
    ZG_KEYUP,                  /**< Key released */
    ZG_TEXTEDITING,            /**< Keyboard text editing (composition) */
    ZG_TEXTINPUT,              /**< Keyboard text input */
    ZG_KEYMAPCHANGED,          /**< Keymap changed due to a system event such as an
                                     input language or keyboard layout change.
                                */
    ZG_TEXTEDITING_EXT,       /**< Extended keyboard text editing (composition) */

    /* Mouse events */
    ZG_MOUSEMOTION    = 0x400, /**< Mouse moved */
    ZG_MOUSEBUTTONDOWN,        /**< Mouse button pressed */
    ZG_MOUSEBUTTONUP,          /**< Mouse button released */
    ZG_MOUSEWHEEL,             /**< Mouse wheel motion */

    /* Joystick events */
    ZG_JOYAXISMOTION  = 0x600, /**< Joystick axis motion */
    ZG_JOYBALLMOTION,          /**< Joystick trackball motion */
    ZG_JOYHATMOTION,           /**< Joystick hat position change */
    ZG_JOYBUTTONDOWN,          /**< Joystick button pressed */
    ZG_JOYBUTTONUP,            /**< Joystick button released */
    ZG_JOYDEVICEADDED,         /**< A new joystick has been inserted into the system */
    ZG_JOYDEVICEREMOVED,       /**< An opened joystick has been removed */
    ZG_JOYBATTERYUPDATED,      /**< Joystick battery level change */

    /* Game controller events */
    ZG_CONTROLLERAXISMOTION  = 0x650, /**< Game controller axis motion */
    ZG_CONTROLLERBUTTONDOWN,          /**< Game controller button pressed */
    ZG_CONTROLLERBUTTONUP,            /**< Game controller button released */
    ZG_CONTROLLERDEVICEADDED,         /**< A new Game controller has been inserted into the system */
    ZG_CONTROLLERDEVICEREMOVED,       /**< An opened Game controller has been removed */
    ZG_CONTROLLERDEVICEREMAPPED,      /**< The controller mapping was updated */
    ZG_CONTROLLERTOUCHPADDOWN,        /**< Game controller touchpad was touched */
    ZG_CONTROLLERTOUCHPADMOTION,      /**< Game controller touchpad finger was moved */
    ZG_CONTROLLERTOUCHPADUP,          /**< Game controller touchpad finger was lifted */
    ZG_CONTROLLERSENSORUPDATE,        /**< Game controller sensor was updated */

    /* Touch events */
    ZG_FINGERDOWN      = 0x700,
    ZG_FINGERUP,
    ZG_FINGERMOTION,

    /* Gesture events */
    ZG_DOLLARGESTURE   = 0x800,
    ZG_DOLLARRECORD,
    ZG_MULTIGESTURE,

    /* Clipboard events */
    ZG_CLIPBOARDUPDATE = 0x900, /**< The clipboard changed */

    /* Drag and drop events */
    ZG_DROPFILE        = 0x1000, /**< The system requests a file open */
    ZG_DROPTEXT,                 /**< text/plain drag-and-drop event */
    ZG_DROPBEGIN,                /**< A new set of drops is beginning (NULL filename) */
    ZG_DROPCOMPLETE,             /**< Current set of drops is now complete (NULL filename) */

    /* Audio hotplug events */
    ZG_AUDIODEVICEADDED = 0x1100, /**< A new audio device is available */
    ZG_AUDIODEVICEREMOVED,        /**< An audio device has been removed. */

    /* Sensor events */
    ZG_SENSORUPDATE = 0x1200,     /**< A sensor was updated */

    /* Render events */
    ZG_RENDER_TARGETS_RESET = 0x2000, /**< The render targets have been reset and their contents need to be updated */
    ZG_RENDER_DEVICE_RESET, /**< The device has been reset and all textures need to be recreated */

    /* Internal events */
    ZG_POLLSENTINEL = 0x7F00, /**< Signals the end of an event poll cycle */

    /** Events ::ZG_USEREVENT through ::ZG_LASTEVENT are for your use,
     *  and should be allocated with ZG_RegisterEvents()
     */
    ZG_USEREVENT    = 0x8000,

    /**
     *  This last event is only for bounding internal arrays
     */
    ZG_LASTEVENT    = 0xFFFF
} ZG_EventType;

/**
 *  \brief The SDL keysym structure, used in key events.
 *
 *  \note  If you are looking for translated character input, see the ::ZG_TEXTINPUT event.
 */
typedef struct ZG_Keysym
{
    ZG_Scancode scancode;      /**< SDL physical key code - see ::ZG_Scancode for details */
    ZG_Keycode sym;            /**< SDL virtual key code - see ::ZG_Keycode for details */
    uint16_t mod;                 /**< current key modifiers */
    uint32_t unused;
} ZG_Keysym;

typedef struct ZG_KeyboardEvent
{
    uint32_t type;        /**< ::ZG_KEYDOWN or ::ZG_KEYUP */
    uint32_t timestamp;   /**< In milliseconds, populated using ZG_GetTicks() */
    uint32_t windowID;    /**< The window with keyboard focus, if any */
    uint8_t state;        /**< ::ZG_PRESSED or ::ZG_RELEASED */
    uint8_t repeat;       /**< Non-zero if this is a key repeat */
    uint8_t padding2;
    uint8_t padding3;
    ZG_Keysym keysym;  /**< The key that was pressed or released */
} ZG_KeyboardEvent;

/**
 *  \brief Mouse motion event structure (event.motion.*)
 */
typedef struct ZG_MouseMotionEvent
{
    uint32_t type;        /**< ::ZG_MOUSEMOTION */
    uint32_t timestamp;   /**< In milliseconds, populated using ZG_GetTicks() */
    uint32_t windowID;    /**< The window with mouse focus, if any */
    uint32_t which;       /**< The mouse instance id, or ZG_TOUCH_MOUSEID */
    uint32_t state;       /**< The current button state */
    int32_t x;           /**< X coordinate, relative to window */
    int32_t y;           /**< Y coordinate, relative to window */
    int32_t xrel;        /**< The relative motion in the X direction */
    int32_t yrel;        /**< The relative motion in the Y direction */
} ZG_MouseMotionEvent;

/**
 *  \brief Mouse button event structure (event.button.*)
 */
typedef struct ZG_MouseButtonEvent
{
    uint32_t type;        /**< ::ZG_MOUSEBUTTONDOWN or ::ZG_MOUSEBUTTONUP */
    uint32_t timestamp;   /**< In milliseconds, populated using ZG_GetTicks() */
    uint32_t windowID;    /**< The window with mouse focus, if any */
    uint32_t which;       /**< The mouse instance id, or ZG_TOUCH_MOUSEID */
    uint8_t button;       /**< The mouse button index */
    uint8_t state;        /**< ::ZG_PRESSED or ::ZG_RELEASED */
    uint8_t clicks;       /**< 1 for single-click, 2 for double-click, etc. */
    uint8_t padding1;
    int32_t x;           /**< X coordinate, relative to window */
    int32_t y;           /**< Y coordinate, relative to window */
} ZG_MouseButtonEvent;

/**
 *  \brief Mouse wheel event structure (event.wheel.*)
 */
typedef struct ZG_MouseWheelEvent
{
    uint32_t type;        /**< ::ZG_MOUSEWHEEL */
    uint32_t timestamp;   /**< In milliseconds, populated using ZG_GetTicks() */
    uint32_t windowID;    /**< The window with mouse focus, if any */
    uint32_t which;       /**< The mouse instance id, or ZG_TOUCH_MOUSEID */
    int32_t x;           /**< The amount scrolled horizontally, positive to the right and negative to the left */
    int32_t y;           /**< The amount scrolled vertically, positive away from the user and negative toward the user */
    uint32_t direction;   /**< Set to one of the ZG_MOUSEWHEEL_* defines. When FLIPPED the values in X and Y will be opposite. Multiply by -1 to change them back */
    float preciseX;     /**< The amount scrolled horizontally, positive to the right and negative to the left, with float precision (added in 2.0.18) */
    float preciseY;     /**< The amount scrolled vertically, positive away from the user and negative toward the user, with float precision (added in 2.0.18) */
} ZG_MouseWheelEvent;

/**
 *  \brief A user-defined event type (event.user.*)
 */
typedef struct{
    uint32_t type;        /**< ::ZG_USEREVENT through ::ZG_LASTEVENT-1 */
    uint32_t timestamp;   /**< In milliseconds, populated using ZG_GetTicks() */
    uint32_t windowID;    /**< The associated window if any */
    int32_t code;        /**< User defined event code */
    void *data1;        /**< User defined data pointer */
    void *data2;        /**< User defined data pointer */
} ZG_UserEvent;

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
    ZG_ControllerTouchpadEvent ctouchpad; // < Game Controller touchpad event data
    ZG_ControllerSensorEvent csensor;     // < Game Controller sensor event data
    ZG_AudioDeviceEvent adevice;          // < Audio device event data
    ZG_SensorEvent sensor;                // < Sensor event data
    ZG_QuitEvent quit;                    // < Quit request event data
    ZG_UserEvent user;                    // < Custom event data
    ZG_SysWMEvent syswm;                  // < System dependent window event data
    ZG_TouchFingerEvent tfinger;          // < Touch finger event data
    ZG_MultiGestureEvent mgesture;        // < Gesture event data
    ZG_DollarGestureEvent dgesture;       // < Gesture event data
    ZG_DropEvent drop;                    // < Drag and drop event data

    // This is necessary for ABI compatibility between Visual C++ and GCC.
    // Visual C++ will respect the push pack pragma and use 52 bytes (size of
    // ZG_TextEditingEvent, the largest structure for 32-bit and 64-bit
    // architectures) for this union, and GCC will use the alignment of the
    // largest datatype within the union, which is 8 bytes on 64-bit
    // architectures.
    //
    //   So... we'll add padding to force the size to be 56 bytes for both.
    //
    //   On architectures where pointers are 16 bytes, this needs rounding up to
    //   the next multiple of 16, 64, and on architectures where pointers are
    //   even larger the size of ZG_UserEvent will dominate as being 3 pointers.

    uint8_t padding[sizeof(void *) <= 8 ? 56 : sizeof(void *) == 16 ? 64 : 3 * sizeof(void *)];
} ZG_Event;

#endif
