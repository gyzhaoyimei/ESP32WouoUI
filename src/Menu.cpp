#include "menu.h"

MenuUI ui;
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, SCL, SDA);     // 分辨率：128*64  驱动：SSD1306  接口：IIC（硬件）
uint8_t   *buf_ptr;                 //指向屏幕缓冲的指针
uint16_t  buf_len;                  //缓冲长度

//OLED初始化函数
void oled_init()
{
  u8g2.setBusClock(1000000);  //硬件IIC接口使用
  u8g2.begin();

   u8g2.enableUTF8Print();
   u8g2.setFontDirection(0);
   //u8g2.setFontPosTop();
  //u8g2.setFont(u8g2_font_wqy12_t_gb2312);
  //u8g2.setDrawColor(1);
   //u8g2.setFontMode(1);

  u8g2.setContrast(ui.param[DISP_BRI]);

  buf_ptr = u8g2.getBufferPtr();
  buf_len = 8 * u8g2.getBufferTileHeight() * u8g2.getBufferTileWidth();

}


/************************************* 文字内容 *************************************/

MenuItem main_menu[]
{
  {"Sleep"},
  {"Editor"},
  {"Volt"},
  {"Setting"},
  {"Like"},
  // {"睡眠"},
  // {"编辑"},
  // {"电压"},
  // {"设置"},
  // {"退出"},
};

MenuItem editor_menu[]
{
  {"[ Editor ]"},
  {"- Function 0"},
  {"- Function 1"},
  {"- Knob"},
  //   {"[ 编辑 ]"},
  // {"- 功能 0"},
  // {"- 功能 1"},
  // {"- 编码器"},
};

MenuItem knob_menu[]
{
  {"[ Knob ]"},
  {"# Rotate Func"},
  {"$ Press Func"},
};

MenuItem krf_menu[]
{
  {"[ Rotate Function ]"},
  {"--------------------------"},
  {"= Disable"},
  {"--------------------------"},
  {"= Volume"},
  {"= Brightness"},
  {"--------------------------"},
};

MenuItem kpf_menu[]
{
  {"[ Press Function ]"},
  {"--------------------------"},
  {"= Disable"},
  {"--------------------------"},
  {"= A"},
  {"= B"},
  {"= C"},
  {"= D"},
  {"--------------------------"},
};

MenuItem volt_menu[]
{
  {"A0"},
  {"A1"},
  {"A2"},
  {"A3"},
  {"A4"},
  {"A5"},
  {"A6"},
  {"A7"},
};

MenuItem setting_menu[]
{
  {"[ Setting ]"},
  {"~ Disp Bri"},
  {"~ Tile Ani"},
  {"~ List Ani"},
  {"~ Win Ani"},
  {"~ Spot Ani"},
  {"~ Tag Ani"},
  {"~ Fade Ani"},
  {"~ Btn SPT"},
  {"~ Btn LPT"},
  {"+ T Ufd Fm Scr"},
  {"+ L Ufd Fm Scr"},
  {"+ T Loop Mode"},
  {"+ L Loop Mode"},
  {"+ Win Bokeh Bg"},
  {"+ Knob Rot Dir"},
  {"+ Dark Mode"},
  {"- [ About ]"},
};

MenuItem about_menu[]
{
  {"[ WouoUI ]"},
  {"- Version: v2.0"},
  {"- Board: STM32F103"},
  {"- Ram: 20k"},
  {"- Flash: 64k"},
  {"- Freq: 72Mhz"},
  {"- Creator: RQNG"},
  {"- Billi UID: 9182439"},  
};


//电压测量页面变量
//开发板模拟引脚
uint8_t   analog_pin[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 12, 13 };
/*********************************** UI 初始化函数 *********************************/
 
