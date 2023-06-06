
/*
  此项目模仿自稚晖君暂未开源的 MonoUI，用于实现类似 UltraLink 的丝滑界面

  有四个版本：

    * 分辨率 128 * 128 ：主菜单，列表和关于本机页基本还原了 UltraLink 的界面和动态效果，电压测量页为原创设计
    * 分辨率 128 * 64 ：主菜单模仿 UltraLink 重新设计，去掉了小标题，电压测量页重新设计，关于本机页面改为列表，列表适配了该分辨率
    * 分辨率 128 * 32 ：在 128 * 64 分辨率的基础上，主界面只保留图标，电压测量页重新设计
    * 通用版本：仅保留列表，主菜单也改为列表，删除电压测量页和与列表无关的动画（保留弹窗效果），经过简单修改可以适配任何分辨率，任何行高度的情况

  WouoUI v2.0 功能：

    * 全部使用非线性的平滑缓动动画，包括列表，弹窗，甚至进度条
    * 优化平滑动画算法到只有两行，分类别定义平滑权重，并且每个权重值都分别可调
    * 可以打断的非线性动画，当前动画未结束但下一次动画已经被触发时，动画可以自然过渡
    * 非通用版本分别适配了类似 UltraLink 主菜单的磁贴界面（因为让我想起 WP7 的 Metron 风格，所以称之为磁贴）
    * 通用版本仅保留列表类界面，经过简单修改可以适配所有分辨率的屏幕，包括屏幕内行数不是整数的情况
    * 列表菜单，列表可以无限延长
    * 列表文字选择框，选择框可根据选择的字符串长度自动伸缩，进入菜单时从列表开头从长度 0 展开，转到上一级列表时，长度和纵坐标平滑移动到上一级选择的位置
    * 列表单选框，储存数据时也储存该值在列表中所在的位置，展开列表时根据每行开头的字符判断是否绘制外框，再根据位置数据判断是否绘制里面的点
    * 列表多选框，储存数据的数组跟多选框列表的行数对应，不要求连续排列，展开列表时根据每行开头的字符判断是否绘制外框，再根据行数对应的储存数据位置的数值是否为1判断是否绘制里面的点
    * 列表显示数值，与多选框原理相似，但不涉及修改操作
    * 列表展开动画，初始化列表时，可以选择列表从头开始展开，或者从上次选中的位置展开
    * 图标展开动画，初始化磁贴类界面时，可以选择图标从头开始展开，或者从上次选中的位置展开
    * 弹出窗口，实现了窗口弹出的动画效果，可以自定义最大值，最小值，步进值，需要修改的参数等，窗口独立运行，调用非常简单
    * 弹出窗口背景虚化可选项，背景虚化会产生卡顿感，但删掉代码有些可惜，因此做成可选项，默认关闭
    * 亮度调节，在弹出窗口中调节亮度值可以实时改变当前亮度值
    * 旋钮功能，使用EC11旋钮控制，旋钮方向可以软件调整，内置一个USB控制电脑的示例，在睡眠模式下旋转调整音量或者亮度，短按输入一个键值，长按进入主菜单，旋钮消抖时长等参数可以在弹出窗口中调整
    * 循环模式，选择项超过头尾时，选择项跳转到另一侧继续，列表和磁贴类可以分别选择
    * 黑暗模式，其实本来就是黑暗模式，是增加了白天模式，默认开启黑暗模式
    * 消失动画适配两种模式，一种是渐变成全黑，另一种渐变成全白
    * 断电存储，用简单直观的方式将每种功能参数写入EEPROM，只在修改过参数，进入睡眠模式时写入，避免重复擦写，初始化时检查11个标志位，允许一位误码

  项目参考：

    * B站：路徍要什么自行车：在线仿真：https://wokwi.com/projects/350306511434547796，https://www.bilibili.com/video/BV1HA411S7pv/
    * Github：createskyblue：OpenT12：https://github.com/createskyblue/OpenT12
    * Github：peng-zhihui：OpenHeat：https://github.com/peng-zhihui/OpenHeat

  注意事项：

    * 示例使用的单片机为 STM32F103C8T6 ，旋钮为 EC11 ，旋钮旋转使用中断程序，旋钮质量不好可能使界面卡死，原因不明，如果能查明原因请联系我
    * 为防止使用者在上传程序后无法直接使用，认为是代码有问题，HID功能默认禁用，如需使用旋钮音量控制和点按输入功能，请在初始化函数中启用相关功能，上传成功后设置两个跳线帽都为0，断电再插上 USB 线

  本项目使用Apache 2.0开源协议，如需商用或借鉴，请在醒目处标注本项目开源地址
  欢迎关注我的B站账号，用户名：音游玩的人，B站主页：https://space.bilibili.com/9182439?spm_id_from=..0.0
*/

