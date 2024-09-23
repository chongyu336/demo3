/*
 * @Author: ADSW514610085 Z514610085@163.com
 * @Date: 2024-09-20 14:15:33
 * @LastEditors: ADSW514610085 Z514610085@163.com
 * @LastEditTime: 2024-09-23 11:36:11
 * @FilePath: \demo3\src\ins_interface\systim.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "systim.h"


uint8_t check_timetag3(TimeTag* timetag, uint32_t now, uint32_t period)
{
    if (period > 0 && now - timetag->tag >= period) {
        timetag->tag = now;
        return 1;
    }
    return 0;
}

uint32_t systime_now_ms()
{
    return 0;
}


uint8_t check_timetag(TimeTag* timetag)
{
    uint32_t now = systime_now_ms();

    if (timetag->period > 0 && now - timetag->tag >= timetag->period) {
        timetag->tag = now;
        return 1;
    }
    return 0;
}