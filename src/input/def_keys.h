



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
	ZGK_UNKNOWN		= 0,
	ZGK_FIRST		= 0,
	ZGK_BACKSPACE	= 8,
	ZGK_TAB			= 9,
	ZGK_CLEAR		= 12,
	ZGK_RETURN		= 13,
	ZGK_PAUSE		= 19,
	ZGK_ESCAPE		= 27,
	ZGK_SPACE		= 32,
	ZGK_EXCLAIM		= 33,
	ZGK_QUOTEDBL		= 34,
	ZGK_HASH		= 35,
	ZGK_DOLLAR		= 36,
	ZGK_AMPERSAND		= 38,
	ZGK_QUOTE		= 39,
	ZGK_LEFTPAREN		= 40,
	ZGK_RIGHTPAREN		= 41,
	ZGK_ASTERISK		= 42,
	ZGK_PLUS		= 43,
	ZGK_COMMA		= 44,
	ZGK_MINUS		= 45,
	ZGK_PERIOD		= 46,
	ZGK_SLASH		= 47,
	ZGK_0			= 48,
	ZGK_1			= 49,
	ZGK_2			= 50,
	ZGK_3			= 51,
	ZGK_4			= 52,
	ZGK_5			= 53,
	ZGK_6			= 54,
	ZGK_7			= 55,
	ZGK_8			= 56,
	ZGK_9			= 57,
	ZGK_COLON		= 58,
	ZGK_SEMICOLON		= 59,
	ZGK_LESS		= 60,
	ZGK_EQUALS		= 61,
	ZGK_GREATER		= 62,
	ZGK_QUESTION		= 63,
	ZGK_AT			= 64,
	/*
	   Skip uppercase letters
	 */
	ZGK_LEFTBRACKET	= 91,
	ZGK_BACKSLASH		= 92,
	ZGK_RIGHTBRACKET	= 93,
	ZGK_CARET		= 94,
	ZGK_UNDERSCORE		= 95,
	ZGK_BACKQUOTE		= 96,
	ZGK_a			= 97,
	ZGK_b			= 98,
	ZGK_c			= 99,
	ZGK_d			= 100,
	ZGK_e			= 101,
	ZGK_f			= 102,
	ZGK_g			= 103,
	ZGK_h			= 104,
	ZGK_i			= 105,
	ZGK_j			= 106,
	ZGK_k			= 107,
	ZGK_l			= 108,
	ZGK_m			= 109,
	ZGK_n			= 110,
	ZGK_o			= 111,
	ZGK_p			= 112,
	ZGK_q			= 113,
	ZGK_r			= 114,
	ZGK_s			= 115,
	ZGK_t			= 116,
	ZGK_u			= 117,
	ZGK_v			= 118,
	ZGK_w			= 119,
	ZGK_x			= 120,
	ZGK_y			= 121,
	ZGK_z			= 122,
	ZGK_DELETE		= 127,
	/* End of ASCII mapped keysyms */
	/*@}*/

	/** @name International keyboard syms */
	/*@{*/
	ZGK_WORLD_0		= 160,		/* 0xA0 */
	ZGK_WORLD_1		= 161,
	ZGK_WORLD_2		= 162,
	ZGK_WORLD_3		= 163,
	ZGK_WORLD_4		= 164,
	ZGK_WORLD_5		= 165,
	ZGK_WORLD_6		= 166,
	ZGK_WORLD_7		= 167,
	ZGK_WORLD_8		= 168,
	ZGK_WORLD_9		= 169,
	ZGK_WORLD_10		= 170,
	ZGK_WORLD_11		= 171,
	ZGK_WORLD_12		= 172,
	ZGK_WORLD_13		= 173,
	ZGK_WORLD_14		= 174,
	ZGK_WORLD_15		= 175,
	ZGK_WORLD_16		= 176,
	ZGK_WORLD_17		= 177,
	ZGK_WORLD_18		= 178,
	ZGK_WORLD_19		= 179,
	ZGK_WORLD_20		= 180,
	ZGK_WORLD_21		= 181,
	ZGK_WORLD_22		= 182,
	ZGK_WORLD_23		= 183,
	ZGK_WORLD_24		= 184,
	ZGK_WORLD_25		= 185,
	ZGK_WORLD_26		= 186,
	ZGK_WORLD_27		= 187,
	ZGK_WORLD_28		= 188,
	ZGK_WORLD_29		= 189,
	ZGK_WORLD_30		= 190,
	ZGK_WORLD_31		= 191,
	ZGK_WORLD_32		= 192,
	ZGK_WORLD_33		= 193,
	ZGK_WORLD_34		= 194,
	ZGK_WORLD_35		= 195,
	ZGK_WORLD_36		= 196,
	ZGK_WORLD_37		= 197,
	ZGK_WORLD_38		= 198,
	ZGK_WORLD_39		= 199,
	ZGK_WORLD_40		= 200,
	ZGK_WORLD_41		= 201,
	ZGK_WORLD_42		= 202,
	ZGK_WORLD_43		= 203,
	ZGK_WORLD_44		= 204,
	ZGK_WORLD_45		= 205,
	ZGK_WORLD_46		= 206,
	ZGK_WORLD_47		= 207,
	ZGK_WORLD_48		= 208,
	ZGK_WORLD_49		= 209,
	ZGK_WORLD_50		= 210,
	ZGK_WORLD_51		= 211,
	ZGK_WORLD_52		= 212,
	ZGK_WORLD_53		= 213,
	ZGK_WORLD_54		= 214,
	ZGK_WORLD_55		= 215,
	ZGK_WORLD_56		= 216,
	ZGK_WORLD_57		= 217,
	ZGK_WORLD_58		= 218,
	ZGK_WORLD_59		= 219,
	ZGK_WORLD_60		= 220,
	ZGK_WORLD_61		= 221,
	ZGK_WORLD_62		= 222,
	ZGK_WORLD_63		= 223,
	ZGK_WORLD_64		= 224,
	ZGK_WORLD_65		= 225,
	ZGK_WORLD_66		= 226,
	ZGK_WORLD_67		= 227,
	ZGK_WORLD_68		= 228,
	ZGK_WORLD_69		= 229,
	ZGK_WORLD_70		= 230,
	ZGK_WORLD_71		= 231,
	ZGK_WORLD_72		= 232,
	ZGK_WORLD_73		= 233,
	ZGK_WORLD_74		= 234,
	ZGK_WORLD_75		= 235,
	ZGK_WORLD_76		= 236,
	ZGK_WORLD_77		= 237,
	ZGK_WORLD_78		= 238,
	ZGK_WORLD_79		= 239,
	ZGK_WORLD_80		= 240,
	ZGK_WORLD_81		= 241,
	ZGK_WORLD_82		= 242,
	ZGK_WORLD_83		= 243,
	ZGK_WORLD_84		= 244,
	ZGK_WORLD_85		= 245,
	ZGK_WORLD_86		= 246,
	ZGK_WORLD_87		= 247,
	ZGK_WORLD_88		= 248,
	ZGK_WORLD_89		= 249,
	ZGK_WORLD_90		= 250,
	ZGK_WORLD_91		= 251,
	ZGK_WORLD_92		= 252,
	ZGK_WORLD_93		= 253,
	ZGK_WORLD_94		= 254,
	ZGK_WORLD_95		= 255,		/* 0xFF */
	/*@}*/

	/** @name Numeric keypad */
	/*@{*/
	ZGK_KP0		= 256,
	ZGK_KP1		= 257,
	ZGK_KP2		= 258,
	ZGK_KP3		= 259,
	ZGK_KP4		= 260,
	ZGK_KP5		= 261,
	ZGK_KP6		= 262,
	ZGK_KP7		= 263,
	ZGK_KP8		= 264,
	ZGK_KP9		= 265,
	ZGK_KP_PERIOD		= 266,
	ZGK_KP_DIVIDE		= 267,
	ZGK_KP_MULTIPLY	= 268,
	ZGK_KP_MINUS		= 269,
	ZGK_KP_PLUS		= 270,
	ZGK_KP_ENTER		= 271,
	ZGK_KP_EQUALS		= 272,
	/*@}*/

	/** @name Arrows + Home/End pad */
	/*@{*/
	ZGK_UP			= 273,
	ZGK_DOWN		= 274,
	ZGK_RIGHT		= 275,
	ZGK_LEFT		= 276,
	ZGK_INSERT		= 277,
	ZGK_HOME		= 278,
	ZGK_END		= 279,
	ZGK_PAGEUP		= 280,
	ZGK_PAGEDOWN		= 281,
	/*@}*/

	/** @name Function keys */
	/*@{*/
	ZGK_F1			= 282,
	ZGK_F2			= 283,
	ZGK_F3			= 284,
	ZGK_F4			= 285,
	ZGK_F5			= 286,
	ZGK_F6			= 287,
	ZGK_F7			= 288,
	ZGK_F8			= 289,
	ZGK_F9			= 290,
	ZGK_F10		= 291,
	ZGK_F11		= 292,
	ZGK_F12		= 293,
	ZGK_F13		= 294,
	ZGK_F14		= 295,
	ZGK_F15		= 296,
	/*@}*/

	/** @name Key state modifier keys */
	/*@{*/
	ZGK_NUMLOCK		= 300,
	ZGK_CAPSLOCK		= 301,
	ZGK_SCROLLOCK		= 302,
	ZGK_RSHIFT		= 303,
	ZGK_LSHIFT		= 304,
	ZGK_RCTRL		= 305,
	ZGK_LCTRL		= 306,
	ZGK_RALT		= 307,
	ZGK_LALT		= 308,
	ZGK_RMETA		= 309,
	ZGK_LMETA		= 310,
	ZGK_LSUPER		= 311,		/**< Left "Windows" key */
	ZGK_RSUPER		= 312,		/**< Right "Windows" key */
	ZGK_MODE		= 313,		/**< "Alt Gr" key */
	ZGK_COMPOSE		= 314,		/**< Multi-key compose key */
	/*@}*/

	/** @name Miscellaneous function keys */
	/*@{*/
	ZGK_HELP		= 315,
	ZGK_PRINT		= 316,
	ZGK_SYSREQ		= 317,
	ZGK_BREAK		= 318,
	ZGK_MENU		= 319,
	ZGK_POWER		= 320,		/**< Power Macintosh power key */
	ZGK_EURO		= 321,		/**< Some european keyboards */
	ZGK_UNDO		= 322,		/**< Atari keyboard has Undo */
	/*@}*/

	/* Add any other keys here */

	ZGK_LAST
} ZG_Key;

