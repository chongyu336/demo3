#ifndef CONTROL_INTERFACE_H
#define CONTROL_INTERFACE_H
#include "fms_interface.h"
#include "pid.h"
#include "main.h"
#include "ins_interface.h"
typedef enum
{
  Disarm = 0,           
  Arm,              
} fms_status;

typedef enum
{
  Stabilize = 0,         
  Atthold,
  Depthhold,            
} fms_mode;

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


static void roll_pitch_control(uint32_t set_roll, uint32_t set_pitch);

static void yaw_rate_control(uint32_t set_yaw_rate);

static void yaw_heading_control(uint32_t set_yaw);

static void depth_control(uint32_t set_depth);

static void depth_rate_control(uint32_t set_depth_vel);

static void forward_lateral_control(uint32_t set_forward, uint32_t set_lateral);

static void thrust_alloc();

static void thrust_zero();

static void control_step();

void control_interface_init(void);

void control_interface_step(uint32_t timestamp);

#endif