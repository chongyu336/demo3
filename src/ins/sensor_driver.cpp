#include "sensor_driver.h"
#include "systim.h"
#include "i2c.h"
#include "uart_imu.h"
#include "uart_init.h"

extern float pressure, temperature;
extern UartIMU uartImu;
extern UartAtkTof uartTof;
static float fluidensity;

imu_data_t imu_data;
mag_data_t mag_data;
bar_data_t bar_data;
rnf_data_t rnf_data;

TimeTag imu_interval = {
    .tag = 0,
    .period = 1,
};
TimeTag mag_interval ={
    .tag = 0,
    .period = 10,
};
TimeTag bar_interval ={
    .tag = 0,
    .period = 10,
};

TimeTag rnf_interval ={
    .tag = 0,
    .period = 10,
};

static void imu_collect();
static void mag_collect();
static void bar_collect();
static void rnf_collect();

void sensor_init()
{
    //待实现,波特率 模式设置
    //金工已实现 imu_init();
    //金工已实现 mag_init();
    //金工已实现 bar_init();
    //金工已实现 rnf_init();
    fluidensity = 1029.0f;
}

/**
 * @brief Collect sensor data
 * @note Should be invoked periodically. e.g, at 1KHz
 */
void sensor_collect()
{
    imu_collect();

    mag_collect();
    
    bar_collect();

    rnf_collect();
    
}

static void imu_collect()
{
    //float         temp[3];
    //float         temp1[3];
    if (check_timetag(&imu_interval)) //判断是否到达采样时间
    {
        imu_data.timestamp_ms = systime_now_ms();
        //待实现
        //if() //里面是读数据的功能，并判断是否读成功，注意数据的顺序
        //{
            

            imu_data.gyr_radDs[0] =   uartImu.imuHandle.gyro[0];
            imu_data.gyr_radDs[1] =   uartImu.imuHandle.gyro[1];
            imu_data.gyr_radDs[2] =   uartImu.imuHandle.gyro[2];

            imu_data.acc_mDs2[0] =    uartImu.imuHandle.acc[0];
            imu_data.acc_mDs2[1] =    uartImu.imuHandle.acc[1];
            imu_data.acc_mDs2[2] =    uartImu.imuHandle.acc[2];

            //FMT在这再使用了三阶Butterworth滤波器处理
        //}
    }
}

static void mag_collect()
{
    //float         temp[3];
    if(check_timetag(&mag_interval))
    {
        mag_data.timestamp_ms = systime_now_ms();
        //待实现
        //if()
        //{
            /************************************ */
            mag_data.mag_gauss[0] =  uartImu.imuHandle.magnet[0];
            mag_data.mag_gauss[1] =  uartImu.imuHandle.magnet[1];
            mag_data.mag_gauss[2] =  uartImu.imuHandle.magnet[2];
            /************************************ */
        //}
    }
}

static void bar_collect()
{
    float   temp[3];
    if(check_timetag(&bar_interval))
    {
        // bar_data.dt_ms = systime_now_ms() - bar_data.timestamp_ms;
        bar_data.timestamp_ms = systime_now_ms();
        //待实现
        //if()
        //{
        /************************************ */
            bar_data.temperature_deg = temperature;
            bar_data.pressure_pa = pressure/1000.0f*101325.0f;
            bar_data.depth_m = (bar_data.pressure_pa-101325.0f)/(fluidensity*9.80665f);
        /************************************ */
        //}
    }
}

static void rnf_collect()
{
    //float         temp[3];
    if(check_timetag(&rnf_interval))
    {
        rnf_data.timestamp_ms = systime_now_ms();
        //待实现
        //if()
        //{
        /************************************ */
            rnf_data.distance_m = uartTof.longestDistance;
        /************************************ */
        //}
    }
}
