#ifndef _MENUEEPROM_H_
#define _MENUEEPROM_H_

#include <EEPROM.h>
#include "knob.h"

// EEPROM变量

#define EEPROM_SIZE EEPROM_CHECK + UI_PARAM + KNOB_PARAM + 1

// 声明 EEPROM 函数
void eeprom_write_all_data();
void eeprom_read_all_data();
void eeprom_init();


#endif /* _EEPROM_H_ */