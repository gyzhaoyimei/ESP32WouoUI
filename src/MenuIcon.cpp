#include "MenuIcon.h"

//进入磁贴类时的初始化
void tile_param_init()
{
  ui.init = false;
  tile.icon_x = 0;
  tile.icon_x_trg = TILE_ICON_S;
  tile.icon_y = -TILE_ICON_H;
  tile.icon_y_trg = 0;
  tile.indi_x = 0;
  tile.indi_x_trg = TILE_INDI_W;
  tile.title_y = tile.title_y_calc;
  tile.title_y_trg = tile.title_y_trg_calc;
}


//磁贴类页面通用显示函数
void tile_show(struct MenuItem arr_1[], const uint8_t icon_pic[][120])
{
  //计算动画过渡值
  animation(&tile.icon_x, &tile.icon_x_trg, TILE_ANI);
  animation(&tile.icon_y, &tile.icon_y_trg, TILE_ANI);
  animation(&tile.indi_x, &tile.indi_x_trg, TILE_ANI);
  animation(&tile.title_y, &tile.title_y_trg, TILE_ANI);

  //设置大标题的颜色，0透显，1实显，2反色，这里用实显
  u8g2.setDrawColor(1);

  //绘制大标题
  u8g2.setFont(TILE_B_FONT); 
//u8g2.drawStr(((DISP_W - TILE_INDI_W) - u8g2.getStrWidth(arr_1[ui.select[ui.layer]].m_select)) / 2 + TILE_INDI_W, tile.title_y, arr_1[ui.select[ui.layer]].m_select);
  u8g2.drawUTF8(((DISP_W - TILE_INDI_W) - u8g2.getStrWidth(arr_1[ui.select[ui.layer]].m_select)) / 2 + TILE_INDI_W, tile.title_y, arr_1[ui.select[ui.layer]].m_select);
  //绘制大标题指示器
  u8g2.drawBox(0, TILE_ICON_S, tile.indi_x, TILE_INDI_H);

  //绘制图标
  if (!ui.init)
  {
    for (uint8_t i = 0; i < ui.num[ui.index]; ++i)  
    {
      if (ui.param[TILE_UFD]) tile.temp = (DISP_W - TILE_ICON_W) / 2 + i * tile.icon_x - TILE_ICON_S * ui.select[ui.layer];
      else tile.temp = (DISP_W - TILE_ICON_W) / 2 + (i - ui.select[ui.layer]) * tile.icon_x;
      u8g2.drawXBMP(tile.temp, (int16_t)tile.icon_y, TILE_ICON_W, TILE_ICON_H, icon_pic[i]); 
    }
    if (tile.icon_x == tile.icon_x_trg) 
    {
      ui.init = true;
      tile.icon_x = tile.icon_x_trg = - ui.select[ui.layer] * TILE_ICON_S;
    }
  }
  else for (uint8_t i = 0; i < ui.num[ui.index]; ++i) u8g2.drawXBMP((DISP_W - TILE_ICON_W) / 2 + (int16_t)tile.icon_x + i * TILE_ICON_S, 0, TILE_ICON_W, TILE_ICON_H, icon_pic[i]);

  //反转屏幕内元素颜色，白天模式遮罩
  u8g2.setDrawColor(2);
  if (!ui.param[DARK_MODE]) u8g2.drawBox(0, 0, DISP_W, DISP_H);
}




/*********************************** 通用处理函数 ***********************************/

//磁贴类页面旋转时判断通用函数
void tile_rotate_switch()
{
  switch (btn.id)
  { 
    case BTN_ID_CC:
      if (ui.init)
      {
        if (ui.select[ui.layer] > 0)
        {
          ui.select[ui.layer] -= 1;
          tile.icon_x_trg += TILE_ICON_S;
          tile.select_flag = false;
        }
        else 
        {
          if (ui.param[TILE_LOOP])
          {
            ui.select[ui.layer] = ui.num[ui.index] - 1;
            tile.icon_x_trg = - TILE_ICON_S * (ui.num[ui.index] - 1);
            break;
          }
          else tile.select_flag = true;
        }
      }
      break;

    case BTN_ID_CW:
      if (ui.init)
      {
        if (ui.select[ui.layer] < (ui.num[ui.index] - 1)) 
        {
          ui.select[ui.layer] += 1;
          tile.icon_x_trg -= TILE_ICON_S;
          tile.select_flag = false;
        }
        else 
        {
          if (ui.param[TILE_LOOP])
          {
            ui.select[ui.layer] = 0;
            tile.icon_x_trg = 0;
            break;
          }
          else tile.select_flag = true;
        }
      }
      break;
  }
}