//---------------------------------------------------
// KEYS...

#define  K_0          	zg_key[ZGK_0]
#define  K_1          	zg_key[ZGK_1]
#define  K_2          	zg_key[ZGK_2]
#define  K_3          	zg_key[ZGK_3]
#define  K_4          	zg_key[ZGK_4]
#define  K_5          	zg_key[ZGK_5]
#define  K_6          	zg_key[ZGK_6]
#define  K_7          	zg_key[ZGK_7]
#define  K_8          	zg_key[ZGK_8]
#define  K_9          	zg_key[ZGK_9]
#define  K_ESC	      	zg_key[ZGK_ESCAPE]


#define  K_A          	zg_key[ZGK_a]
#define  K_B          	zg_key[ZGK_b]
#define  K_C          	zg_key[ZGK_c]
#define  K_D          	zg_key[ZGK_d]
#define  K_E          	zg_key[ZGK_e]
#define  K_F	      	zg_key[ZGK_f]
#define  K_J	      	zg_key[ZGK_j]
#define  K_G	      	zg_key[ZGK_g]
#define  K_I		  	zg_key[ZGK_i]
#define  K_H          	zg_key[ZGK_h]
#define  K_K          	zg_key[ZGK_k]
#define  K_L          	zg_key[ZGK_l]
#define  K_N	      	zg_key[ZGK_n]
#define  K_M	      	zg_key[ZGK_m]
#define  K_P	      	zg_key[ZGK_p]
#define  K_O	      	zg_key[ZGK_o]
#define  K_W	      	zg_key[ZGK_w]
#define  K_Z	      	zg_key[ZGK_z]
#define  K_MINUS	  	zg_key[ZGK_MINUS]
#define  K_PERIOD	  	zg_key[ZGK_PERIOD]
#define  K_SPACE	  	zg_key[ZGK_SPACE]
#define  K_COMMA	  	zg_key[ZGK_COMMA]
#define  K_PLUS			zg_key[ZGK_PLUS]

