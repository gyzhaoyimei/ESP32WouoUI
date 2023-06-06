#ifndef _MENULIST_H_
#define _MENULIST_H_

#include "knob.h"
#include "MenuEeprom.h"
#include "MenuEasing.h"

// 显示数值的初始化
void check_box_v_init(uint8_t *param);

// 多选框的初始化
void check_box_m_init(uint8_t *param);

// 单选框时的初始化
void check_box_s_init(uint8_t *param, uint8_t *param_p);

// 多选框处理函数
void check_box_m_select(uint8_t param);

// 单选框处理函数
void check_box_s_select(uint8_t val, uint8_t pos);

// 列表显示数值
void list_draw_value(int n);

// 绘制外框
void list_draw_check_box_frame();

// 绘制框里面的点
void list_draw_check_box_dot();

// 列表类页面通用显示函数
void list_show(struct MenuItem arr[], uint8_t ui_index);

// 列表类页面旋转时判断通用函数
void list_rotate_switch();
#endif