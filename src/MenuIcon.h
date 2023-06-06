#ifndef _MENUICON_H_
#define _MENUICON_H_

#include "knob.h"
#include "MenuEeprom.h"
#include "MenuEasing.h"

// 进入磁贴类时的初始化
void tile_param_init();
// 磁贴类页面通用显示函数
void tile_show(struct MenuItem arr_1[], const uint8_t icon_pic[][120]);
// 磁贴类页面旋转时判断通用函数
void tile_rotate_switch();

#endif