//列表类页面列表行数初始化，必须初始化的参数
void menuitem_num_init()
{
  ui.num[M_MAIN]      = sizeof( main_menu     )   / sizeof(MenuItem);
  ui.num[M_EDITOR]    = sizeof( editor_menu   )   / sizeof(MenuItem);
  ui.num[M_KNOB]      = sizeof( knob_menu     )   / sizeof(MenuItem);
  ui.num[M_KRF]       = sizeof( krf_menu      )   / sizeof(MenuItem);
  ui.num[M_KPF]       = sizeof( kpf_menu      )   / sizeof(MenuItem);
  ui.num[M_VOLT]      = sizeof( volt_menu     )   / sizeof(MenuItem);
  ui.num[M_SETTING]   = sizeof( setting_menu  )   / sizeof(MenuItem);
  ui.num[M_ABOUT]     = sizeof( about_menu    )   / sizeof(MenuItem);
}

void ui_init()
{
#ifdef DEBUG
  Serial.begin(115200);
  Serial.println("UI Testing...");
#endif

  eeprom_init();
  menuitem_num_init();
  oled_init();
  btn_init();
}

//进入睡眠时的初始化
void sleep_param_init()
{
  u8g2.setDrawColor(0);
  u8g2.drawBox(0, 0, DISP_W, DISP_H);
  u8g2.setPowerSave(1);
  ui.state = S_NONE;  
  ui.sleep = true;
  if (eeprom.change)
  {
    eeprom_write_all_data();
    eeprom.change = false;
  }
}


//旋钮设置页初始化
void knob_param_init() { check_box_v_init(knob.param); }

//旋钮旋转页初始化
void krf_param_init() { check_box_s_init(&knob.param[KNOB_ROT], &knob.param[KNOB_ROT_P]); }

//旋钮点按页初始化
void kpf_param_init() { check_box_s_init(&knob.param[KNOB_COD], &knob.param[KNOB_COD_P]); }

//列表显示旋钮功能
void list_draw_krf(int n) 
{ 
  switch (check_box.v[n - 1])
  {
    case 0: u8g2.print("OFF"); break;
    case 1: u8g2.print("VOL"); break;
    case 2: u8g2.print("BRI"); break;
  }
}

//列表显示按键键值
void list_draw_kpf(int n) 
{ 
  if (check_box.v[n - 1] == 0) u8g2.print("OFF");
  else if (check_box.v[n - 1] <= 90) u8g2.print((char)check_box.v[n - 1]);
  else u8g2.print("?");
}
//电压测量页初始化
void volt_param_init()
{
  volt.text_bg_r = 0;
  volt.text_bg_r_trg = VOLT_TEXT_BG_W; 
}

//设置页初始化
void setting_param_init()
{
  check_box_v_init(ui.param);
  check_box_m_init(ui.param);
}


//进入更深层级时的初始化
void layer_init_in()
{
  ui.layer ++;
  ui.init = 0;
  list.y = 0;
  list.y_trg = LIST_LINE_H;
  list.box_x = 0;
  list.box_y = 0;
  list.bar_y = 0;
  ui.state = S_FADE;
  switch (ui.index)
  {
    case M_MAIN:    tile_param_init();    break;  //睡眠进入主菜单，动画初始化   
    case M_KNOB:    knob_param_init();    break;  //旋钮设置页，行末尾文字初始化
    case M_KRF:     krf_param_init();     break;  //旋钮旋转页，单选框初始化  
    case M_KPF:     kpf_param_init();     break;  //旋钮点按页，单选框初始化  
    case M_VOLT:    volt_param_init();    break;  //主菜单进入电压测量页，动画初始化
    case M_SETTING: setting_param_init(); break;  //主菜单进入设置页，单选框初始化
  }
}

//进入更浅层级时的初始化
void layer_init_out()
{
  ui.select[ui.layer] = 0;
  list.box_y_trg[ui.layer] = 0;
  ui.layer --;
  ui.init = 0;
  list.y = 0;
  list.y_trg = LIST_LINE_H;
  list.bar_y = 0;
  ui.state = S_FADE;
  switch (ui.index)
  {
    case M_SLEEP: sleep_param_init(); break;    //主菜单进入睡眠页，检查是否需要写EEPROM
    case M_MAIN:  tile_param_init();  break;    //不管什么页面进入主菜单时，动画初始化
  }
}

