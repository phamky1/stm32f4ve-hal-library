/*
 * stm_log.h
 *
 *  Created on: Apr 17, 2023
 *      Author: dieup
 */

#ifndef STM_LOG_STM_LOG_H_
#define STM_LOG_STM_LOG_H_

#include <stdio.h>
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"

typedef enum {
	STM_LOG_LEVEL_NONE = 0,
	STM_LOG_LEVEL_ERROR,
	STM_LOG_LEVEL_WARNING,
	STM_LOG_LEVEL_INFO,
	STM_LOG_LEVEL_VERBOSE,
} stm_log_level_t;

extern stm_log_level_t log_level;

#define STM_LOGI(TAG, ...) do {\
		if (log_level >= STM_LOG_LEVEL_INFO) {\
			STM_LOG(STM_LOG_LEVEL_INFO, TAG, __VA_ARGS__);\
		}\
} while(0)

#define STM_LOGV(TAG, ...) do {\
		if (log_level >= STM_LOG_LEVEL_VERBOSE) {\
			STM_LOG(STM_LOG_LEVEL_VERBOSE, TAG, __VA_ARGS__);\
		}\
} while(0)

#define STM_LOGE(TAG, ...) do {\
		if (log_level >= STM_LOG_LEVEL_ERROR) {\
			STM_LOG(STM_LOG_LEVEL_ERROR, TAG, __VA_ARGS__);\
		}\
} while(0)

#define STM_LOGW(TAG, ...) do {\
		if (log_level >= STM_LOG_LEVEL_WARNING) {\
			STM_LOG(STM_LOG_LEVEL_WARNING, TAG, __VA_ARGS__);\
		}\
} while(0)

#define STM_LOG(level, TAG, ...) do { 							\
	const char* level_str; 										\
	switch(level) { 											\
	case STM_LOG_LEVEL_VERBOSE: level_str = "V"; break; 		\
	case STM_LOG_LEVEL_INFO: level_str = "I"; break; 			\
	case STM_LOG_LEVEL_WARNING: level_str = "W"; break; 		\
	case STM_LOG_LEVEL_ERROR: level_str = "E"; break; 			\
	default: level_str = "UNKNOWN"; break; 						\
	} 															\
	uint32_t tickstart = HAL_GetTick();							\
	printf("%s [%04ld, %s]: ", level_str, tickstart, TAG);		\
	printf(__VA_ARGS__); 										\
  printf("\n"); 												\
} while (0)

void STM_Logging_Init(UART_HandleTypeDef* p_uart, stm_log_level_t level);

#endif /* STM_LOG_STM_LOG_H_ */
