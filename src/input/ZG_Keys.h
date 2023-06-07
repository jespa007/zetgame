/** What we really want is a mapping of every raw key on the keyboard.
 *  To support international keyboards, we use the range 0xA1 - 0xFF
 *  as international virtual keycodes.  We'll follow in the footsteps of X11...
 *  @brief The names of the keys
 */
typedef enum {
	/** @name ASCII mapped keysyms
	 *  The keyboard syms have been cleverly chosen to map to ASCII
	 */
	/*@{*/
	ZG_KEY_ID_UNKNOWN		= 0,
	ZG_KEY_ID_FIRST		= 0,
	ZG_KEY_ID_BACKSPACE	= 8,
	ZG_KEY_ID_TAB			= 9,
	ZG_KEY_ID_CLEAR		= 12,
	ZG_KEY_ID_RETURN		= 13,
	ZG_KEY_ID_PAUSE		= 19,
	ZG_KEY_ID_ESCAPE		= 27,
	ZG_KEY_ID_SPACE		= 32,
	ZG_KEY_ID_EXCLAIM		= 33,
	ZG_KEY_ID_QUOTEDBL		= 34,
	ZG_KEY_ID_HASH		= 35,
	ZG_KEY_ID_DOLLAR		= 36,
	ZG_KEY_ID_AMPERSAND		= 38,
	ZG_KEY_ID_QUOTE		= 39,
	ZG_KEY_ID_LEFTPAREN		= 40,
	ZG_KEY_ID_RIGHTPAREN		= 41,
	ZG_KEY_ID_ASTERISK		= 42,
	ZG_KEY_ID_PLUS		= 43,
	ZG_KEY_ID_COMMA		= 44,
	ZG_KEY_ID_MINUS		= 45,
	ZG_KEY_ID_PERIOD		= 46,
	ZG_KEY_ID_SLASH		= 47,
	ZG_KEY_ID_0			= 48,
	ZG_KEY_ID_1			= 49,
	ZG_KEY_ID_2			= 50,
	ZG_KEY_ID_3			= 51,
	ZG_KEY_ID_4			= 52,
	ZG_KEY_ID_5			= 53,
	ZG_KEY_ID_6			= 54,
	ZG_KEY_ID_7			= 55,
	ZG_KEY_ID_8			= 56,
	ZG_KEY_ID_9			= 57,
	ZG_KEY_ID_COLON		= 58,
	ZG_KEY_ID_SEMICOLON		= 59,
	ZG_KEY_ID_LESS		= 60,
	ZG_KEY_ID_EQUALS		= 61,
	ZG_KEY_ID_GREATER		= 62,
	ZG_KEY_ID_QUESTION		= 63,
	ZG_KEY_ID_AT			= 64,
	/*
	   Skip uppercase letters
	 */
	ZG_KEY_ID_LEFTBRACKET	= 91,
	ZG_KEY_ID_BACKSLASH		= 92,
	ZG_KEY_ID_RIGHTBRACKET	= 93,
	ZG_KEY_ID_CARET		= 94,
	ZG_KEY_ID_UNDERSCORE		= 95,
	ZG_KEY_ID_BACKQUOTE		= 96,
	ZG_KEY_ID_A			= 97,
	ZG_KEY_ID_B			= 98,
	ZG_KEY_ID_C			= 99,
	ZG_KEY_ID_D			= 100,
	ZG_KEY_ID_E			= 101,
	ZG_KEY_ID_F			= 102,
	ZG_KEY_ID_G			= 103,
	ZG_KEY_ID_H			= 104,
	ZG_KEY_ID_I			= 105,
	ZG_KEY_ID_J			= 106,
	ZG_KEY_ID_K			= 107,
	ZG_KEY_ID_L			= 108,
	ZG_KEY_ID_M			= 109,
	ZG_KEY_ID_N			= 110,
	ZG_KEY_ID_O			= 111,
	ZG_KEY_ID_P			= 112,
	ZG_KEY_ID_Q			= 113,
	ZG_KEY_ID_R			= 114,
	ZG_KEY_ID_S			= 115,
	ZG_KEY_ID_T			= 116,
	ZG_KEY_ID_U			= 117,
	ZG_KEY_ID_V			= 118,
	ZG_KEY_ID_W			= 119,
	ZG_KEY_ID_X			= 120,
	ZG_KEY_ID_Y			= 121,
	ZG_KEY_ID_Z			= 122,
	ZG_KEY_ID_DELETE		= 127,
	/* End of ASCII mapped keysyms */
	/*@}*/

	/** @name International keyboard syms */
	/*@{*/
	ZG_KEY_ID_WORLD_0		= 160,		/* 0xA0 */
	ZG_KEY_ID_WORLD_1		= 161,
	ZG_KEY_ID_WORLD_2		= 162,
	ZG_KEY_ID_WORLD_3		= 163,
	ZG_KEY_ID_WORLD_4		= 164,
	ZG_KEY_ID_WORLD_5		= 165,
	ZG_KEY_ID_WORLD_6		= 166,
	ZG_KEY_ID_WORLD_7		= 167,
	ZG_KEY_ID_WORLD_8		= 168,
	ZG_KEY_ID_WORLD_9		= 169,
	ZG_KEY_ID_WORLD_10		= 170,
	ZG_KEY_ID_WORLD_11		= 171,
	ZG_KEY_ID_WORLD_12		= 172,
	ZG_KEY_ID_WORLD_13		= 173,
	ZG_KEY_ID_WORLD_14		= 174,
	ZG_KEY_ID_WORLD_15		= 175,
	ZG_KEY_ID_WORLD_16		= 176,
	ZG_KEY_ID_WORLD_17		= 177,
	ZG_KEY_ID_WORLD_18		= 178,
	ZG_KEY_ID_WORLD_19		= 179,
	ZG_KEY_ID_WORLD_20		= 180,
	ZG_KEY_ID_WORLD_21		= 181,
	ZG_KEY_ID_WORLD_22		= 182,
	ZG_KEY_ID_WORLD_23		= 183,
	ZG_KEY_ID_WORLD_24		= 184,
	ZG_KEY_ID_WORLD_25		= 185,
	ZG_KEY_ID_WORLD_26		= 186,
	ZG_KEY_ID_WORLD_27		= 187,
	ZG_KEY_ID_WORLD_28		= 188,
	ZG_KEY_ID_WORLD_29		= 189,
	ZG_KEY_ID_WORLD_30		= 190,
	ZG_KEY_ID_WORLD_31		= 191,
	ZG_KEY_ID_WORLD_32		= 192,
	ZG_KEY_ID_WORLD_33		= 193,
	ZG_KEY_ID_WORLD_34		= 194,
	ZG_KEY_ID_WORLD_35		= 195,
	ZG_KEY_ID_WORLD_36		= 196,
	ZG_KEY_ID_WORLD_37		= 197,
	ZG_KEY_ID_WORLD_38		= 198,
	ZG_KEY_ID_WORLD_39		= 199,
	ZG_KEY_ID_WORLD_40		= 200,
	ZG_KEY_ID_WORLD_41		= 201,
	ZG_KEY_ID_WORLD_42		= 202,
	ZG_KEY_ID_WORLD_43		= 203,
	ZG_KEY_ID_WORLD_44		= 204,
	ZG_KEY_ID_WORLD_45		= 205,
	ZG_KEY_ID_WORLD_46		= 206,
	ZG_KEY_ID_WORLD_47		= 207,
	ZG_KEY_ID_WORLD_48		= 208,
	ZG_KEY_ID_WORLD_49		= 209,
	ZG_KEY_ID_WORLD_50		= 210,
	ZG_KEY_ID_WORLD_51		= 211,
	ZG_KEY_ID_WORLD_52		= 212,
	ZG_KEY_ID_WORLD_53		= 213,
	ZG_KEY_ID_WORLD_54		= 214,
	ZG_KEY_ID_WORLD_55		= 215,
	ZG_KEY_ID_WORLD_56		= 216,
	ZG_KEY_ID_WORLD_57		= 217,
	ZG_KEY_ID_WORLD_58		= 218,
	ZG_KEY_ID_WORLD_59		= 219,
	ZG_KEY_ID_WORLD_60		= 220,
	ZG_KEY_ID_WORLD_61		= 221,
	ZG_KEY_ID_WORLD_62		= 222,
	ZG_KEY_ID_WORLD_63		= 223,
	ZG_KEY_ID_WORLD_64		= 224,
	ZG_KEY_ID_WORLD_65		= 225,
	ZG_KEY_ID_WORLD_66		= 226,
	ZG_KEY_ID_WORLD_67		= 227,
	ZG_KEY_ID_WORLD_68		= 228,
	ZG_KEY_ID_WORLD_69		= 229,
	ZG_KEY_ID_WORLD_70		= 230,
	ZG_KEY_ID_WORLD_71		= 231,
	ZG_KEY_ID_WORLD_72		= 232,
	ZG_KEY_ID_WORLD_73		= 233,
	ZG_KEY_ID_WORLD_74		= 234,
	ZG_KEY_ID_WORLD_75		= 235,
	ZG_KEY_ID_WORLD_76		= 236,
	ZG_KEY_ID_WORLD_77		= 237,
	ZG_KEY_ID_WORLD_78		= 238,
	ZG_KEY_ID_WORLD_79		= 239,
	ZG_KEY_ID_WORLD_80		= 240,
	ZG_KEY_ID_WORLD_81		= 241,
	ZG_KEY_ID_WORLD_82		= 242,
	ZG_KEY_ID_WORLD_83		= 243,
	ZG_KEY_ID_WORLD_84		= 244,
	ZG_KEY_ID_WORLD_85		= 245,
	ZG_KEY_ID_WORLD_86		= 246,
	ZG_KEY_ID_WORLD_87		= 247,
	ZG_KEY_ID_WORLD_88		= 248,
	ZG_KEY_ID_WORLD_89		= 249,
	ZG_KEY_ID_WORLD_90		= 250,
	ZG_KEY_ID_WORLD_91		= 251,
	ZG_KEY_ID_WORLD_92		= 252,
	ZG_KEY_ID_WORLD_93		= 253,
	ZG_KEY_ID_WORLD_94		= 254,
	ZG_KEY_ID_WORLD_95		= 255,		/* 0xFF */
	/*@}*/

	/** @name Numeric keypad */
	/*@{*/
	ZG_KEY_ID_KP0		= 256,
	ZG_KEY_ID_KP1		= 257,
	ZG_KEY_ID_KP2		= 258,
	ZG_KEY_ID_KP3		= 259,
	ZG_KEY_ID_KP4		= 260,
	ZG_KEY_ID_KP5		= 261,
	ZG_KEY_ID_KP6		= 262,
	ZG_KEY_ID_KP7		= 263,
	ZG_KEY_ID_KP8		= 264,
	ZG_KEY_ID_KP9		= 265,
	ZG_KEY_ID_KP_PERIOD		= 266,
	ZG_KEY_ID_KP_DIVIDE		= 267,
	ZG_KEY_ID_KP_MULTIPLY	= 268,
	ZG_KEY_ID_KP_MINUS		= 269,
	ZG_KEY_ID_KP_PLUS		= 270,
	ZG_KEY_ID_KP_ENTER		= 271,
	ZG_KEY_ID_KP_EQUALS		= 272,
	/*@}*/

	/** @name Arrows + Home/End pad */
	/*@{*/
	ZG_KEY_ID_UP			= 273,
	ZG_KEY_ID_DOWN		= 274,
	ZG_KEY_ID_RIGHT		= 275,
	ZG_KEY_ID_LEFT		= 276,
	ZG_KEY_ID_INSERT		= 277,
	ZG_KEY_ID_HOME		= 278,
	ZG_KEY_ID_END		= 279,
	ZG_KEY_ID_PAGEUP		= 280,
	ZG_KEY_ID_PAGEDOWN		= 281,
	/*@}*/

	/** @name Function keys */
	/*@{*/
	ZG_KEY_ID_F1			= 282,
	ZG_KEY_ID_F2			= 283,
	ZG_KEY_ID_F3			= 284,
	ZG_KEY_ID_F4			= 285,
	ZG_KEY_ID_F5			= 286,
	ZG_KEY_ID_F6			= 287,
	ZG_KEY_ID_F7			= 288,
	ZG_KEY_ID_F8			= 289,
	ZG_KEY_ID_F9			= 290,
	ZG_KEY_ID_F10		= 291,
	ZG_KEY_ID_F11		= 292,
	ZG_KEY_ID_F12		= 293,
	ZG_KEY_ID_F13		= 294,
	ZG_KEY_ID_F14		= 295,
	ZG_KEY_ID_F15		= 296,
	/*@}*/

	/** @name Key state modifier keys */
	/*@{*/
	ZG_KEY_ID_NUMLOCK		= 300,
	ZG_KEY_ID_CAPSLOCK		= 301,
	ZG_KEY_ID_SCROLLOCK		= 302,
	ZG_KEY_ID_RSHIFT		= 303,
	ZG_KEY_ID_LSHIFT		= 304,
	ZG_KEY_ID_RCTRL		= 305,
	ZG_KEY_ID_LCTRL		= 306,
	ZG_KEY_ID_RALT		= 307,
	ZG_KEY_ID_LALT		= 308,
	ZG_KEY_ID_RMETA		= 309,
	ZG_KEY_ID_LMETA		= 310,
	ZG_KEY_ID_LSUPER		= 311,		/**< Left "Windows" key */
	ZG_KEY_ID_RSUPER		= 312,		/**< Right "Windows" key */
	ZG_KEY_ID_MODE		= 313,		/**< "Alt Gr" key */
	ZG_KEY_ID_COMPOSE		= 314,		/**< Multi-key compose key */
	/*@}*/

	/** @name Miscellaneous function keys */
	/*@{*/
	ZG_KEY_ID_HELP		= 315,
	ZG_KEY_ID_PRINT		= 316,
	ZG_KEY_ID_SYSREQ		= 317,
	ZG_KEY_ID_BREAK		= 318,
	ZG_KEY_ID_MENU		= 319,
	ZG_KEY_ID_POWER		= 320,		/**< Power Macintosh power key */
	ZG_KEY_ID_EURO		= 321,		/**< Some european keyboards */
	ZG_KEY_ID_UNDO		= 322,		/**< Atari keyboard has Undo */
	/*@}*/

	/* Add any other keys here */

	ZG_KEY_ID_LAST
} ZG_KeyId;