/********************************** 分页面处理函数 **********************************/

//睡眠页面处理函数
void sleep_proc()
{
  while (ui.sleep)
  {
    //睡眠时循环执行的函数

    //睡眠时需要扫描旋钮才能退出睡眠
    btn_scan();

    //当旋钮有动作时
    if (btn.pressed) { btn.pressed = false; switch (btn.id) {    

        //睡眠时顺时针旋转执行的函数
        case BTN_ID_CW:
          switch (knob.param[KNOB_ROT])
          {
           // case KNOB_ROT_VOL: Consumer.press(HIDConsumer::VOLUME_UP);       Consumer.release(); break;
           // case KNOB_ROT_BRI: Consumer.press(HIDConsumer::BRIGHTNESS_UP);   Consumer.release(); break;
          }
          break;

        //睡眠时逆时针旋转执行的函数
        case BTN_ID_CC:
          switch (knob.param[KNOB_ROT])
          {
           // case KNOB_ROT_VOL: Consumer.press(HIDConsumer::VOLUME_DOWN);     Consumer.release(); break;
           // case KNOB_ROT_BRI: Consumer.press(HIDConsumer::BRIGHTNESS_DOWN); Consumer.release(); break;
          }
          break;

        //睡眠时短按执行的函数
        case BTN_ID_SP:
        // Keyboard.press(knob.param[KNOB_COD]);  Keyboard.release(knob.param[KNOB_COD]);
         break;   
        
        //睡眠时长按执行的函数
        case BTN_ID_LP: ui.index = M_MAIN;  ui.state = S_LAYER_IN; u8g2.setPowerSave(0); ui.sleep = false; break;
      }
    }
  }
}

//主菜单处理函数，磁贴类模板
void main_proc()
{
  tile_show(main_menu, main_icon_pic);
  if (btn.pressed)
  {
#ifdef DEBUG
    printf("btn.pressed=%d  btn.id=%d\n", btn.pressed, btn.id);
#endif

    btn.pressed = false;
    switch (btn.id)
    {
    case BTN_ID_CW:
    case BTN_ID_CC:
      tile_rotate_switch();
      break;
    case BTN_ID_SP:
      switch (ui.select[ui.layer])
      {

      case 0:
         ui.index = M_SLEEP;
         ui.state = S_LAYER_OUT;
         break;
      case 1:
         ui.index = M_EDITOR;
         ui.state = S_LAYER_IN;
         break;
      case 2:
         ui.index = M_VOLT;
         ui.state = S_LAYER_IN;
         break;
      case 3:
         ui.index = M_SETTING;
         ui.state = S_LAYER_IN;
         break;
      }
    }
    if (!tile.select_flag && ui.init)
    {
      tile.indi_x = 0;
      tile.title_y = tile.title_y_calc;
    }
  }
}

//编辑器菜单处理函数
void editor_proc()
{
  list_show(editor_menu, M_EDITOR); 
  if (btn.pressed) { btn.pressed = false; switch (btn.id) { case BTN_ID_CW: case BTN_ID_CC: list_rotate_switch(); break; case BTN_ID_LP: ui.select[ui.layer] = 0; case BTN_ID_SP: switch (ui.select[ui.layer]) {
        
        case 0:   ui.index = M_MAIN;  ui.state = S_LAYER_OUT; break;
        case 3:  ui.index = M_KNOB;  ui.state = S_LAYER_IN;  break;
      }
    }
  }
}

