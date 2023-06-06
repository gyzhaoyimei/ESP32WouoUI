#include "MenuList.h"


/********************************* 初始化数据处理函数 *******************************/

//显示数值的初始化
void check_box_v_init(uint8_t *param)
{
  check_box.v = param;
}

//多选框的初始化
void check_box_m_init(uint8_t *param)
{
  check_box.m = param;
}

//单选框时的初始化
void check_box_s_init(uint8_t *param, uint8_t *param_p)
{
  check_box.s = param;
  check_box.s_p = param_p;
}

//多选框处理函数
void check_box_m_select(uint8_t param)
{
  check_box.m[param] = !check_box.m[param];
  eeprom.change = true;
}

//单选框处理函数
void check_box_s_select(uint8_t val, uint8_t pos)
{
  *check_box.s = val;
  *check_box.s_p = pos;
  eeprom.change = true;
}


/*************** 根据列表每行开头符号，判断每行尾部是否绘制以及绘制什么内容 *************/

//列表显示数值
void list_draw_value(int n) { u8g2.print(check_box.v[n - 1]); }

//绘制外框
void list_draw_check_box_frame() { u8g2.drawRFrame(CHECK_BOX_L_S, list.temp + CHECK_BOX_U_S, CHECK_BOX_F_W, CHECK_BOX_F_H, 1); }

//绘制框里面的点
void list_draw_check_box_dot() { u8g2.drawBox(CHECK_BOX_L_S + CHECK_BOX_D_S + 1, list.temp + CHECK_BOX_U_S + CHECK_BOX_D_S + 1, CHECK_BOX_F_W - (CHECK_BOX_D_S + 1) * 2, CHECK_BOX_F_H - (CHECK_BOX_D_S + 1) * 2); }


//判断列表尾部内容
void list_draw_text_and_check_box(struct MenuItem arr[], int i)
{
  //u8g2.drawStr(LIST_TEXT_S, list.temp + LIST_TEXT_H + LIST_TEXT_S, arr[i].m_select);
  u8g2.drawUTF8(LIST_TEXT_S, list.temp + LIST_TEXT_H + LIST_TEXT_S, arr[i].m_select);
  u8g2.setCursor(CHECK_BOX_L_S, list.temp + LIST_TEXT_H + LIST_TEXT_S);
  switch (arr[i].m_select[0])
  {
    case '~': list_draw_value(i); break;
    case '+': list_draw_check_box_frame(); if (check_box.m[i - 1] == 1)  list_draw_check_box_dot(); break;
    case '=': list_draw_check_box_frame(); if (*check_box.s_p == i)      list_draw_check_box_dot(); break;
   // case '#': list_draw_krf(i);   break;
    //case '$': list_draw_kpf(i);   break;
  }
}

/******************************** 列表显示函数 **************************************/

//列表类页面通用显示函数
void list_show(struct MenuItem arr[], uint8_t ui_index)
{
  //更新动画目标值
  u8g2.setFont(LIST_FONT);
  list.box_x_trg = u8g2.getStrWidth(arr[ui.select[ui.layer]].m_select) + LIST_TEXT_S * 2;
  list.bar_y_trg = ceil((ui.select[ui.layer]) * ((float)DISP_H / (ui.num[ui_index] - 1)));
  
  //计算动画过渡值
  animation(&list.y, &list.y_trg, LIST_ANI);
  animation(&list.box_x, &list.box_x_trg, LIST_ANI);
  animation(&list.box_y, &list.box_y_trg[ui.layer], LIST_ANI);
  animation(&list.bar_y, &list.bar_y_trg, LIST_ANI);

  //检查循环动画是否结束
  if (list.loop && list.box_y == list.box_y_trg[ui.layer]) list.loop = false;

  //设置文字和进度条颜色，0透显，1实显，2反色，这里都用实显
  u8g2.setDrawColor(1);
  
  //绘制进度条
  u8g2.drawHLine(DISP_W - LIST_BAR_W, 0, LIST_BAR_W);
  u8g2.drawHLine(DISP_W - LIST_BAR_W, DISP_H - 1, LIST_BAR_W);
  u8g2.drawVLine(DISP_W - ceil((float)LIST_BAR_W / 2), 0, DISP_H);
  u8g2.drawBox(DISP_W - LIST_BAR_W, 0, LIST_BAR_W, list.bar_y);

  //绘制列表文字  
  if (!ui.init)
  {
    for (int i = 0; i < ui.num[ui_index]; ++ i)
    {
      if (ui.param[LIST_UFD]) list.temp = i * list.y - LIST_LINE_H * ui.select[ui.layer] + list.box_y_trg[ui.layer];
      else list.temp = (i - ui.select[ui.layer]) * list.y + list.box_y_trg[ui.layer];
      list_draw_text_and_check_box(arr, i);
    }
    if (list.y == list.y_trg) 
    {
      ui.init = true;
      list.y = list.y_trg = - LIST_LINE_H * ui.select[ui.layer] + list.box_y_trg[ui.layer];
    }
  }
  else for (int i = 0; i < ui.num[ui_index]; ++ i)
  {
    list.temp = LIST_LINE_H * i + list.y;
    list_draw_text_and_check_box(arr, i);
  }

  //绘制文字选择框，0透显，1实显，2反色，这里用反色
  u8g2.setDrawColor(2);
  u8g2.drawRBox(0, list.box_y, list.box_x, LIST_LINE_H, LIST_BOX_R);

  //反转屏幕内元素颜色，白天模式遮罩，在这里屏蔽有列表参与的页面，使遮罩作用在那个页面上
  if (!ui.param[DARK_MODE])
  {
    u8g2.drawBox(0, 0, DISP_W, DISP_H);
    switch(ui.index)
    {
      case M_WINDOW: 
      case M_VOLT:
      u8g2.drawBox(0, 0, DISP_W, DISP_H);  
    }
  }
}


