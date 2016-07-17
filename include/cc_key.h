/*
 * This program is free software: you can redistribute it and/or modify 
 * it under the terms of the 3-clause BSD license.
 *
 * You should have received a copy of the 3-clause BSD License along with
 * this program. If not, see <http:opensource.org/licenses/>.
 */

#ifndef __CC_KEY_H__
#define __CC_KEY_H__

#include "cc_system.h"

#ifdef CC_OS_LINUX
#include <X11/keysym.h>
#define KEYDEF(x11,win) x11
#elif defined CC_OS_WIN32
#include <windows.h>
#define KEYDEF(x11,win) win
#endif

#define CC_KEY_0              KEYDEF(XK_0, '0')
#define CC_KEY_1              KEYDEF(XK_1, '1')
#define CC_KEY_2              KEYDEF(XK_2, '2')
#define CC_KEY_3              KEYDEF(XK_3, '3')
#define CC_KEY_4              KEYDEF(XK_4, '4')
#define CC_KEY_5              KEYDEF(XK_5, '5')
#define CC_KEY_6              KEYDEF(XK_6, '6')
#define CC_KEY_7              KEYDEF(XK_7, '7')
#define CC_KEY_8              KEYDEF(XK_8, '8')
#define CC_KEY_9              KEYDEF(XK_9, '9')

#define CC_KEY_A              KEYDEF(XK_a, 'A')
#define CC_KEY_B              KEYDEF(XK_b, 'B')
#define CC_KEY_C              KEYDEF(XK_c, 'C')
#define CC_KEY_D              KEYDEF(XK_d, 'D')
#define CC_KEY_E              KEYDEF(XK_e, 'E')
#define CC_KEY_F              KEYDEF(XK_f, 'F')
#define CC_KEY_G              KEYDEF(XK_g, 'G')
#define CC_KEY_H              KEYDEF(XK_h, 'H')
#define CC_KEY_I              KEYDEF(XK_i, 'I')
#define CC_KEY_J              KEYDEF(XK_j, 'J')
#define CC_KEY_K              KEYDEF(XK_k, 'K')
#define CC_KEY_L              KEYDEF(XK_l, 'L')
#define CC_KEY_M              KEYDEF(XK_m, 'M')
#define CC_KEY_N              KEYDEF(XK_n, 'N')
#define CC_KEY_O              KEYDEF(XK_o, 'O')
#define CC_KEY_P              KEYDEF(XK_p, 'P')
#define CC_KEY_Q              KEYDEF(XK_q, 'Q')
#define CC_KEY_R              KEYDEF(XK_r, 'R')
#define CC_KEY_S              KEYDEF(XK_s, 'S')
#define CC_KEY_T              KEYDEF(XK_t, 'T')
#define CC_KEY_U              KEYDEF(XK_u, 'U')
#define CC_KEY_V              KEYDEF(XK_v, 'V')
#define CC_KEY_W              KEYDEF(XK_w, 'W')
#define CC_KEY_X              KEYDEF(XK_x, 'X')
#define CC_KEY_Y              KEYDEF(XK_y, 'Y')
#define CC_KEY_Z              KEYDEF(XK_z, 'Z')

#define CC_KEY_F1             KEYDEF(XK_F1, VK_F1)
#define CC_KEY_F2             KEYDEF(XK_F2, VK_F2)
#define CC_KEY_F3             KEYDEF(XK_F3, VK_F3)
#define CC_KEY_F4             KEYDEF(XK_F4, VK_F4)
#define CC_KEY_F5             KEYDEF(XK_F5, VK_F5)
#define CC_KEY_F6             KEYDEF(XK_F6, VK_F6)
#define CC_KEY_F7             KEYDEF(XK_F7, VK_F7)
#define CC_KEY_F8             KEYDEF(XK_F8, VK_F8)
#define CC_KEY_F9             KEYDEF(XK_F9, VK_F9)
#define CC_KEY_F10            KEYDEF(XK_F10, VK_F10)
#define CC_KEY_F11            KEYDEF(XK_F11, VK_F11)
#define CC_KEY_F12            KEYDEF(XK_F12, VK_F12)

