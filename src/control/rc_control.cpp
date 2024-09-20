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