//旋钮编辑菜单处理函数
void knob_proc()
{
  list_show(knob_menu, M_KNOB);
  if (btn.pressed) { btn.pressed = false; switch (btn.id) { case BTN_ID_CW: case BTN_ID_CC: list_rotate_switch(); break; case BTN_ID_LP: ui.select[ui.layer] = 0; case BTN_ID_SP: switch (ui.select[ui.layer]) {
        
        case 0: ui.index = M_EDITOR;  ui.state = S_LAYER_OUT; break;
        case 1: ui.index = M_KRF;     ui.state = S_LAYER_IN;  check_box_s_init(&knob.param[KNOB_ROT], &knob.param[KNOB_ROT_P]); break;
        case 2: ui.index = M_KPF;     ui.state = S_LAYER_IN;  check_box_s_init(&knob.param[KNOB_COD], &knob.param[KNOB_COD_P]); break;
      }
    }
  }
}

//旋钮旋转功能菜单处理函数
void krf_proc()
{
  list_show(krf_menu, M_KRF);
  if (btn.pressed) { btn.pressed = false; switch (btn.id) { case BTN_ID_CW: case BTN_ID_CC: list_rotate_switch(); break; case BTN_ID_LP: ui.select[ui.layer] = 0; case BTN_ID_SP: switch (ui.select[ui.layer]) {
        
        case 0: ui.index = M_KNOB;  ui.state = S_LAYER_OUT; break;
        case 1: break;
        case 2: check_box_s_select(KNOB_DISABLE, ui.select[ui.layer]); break;
        case 3: break;
        case 4: check_box_s_select(KNOB_ROT_VOL, ui.select[ui.layer]); break;
        case 5: check_box_s_select(KNOB_ROT_BRI, ui.select[ui.layer]); break;
        case 6: break;
      }
    }
  }
}

//旋钮点按功能菜单处理函数
void kpf_proc()
{
  list_show(kpf_menu, M_KPF);
  if (btn.pressed) { btn.pressed = false; switch (btn.id) { case BTN_ID_CW: case BTN_ID_CC: list_rotate_switch(); break;  case BTN_ID_LP: ui.select[ui.layer] = 0; case BTN_ID_SP: switch (ui.select[ui.layer]) {
    
        case 0:   ui.index = M_KNOB;  ui.state = S_LAYER_OUT; break;
        case 1:   break;
        case 2:   check_box_s_select(KNOB_DISABLE, ui.select[ui.layer]); break;
        case 3:   break;
        case 4:   check_box_s_select('A', ui.select[ui.layer]); break;
        case 5:   check_box_s_select('B', ui.select[ui.layer]); break;
        case 6:   check_box_s_select('C', ui.select[ui.layer]); break;
        case 7:   check_box_s_select('D', ui.select[ui.layer]); break;
        case 8:   check_box_s_select('E', ui.select[ui.layer]); break;
        case 9:   check_box_s_select('F', ui.select[ui.layer]); break;
        case 10:  check_box_s_select('G', ui.select[ui.layer]); break;
        case 11:  check_box_s_select('H', ui.select[ui.layer]); break;
        case 12:  check_box_s_select('I', ui.select[ui.layer]); break;
        case 13:  check_box_s_select('J', ui.select[ui.layer]); break;
        case 14:  check_box_s_select('K', ui.select[ui.layer]); break;
        case 15:  check_box_s_select('L', ui.select[ui.layer]); break;
        case 16:  check_box_s_select('M', ui.select[ui.layer]); break;
        case 17:  check_box_s_select('N', ui.select[ui.layer]); break;
        case 18:  check_box_s_select('O', ui.select[ui.layer]); break;
        case 19:  check_box_s_select('P', ui.select[ui.layer]); break;
        case 20:  check_box_s_select('Q', ui.select[ui.layer]); break;
        case 21:  check_box_s_select('R', ui.select[ui.layer]); break;
        case 22:  check_box_s_select('S', ui.select[ui.layer]); break;
        case 23:  check_box_s_select('T', ui.select[ui.layer]); break;
        case 24:  check_box_s_select('U', ui.select[ui.layer]); break;
        case 25:  check_box_s_select('V', ui.select[ui.layer]); break;
        case 26:  check_box_s_select('W', ui.select[ui.layer]); break;
        case 27:  check_box_s_select('X', ui.select[ui.layer]); break;
        case 28:  check_box_s_select('Y', ui.select[ui.layer]); break;
        case 29:  check_box_s_select('Z', ui.select[ui.layer]); break;
        case 30:  break;
        case 31:  check_box_s_select('0', ui.select[ui.layer]); break;
        case 32:  check_box_s_select('1', ui.select[ui.layer]); break;
        case 33:  check_box_s_select('2', ui.select[ui.layer]); break;
        case 34:  check_box_s_select('3', ui.select[ui.layer]); break;
        case 35:  check_box_s_select('4', ui.select[ui.layer]); break;
        case 36:  check_box_s_select('5', ui.select[ui.layer]); break;
        case 37:  check_box_s_select('6', ui.select[ui.layer]); break;
        case 38:  check_box_s_select('7', ui.select[ui.layer]); break;
        case 39:  check_box_s_select('8', ui.select[ui.layer]); break;
        case 40:  check_box_s_select('9', ui.select[ui.layer]); break;
        case 41:  break;       
      }
    }
  }
}

