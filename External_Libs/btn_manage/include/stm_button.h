/*
 * stm_button.h
 *
 *  Created on: Apr 18, 2023
 *      Author: kyph(phamhuuky20@gmail.com)
 */

#ifndef _STM_BUTTON_H_
#define _STM_BUTTON_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "stdbool.h"

/***
 * BUTTON MANAGER CONFIG
 */

#define NUM_BUTTONS				3
#define DEFAULT_LONG_PRESS_MS	2000
#define DEFAULT_PRESS_MS		150
#define DEFAULT_MULTI_PRESS		5

#define BUTTON_FAIL			-1
#define BUTTON_OK			0

typedef int button_err_t;

typedef enum {
	BUTTON_EVENT_UNKNOWN = 0,
	BUTTON_EVENT_PRESS = (1 << 1),
	BUTTON_EVENT_DOUBLE_PRESS = (1 << 2),
	BUTTON_EVENT_LONG_PRESS = (1 << 3),
	BUTTON_EVENT_MULTI_PRESS = (1 << 4),
	BUTTON_EVENT_MAX
} btn_ev_t;

typedef enum {
	BUTTON_STATE_UNKNOWN = 0,
	BUTTON_STATE_RISING,
	BUTTON_STATE_FAILLING,
	BUTTON_STATE_LONG_PRESS,
	// IDLE
	BUTTON_STATE_RISING_IDLE,
	BUTTON_STATE_FAILLING_IDLE,
	BUTTON_EVENT_MULTI_PRESS_IDLE,
	BUTTON_STATE_LONG_PRESS_IDLE,
} button_state_t;

typedef void (*btn_cb_t)(void *user, int pin, btn_ev_t event);

typedef struct {
	uint8_t num_btns;
	uint8_t multi_press;
	uint32_t long_press_timeout;
	uint32_t press_timeout;
} btn_manage_cfg_t;

button_err_t btn_manage_process(void);
button_err_t btn_manage_init(btn_manage_cfg_t *cfg);
button_err_t btn_manage_add_new_button(int pin, int event_mask);
button_err_t btn_manage_update_state(int pin, button_state_t new_state);
button_err_t btn_register_event_callback(btn_cb_t event_cb, void *user);

#endif /* STM_BUTTON_INCLUDE_STM_BUTTON_H_ */