#ifndef _CONFIG_H_
#define _CONFIG_H_
 
#include <Arduino.h>
#include <U8g2lib.h>

#define  DEBUG //输出调试信息

/************************************* 定义页面 *************************************/

//总目录，缩进表示页面层级
enum 
{
  M_LOGO,//开始界面
  M_WINDOW,
  M_SLEEP,
    M_MAIN, 
      M_EDITOR,
        M_KNOB,
          M_KRF,
          M_KPF,
      M_VOLT,
      M_SETTING,
        M_ABOUT,
};

//状态，初始化标签
enum
{
  S_FADE,       //转场动画
  S_WINDOW,     //弹窗初始化
  S_LAYER_IN,   //层级初始化
  S_LAYER_OUT,  //层级初始化
  S_NONE,       //直接选择页面
};

//菜单结构体 
struct MenuItem
{
  const char *m_select;
};

/************************************* 页面变量 *************************************/

//OLED变量
#define   DISP_H              64    //屏幕高度
#define   DISP_W              128   //屏幕宽度

extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;
extern uint8_t *buf_ptr; // 指向屏幕缓冲的指针
extern uint16_t buf_len; // 缓冲长度

//UI变量
#define   UI_DEPTH            20    //最深层级数
#define   UI_MNUMB            100   //菜单数量
#define   UI_PARAM            16    //参数数量
enum 
{
  DISP_BRI,     //屏幕亮度
  TILE_ANI,     //磁贴动画速度
  LIST_ANI,     //列表动画速度
  WIN_ANI,      //弹窗动画速度
  SPOT_ANI,     //聚光动画速度
  TAG_ANI,      //标签动画速度
  FADE_ANI,     //消失动画速度
  BTN_SPT,      //按键短按时长
  BTN_LPT,      //按键长按时长
  TILE_UFD,     //磁贴图标从头展开开关
  LIST_UFD,     //菜单列表从头展开开关
  TILE_LOOP,    //磁贴图标循环模式开关
  LIST_LOOP,    //菜单列表循环模式开关
  WIN_BOK,      //弹窗背景虚化开关
  KNOB_DIR,     //旋钮方向切换开关
  DARK_MODE,    //黑暗模式开关
};

struct MenuUI
{
  bool init;
  uint8_t num[UI_MNUMB];
  uint8_t select[UI_DEPTH];
  uint8_t layer;
  uint8_t index = M_MAIN;
  uint8_t state = S_LAYER_IN; 
  bool sleep = false;
  uint8_t fade = 1;
  uint8_t param[UI_PARAM];
};
extern MenuUI ui;

struct Button
{
  uint8_t id;
  bool flag;
  bool pressed;
  bool CW_1;
  bool CW_2;
  bool val;
  bool val_last;
  bool alv;
  bool blv;
  long count;
};
extern Button btn;

#define EEPROM_CHECK 11
struct MenuEeprom
{
  bool init;
  bool change;
  int address;
  uint8_t check;
  uint8_t check_param[EEPROM_CHECK] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k'};
};
extern MenuEeprom eeprom;

/************************************* 磁贴变量 *************************************/
//磁贴变量
//所有磁贴页面都使用同一套参数
#define   TILE_B_FONT         u8g2_font_helvB18_tr        //磁贴大标题字体
//#define   TILE_B_FONT         u8g2_font_wqy16_t_gb2312
#define   TILE_B_TITLE_H      18                          //磁贴大标题字体高度
#define   TILE_ICON_H         30                          //磁贴图标高度
#define   TILE_ICON_W         30                          //磁贴图标宽度
#define   TILE_ICON_S         36                          //磁贴图标间距
#define   TILE_INDI_H         27                          //磁贴大标题指示器高度
#define   TILE_INDI_W         7                           //磁贴大标题指示器宽度
#define   TILE_INDI_S         36                          //磁贴大标题指示器上边距
struct 
{
  float   title_y_calc = TILE_INDI_S + (TILE_INDI_H - TILE_B_TITLE_H) / 2 + TILE_B_TITLE_H * 2;
  float   title_y_trg_calc = TILE_INDI_S + (TILE_INDI_H - TILE_B_TITLE_H) / 2 + TILE_B_TITLE_H;
  int16_t temp;
  bool    select_flag;
  float   icon_x;
  float   icon_x_trg;
  float   icon_y;
  float   icon_y_trg;
  float   indi_x; 
  float   indi_x_trg;
  float   title_y;
  float   title_y_trg;
} tile;

