#ifndef RC_CONTROL_H
#define RC_CONTROL_H

typedef struct
{
    uint32_t timestamp;
    int16_t ch1;   //右侧左右 roll
    int16_t ch2;   //右侧上下 pitch  
    int16_t ch3;   //左侧上下 油门
    int16_t ch4;   //左侧左右 yaw
   
    int16_t ch5;   //前后移动
    int16_t ch6;   //左右移动

    uint8_t sw1;   //SWA，二档
    uint8_t sw2;   //SWB，二档
    uint8_t sw3;   //SWC，三档
    uint8_t sw4;   //SWD，二档
} RC_input_t;



void rc_control_init(void);

void pilot_cmd_collect();

static void ppm_to_rc(void);
#endif