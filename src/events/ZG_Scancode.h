/*
  Simple DirectMedia Layer
  Copyright (C) 1997-2022 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

/**
 *  \file ZG_scancode.h
 *
 *  Defines keyboard scancodes.
 */

#ifndef ZG_scancode_h_
#define ZG_scancode_h_

#include "ZG_stdinc.h"

/**
 *  \brief The SDL keyboard scancode representation.
 *
 *  Values of this type are used to represent keyboard keys, among other places
 *  in the \link ZG_Keysym::scancode key.keysym.scancode \endlink field of the
 *  ZG_Event structure.
 *
 *  The values in this enumeration are based on the USB usage page standard:
 *  https://www.usb.org/sites/default/files/documents/hut1_12v2.pdf
 */
typedef enum
{
    ZG_SCANCODE_UNKNOWN = 0,

    /**
     *  \name Usage page 0x07
     *
     *  These values are from usage page 0x07 (USB keyboard page).
     */
    /* @{ */

    ZG_SCANCODE_A = 4,
    ZG_SCANCODE_B = 5,
    ZG_SCANCODE_C = 6,
    ZG_SCANCODE_D = 7,
    ZG_SCANCODE_E = 8,
    ZG_SCANCODE_F = 9,
    ZG_SCANCODE_G = 10,
    ZG_SCANCODE_H = 11,
    ZG_SCANCODE_I = 12,
    ZG_SCANCODE_J = 13,
    ZG_SCANCODE_K = 14,
    ZG_SCANCODE_L = 15,
    ZG_SCANCODE_M = 16,
    ZG_SCANCODE_N = 17,
    ZG_SCANCODE_O = 18,
    ZG_SCANCODE_P = 19,
    ZG_SCANCODE_Q = 20,
    ZG_SCANCODE_R = 21,
    ZG_SCANCODE_S = 22,
    ZG_SCANCODE_T = 23,
    ZG_SCANCODE_U = 24,
    ZG_SCANCODE_V = 25,
    ZG_SCANCODE_W = 26,
    ZG_SCANCODE_X = 27,
    ZG_SCANCODE_Y = 28,
    ZG_SCANCODE_Z = 29,

    ZG_SCANCODE_1 = 30,
    ZG_SCANCODE_2 = 31,
    ZG_SCANCODE_3 = 32,
    ZG_SCANCODE_4 = 33,
    ZG_SCANCODE_5 = 34,
    ZG_SCANCODE_6 = 35,
    ZG_SCANCODE_7 = 36,
    ZG_SCANCODE_8 = 37,
    ZG_SCANCODE_9 = 38,
    ZG_SCANCODE_0 = 39,

    ZG_SCANCODE_RETURN = 40,
    ZG_SCANCODE_ESCAPE = 41,
    ZG_SCANCODE_BACKSPACE = 42,
    ZG_SCANCODE_TAB = 43,
    ZG_SCANCODE_SPACE = 44,

    ZG_SCANCODE_MINUS = 45,
    ZG_SCANCODE_EQUALS = 46,
    ZG_SCANCODE_LEFTBRACKET = 47,
    ZG_SCANCODE_RIGHTBRACKET = 48,
    ZG_SCANCODE_BACKSLASH = 49, /**< Located at the lower left of the return
                                  *   key on ISO keyboards and at the right end
                                  *   of the QWERTY row on ANSI keyboards.
                                  *   Produces REVERSE SOLIDUS (backslash) and
                                  *   VERTICAL LINE in a US layout, REVERSE
                                  *   SOLIDUS and VERTICAL LINE in a UK Mac
                                  *   layout, NUMBER SIGN and TILDE in a UK
                                  *   Windows layout, DOLLAR SIGN and POUND SIGN
                                  *   in a Swiss German layout, NUMBER SIGN and
                                  *   APOSTROPHE in a German layout, GRAVE
                                  *   ACCENT and POUND SIGN in a French Mac
                                  *   layout, and ASTERISK and MICRO SIGN in a
                                  *   French Windows layout.
                                  */
    ZG_SCANCODE_NONUSHASH = 50, /**< ISO USB keyboards actually use this code
                                  *   instead of 49 for the same key, but all
                                  *   OSes I've seen treat the two codes
                                  *   identically. So, as an implementor, unless
                                  *   your keyboard generates both of those
                                  *   codes and your OS treats them differently,
                                  *   you should generate ZG_SCANCODE_BACKSLASH
                                  *   instead of this code. As a user, you
                                  *   should not rely on this code because SDL
                                  *   will never generate it with most (all?)
                                  *   keyboards.
                                  */
    ZG_SCANCODE_SEMICOLON = 51,
    ZG_SCANCODE_APOSTROPHE = 52,
    ZG_SCANCODE_GRAVE = 53, /**< Located in the top left corner (on both ANSI
                              *   and ISO keyboards). Produces GRAVE ACCENT and
                              *   TILDE in a US Windows layout and in US and UK
                              *   Mac layouts on ANSI keyboards, GRAVE ACCENT
                              *   and NOT SIGN in a UK Windows layout, SECTION
                              *   SIGN and PLUS-MINUS SIGN in US and UK Mac
                              *   layouts on ISO keyboards, SECTION SIGN and
                              *   DEGREE SIGN in a Swiss German layout (Mac:
                              *   only on ISO keyboards), CIRCUMFLEX ACCENT and
                              *   DEGREE SIGN in a German layout (Mac: only on
                              *   ISO keyboards), SUPERSCRIPT TWO and TILDE in a
                              *   French Windows layout, COMMERCIAL AT and
                              *   NUMBER SIGN in a French Mac layout on ISO
                              *   keyboards, and LESS-THAN SIGN and GREATER-THAN
                              *   SIGN in a Swiss German, German, or French Mac
                              *   layout on ANSI keyboards.
                              */
    ZG_SCANCODE_COMMA = 54,
    ZG_SCANCODE_PERIOD = 55,
    ZG_SCANCODE_SLASH = 56,

    ZG_SCANCODE_CAPSLOCK = 57,

    ZG_SCANCODE_F1 = 58,
    ZG_SCANCODE_F2 = 59,
    ZG_SCANCODE_F3 = 60,
    ZG_SCANCODE_F4 = 61,
    ZG_SCANCODE_F5 = 62,
    ZG_SCANCODE_F6 = 63,
    ZG_SCANCODE_F7 = 64,
    ZG_SCANCODE_F8 = 65,
    ZG_SCANCODE_F9 = 66,
    ZG_SCANCODE_F10 = 67,
    ZG_SCANCODE_F11 = 68,
    ZG_SCANCODE_F12 = 69,

    ZG_SCANCODE_PRINTSCREEN = 70,
    ZG_SCANCODE_SCROLLLOCK = 71,
    ZG_SCANCODE_PAUSE = 72,
    ZG_SCANCODE_INSERT = 73, /**< insert on PC, help on some Mac keyboards (but
                                   does send code 73, not 117) */
    ZG_SCANCODE_HOME = 74,
    ZG_SCANCODE_PAGEUP = 75,
    ZG_SCANCODE_DELETE = 76,
    ZG_SCANCODE_END = 77,
    ZG_SCANCODE_PAGEDOWN = 78,
    ZG_SCANCODE_RIGHT = 79,
    ZG_SCANCODE_LEFT = 80,
    ZG_SCANCODE_DOWN = 81,
    ZG_SCANCODE_UP = 82,

    ZG_SCANCODE_NUMLOCKCLEAR = 83, /**< num lock on PC, clear on Mac keyboards
                                     */
    ZG_SCANCODE_KP_DIVIDE = 84,
    ZG_SCANCODE_KP_MULTIPLY = 85,
    ZG_SCANCODE_KP_MINUS = 86,
    ZG_SCANCODE_KP_PLUS = 87,
    ZG_SCANCODE_KP_ENTER = 88,
    ZG_SCANCODE_KP_1 = 89,
    ZG_SCANCODE_KP_2 = 90,
    ZG_SCANCODE_KP_3 = 91,
    ZG_SCANCODE_KP_4 = 92,
    ZG_SCANCODE_KP_5 = 93,
    ZG_SCANCODE_KP_6 = 94,
    ZG_SCANCODE_KP_7 = 95,
    ZG_SCANCODE_KP_8 = 96,
    ZG_SCANCODE_KP_9 = 97,
    ZG_SCANCODE_KP_0 = 98,
    ZG_SCANCODE_KP_PERIOD = 99,

    ZG_SCANCODE_NONUSBACKSLASH = 100, /**< This is the additional key that ISO
                                        *   keyboards have over ANSI ones,
                                        *   located between left shift and Y.
                                        *   Produces GRAVE ACCENT and TILDE in a
                                        *   US or UK Mac layout, REVERSE SOLIDUS
                                        *   (backslash) and VERTICAL LINE in a
                                        *   US or UK Windows layout, and
                                        *   LESS-THAN SIGN and GREATER-THAN SIGN
                                        *   in a Swiss German, German, or French
                                        *   layout. */
    ZG_SCANCODE_APPLICATION = 101, /**< windows contextual menu, compose */
    ZG_SCANCODE_POWER = 102, /**< The USB document says this is a status flag,
                               *   not a physical key - but some Mac keyboards
                               *   do have a power key. */
    ZG_SCANCODE_KP_EQUALS = 103,
    ZG_SCANCODE_F13 = 104,
    ZG_SCANCODE_F14 = 105,
    ZG_SCANCODE_F15 = 106,
    ZG_SCANCODE_F16 = 107,
    ZG_SCANCODE_F17 = 108,
    ZG_SCANCODE_F18 = 109,
    ZG_SCANCODE_F19 = 110,
    ZG_SCANCODE_F20 = 111,
    ZG_SCANCODE_F21 = 112,
    ZG_SCANCODE_F22 = 113,
    ZG_SCANCODE_F23 = 114,
    ZG_SCANCODE_F24 = 115,
    ZG_SCANCODE_EXECUTE = 116,
    ZG_SCANCODE_HELP = 117,
    ZG_SCANCODE_MENU = 118,
    ZG_SCANCODE_SELECT = 119,
    ZG_SCANCODE_STOP = 120,
    ZG_SCANCODE_AGAIN = 121,   /**< redo */
    ZG_SCANCODE_UNDO = 122,
    ZG_SCANCODE_CUT = 123,
    ZG_SCANCODE_COPY = 124,
    ZG_SCANCODE_PASTE = 125,
    ZG_SCANCODE_FIND = 126,
    ZG_SCANCODE_MUTE = 127,
    ZG_SCANCODE_VOLUMEUP = 128,
    ZG_SCANCODE_VOLUMEDOWN = 129,
/* not sure whether there's a reason to enable these */
/*     ZG_SCANCODE_LOCKINGCAPSLOCK = 130,  */
/*     ZG_SCANCODE_LOCKINGNUMLOCK = 131, */
/*     ZG_SCANCODE_LOCKINGSCROLLLOCK = 132, */
    ZG_SCANCODE_KP_COMMA = 133,
    ZG_SCANCODE_KP_EQUALSAS400 = 134,

    ZG_SCANCODE_INTERNATIONAL1 = 135, /**< used on Asian keyboards, see
                                            footnotes in USB doc */
    ZG_SCANCODE_INTERNATIONAL2 = 136,
    ZG_SCANCODE_INTERNATIONAL3 = 137, /**< Yen */
    ZG_SCANCODE_INTERNATIONAL4 = 138,
    ZG_SCANCODE_INTERNATIONAL5 = 139,
    ZG_SCANCODE_INTERNATIONAL6 = 140,
    ZG_SCANCODE_INTERNATIONAL7 = 141,
    ZG_SCANCODE_INTERNATIONAL8 = 142,
    ZG_SCANCODE_INTERNATIONAL9 = 143,
    ZG_SCANCODE_LANG1 = 144, /**< Hangul/English toggle */
    ZG_SCANCODE_LANG2 = 145, /**< Hanja conversion */
    ZG_SCANCODE_LANG3 = 146, /**< Katakana */
    ZG_SCANCODE_LANG4 = 147, /**< Hiragana */
    ZG_SCANCODE_LANG5 = 148, /**< Zenkaku/Hankaku */
    ZG_SCANCODE_LANG6 = 149, /**< reserved */
    ZG_SCANCODE_LANG7 = 150, /**< reserved */
    ZG_SCANCODE_LANG8 = 151, /**< reserved */
    ZG_SCANCODE_LANG9 = 152, /**< reserved */

    ZG_SCANCODE_ALTERASE = 153, /**< Erase-Eaze */
    ZG_SCANCODE_SYSREQ = 154,
    ZG_SCANCODE_CANCEL = 155,
    ZG_SCANCODE_CLEAR = 156,
    ZG_SCANCODE_PRIOR = 157,
    ZG_SCANCODE_RETURN2 = 158,
    ZG_SCANCODE_SEPARATOR = 159,
    ZG_SCANCODE_OUT = 160,
    ZG_SCANCODE_OPER = 161,
    ZG_SCANCODE_CLEARAGAIN = 162,
    ZG_SCANCODE_CRSEL = 163,
    ZG_SCANCODE_EXSEL = 164,

    ZG_SCANCODE_KP_00 = 176,
    ZG_SCANCODE_KP_000 = 177,
    ZG_SCANCODE_THOUSANDSSEPARATOR = 178,
    ZG_SCANCODE_DECIMALSEPARATOR = 179,
    ZG_SCANCODE_CURRENCYUNIT = 180,
    ZG_SCANCODE_CURRENCYSUBUNIT = 181,
    ZG_SCANCODE_KP_LEFTPAREN = 182,
    ZG_SCANCODE_KP_RIGHTPAREN = 183,
    ZG_SCANCODE_KP_LEFTBRACE = 184,
    ZG_SCANCODE_KP_RIGHTBRACE = 185,
    ZG_SCANCODE_KP_TAB = 186,
    ZG_SCANCODE_KP_BACKSPACE = 187,
    ZG_SCANCODE_KP_A = 188,
    ZG_SCANCODE_KP_B = 189,
    ZG_SCANCODE_KP_C = 190,
    ZG_SCANCODE_KP_D = 191,
    ZG_SCANCODE_KP_E = 192,
    ZG_SCANCODE_KP_F = 193,
    ZG_SCANCODE_KP_XOR = 194,
    ZG_SCANCODE_KP_POWER = 195,
    ZG_SCANCODE_KP_PERCENT = 196,
    ZG_SCANCODE_KP_LESS = 197,
    ZG_SCANCODE_KP_GREATER = 198,
    ZG_SCANCODE_KP_AMPERSAND = 199,
    ZG_SCANCODE_KP_DBLAMPERSAND = 200,
    ZG_SCANCODE_KP_VERTICALBAR = 201,
    ZG_SCANCODE_KP_DBLVERTICALBAR = 202,
    ZG_SCANCODE_KP_COLON = 203,
    ZG_SCANCODE_KP_HASH = 204,
    ZG_SCANCODE_KP_SPACE = 205,
    ZG_SCANCODE_KP_AT = 206,
    ZG_SCANCODE_KP_EXCLAM = 207,
    ZG_SCANCODE_KP_MEMSTORE = 208,
    ZG_SCANCODE_KP_MEMRECALL = 209,
    ZG_SCANCODE_KP_MEMCLEAR = 210,
    ZG_SCANCODE_KP_MEMADD = 211,
    ZG_SCANCODE_KP_MEMSUBTRACT = 212,
    ZG_SCANCODE_KP_MEMMULTIPLY = 213,
    ZG_SCANCODE_KP_MEMDIVIDE = 214,
    ZG_SCANCODE_KP_PLUSMINUS = 215,
    ZG_SCANCODE_KP_CLEAR = 216,
    ZG_SCANCODE_KP_CLEARENTRY = 217,
    ZG_SCANCODE_KP_BINARY = 218,
    ZG_SCANCODE_KP_OCTAL = 219,
    ZG_SCANCODE_KP_DECIMAL = 220,
    ZG_SCANCODE_KP_HEXADECIMAL = 221,

    ZG_SCANCODE_LCTRL = 224,
    ZG_SCANCODE_LSHIFT = 225,
    ZG_SCANCODE_LALT = 226, /**< alt, option */
    ZG_SCANCODE_LGUI = 227, /**< windows, command (apple), meta */
    ZG_SCANCODE_RCTRL = 228,
    ZG_SCANCODE_RSHIFT = 229,
    ZG_SCANCODE_RALT = 230, /**< alt gr, option */
    ZG_SCANCODE_RGUI = 231, /**< windows, command (apple), meta */

    ZG_SCANCODE_MODE = 257,    /**< I'm not sure if this is really not covered
                                 *   by any of the above, but since there's a
                                 *   special KMOD_MODE for it I'm adding it here
                                 */

    /* @} *//* Usage page 0x07 */

    /**
     *  \name Usage page 0x0C
     *
     *  These values are mapped from usage page 0x0C (USB consumer page).
     */
    /* @{ */

    ZG_SCANCODE_AUDIONEXT = 258,
    ZG_SCANCODE_AUDIOPREV = 259,
    ZG_SCANCODE_AUDIOSTOP = 260,
    ZG_SCANCODE_AUDIOPLAY = 261,
    ZG_SCANCODE_AUDIOMUTE = 262,
    ZG_SCANCODE_MEDIASELECT = 263,
    ZG_SCANCODE_WWW = 264,
    ZG_SCANCODE_MAIL = 265,
    ZG_SCANCODE_CALCULATOR = 266,
    ZG_SCANCODE_COMPUTER = 267,
    ZG_SCANCODE_AC_SEARCH = 268,
    ZG_SCANCODE_AC_HOME = 269,
    ZG_SCANCODE_AC_BACK = 270,
    ZG_SCANCODE_AC_FORWARD = 271,
    ZG_SCANCODE_AC_STOP = 272,
    ZG_SCANCODE_AC_REFRESH = 273,
    ZG_SCANCODE_AC_BOOKMARKS = 274,

    /* @} *//* Usage page 0x0C */

    /**
     *  \name Walther keys
     *
     *  These are values that Christian Walther added (for mac keyboard?).
     */
    /* @{ */

    ZG_SCANCODE_BRIGHTNESSDOWN = 275,
    ZG_SCANCODE_BRIGHTNESSUP = 276,
    ZG_SCANCODE_DISPLAYSWITCH = 277, /**< display mirroring/dual display
                                           switch, video mode switch */
    ZG_SCANCODE_KBDILLUMTOGGLE = 278,
    ZG_SCANCODE_KBDILLUMDOWN = 279,
    ZG_SCANCODE_KBDILLUMUP = 280,
    ZG_SCANCODE_EJECT = 281,
    ZG_SCANCODE_SLEEP = 282,

    ZG_SCANCODE_APP1 = 283,
    ZG_SCANCODE_APP2 = 284,

    /* @} *//* Walther keys */

    /**
     *  \name Usage page 0x0C (additional media keys)
     *
     *  These values are mapped from usage page 0x0C (USB consumer page).
     */
    /* @{ */

    ZG_SCANCODE_AUDIOREWIND = 285,
    ZG_SCANCODE_AUDIOFASTFORWARD = 286,

    /* @} *//* Usage page 0x0C (additional media keys) */

    /**
     *  \name Mobile keys
     *
     *  These are values that are often used on mobile phones.
     */
    /* @{ */

    ZG_SCANCODE_SOFTLEFT = 287, /**< Usually situated below the display on phones and
                                      used as a multi-function feature key for selecting
                                      a software defined function shown on the bottom left
                                      of the display. */
    ZG_SCANCODE_SOFTRIGHT = 288, /**< Usually situated below the display on phones and
                                       used as a multi-function feature key for selecting
                                       a software defined function shown on the bottom right
                                       of the display. */
    ZG_SCANCODE_CALL = 289, /**< Used for accepting phone calls. */
    ZG_SCANCODE_ENDCALL = 290, /**< Used for rejecting phone calls. */

    /* @} *//* Mobile keys */

    /* Add any other keys here. */

    ZG_NUM_SCANCODES = 512 /**< not a key, just marks the number of scancodes
                                 for array bounds */
} ZG_Scancode;
