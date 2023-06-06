#include "MenuEeprom.h"

 MenuEeprom eeprom;

//在初始化EEPROM时，选择性初始化的默认设置
void ui_param_init()
{
  ui.param[DISP_BRI]  = 255;      //屏幕亮度
  ui.param[TILE_ANI]  = 30;       //磁贴动画速度
  ui.param[LIST_ANI]  = 60;       //列表动画速度
  ui.param[WIN_ANI]   = 25;       //弹窗动画速度
  ui.param[SPOT_ANI]  = 50;       //聚光动画速度
  ui.param[TAG_ANI]   = 60;       //标签动画速度
  ui.param[FADE_ANI]  = 30;       //消失动画速度
  ui.param[BTN_SPT]   = 25;       //按键短按时长
  ui.param[BTN_LPT]   = 150;      //按键长按时长
  ui.param[TILE_UFD]  = 1;        //磁贴图标从头展开开关
  ui.param[LIST_UFD]  = 1;        //菜单列表从头展开开关
  ui.param[TILE_LOOP] = 0;        //磁贴图标循环模式开关
  ui.param[LIST_LOOP] = 0;        //菜单列表循环模式开关
  ui.param[WIN_BOK]   = 0;        //弹窗背景虚化开关
  ui.param[KNOB_DIR]  = 0;        //旋钮方向切换开关   
  ui.param[DARK_MODE] = 1;        //黑暗模式开关   
}

//开机检查是否已经修改过，没修改过则跳过读配置步骤，用默认设置
void eeprom_init()
{
  EEPROM.begin(EEPROM_SIZE);

  eeprom.check = 0;
  eeprom.address = 0; for (uint8_t i = 0; i < EEPROM_CHECK; ++i)  if (EEPROM.read(eeprom.address + i) != eeprom.check_param[i])  eeprom.check ++;
  if (eeprom.check <= 1) eeprom_read_all_data();  //允许一位误码
  else ui_param_init();

  #ifdef DEBUG
  uint8_t address = 0;
  printf("eeprom.check=%d\n", eeprom.check);
  for (uint8_t i = 0; i < EEPROM_CHECK; ++i)
  {
    printf("check_param[%d]=%d\n", address + i, EEPROM.read(address + i));
  }

  address += EEPROM_CHECK;
  for (uint8_t i = 0; i < UI_PARAM; ++i)
  {
    printf("ui.param[%d]=%d\n", address + i, EEPROM.read(address + i));
  }

  address += UI_PARAM;
  for (uint8_t i = 0; i < KNOB_PARAM; ++i)
  {
    printf("knob.param[%d]=%d\n", address + i, EEPROM.read(address + i));
  }

#endif
}

//EEPROM写数据，回到睡眠时执行一遍
void eeprom_write_all_data()
{
  eeprom.address = 0;
  for (uint8_t i = 0; i < EEPROM_CHECK; ++i)    EEPROM.write(eeprom.address + i, eeprom.check_param[i]);  eeprom.address += EEPROM_CHECK;
  for (uint8_t i = 0; i < UI_PARAM; ++i)        EEPROM.write(eeprom.address + i, ui.param[i]);            eeprom.address += UI_PARAM;
  for (uint8_t i = 0; i < KNOB_PARAM; ++i)      EEPROM.write(eeprom.address + i, knob.param[i]);          //eeprom.address += KNOB_PARAM;
  EEPROM.commit(); // 保存
}

//EEPROM读数据，开机初始化时执行一遍
void eeprom_read_all_data()
{
  eeprom.address = EEPROM_CHECK;   
  for (uint8_t i = 0; i < UI_PARAM; ++i)        ui.param[i]   = EEPROM.read(eeprom.address + i);          eeprom.address += UI_PARAM;
  for (uint8_t i = 0; i < KNOB_PARAM; ++i)      knob.param[i] = EEPROM.read(eeprom.address + i);          eeprom.address += KNOB_PARAM;
}




