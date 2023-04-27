/*
 * stm_button.c
 *
 *  Created on: Apr 18, 2023
 *      Author: kyph(phamhuuky20@gmail.com)
 */

/***
 * btn_manage.c
 */

#include "../../External_Libs/btn_manage/include/stm_button.h"
#include "../../External_Libs/stm_log/include/stm_log.h"

static const char *TAG = "BUTTON MANAGE";

typedef struct button_ {
	button_state_t state;
	int pin;
	int cnt_press;
	uint32_t press_tick;
	uint32_t long_press_tick;
	int event_registed;
} button_t;

typedef struct btn_manage_ {
	bool run;
	int num_btns;
	button_t list_btn[NUM_BUTTONS];
	btn_cb_t event_cb;
	uint8_t multi_press;
	uint32_t long_press_timeout;
	uint32_t press_timeout;
	void *user;
} btn_manage_t;

static btn_manage_t g_btn_manage = {
	.run = 0,
	.num_btns = 0,
	.long_press_timeout = DEFAULT_LONG_PRESS_MS,
	.press_timeout = DEFAULT_PRESS_MS,
	.multi_press = DEFAULT_MULTI_PRESS,
};

static inline uint32_t _btn_get_tick() {
	return HAL_GetTick();
}

static inline bool _btn_tick_timeout(uint32_t tick_check, uint32_t time_out) {
	if (_btn_get_tick() - tick_check > time_out) {
		return true;
	}
	return false;
}

static button_err_t _btn_send_event(btn_manage_t *btn_handle, button_t *btn,
		btn_ev_t event_id) {
	if (!btn_handle->event_cb) {
		return BUTTON_FAIL;
	}
	if (btn->event_registed & event_id) {
		btn_handle->event_cb(btn_handle->user, btn->pin, event_id);
	}
	return BUTTON_OK;
}

static inline button_t* _find_btn_by_pin(btn_manage_t *btn_handle, int pin) {
	for (int i = 0; i < btn_handle->num_btns; i++) {
		if (btn_handle->list_btn[i].pin == pin) {
			return &btn_handle->list_btn[i];
		}
	}
	STM_LOGE(TAG, "Button [%d] not found", pin);
	return NULL;
}

static button_err_t _btn_update_state(btn_manage_t *btn_handle, button_t *btn,
		button_state_t new_state) {
	if (!btn) {
		return BUTTON_FAIL;
	}
	if ((new_state == BUTTON_STATE_RISING)
			&& ((btn->state == BUTTON_STATE_FAILLING_IDLE)
					|| (btn->state == BUTTON_EVENT_MULTI_PRESS_IDLE))) {
		btn->long_press_tick = _btn_get_tick();
		btn->press_tick = _btn_get_tick();
		btn->state = BUTTON_STATE_RISING_IDLE;
		if (btn->cnt_press == 0) {
//			STM_LOGI(TAG, "BUTTON_EVENT_PRESS");
			_btn_send_event(btn_handle, btn, BUTTON_EVENT_PRESS);
		}
	} else if ((new_state == BUTTON_STATE_FAILLING)
			&& ((btn->state == BUTTON_STATE_RISING_IDLE)
					|| (btn->state == BUTTON_STATE_LONG_PRESS_IDLE))) {
		if (!_btn_tick_timeout(btn->press_tick, btn_handle->press_timeout)) {
			btn->cnt_press++;
			btn->press_tick = _btn_get_tick();
		}
		btn->state = BUTTON_STATE_FAILLING_IDLE;
	}

	return BUTTON_OK;
}

button_err_t btn_manage_update_state(int pin, button_state_t new_state) {
	if (!g_btn_manage.run) {
		STM_LOGW(TAG, "BTManage_Init need run before");
		return BUTTON_FAIL;
	}
	button_t *btn = _find_btn_by_pin(&g_btn_manage, pin);
	_btn_update_state(&g_btn_manage, btn, new_state);
	return BUTTON_OK;
}

