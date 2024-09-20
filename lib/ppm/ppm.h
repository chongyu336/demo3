#ifndef ppm_h
#define ppm_h

#include <stm32f4xx_hal.h>

#define PPM_MAX_NUM (20)

class PPM_CLASS
{
public:
    PPM_CLASS();

public:
    void init(float pwmstep);
    void pwmCallback(bool isPeriod, uint32_t value);

public:
    float speed[PPM_MAX_NUM];
    int ppmChannels;
    // 只能读一次, 读后自动置0;
    bool IsUpdated(); 

private:
    float _ton[PPM_MAX_NUM];
    int _index;
    float _pwmStep;
    bool _isUpdated;
    uint64_t _receivedTick;

private:
    bool _isReady;
};

extern PPM_CLASS ppmInst;

#endif