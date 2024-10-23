//#include <something.h>
#include "stm32f4xx_hal.h" //不会出现数字类型定义错误
//static struct rtos_event event_vehicle;
//static struct rtos_timer timer_vehicle;
#include "ins/ins_interface.h"
#include "fms/fms_interface.h"
#include "control/control_interface.h"
#include "systim/systim.h"
/**
 * @brief Vehicle task init
 */
void task_vehicle_init(void)
{
    /* init ins model */
    ins_interface_init();

    /* init fms model */
    fms_interface_init();

    /* init controller model */
    control_interface_init();

    /* create event */
    //rtos_event_init(&event_vehicle, "vehicle", FLAG);
    
    /* register a 1kHz timer connected with event */
    //rtos_timer_create(&timer_vehicle, "vehicle", timer_vehicle_update, NULL, 1, TIMER_FLAG_PERIODIC);
    //rtos_timer_start(&timer_vehicle);
}

/*static void timer_vehicle_update(void* parameter)
{
   // rtos_event_send(&event_vehicle, EVENT_VEHICLE_UPDATE);
}*/
// TimeTag ins_timtag ={
//     .tag = 0,
//     .period = 100,
// };

// TimeTag fms_timtag ={
//     .tag = 0,
//     .period = 100,
// };

// TimeTag control_timtag ={
//     .tag = 0,
//     .period = 100,
// };
/**
 * @brief Vehicle entry task
 * @param parameter
 */
void task_vehicle_entry(void* parameter)
{
   uint32_t time_start = 0;
    uint32_t time_now;
   uint32_t timestamp;
    /*
    uint32_t ins_period = 100;
    uint32_t fms_period = 100;*/
    //rt_err_t res;
   // rt_uint32_t recv_set = 0;
    //uint32_t wait_set = EVENT_VEHICLE_UPDATE;

    while (1) {
        //res = rtos_event_recv(&event_vehicle, wait_set, FLAG, &recv_set);

        if (1) {
            //原条件res==0
            if (1) {
            //原条件recv_set & EVENT_VEHICLE_UPDATE        
                time_now = systime_now_ms();
                /* record loop start time */
                if (time_start == 0) {
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
                // PERIOD_EXECUTE(&ins_timtag, time_now, ins_interface_step(timestamp));
                /* run FMS model every fms_period */
                // PERIOD_EXECUTE(&fms_timtag, time_now, fms_interface_step(timestamp));
                /* run Controller model every control_period */
                // PERIOD_EXECUTE(&control_timtag, time_now, control_interface_step(timestamp));
                
                /* send actuator command */
                //send_actuator_cmd();
            }
        }
    }
}

// void sensor_collect(void)
// {
//     /* Collect imu data */
//     TODO();
    
//     /* Collect barometer data */
//     TODO();

//     /* Collect rangefinder data */
//     TODO();
// }

// void ins_interface_step(uint32_t timestamp)
// {
//     /* get sensor data */
//     TODO();

//     /* run INS model */
//     INS_step();

//     /* publish INS output */
//     TODO();
// }

// void fms_interface_step(uint32_t timestamp)
// {
//     /* get pilot command */
//     TODO();

//     /* get GCS command */
//     TODO();

//     /* get INS output */
//     TODO();

//     /* get Controller output */
//     TODO();

//     /* run FMS model */
//     FMS_step();

//     /* publish FMS output */
//     TODO();
// }

// void control_interface_step(uint32_t timestamp)
// {
//     /* get INS output */
//     TODO();

//     /* get FMS output */
//     TODO();

//     /* run Controller model */
//     Controller_step();

//     /* publish Controller output */
//     TODO();
// }