#define  K_Q	        zg_key[ZGK_q]
#define  K_R	        zg_key[ZGK_r]
#define  K_S          	zg_key[ZGK_s]
#define  K_U          	zg_key[ZGK_u]
#define  K_T	        zg_key[ZGK_t]
#define  K_V	        zg_key[ZGK_v]
#define  K_Y	        zg_key[ZGK_y]
#define  K_X	        zg_key[ZGK_x]

#define  K_UP         	zg_key[ZGK_UP]
#define  K_DOWN        	zg_key[ZGK_DOWN]
#define  K_LEFT        	zg_key[ZGK_LEFT]
#define  K_RIGHT       	zg_key[ZGK_RIGHT]


#define  K_RETURN      	zg_key[ZGK_RETURN]
#define  K_BACKSPACE   	zg_key[ZGK_BACKSPACE]
#define  K_SPACE      	zg_key[ZGK_SPACE]
#define  K_TAB	       	zg_key[ZGK_TAB]

#define  K_F8	    	zg_key[ZGK_F8]
#define  K_F1	    	zg_key[ZGK_F1]
#define  K_F2	    	zg_key[ZGK_F2]
#define  K_F12	    	zg_key[ZGK_F12]
#define  K_PAUSE	    zg_key[ZGK_PAUSE]