//列表类页面旋转时判断通用函数
void list_rotate_switch()
{
  if (!list.loop)
  {
    switch (btn.id)
    {
      case BTN_ID_CC:
        if (ui.select[ui.layer] == 0)
        {
          if (ui.param[LIST_LOOP] && ui.init)
          {
            list.loop = true;
            ui.select[ui.layer] = ui.num[ui.index] - 1;
            if (ui.num[ui.index] > list.line_n) 
            {
              list.box_y_trg[ui.layer] = DISP_H - LIST_LINE_H;
              list.y_trg = DISP_H - ui.num[ui.index] * LIST_LINE_H;
            }
            else list.box_y_trg[ui.layer] = (ui.num[ui.index] - 1) * LIST_LINE_H;
            break;
          }
          else break;
        }
        if (ui.init)
        {
          ui.select[ui.layer] -= 1;
          if (ui.select[ui.layer] < - (list.y_trg / LIST_LINE_H)) 
          {
            if (!(DISP_H % LIST_LINE_H)) list.y_trg += LIST_LINE_H;
            else
            {
              if (list.box_y_trg[ui.layer] == DISP_H - LIST_LINE_H * list.line_n)
              {
                list.y_trg += (list.line_n + 1) * LIST_LINE_H - DISP_H;
                list.box_y_trg[ui.layer] = 0;
              }
              else if (list.box_y_trg[ui.layer] == LIST_LINE_H)
              {
                list.box_y_trg[ui.layer] = 0;
              }
              else list.y_trg += LIST_LINE_H;
            }
          }
          else list.box_y_trg[ui.layer] -= LIST_LINE_H;
          break;
        }

      case BTN_ID_CW:
        if (ui.select[ui.layer] == (ui.num[ui.index] - 1))
        {
          if (ui.param[LIST_LOOP] && ui.init)
          {
            list.loop = true;
            ui.select[ui.layer] = 0;
            list.y_trg = 0;
            list.box_y_trg[ui.layer] = 0;
            break;
          }
          else break;
        }
        if (ui.init)
        {
          ui.select[ui.layer] += 1;
          if ((ui.select[ui.layer] + 1) > (list.line_n - list.y_trg / LIST_LINE_H))
          {
            if (!(DISP_H % LIST_LINE_H)) list.y_trg -= LIST_LINE_H;
            else
            {
              if (list.box_y_trg[ui.layer] == LIST_LINE_H * (list.line_n - 1))
              {
                list.y_trg -= (list.line_n + 1) * LIST_LINE_H - DISP_H;
                list.box_y_trg[ui.layer] = DISP_H - LIST_LINE_H;
              }
              else if (list.box_y_trg[ui.layer] == DISP_H - LIST_LINE_H * 2)
              {
                list.box_y_trg[ui.layer] = DISP_H - LIST_LINE_H;
              }
              else list.y_trg -= LIST_LINE_H;
            }
          }
          else list.box_y_trg[ui.layer] += LIST_LINE_H;
          break;
        }
        break;
    }
  }
}

