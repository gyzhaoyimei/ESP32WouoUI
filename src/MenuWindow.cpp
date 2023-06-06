#include "MenuWindow.h"

//弹窗数值初始化
void window_value_init(const char *title, uint8_t select, uint8_t *value, uint8_t max, uint8_t min, uint8_t step, MenuItem *bg, uint8_t index)
{
  win.title=title;
  win.select = select;
  win.value = value;
  win.max = max;
  win.min = min;
  win.step = step;
  win.bg = bg;
  win.index = index;  
  ui.index = M_WINDOW;
  ui.state = S_WINDOW;
}

//弹窗动画初始化
void window_param_init()
{
  win.bar = 0;
  win.y = WIN_Y;
  win.y_trg = win.u;
  ui.state = S_NONE;
}


//弹窗通用显示函数
void window_show()
{
  //绘制背景列表，根据开关判断背景是否要虚化
  list_show(win.bg, win.index);
  if (ui.param[WIN_BOK]) for (uint16_t i = 0; i < buf_len; ++i)  buf_ptr[i] = buf_ptr[i] & (i % 2 == 0 ? 0x55 : 0xAA);

  //更新动画目标值
  u8g2.setFont(WIN_FONT);
  win.bar_trg = (float)(*win.value - win.min) / (float)(win.max - win.min) * (WIN_BAR_W - 4);

  //计算动画过渡值
  animation(&win.bar, &win.bar_trg, WIN_ANI);
  animation(&win.y, &win.y_trg, WIN_ANI);

  //绘制窗口
  u8g2.setDrawColor(0); u8g2.drawRBox(win.l, (int16_t)win.y, WIN_W, WIN_H, 2);    //绘制外框背景
  u8g2.setDrawColor(1); u8g2.drawRFrame(win.l, (int16_t)win.y, WIN_W, WIN_H, 2);  //绘制外框描边
  u8g2.drawRFrame(win.l + 5, (int16_t)win.y + 20, WIN_BAR_W, WIN_BAR_H, 1);       //绘制进度条外框
  u8g2.drawBox(win.l + 7, (int16_t)win.y + 22, win.bar, WIN_BAR_H - 4);           //绘制进度条
  u8g2.setCursor(win.l + 5, (int16_t)win.y + 14); u8g2.print(win.title);          //绘制标题
  u8g2.setCursor(win.l + 78, (int16_t)win.y + 14); u8g2.print(*win.value);        //绘制当前值
  
  //需要在窗口修改参数时立即见效的函数
  if (!strcmp(win.title, "Disp Bri")) u8g2.setContrast(ui.param[DISP_BRI]);

  //反转屏幕内元素颜色，白天模式遮罩
  u8g2.setDrawColor(2);
  if (!ui.param[DARK_MODE]) u8g2.drawBox(0, 0, DISP_W, DISP_H);
}

//弹窗通用处理函数
void window_proc()
{
  window_show();
  if (win.y == WIN_Y_TRG) ui.index = win.index;
  if (btn.pressed && win.y == win.y_trg && win.y != WIN_Y_TRG)
  {
    btn.pressed = false;
    switch (btn.id)
    {
      case BTN_ID_CW: if (*win.value < win.max)  *win.value += win.step;  eeprom.change = true;  break;
      case BTN_ID_CC: if (*win.value > win.min)  *win.value -= win.step;  eeprom.change = true;  break;  
      case BTN_ID_SP: case BTN_ID_LP: win.y_trg = WIN_Y_TRG; break;
    }
  }
}
