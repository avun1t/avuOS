#ifndef __KEYBOARD_H_
#define __KEYBOARD_H_

#define KBD_PORT_DATA
#define KBD_PORT_STATUS 0x64
#define KBD_PORT_COMMAND 0x64

#define KBD_STATUS_OUTBUF_FULL 0x1u
#define KBD_STATUS_INBUF_FULL 0x2u
#define KBD_STATUS_SYSFLAG 0x4u
#define KBD_STATUS_CMDORDATA 0x8u
#define KBD_STATUS_WHICHBUF 0x20u
#define KBD_STATUS_TIMEOUT 0x40u
#define KBD_STATUS_PARITYERR 0x80u

#define KBD_MOD_NONE 0x0u
#define KBD_MOD_ALT 0x1u
#define KBD_MOD_CTRL 0x2u
#define KBD_MOD_SHIFT 0x4u
#define KBD_MOD_SUPER 0x8u
#define KBD_MOD_ALTGR 0x10u
#define KBD_MOD_MASK 0x1Fu

#define KBD_SCANCODE_LSHIFT 0x2au
#define KBD_SCANCODE_RSHIFT 0x36u
#define KBD_SCANCODE_ALT 0x38u
#define KBD_SCANCODE_CTRL 0x1Du
#define KBD_SCANCODE_SUPER 0x5B
#define KBD_ACK 0xFAu

#define KBD_IS_PRESSED 0x80u

extern char kbd_us_shift_map[256];
extern char kbd_us_map[256];

#endif // __KEYBOARD_H_