//---------------------------------------------------
// KEYS...

#define  ZG_KP_0         		g_zg_key_pressed[ZG_KEY_ID_0]
#define  ZG_KP_1          		g_zg_key_pressed[ZG_KEY_ID_1]
#define  ZG_KP_2          		g_zg_key_pressed[ZG_KEY_ID_2]
#define  ZG_KP_3          		g_zg_key_pressed[ZG_KEY_ID_3]
#define  ZG_KP_4          		g_zg_key_pressed[ZG_KEY_ID_4]
#define  ZG_KP_5          		g_zg_key_pressed[ZG_KEY_ID_5]
#define  ZG_KP_6          		g_zg_key_pressed[ZG_KEY_ID_6]
#define  ZG_KP_7          		g_zg_key_pressed[ZG_KEY_ID_7]
#define  ZG_KP_8          		g_zg_key_pressed[ZG_KEY_ID_8]
#define  ZG_KP_9          		g_zg_key_pressed[ZG_KEY_ID_9]
#define  ZG_KP_ESC	      		g_zg_key_pressed[ZG_KEY_ID_ESCAPE]


#define  ZG_KP_A          		g_zg_key_pressed[ZG_KEY_ID_A]
#define  ZG_KP_B          		g_zg_key_pressed[ZG_KEY_ID_B]
#define  ZG_KP_C          		g_zg_key_pressed[ZG_KEY_ID_C]
#define  ZG_KP_D          		g_zg_key_pressed[ZG_KEY_ID_D]
#define  ZG_KP_E          		g_zg_key_pressed[ZG_KEY_ID_E]
#define  ZG_KP_F	      		g_zg_key_pressed[ZG_KEY_ID_F]
#define  ZG_KP_J	      		g_zg_key_pressed[ZG_KEY_ID_J]
#define  ZG_KP_G	      		g_zg_key_pressed[ZG_KEY_ID_G]
#define  ZG_KP_I		  		g_zg_key_pressed[ZG_KEY_ID_I]
#define  ZG_KP_H          		g_zg_key_pressed[ZG_KEY_ID_H]
#define  ZG_KP_K          		g_zg_key_pressed[ZG_KEY_ID_K]
#define  ZG_KP_L          		g_zg_key_pressed[ZG_KEY_ID_L]
#define  ZG_KP_N	      		g_zg_key_pressed[ZG_KEY_ID_N]
#define  ZG_KP_M	      		g_zg_key_pressed[ZG_KEY_ID_M]
#define  ZG_KP_P	      		g_zg_key_pressed[ZG_KEY_ID_P]
#define  ZG_KP_O	      		g_zg_key_pressed[ZG_KEY_ID_O]
#define  ZG_KP_W	      		g_zg_key_pressed[ZG_KEY_ID_W]
#define  ZG_KP_Z	      		g_zg_key_pressed[ZG_KEY_ID_Z]
#define  ZG_KP_MINUS	  		g_zg_key_pressed[ZG_KEY_ID_MINUS]
#define  ZG_KP_PERIOD	  		g_zg_key_pressed[ZG_KEY_ID_PERIOD]
#define  ZG_KP_SPACE	  		g_zg_key_pressed[ZG_KEY_ID_SPACE]
#define  ZG_KP_COMMA	  		g_zg_key_pressed[ZG_KEY_ID_COMMA]
#define  ZG_KP_PLUS				g_zg_key_pressed[ZG_KEY_ID_PLUS]