//电压页面显示函数
void volt_show()
{
  //使用列表类显示选项
  list_show(volt_menu, M_VOLT); 

  //计算动画过渡值  
  animation(&volt.text_bg_r, &volt.text_bg_r_trg, TAG_ANI);

  //设置曲线颜色，0透显，1实显，2反色，这里用实显
  u8g2.setDrawColor(1);  

  //绘制电压曲线和外框
  volt.val = 0;
  u8g2.drawFrame(WAVE_BOX_L_S, 0, WAVE_BOX_W, WAVE_BOX_H);
  u8g2.drawFrame(WAVE_BOX_L_S + 1, 1, WAVE_BOX_W - 2, WAVE_BOX_H - 2);
  if (list.box_y == list.box_y_trg[ui.layer] && list.y == list.y_trg)
  {
    for (int i = 0; i < WAVE_SAMPLE * WAVE_W; i++) volt.ch0_adc[i] = volt.val = analogRead(analog_pin[ui.select[ui.layer]]);
    for (int i = 1; i < WAVE_W - 1; i++)
    { 
      volt.ch0_wave[i] = map(volt.ch0_adc[int(5 * i)], 0, 4095, WAVE_MAX, WAVE_MIN);   
      u8g2.drawLine(WAVE_L + i - 1, WAVE_U + volt.ch0_wave[i - 1], WAVE_L + i, WAVE_U + volt.ch0_wave[i]);
    }
  }

  //绘制电压值
  u8g2.setFontDirection(0);
  u8g2.setFont(VOLT_FONT); 
  u8g2.setCursor(39, DISP_H - 6);
  u8g2.print(volt.val / 4096.0 * 3.3);
  u8g2.print("V");

  //绘制列表选择框和电压文字背景
  u8g2.setDrawColor(2);
  u8g2.drawBox(VOLT_TEXT_BG_L_S, DISP_H - VOLT_TEXT_BG_H, volt.text_bg_r, VOLT_TEXT_BG_H);

  //反转屏幕内元素颜色，白天模式遮罩
  if (!ui.param[DARK_MODE]) u8g2.drawBox(0, 0, DISP_W, DISP_H);
}

//电压测量页处理函数
void volt_proc()
{
  volt_show();
  if (btn.pressed) { btn.pressed = false; switch (btn.id) { case BTN_ID_CW: case BTN_ID_CC: list_rotate_switch(); break;

      case BTN_ID_SP: case BTN_ID_LP: ui.index = M_MAIN;  ui.state = S_LAYER_OUT; break;
    }
  }
}

