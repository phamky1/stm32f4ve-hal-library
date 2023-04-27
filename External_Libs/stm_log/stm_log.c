#include "../../External_Libs/stm_log/include/stm_log.h"

static UART_HandleTypeDef *p_huart = NULL;
stm_log_level_t log_level = STM_LOG_LEVEL_NONE;

#if defined(__GNUC__)
int _write(int fd, char * ptr, int len) {
	if (!p_huart) {
		return -1;
	}
	HAL_UART_Transmit(p_huart, (uint8_t*)ptr, len, HAL_MAX_DELAY);
	return len;
}
#elif defined(__ICCARM__)#include "LowLevelIOInterface.h"

size_t __write(int handle, const unsigned char * buffer, size_t size) {
	if (!p_huart) {
			return -1;
	}
	HAL_UART_Transmit(p_huart, (uint8_t*)buffer, size, HAL_MAX_DELAY);
	return size;
}
#elif defined(__CC_ARM)
int fputc(int ch, FILE * f) {
	if (!p_huart) {
				return -1;
	}
	HAL_UART_Transmit(p_huart, (uint8_t*)&ch, 1, HAL_MAX_DELAY);
	return ch;
}
#endif

void STM_Logging_Init(UART_HandleTypeDef* p_uart, stm_log_level_t level) {
	p_huart = p_uart;
	log_level = level;
}