#define  ZG_KP_Q	        	g_zg_key_pressed[ZG_KEY_ID_q]
#define  ZG_KP_R	        	g_zg_key_pressed[ZG_KEY_ID_r]
#define  ZG_KP_S          		g_zg_key_pressed[ZG_KEY_ID_s]
#define  ZG_KP_U          		g_zg_key_pressed[ZG_KEY_ID_u]
#define  ZG_KP_T	        	g_zg_key_pressed[ZG_KEY_ID_t]
#define  ZG_KP_V	        	g_zg_key_pressed[ZG_KEY_ID_v]
#define  ZG_KP_Y	        	g_zg_key_pressed[ZG_KEY_ID_y]
#define  ZG_KP_X	        	g_zg_key_pressed[ZG_KEY_ID_x]

#define  ZG_KP_UP         		g_zg_key_pressed[ZG_KEY_ID_UP]
#define  ZG_KP_DOWN        		g_zg_key_pressed[ZG_KEY_ID_DOWN]
#define  ZG_KP_LEFT        		g_zg_key_pressed[ZG_KEY_ID_LEFT]
#define  ZG_KP_RIGHT       		g_zg_key_pressed[ZG_KEY_ID_RIGHT]


#define  ZG_KP_RETURN      		g_zg_key_pressed[ZG_KEY_ID_RETURN]
#define  ZG_KP_BACKSPACE   		g_zg_key_pressed[ZG_KEY_ID_BACKSPACE]
#define  ZG_KP_SPACE      		g_zg_key_pressed[ZG_KEY_ID_SPACE]
#define  ZG_KP_TAB	       		g_zg_key_pressed[ZG_KEY_ID_TAB]

