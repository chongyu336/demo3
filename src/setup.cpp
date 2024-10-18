/************************************************************
 * FileName:        Enable Management.cpp
 * Description:     所有开关的使能控制
 *                  STM32
 * Auther:          Jinsheng
 * CreateDate:      2021-07-12
 * ModifyDate:
 * Company:         Haivoo
 * Contact:         sales@haivoo.com
 *                  www.haivoo.com
 * **********************************************************/

#include "setup.h"
#include "main.h"
#include "signalAcquisition.h"
#include "stm32f4xx_hal.h"
#include "ins/ins_interface.h"
#include "fms/fms_interface.h"
#include "control/control_interface.h"

#include "stm32f4xx_hal.h"
#include "ins/ins_interface.h"
#include "fms/fms_interface.h"
#include "control/control_interface.h"

// ------------------------------------------------------------------------------------
uint64_t adcTick = 0;
sAdcVal adcVal;
// 如果需要adc中断处理则使用该函数
static void adc_callback(sAdcVal *val)
{
    adcVal = *val;
    adcTick++;
}

extern float motor_out[MOTORS_MAX_NUM_MOTORS];



TimeTag ins_timtag ={
    .tag = 0,
    .period = 100,
};

TimeTag fms_timtag ={
    .tag = 0,
    .period = 100,
};

TimeTag control_timtag ={
    .tag = 0,
    .period = 100,
};

void vThreadSetup(void *pvParameters)
{
    // uint8_t state = 0;
    char info[256];
    UNUSED(info);

    delay(10); // wait for uart ready
    usb_init();

    adc_received_callback = adc_callback;
    ADC_init();

    can_init();

    uart_init();

    // 建立I2C任务，在I2C任务里进行 验证
    osThreadDef(i2ctask, vThreadI2c, osPriority::osPriorityLow, 0, 1024);
    osThreadCreate(osThread(i2ctask), NULL);

    tim_init();
    pwm_caputure_init();

    W25QXX_Init();
    // W25QXX_test_write(info);

    SignalAquisition::init((CommunicationProtocol **)&userComm);
    osThreadDef(dataAquisition, vThreadDataAquisition, osPriority::osPriorityHigh, 0, 512);
    dataAquisition_thread_id = osThreadCreate(osThread(dataAquisition), NULL);
    delay(10);

    // sprintf(info, "starting acquisition system...\n");
    // userComm->send(info);
    // vTaskDelay(10);

    // pwm_shutdown();

    // sampler.powerEnSet(10, 1); // 52V 电源常开

    // pwm_speed_set(0, 0.0f);
    //  pwm_speed_set(1, 0.5f);
    //  pwm_speed_set(2, 1.0f);
    //  pwm_speed_set(3, -0.5f);
    // pwm_speed_set(4, -1.0f);

    //初始化
    osDelay(10000);
    pwm_speed_set(0, 0);
    pwm_speed_set(1, 0);
    pwm_speed_set(2, 0);
    pwm_speed_set(3, 0);
    pwm_speed_set(4, 0);
    pwm_speed_set(5, 0);
    
    

    uint32_t time_start = 0;
    uint32_t time_now;
    uint32_t timestamp;
    task_vehicle_init();
    while (1)
    {
        time_now = systime_now_ms();
        /* record loop start time */
        if (time_start == 0)
        {
            time_start = time_now;
        }
        /* the model simulation start from 0, so we calcualtet the timestamp relative to start time */
        timestamp = time_now - time_start;

        /* collect sensor data */
        sensor_collect();

        /* collect RC command */
        pilot_cmd_collect();

        /* collect GCS command */
        gcs_cmd_collect();

        // motor_out[]
        // pwm_speed_set(1, 0.1);

        time_now = systime_now_ms();
        /* record loop start time */
        if (time_start == 0)
        {
            time_start = time_now;
        }
        /* the model simulation start from 0, so we calcualtet the timestamp relative to start time */
        timestamp = time_now - time_start;

        /* collect sensor data */
        sensor_collect();

        /* collect RC command */
        pilot_cmd_collect();

        /* collect GCS command */
        gcs_cmd_collect();
        
         /* run INS model every ins_period */
        PERIOD_EXECUTE(&ins_timtag, time_now, ins_interface_step(timestamp));
         /* run FMS model every fms_period */
        PERIOD_EXECUTE(&fms_timtag, time_now, fms_interface_step(timestamp));
        /* run Controller model every control_period */
        PERIOD_EXECUTE(&control_timtag, time_now, control_interface_step(timestamp));
                
        pwm_speed_set(0,motor_out[0]);
        pwm_speed_set(1,motor_out[1]);
        pwm_speed_set(2,motor_out[2]);
        pwm_speed_set(3,motor_out[3]);
        pwm_speed_set(4,motor_out[4]);
        pwm_speed_set(5,motor_out[5]);
        
        {
            // adc
            // adc_softTrigger();

            // can
            // can_test(0); // id可以选0或者1

            // 电调
            // float duty = PWM_duty_get(3);
            // sprintf(info, "duty = %0.1f\r\n", duty);
            // usbComm.send(info);

            // 激光测距仪
            sprintf(info, "tof longest distance = %0.3fm\r\n", uartTof.longestDistance[0]);
            usbComm.send(info);

            // 水压计
            // sprintf(info, "water pressure : %0.1fmbar, temperature : %0.1fC\r\n", pressure, temperature);
            // usbComm.send(info);

            // ppm遥控器
            // if (ppmInst.IsUpdated())
            // {
            //     sprintf(info, "ppm channels=%d, value:[%0.1f, %0.1f, %0.1f, %0.1f, %0.1f, %0.1f, %0.1f, %0.1f, %0.1f, %0.1f]\r\n", ppmInst.ppmChannels,
            //             ppmInst.speed[0], ppmInst.speed[1], ppmInst.speed[2], ppmInst.speed[3], ppmInst.speed[4],
            //             ppmInst.speed[5], ppmInst.speed[6], ppmInst.speed[7], ppmInst.speed[8], ppmInst.speed[9]);
            //     usbComm.send(info);
            // }

            // flash
            // W25QXX_test_read(info);
            // usbComm.send(info);

            // imu
            // sprintf(info, "imu zitai : [%0.3f, %0.3f, %0.3f]\r\n", uartImu.imuHandle.zitai[0], uartImu.imuHandle.zitai[1], uartImu.imuHandle.zitai[2]);
            // usbComm.send(info);

            // HAL_GetTick();

            osDelay(2000);

            // ADC_DMA_callback();
        }
    }

    // 查看任务stack是否溢出
    // sprintf(info, "heap remainsize %d, minimum freesize %d, stack remainsize %d\n",
    //        (int)xPortGetFreeHeapSize(), (int)xPortGetMinimumEverFreeHeapSize(), (int)uxTaskGetStackHighWaterMark(NULL));
    // sprintf(info, "led stack remainsize %d, uart stack remainsize %d, stack remainsize %d\n",
    //        (int)uxTaskGetStackHighWaterMark(led_thread_id), (int)uxTaskGetStackHighWaterMark(uart_thread_id), (int)uxTaskGetStackHighWaterMark(NULL));
}

// uint32_t systime_now_ms()
//{

//}