//列表变量
//默认参数

#define   LIST_FONT           u8g2_font_HelvetiPixel_tr   //列表字体
//#define   LIST_FONT           u8g2_font_wqy12_t_gb2312   //列表字体
#define   LIST_TEXT_H         8                           //列表每行文字字体的高度
#define   LIST_LINE_H         16                          //列表单行高度
#define   LIST_TEXT_S         4                           //列表每行文字的上边距，左边距和右边距，下边距由它和字体高度和行高度决定
#define   LIST_BAR_W          5                           //列表进度条宽度，需要是奇数，因为正中间有1像素宽度的线
#define   LIST_BOX_R          0.5                         //列表选择框圆角

/*
//超窄行高度测试
#define   LIST_FONT           u8g2_font_4x6_tr            //列表字体
#define   LIST_TEXT_H         5                           //列表每行文字字体的高度
#define   LIST_LINE_H         7                           //列表单行高度
#define   LIST_TEXT_S         1                           //列表每行文字的上边距，左边距和右边距，下边距由它和字体高度和行高度决定
#define   LIST_BAR_W          7                           //列表进度条宽度，需要是奇数，因为正中间有1像素宽度的线
#define   LIST_BOX_R          0.5                         //列表选择框圆角
*/
struct
{
  uint8_t line_n = DISP_H / LIST_LINE_H;
  int16_t temp;
  bool    loop;
  float   y;
  float   y_trg;
  float   box_x;
  float   box_x_trg;
  float   box_y;
  float   box_y_trg[UI_DEPTH];
  float   bar_y;
  float   bar_y_trg;
} list;


//曲线相关
#define   WAVE_SAMPLE         20                          //采集倍数
#define   WAVE_W              94                          //波形宽度
#define   WAVE_L              24                          //波形左边距
#define   WAVE_U              0                           //波形上边距
#define   WAVE_MAX            27                          //最大值
#define   WAVE_MIN            4                           //最小值
#define   WAVE_BOX_H          32                          //波形边框高度
#define   WAVE_BOX_W          94                          //波形边框宽度
#define   WAVE_BOX_L_S        24                          //波形边框左边距
//列表和文字背景框相关
#define   VOLT_FONT           u8g2_font_helvB18_tr        //电压数字字体
#define   VOLT_TEXT_BG_L_S    24                          //文字背景框左边距
#define   VOLT_TEXT_BG_W      94                          //文字背景框宽度
#define   VOLT_TEXT_BG_H      29                          //文字背景框高度
struct
{
  int     ch0_adc[WAVE_SAMPLE * WAVE_W];
  int     ch0_wave[WAVE_W];
  int     val;
  float   text_bg_r; 
  float   text_bg_r_trg; 
} volt;


//选择框变量

//默认参数
#define   CHECK_BOX_L_S       95                          //选择框在每行的左边距
#define   CHECK_BOX_U_S       2                           //选择框在每行的上边距
#define   CHECK_BOX_F_W       12                          //选择框外框宽度
#define   CHECK_BOX_F_H       12                          //选择框外框高度
#define   CHECK_BOX_D_S       2                           //选择框里面的点距离外框的边距

struct
{
  uint8_t *v;
  uint8_t *m;
  uint8_t *s;
  uint8_t *s_p;
} check_box;

//弹窗变量
#define   WIN_FONT            u8g2_font_HelvetiPixel_tr   //弹窗字体
#define   WIN_H               32                          //弹窗高度
#define   WIN_W               102                         //弹窗宽度
#define   WIN_BAR_W           92                          //弹窗进度条宽度
#define   WIN_BAR_H           7                           //弹窗进度条高度
#define   WIN_Y               - WIN_H - 2                 //弹窗竖直方向出场起始位置
#define   WIN_Y_TRG           - WIN_H - 2                 //弹窗竖直方向退场终止位置
struct
{
  // uint8_t
  uint8_t *value;
  uint8_t max;
  uint8_t min;
  uint8_t step;

  MenuItem *bg;   
  uint8_t index;
  const char *title;
  uint8_t select;
  uint8_t l = (DISP_W - WIN_W) / 2;
  uint8_t u = (DISP_H - WIN_H) / 2;
  float bar;
  float bar_trg;
  float y;
  float y_trg;
} win;

//聚光灯变量
struct
{
  float   l; 
  float   l_trg; 
  float   r; 
  float   r_trg; 
  float   u; 
  float   u_trg; 
  float   d; 
  float   d_trg; 
} spot;



#endif // _CONFIG_H_