#define  ZG_KP_F8	    		g_zg_key_pressed[ZG_KEY_ID_F8]
#define  ZG_KP_F1	    		g_zg_key_pressed[ZG_KEY_ID_F1]
#define  ZG_KP_F2	    		g_zg_key_pressed[ZG_KEY_ID_F2]
#define  ZG_KP_F12	    		g_zg_key_pressed[ZG_KEY_ID_F12]
#define  ZG_KP_PAUSE	    	g_zg_key_pressed[ZG_KEY_ID_PAUSE]

//---------------------------------------------------
// REPEAT KEYS...

#define  ZG_K_SPACE				g_zg_key[ZG_KEY_ID_SPACE]
#define  ZG_K_RETURN			g_zg_key[ZG_KEY_ID_RETURN]
#define  ZG_K_BACKSPACE 		g_zg_key[ZG_KEY_ID_BACKSPACE]
#define  ZG_K_A	        		g_zg_key[ZG_KEY_ID_A]
#define  ZG_K_C	        		g_zg_key[ZG_KEY_ID_C]
#define  ZG_K_D	        		g_zg_key[ZG_KEY_ID_D]
#define  ZG_K_E	        		g_zg_key[ZG_KEY_ID_E]
#define  ZG_K_F	        		g_zg_key[ZG_KEY_ID_F]
#define  ZG_K_J	       			g_zg_key[ZG_KEY_ID_J]
#define  ZG_K_Q         		g_zg_key[ZG_KEY_ID_Q]
#define  ZG_K_K	        		g_zg_key[ZG_KEY_ID_K]
#define  ZG_K_L	        		g_zg_key[ZG_KEY_ID_L]
#define  ZG_K_M	        		g_zg_key[ZG_KEY_ID_M]
#define  ZG_K_P	        		g_zg_key[ZG_KEY_ID_P]
#define  ZG_K_R	        		g_zg_key[ZG_KEY_ID_R]
#define  ZG_K_P	       			g_zg_key[ZG_KEY_ID_P]
#define  ZG_K_O	        		g_zg_key[ZG_KEY_ID_O]
#define  ZG_K_S        			g_zg_key[ZG_KEY_ID_S]
#define  ZG_K_X           		g_zg_key[ZG_KEY_ID_X]
#define  ZG_K_Z        			g_zg_key[ZG_KEY_ID_Z]
#define  ZG_K_W           		g_zg_key[ZG_KEY_ID_W]