//设置菜单处理函数，多选框列表类模板，弹窗模板
void setting_proc()
{
  list_show(setting_menu, M_SETTING);
  if (btn.pressed) { btn.pressed = false; switch (btn.id) { case BTN_ID_CW: case BTN_ID_CC: list_rotate_switch(); break; case BTN_ID_LP: ui.select[ui.layer] = 0; case BTN_ID_SP: switch (ui.select[ui.layer]) {
        
        //返回更浅层级，长按被当作选择这一项，也是执行这一行
        case 0:   ui.index = M_MAIN;  ui.state = S_LAYER_OUT; break;
        
        //弹出窗口，参数初始化：标题，参数名，参数值，最大值，最小值，步长，背景列表名，背景列表标签
        case 1:   window_value_init("Disp Bri", DISP_BRI, &ui.param[DISP_BRI],  255,  0,  5, setting_menu, M_SETTING);  break;
        case 2:   window_value_init("Tile Ani", TILE_ANI, &ui.param[TILE_ANI],  100, 10,  1, setting_menu, M_SETTING);  break;
        case 3:   window_value_init("List Ani", LIST_ANI, &ui.param[LIST_ANI],  100, 10,  1, setting_menu, M_SETTING);  break;
        case 4:   window_value_init("Win Ani",  WIN_ANI,  &ui.param[WIN_ANI],   100, 10,  1, setting_menu, M_SETTING);  break;
        case 5:   window_value_init("Spot Ani", SPOT_ANI, &ui.param[SPOT_ANI],  100, 10,  1, setting_menu, M_SETTING);  break;
        case 6:   window_value_init("Tag Ani",  TAG_ANI,  &ui.param[TAG_ANI],   100, 10,  1, setting_menu, M_SETTING);  break;
        case 7:   window_value_init("Fade Ani", FADE_ANI, &ui.param[FADE_ANI],  255,  0,  1, setting_menu, M_SETTING);  break;
        case 8:   window_value_init("Btn SPT",  BTN_SPT,  &ui.param[BTN_SPT],   255,  0,  1, setting_menu, M_SETTING);  break;
        case 9:   window_value_init("Btn LPT",  BTN_LPT,  &ui.param[BTN_LPT],   255,  0,  1, setting_menu, M_SETTING);  break;

        //多选框
        case 10:  check_box_m_select( TILE_UFD  );  break;
        case 11:  check_box_m_select( LIST_UFD  );  break;
        case 12:  check_box_m_select( TILE_LOOP );  break;
        case 13:  check_box_m_select( LIST_LOOP );  break;
        case 14:  check_box_m_select( WIN_BOK   );  break;
        case 15:  check_box_m_select( KNOB_DIR  );  break;
        case 16:  check_box_m_select( DARK_MODE );  break;

        //关于本机
        case 17:  ui.index = M_ABOUT; ui.state = S_LAYER_IN; break;
      }
    }
  }
}

//关于本机页
void about_proc()
{
  list_show(about_menu, M_ABOUT);
  if (btn.pressed) { btn.pressed = false; switch (btn.id) { case BTN_ID_CW: case BTN_ID_CC: list_rotate_switch(); break; case BTN_ID_LP: ui.select[ui.layer] = 0; case BTN_ID_SP: switch (ui.select[ui.layer]) {

        case 0:   ui.index = M_SETTING;  ui.state = S_LAYER_OUT; break;
      }
    }
  }
}

//总的UI进程
void ui_proc()
{
  btn_scan();

  u8g2.sendBuffer();
  switch (ui.state)
  {
    case S_FADE:          fade();                   break;  //转场动画
    case S_WINDOW:        window_param_init();      break;  //弹窗初始化
    case S_LAYER_IN:      layer_init_in();          break;  //层级初始化
    case S_LAYER_OUT:     layer_init_out();         break;  //层级初始化
  
    case S_NONE: u8g2.clearBuffer(); switch (ui.index)      //直接选择页面
    {
      case M_WINDOW:      window_proc();            break;
      case M_SLEEP:       sleep_proc();             break;
      case M_MAIN:        main_proc();              break;
      case M_EDITOR:      editor_proc();            break;
      case M_KNOB:        knob_proc();              break;
      case M_KRF:         krf_proc();               break;
      case M_KPF:         kpf_proc();               break;
      case M_VOLT:        volt_proc();              break;
      case M_SETTING:     setting_proc();           break;
      case M_ABOUT:       about_proc();             break;
    }
  }


}



