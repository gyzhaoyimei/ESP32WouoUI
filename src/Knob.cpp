#include "knob.h"

Button btn;

void btn_init()
{
  pinMode(AIO, INPUT);
  pinMode(BIO, INPUT);
  pinMode(SW, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(AIO), knob_inter, CHANGE);
}
void knob_inter()  
{ 
  // 读取A、B旋钮信号输入的电平值 
  btn.alv = digitalRead(AIO); 
  btn.blv = digitalRead(BIO); 
   
  // 检测旋钮是否被旋转 
  if (!btn.flag && btn.alv == LOW) // 如果之前没有检测到旋转并且A电平为低电平（即旋钮被旋转） 
  { 
    btn.CW_1 = btn.blv; // 记录旋转方向 
    btn.flag = true; // 标记已经检测到旋转 
  } 
   
  // 检测旋钮是否停止旋转 
  if (btn.flag && btn.alv) // 如果之前检测到旋转且A电平为高电平（即旋钮停止旋转） 
  { 
    btn.CW_2 = !btn.blv; // 记录旋转方向 
    if (btn.CW_1 && btn.CW_2) // 如果两次检测到的方向一致 
    { 
      btn.id = ui.param[KNOB_DIR]; // 设置旋钮方向为参数中设置的方向 
      btn.pressed = true; // 标记旋钮被按下 
    } 
    if (btn.CW_1 == false && btn.CW_2 == false) // 如果两次检测到的方向相反 
    { 
      btn.id = !ui.param[KNOB_DIR]; // 设置旋钮方向为参数中设置的反方向 
      btn.pressed = true; // 标记旋钮被按下 
    } 
    btn.flag = false; // 取消旋钮旋转标记 
  } 
} 
 
void btn_scan()  
{ 
  // 读取按键输入的电平值 
  btn.val = digitalRead(SW); 
   
  // 检测按键是否被按下 
  if (btn.val != btn.val_last) // 如果当前读取到的电平值和之前的电平值不一致 
  { 
    btn.val_last = btn.val; // 记录当前电平值 
    delay(ui.param[BTN_SPT] * BTN_PARAM_TIMES); // 等待一段时间防止抖动 
    btn.val = digitalRead(SW); // 重新读取电平值 
    if (btn.val == LOW) // 如果电平值为低电平（即按键被按下） 
    { 
      btn.pressed = true; // 标记按键被按下 
      btn.count = 0; // 计数器清零 
      while (!digitalRead(SW)) // 如果按键一直没有松开 
      { 
        btn.count++; // 计数器加一 
        delay(1); // 短暂延时 
      } 
      if (btn.count < ui.param[BTN_LPT] * BTN_PARAM_TIMES) // 如果计数次数小于参数中设置的值 
        btn.id = BTN_ID_SP; // 设置按键ID为短按 
      else // 如果计数次数大于等于参数中设置的值 
        btn.id = BTN_ID_LP; // 设置按键ID为长按 
    } 
  } 
}