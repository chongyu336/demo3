/*
 * @Author: ADSW514610085 Z514610085@163.com
 * @Date: 2024-09-20 17:16:54
 * @LastEditors: ADSW514610085 Z514610085@163.com
 * @LastEditTime: 2024-09-20 17:18:42
 * @FilePath: \demo3\src\control\rc_control.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "rc_control.h"
#include "systim.h"

bool rc_updated;
RC_input_t rc_input;

TimeTag rc_interval = {
    .tag = 0,
    .period = 50,
};

void rc_control_init(void)
{
    rc_updated = false;
}
//将PPM转成rc的输入信号
//待实现
static void ppm_to_rc(void)
{
    rc_input.ch1 = ;
    //todo
    //todo
    //todo
}

void pilot_cmd_collect()
{
    rc_updated = false;
    if (check_timetag(&rc_interval)) 
    {
        //待实现
        ppm_to_rc();
        rc_updated = true;
    }
}