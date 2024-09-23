#include "rc_control.h"
#include "systim.h"
#include "ppm.h"

bool rc_updated;
RC_input_t rc_input;
extern PPM_CLASS ppmInst;

TimeTag rc_interval = {
    .tag = 0,
    .period = 50,
};

void rc_control_init(void)
{
    ppmInst.init(1E-6F);
    rc_updated = false;
}
//将PPM转成rc的输入信号
//待实现
static void ppm_to_rc(void)
{
    rc_input.ch1 =ppmInst.speed[0];
    rc_input.ch2 =ppmInst.speed[1];
    rc_input.ch3 =ppmInst.speed[2];
    rc_input.ch4 =ppmInst.speed[3];
    rc_input.ch5 =ppmInst.speed[4];
    rc_input.ch6 =ppmInst.speed[5];
    rc_input.sw1 =ppmInst.speed[6];
    rc_input.sw2 =ppmInst.speed[7];
    rc_input.sw3 =ppmInst.speed[8];
    rc_input.sw4 =ppmInst.speed[9];
 
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