#ifndef _MENUWINDOW_H_
#define _MENUWINDOW_H_

#include "knob.h"
#include "MenuEasing.h"
#include "MenuList.h"

// 弹窗数值初始化
void window_value_init(const char *title, uint8_t select, uint8_t *value, uint8_t max, uint8_t min, uint8_t step, MenuItem *bg, uint8_t index);
// 弹窗动画初始化
void window_param_init();
// 弹窗通用显示函数
void window_show();
// 弹窗通用处理函数
void window_proc();

#endif