static button_err_t _btn_manage_check_state(btn_manage_t *btn_handle,
		button_t *btn) {
	if (btn->state == BUTTON_STATE_RISING_IDLE) {
		if (_btn_tick_timeout(btn->long_press_tick,
				btn_handle->long_press_timeout) && btn->cnt_press == 0) {
			// STM_LOGI(TAG,"BUTTON_EVENT_LONG_PRESS");
			_btn_send_event(btn_handle, btn, BUTTON_EVENT_LONG_PRESS);
			btn->cnt_press = 0;
			btn->state = BUTTON_STATE_LONG_PRESS_IDLE;
		}
		if (_btn_tick_timeout(btn->press_tick, btn_handle->press_timeout)) {
			btn->cnt_press = 0;
		}
		return BUTTON_OK;
	} else if (btn->state == BUTTON_STATE_FAILLING_IDLE) {
		if (btn->cnt_press == btn_handle->multi_press) {
			_btn_send_event(btn_handle, btn, BUTTON_EVENT_MULTI_PRESS);
			btn->state = BUTTON_EVENT_MULTI_PRESS_IDLE;
		}
		if (!_btn_tick_timeout(btn->press_tick, btn_handle->press_timeout)) {
			return BUTTON_OK;
		}
		if (btn->cnt_press == 2) {
			_btn_send_event(btn_handle, btn, BUTTON_EVENT_DOUBLE_PRESS);
		}
		btn->cnt_press = 0;
		return BUTTON_OK;
	} else if (btn->state == BUTTON_EVENT_MULTI_PRESS_IDLE) {
		if (_btn_tick_timeout(btn->press_tick, btn_handle->press_timeout)) {
			btn->cnt_press = 0;
		}
		return BUTTON_OK;
	} else if (btn->state == BUTTON_STATE_LONG_PRESS_IDLE) {
		return BUTTON_OK;
	}

	return BUTTON_FAIL;
}

button_err_t btn_manage_process(void) {
	if (!g_btn_manage.run) {
		STM_LOGW(TAG, "BTManage_Init need run before");
		return BUTTON_FAIL;
	}
	button_t *btn = NULL;
	for (int i = 0; i < g_btn_manage.num_btns; i++) {
		btn = &g_btn_manage.list_btn[i];
		if ((!btn) || (btn->state < BUTTON_STATE_RISING_IDLE)) {
			continue;
		}
		_btn_manage_check_state(&g_btn_manage, btn);
	}
	return BUTTON_OK;
}

button_err_t btn_register_event_callback(btn_cb_t event_cb, void *user) {
	if (!g_btn_manage.run) {
		STM_LOGW(TAG, "BTManage_Init need run before");
		return BUTTON_FAIL;
	}
	g_btn_manage.event_cb = event_cb;
	g_btn_manage.user = user;
	return BUTTON_OK;
}

button_err_t btn_manage_add_new_button(int pin, int event_mask) {
	if (!g_btn_manage.run) {
		STM_LOGW(TAG, "BTManage_Init need run before");
		return BUTTON_FAIL;
	}
	button_t *button = NULL;
	for (int i = 0; i < g_btn_manage.num_btns; i++) {
		button = &g_btn_manage.list_btn[i];
		if (pin == button->pin) {
			STM_LOGW(TAG, "Button Exist");
			return BUTTON_FAIL;
		}
		if (button->pin == -1) {
			button->event_registed = event_mask;
			button->cnt_press = 0;
			button->pin = pin;
			button->state = BUTTON_STATE_FAILLING_IDLE;
			STM_LOGW(TAG, "Register Device : '%d' to index [%d]", pin, i);
			return BUTTON_OK;
		}
	}
	STM_LOGE(TAG, "Register Device Failed : Not Enough Space");
	return BUTTON_FAIL;
}

button_err_t btn_manage_init(btn_manage_cfg_t *cfg) {
	if (cfg->num_btns > NUM_BUTTONS) {
		STM_LOGE(TAG, "cfg->num_btns > NUM_BUTTONS");
		return BUTTON_FAIL;
	}
	g_btn_manage.num_btns = cfg->num_btns;

	button_t *button = NULL;
	for (int i = 0; i < g_btn_manage.num_btns; i++) {
		button = &g_btn_manage.list_btn[i];
		button->pin = -1;
	}

	g_btn_manage.user = NULL;
	g_btn_manage.multi_press =
			(cfg->multi_press > 0) ? cfg->multi_press : DEFAULT_MULTI_PRESS;
	g_btn_manage.long_press_timeout = cfg->long_press_timeout;
	g_btn_manage.press_timeout = cfg->press_timeout;
	g_btn_manage.run = true;
	return BUTTON_OK;
}

