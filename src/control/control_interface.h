/*
 * @Author: ADSW514610085 Z514610085@163.com
 * @Date: 2024-09-20 10:55:02
 * @LastEditors: ADSW514610085 Z514610085@163.com
 * @LastEditTime: 2024-09-23 16:14:57
 * @FilePath: \demo3\src\control\control_interface.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef CONTROL_INTERFACE_H
#define CONTROL_INTERFACE_H
#include "../fms/fms_interface.h"
#include "pid.h"
#include "main.h"
#include "../ins/ins_interface.h"

/*typedef enum
{
  Disarm = 0,           
  Arm,              
} fms_status;
重定义了
typedef enum
{
  Stabilize = 0,         
  Atthold,
  Depthhold,            
} fms_mode;
重定义了
*/

typedef struct {
    PID_CLASS roll_pid;
    PID_CLASS pitch_pid;
    PID_CLASS yaw_pid;
    PID_CLASS roll_rate_pid;
    PID_CLASS pitch_rate_pid;
    PID_CLASS yaw_rate_pid;
    float   roll_thrust;                // roll thrust input value, +/- 1.0
    float   pitch_thrust;               // pitch thrust input value, +/- 1.0
    float   yaw_thrust;                 // yaw thrust input value, +/- 1.0
} attitude_controller_t;

typedef struct {
    PID_CLASS depth_pid;
    PID_CLASS depth_vel_pid;
    float   throttle_thrust;            // throttle thrust input value, +/- 1.0
    float   forward_thrust;             // forward thrust input value, +/- 1.0
    float   lateral_thrust;             // lateral thrust input value, +/- 1.0
} depth_controller_t;

typedef struct
{
  INS_Bus *ctrl_ins_bus;
  FMS_Bus *ctrl_fms_bus;
  attitude_controller_t att_controller;
  depth_controller_t depth_controller;
} CTRL_Bus;




void control_interface_init(void);

void control_interface_step(uint32_t timestamp);

#endif