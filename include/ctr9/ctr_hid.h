/*******************************************************************************
 * Copyright (C) 2016 Gabriel Marcano
 *
 * Refer to the COPYING.txt file at the top of the project directory. If that is
 * missing, this file is licensed under the GPL version 2.0 or later.
 *
 ******************************************************************************/

/** @file */

#ifndef CTR_HID_H_
#define CTR_HID_H_

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef uint32_t ctr_hid_button_type;

#define CTR_HID_REG     ((volatile ctr_hid_button_type*)0x10146000)

#define CTR_HID_NON     0x000u
#define CTR_HID_A       0x001u
#define CTR_HID_B       0x002u
#define CTR_HID_SELECT  0x004u
#define CTR_HID_START   0x008u
#define CTR_HID_RIGHT   0x010u
#define CTR_HID_LEFT    0x020u
#define CTR_HID_UP      0x040u
#define CTR_HID_DOWN    0x080u
#define CTR_HID_RT      0x100u
#define CTR_HID_LT      0x200u
#define CTR_HID_X       0x400u
#define CTR_HID_Y       0x800u

bool ctr_hid_button_status(ctr_hid_button_type buttons);
ctr_hid_button_type ctr_hid_get_buttons(void);

#ifdef __cplusplus
}
#endif

#endif//CTR_HID_H_

