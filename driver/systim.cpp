#include "systim.h"


uint8_t check_timetag3(TimeTag* timetag, uint32_t now, uint32_t period)
{
    if (period > 0 && now - timetag->tag >= period) {
        timetag->tag = now;
        return 1;
    }
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
