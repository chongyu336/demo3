/*
 * @Author: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2024-09-20 10:55:02
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2024-09-20 14:31:54
 * @FilePath: \demo3\src\control\rc_control.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef RC_CONTROL_H
#define RC_CONTROL_H

typedef struct
{
    uint32_t timestamp;
    float ch1;   //右侧左右 roll
    float ch2;   //右侧上下 pitch  
    float ch3;   //左侧上下 油门
    float ch4;   //左侧左右 yaw
   
    float ch5;   //前后移动
    float ch6;   //左右移动

    float sw1;   //SWA，二档
    float sw2;   //SWB，二档
    float sw3;   //SWC，三档
    float sw4;   //SWD，二档
} RC_input_t;



void rc_control_init(void);

void pilot_cmd_collect();

static void ppm_to_rc(void);
#endif