//---------------------------------------------------
// REPEAT KEYS...

#define  KR_SPACE		zg_keyR[ZGK_SPACE]
#define  KR_RETURN		zg_keyR[ZGK_RETURN]
#define  KR_BACKSPACE   zg_keyR[ZGK_BACKSPACE]
#define  KR_A	        zg_keyR[ZGK_a]
#define  KR_C	        zg_keyR[ZGK_c]
#define  KR_D	        zg_keyR[ZGK_d]
#define  KR_E	        zg_keyR[ZGK_e]
#define  KR_F	        zg_keyR[ZGK_f]
#define  KR_J	        zg_keyR[ZGK_j]
#define  KR_Q           zg_keyR[ZGK_q]
#define  KR_K	        zg_keyR[ZGK_k]
#define  KR_L	        zg_keyR[ZGK_l]
#define  KR_M	        zg_keyR[ZGK_m]
#define  KR_P	        zg_keyR[ZGK_p]
#define  KR_R	        zg_keyR[ZGK_r]
#define  KR_P	        zg_keyR[ZGK_p]
#define  KR_O	        zg_keyR[ZGK_o]
#define  KR_S        	zg_keyR[ZGK_s]
#define  KR_X           zg_keyR[ZGK_x]
#define  KR_Z        	zg_keyR[ZGK_z]
#define  KR_W           zg_keyR[ZGK_w]

#define  KR_0	        zg_keyR[ZGK_0]
#define  KR_1	        zg_keyR[ZGK_1]
#define  KR_2	        zg_keyR[ZGK_2]
#define  KR_3	      	zg_keyR[ZGK_3]
#define  KR_4	      	zg_keyR[ZGK_4]
#define  KR_5	      	zg_keyR[ZGK_5]
#define  KR_6	      	zg_keyR[ZGK_6]
#define  KR_7	      	zg_keyR[ZGK_7]
#define  KR_8	      	zg_keyR[ZGK_8]
#define  KR_9	      	zg_keyR[ZGK_9]
#define  KR_F8	      	zg_keyR[ZGK_F8]
#define  KR_F1	        zg_keyR[ZGK_F1]
#define  KR_F2	      	zg_keyR[ZGK_F2]
#define  KR_F12	      	zg_keyR[ZGK_F12]

#define  KR_J	        zg_keyR[ZGK_j]
#define  KR_TAB	        zg_keyR[ZGK_TAB]

#define  KR_UP          zg_keyR[ZGK_UP]
#define  KR_LEFT        zg_keyR[ZGK_LEFT]
#define  KR_RIGHT       zg_keyR[ZGK_RIGHT]
#define  KR_DOWN        zg_keyR[ZGK_DOWN]

extern bool 	       	zg_key[ZGK_LAST];
extern bool    	     	zg_keyR[ZGK_LAST];
