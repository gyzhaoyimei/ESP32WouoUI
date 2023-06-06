#ifndef _KNOB_H_
#define _KNOB_H_
#include "config.h"
/********************************** 自定义功能变量 **********************************/

// 旋钮功能变量
#define KNOB_PARAM 4
#define KNOB_DISABLE 0
#define KNOB_ROT_VOL 1
#define KNOB_ROT_BRI 2
enum
{
  KNOB_ROT,   // 睡眠下旋转旋钮的功能，0 禁用，1 音量，2 亮度
  KNOB_COD,   // 睡眠下短按旋钮输入的字符码，0 禁用
  KNOB_ROT_P, // 旋转旋钮功能在单选框中选择的位置
  KNOB_COD_P, // 字符码在单选框中选择的位置
};
struct
{
  uint8_t param[KNOB_PARAM] = {KNOB_DISABLE, KNOB_DISABLE, 2, 2}; // 禁用在列表的第2个选项，第0个是标题，第1个是分界线
} knob;

/************************************* 旋钮相关 *************************************/

// 可按下旋钮引脚
#define AIO 14
#define BIO 27
#define SW 12

// 按键ID
#define BTN_ID_CC 0 // 逆时针旋转
#define BTN_ID_CW 1 // 顺时针旋转
#define BTN_ID_SP 2 // 短按
#define BTN_ID_LP 3 // 长按

// 按键变量
#define BTN_PARAM_TIMES 2 // 由于uint8_t最大值可能不够，但它存储起来方便，这里放大两倍使用

void btn_init();
void knob_inter();
void btn_scan();



#endif // _KNOB_H_