#define CC_KEY_NUM0           KEYDEF(XK_KP_0, VK_NUMPAD0)
#define CC_KEY_NUM1           KEYDEF(XK_KP_1, VK_NUMPAD1)
#define CC_KEY_NUM2           KEYDEF(XK_KP_2, VK_NUMPAD2)
#define CC_KEY_NUM3           KEYDEF(XK_KP_3, VK_NUMPAD3)
#define CC_KEY_NUM4           KEYDEF(XK_KP_4, VK_NUMPAD4)
#define CC_KEY_NUM5           KEYDEF(XK_KP_5, VK_NUMPAD5)
#define CC_KEY_NUM6           KEYDEF(XK_KP_6, VK_NUMPAD6)
#define CC_KEY_NUM7           KEYDEF(XK_KP_7, VK_NUMPAD7)
#define CC_KEY_NUM8           KEYDEF(XK_KP_8, VK_NUMPAD8)
#define CC_KEY_NUM9           KEYDEF(XK_KP_9, VK_NUMPAD9)

#define CC_KEY_BACKSPACE      KEYDEF(XK_BackSpace, VK_BACK)
#define CC_KEY_TAB            KEYDEF(XK_Tab, VK_TAB)
#define CC_KEY_RETURN         KEYDEF(XK_Return, VK_RETURN)
#define CC_KEY_ESCAPE         KEYDEF(XK_Escape, VK_ESCAPE)
#define CC_KEY_SPACE          KEYDEF(XK_space, VK_SPACE)
#define CC_KEY_CAPSLOCK       KEYDEF(XK_Caps_Lock, VK_CAPITAL)
#define CC_KEY_INSERT         KEYDEF(XK_Insert, VK_INSERT)
#define CC_KEY_DELETE         KEYDEF(XK_Delete, VK_DELETE)
#define CC_KEY_HOME           KEYDEF(XK_Home, VK_HOME)
#define CC_KEY_END            KEYDEF(XK_End, VK_END)
#define CC_KEY_PAGEUP         KEYDEF(XK_Page_Up, VK_PRIOR)
#define CC_KEY_PAGEDOWN       KEYDEF(XK_Page_Down, VK_NEXT)
#define CC_KEY_PRINTSCREEN    KEYDEF(XK_Print, VK_SNAPSHOT)
#define CC_KEY_SCROLLLOCK     KEYDEF(XK_Scroll_Lock, VK_SCROLL)
#define CC_KEY_NUMLOCK        KEYDEF(XK_Num_Lock, VK_NUMLOCK)
#define CC_KEY_PAUSEBREAK     KEYDEF(XK_Pause, VK_PAUSE)

#define CC_KEY_LSHIFT         KEYDEF(XK_Shift_L, VK_LSHIFT)
#define CC_KEY_RSHIFT         KEYDEF(XK_Shift_R, VK_RSHIFT)
#define CC_KEY_LCONTROL       KEYDEF(XK_Control_L, VK_LCONTROL)
#define CC_KEY_RCONTROL       KEYDEF(XK_Control_R, VK_RCONTROL)

#define CC_KEY_LEFT           KEYDEF(XK_Left, VK_LEFT)
#define CC_KEY_UP             KEYDEF(XK_Up, VK_UP)
#define CC_KEY_RIGHT          KEYDEF(XK_Right, VK_RIGHT)
#define CC_KEY_DOWN           KEYDEF(XK_Down, VK_DOWN)

