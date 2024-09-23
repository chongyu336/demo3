/*
 * @Author: ADSW514610085 Z514610085@163.com
 * @Date: 2024-09-20 14:15:33
 * @LastEditors: ADSW514610085 Z514610085@163.com
 * @LastEditTime: 2024-09-23 10:51:02
 * @FilePath: \demo3\src\ins_interface\systim.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef SYSTIM_H
#define SYSTIM_H
#include "stm32f4xx_hal.h"

typedef struct {
    uint32_t tag;
    uint32_t period;
} TimeTag;

uint32_t systime_now_ms();


uint8_t check_timetag(TimeTag* timetag);
#endif