/*
 * stm_button.h
 *
 *  Created on: Apr 17, 2023
 *      Author: kyph(phamhuuky20@gmail.com)
 */

#ifndef STM_BUTTON_INCLUDE_STM_BUTTON_H_
#define STM_BUTTON_INCLUDE_STM_BUTTON_H_

typedef struct {
	int		gpio_port;
	int		gpio_pin;
	int		press_level;
} button_info_t;

typedef struct {
	button_info_t* 	button_info;
	int				nums_btn;
	int				long_press_time;
	int				press_time;
} button_cfg_t;

#endif /* STM_BUTTON_INCLUDE_STM_BUTTON_H_ */
