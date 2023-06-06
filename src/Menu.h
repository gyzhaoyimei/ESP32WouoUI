#ifndef MENU_H
#define MENU_H

#include "knob.h"
#include "MenuEeprom.h"
#include "icondata.h"

#include "MenuIcon.h"
#include "MenuList.h"
#include "MenuWindow.h"
#include "MenuEasing.h"

void ui_init();
void ui_proc();
void layer_init_in();
// 进入更浅层级时的初始化
void layer_init_out();
#endif