#define CC_KEY_SELECT         KEYDEF(XK_Select, VK_SELECT)
#define CC_KEY_PRINT          KEYDEF(XK_Print, VK_PRINT)
#define CC_KEY_EXECUTE        KEYDEF(XK_Execute, VK_EXECUTE)
#define CC_KEY_INSERT         KEYDEF(XK_Insert, VK_INSERT)
#define CC_KEY_MENU           KEYDEF(XK_Menu, VK_MENU)
#define CC_KEY_FIND           KEYDEF(XK_Find, VK_BROWSER_SEARCH)
#define CC_KEY_CANCEL         KEYDEF(XK_Cancel, VK_CANCEL)
#define CC_KEY_HELP           KEYDEF(XK_Help, VK_HELP)
#define CC_KEY_BREAK          KEYDEF(XK_Break, VK_PAUSE)
#define CC_KEY_MODE_SWITCH    KEYDEF(XK_Mode_switch, VK_MODECHANGE)
#define CC_KEY_NUM_LOCK       KEYDEF(XK_Num_Lock, VK_NUMLOCK)

#define CC_KEY_EXCLAM         KEYDEF(XK_exclam, '!')
#define CC_KEY_QUOTEDBL       KEYDEF(XK_quotedbl, VK_OEM_7)
#define CC_KEY_DOLLAR         KEYDEF(XK_dollar, '$')
#define CC_KEY_PERCENT        KEYDEF(XK_percent, '%')
#define CC_KEY_AMPERSAND      KEYDEF(XK_ampersand, '&')
#define CC_KEY_APOSTROPHE     KEYDEF(XK_apostrophe, '\'')
#define CC_KEY_QUOTERIGHT     KEYDEF(XK_quoteright, VK_OEM_7)
#define CC_KEY_PARENLEFT      KEYDEF(XK_parenleft, '(')
#define CC_KEY_PARENRIGHT     KEYDEF(XK_parenright, ')')
#define CC_KEY_ASTERISK       KEYDEF(XK_asterisk, '*')
#define CC_KEY_PLUS           KEYDEF(XK_plus, VK_OEM_PLUS)
#define CC_KEY_COMMA          KEYDEF(XK_comma, VK_OEM_COMMA)
#define CC_KEY_MINUS          KEYDEF(XK_minus, VK_OEM_MINUS)
#define CC_KEY_PERIOD         KEYDEF(XK_period, VK_OEM_PERIOD)
#define CC_KEY_SLASH          KEYDEF(XK_slash, VK_OEM_5)

#define CC_KEY_COLON          KEYDEF(XK_colon, VK_OEM_1)
#define CC_KEY_SEMICOLON      KEYDEF(XK_semicolon, VK_OEM_1)
#define CC_KEY_LESS           KEYDEF(XK_less, '<')
#define CC_KEY_EQUAL          KEYDEF(XK_equal, '=')
#define CC_KEY_GREATER        KEYDEF(XK_greater, '>')
#define CC_KEY_QUESTION       KEYDEF(XK_question, VK_OEM_2)
#define CC_KEY_AT             KEYDEF(XK_at, '@')

#define CC_KEY_BRACKETLEFT    KEYDEF(XK_bracketleft, VK_OEM_4)
#define CC_KEY_BACKSLASH      KEYDEF(XK_backslash, VK_OEM_102)
#define CC_KEY_BRACKETRIGHT   KEYDEF(XK_bracketright, VK_OEM_6)
#define CC_KEY_UNDERSCORE     KEYDEF(XK_underscore, '_')
#define CC_KEY_GRAVE          KEYDEF(XK_grave, '`')
#define CC_KEY_QUOTELEFT      KEYDEF(XK_quoteleft, '"')

#define CC_KEY_BRACELEFT      KEYDEF(XK_braceleft, '[')
#define CC_KEY_BAR            KEYDEF(XK_bar, '-')
#define CC_KEY_BRACERIGHT     KEYDEF(XK_braceright, ']')
#define CC_KEY_ASCIITILDE     KEYDEF(XK_asciitilde, VK_OEM_3)

#endif /* __CC_KEY_H__ */