#define  ZG_K_0	        		g_zg_key[ZG_KEY_ID_0]
#define  ZG_K_1	        		g_zg_key[ZG_KEY_ID_1]
#define  ZG_K_2	        		g_zg_key[ZG_KEY_ID_2]
#define  ZG_K_3	      			g_zg_key[ZG_KEY_ID_3]
#define  ZG_K_4	      			g_zg_key[ZG_KEY_ID_4]
#define  ZG_K_5	      			g_zg_key[ZG_KEY_ID_5]
#define  ZG_K_6	      			g_zg_key[ZG_KEY_ID_6]
#define  ZG_K_7	      			g_zg_key[ZG_KEY_ID_7]
#define  ZG_K_8	      			g_zg_key[ZG_KEY_ID_8]
#define  ZG_K_9	      			g_zg_key[ZG_KEY_ID_9]
#define  ZG_K_F8	      		g_zg_key[ZG_KEY_ID_F8]
#define  ZG_K_F1	        	g_zg_key[ZG_KEY_ID_F1]
#define  ZG_K_F2	      		g_zg_key[ZG_KEY_ID_F2]
#define  ZG_K_F12	      		g_zg_key[ZG_KEY_ID_F12]

#define  ZG_K_TAB	        	g_zg_key[ZG_KEY_ID_TAB]

#define  ZG_K_UP          		g_zg_key[ZG_KEY_ID_UP]
#define  ZG_K_LEFT        		g_zg_key[ZG_KEY_ID_LEFT]
#define  ZG_K_RIGHT       		g_zg_key[ZG_KEY_ID_RIGHT]
#define  ZG_K_DOWN        		g_zg_key[ZG_KEY_ID_DOWN]

extern bool 	       			g_zg_key_pressed[ZG_KEY_ID_LAST];
extern bool    	     			g_zg_key[ZG_KEY_ID_LAST];
