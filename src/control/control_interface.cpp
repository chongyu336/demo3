#include "control_interface.h"
#include <math.h>

#define MOTORS_MOT_1 0U
#define MOTORS_MOT_2 1U
#define MOTORS_MOT_3 2U
#define MOTORS_MOT_4 3U
#define MOTORS_MOT_5 4U
#define MOTORS_MOT_6 5U

float motor1_factor[MOTORS_MAX_NUM_MOTORS] = {0, 0, 1.0f, 0, -1.0f, 1.0f};
float motor2_factor[MOTORS_MAX_NUM_MOTORS] = {0, 0, -1.0f, 0, -1.0f, -1.0f};
float motor3_factor[MOTORS_MAX_NUM_MOTORS] = {0, 0, -1.0f, 0, 1.0f, 1.0f};
float motor4_factor[MOTORS_MAX_NUM_MOTORS] = {0, 0, 1.0f, 0, 1.0f, -1.0f};
float motor5_factor[MOTORS_MAX_NUM_MOTORS] = {1.0f, 0, 0, -1.0f, 0, 0};
float motor6_factor[MOTORS_MAX_NUM_MOTORS] = {-1.0f, 0, 0, -1.0f, 0, 0};

extern FMS_Bus fms_bus;
extern INS_Bus ins_bus;

CTRL_Bus ctrl_bus;
// controller

float motor_out[MOTORS_MAX_NUM_MOTORS];
float motor_thrust[MOTORS_MAX_NUM_MOTORS];
float thrust_max;

TimeTag ctrl_interval = {
    .tag = 0,
    .period = 100,
};

static void Manual_control();

static void roll_pitch_control(uint32_t set_roll, uint32_t set_pitch);

static void yaw_rate_control(uint32_t set_yaw_rate);

static void yaw_heading_control(uint32_t set_yaw);

static void depth_control(uint32_t set_depth);

static void depth_rate_control(uint32_t set_depth_vel);

static void forward_lateral_control(uint32_t set_forward, uint32_t set_lateral);

static void thrust_alloc();

static void thrust_to_pwm();

static void thrust_max_init();

static void thrust_zero();

static void control_step();

// Speed_x_Max 0.5
// Speed_y_Max 0.5
// Speed_z_Max 0.5

// Acc_z_Max 0.1

// Roll_Max 0.34
// Pitch_Max 0.34
// Yaw_Max 0.785
// Yaw_Rate_Max 2.65
// Roll_Rate_Max 2.65
// Pitch_Rate_Max 2.65

void control_interface_init(void)
{
    thrust_max_init();
    // 初始化姿态控制器PID
    ctrl_bus.att_controller.roll_pid.init();
    ctrl_bus.att_controller.roll_pid.isIncrement = true;
    ctrl_bus.att_controller.roll_pid.vcmax = Roll_Rate_Max;
    ctrl_bus.att_controller.roll_pid.vcmin = -Roll_Rate_Max;
    ctrl_bus.att_controller.roll_pid.kp = 0;
    ctrl_bus.att_controller.roll_pid.ki = 0;
    ctrl_bus.att_controller.roll_pid.kd = 0;
    ctrl_bus.att_controller.pitch_pid.init();
    ctrl_bus.att_controller.pitch_pid.isIncrement = true;
    ctrl_bus.att_controller.pitch_pid.vcmax = Pitch_Rate_Max;
    ctrl_bus.att_controller.pitch_pid.vcmin = -Pitch_Rate_Max;
    ctrl_bus.att_controller.pitch_pid.kp = 0;
    ctrl_bus.att_controller.pitch_pid.ki = 0;
    ctrl_bus.att_controller.pitch_pid.kd = 0;
    ctrl_bus.att_controller.yaw_pid.init();
    ctrl_bus.att_controller.yaw_pid.isIncrement = true;
    ctrl_bus.att_controller.yaw_pid.vcmax = Yaw_Rate_Max;
    ctrl_bus.att_controller.yaw_pid.vcmin = -Yaw_Rate_Max;
    ctrl_bus.att_controller.yaw_pid.kp = 0;
    ctrl_bus.att_controller.yaw_pid.ki = 0;
    ctrl_bus.att_controller.yaw_pid.kd = 0;

    ctrl_bus.att_controller.roll_rate_pid.init();
    ctrl_bus.att_controller.roll_rate_pid.isIncrement = true;
    ctrl_bus.att_controller.roll_rate_pid.vcmax = Roll_Rate_Max; // 待改动，目前按照手动模式最大值来设置 Roll Thrust Max
    ctrl_bus.att_controller.roll_rate_pid.vcmin = -Roll_Rate_Max;
    ctrl_bus.att_controller.roll_rate_pid.kp = 0;
    ctrl_bus.att_controller.roll_rate_pid.ki = 0;
    ctrl_bus.att_controller.roll_rate_pid.kd = 0;
    ctrl_bus.att_controller.pitch_rate_pid.init();
    ctrl_bus.att_controller.pitch_rate_pid.isIncrement = true;
    ctrl_bus.att_controller.pitch_rate_pid.vcmax = Pitch_Rate_Max; // 待改动，目前按照手动模式最大值来设置 Pitch Thrust Max
    ctrl_bus.att_controller.pitch_rate_pid.vcmin = -Pitch_Rate_Max;
    ctrl_bus.att_controller.pitch_rate_pid.kp = 0;
    ctrl_bus.att_controller.pitch_rate_pid.ki = 0;
    ctrl_bus.att_controller.pitch_rate_pid.kd = 0;
    ctrl_bus.att_controller.yaw_rate_pid.init();
    ctrl_bus.att_controller.yaw_rate_pid.isIncrement = true;
    ctrl_bus.att_controller.yaw_rate_pid.vcmax = Yaw_Rate_Max; // 待改动，目前按照手动模式最大值来设置 Yaw Thrust Max
    ctrl_bus.att_controller.yaw_rate_pid.vcmin = -Yaw_Rate_Max;
    ctrl_bus.att_controller.yaw_rate_pid.kp = 0;
    ctrl_bus.att_controller.yaw_rate_pid.ki = 0;
    ctrl_bus.att_controller.yaw_rate_pid.kd = 0;

    // 初始化深度控制器PID
    ctrl_bus.depth_controller.depth_pid.init();
    ctrl_bus.depth_controller.depth_pid.isIncrement = true;
    ctrl_bus.depth_controller.depth_pid.vcmax = Speed_z_Max;
    ctrl_bus.depth_controller.depth_pid.vcmin = -Speed_z_Max;
    ctrl_bus.depth_controller.depth_pid.kp = 0;
    ctrl_bus.depth_controller.depth_pid.ki = 0;
    ctrl_bus.depth_controller.depth_pid.kd = 0;
    ctrl_bus.depth_controller.depth_vel_pid.init();
    ctrl_bus.depth_controller.depth_vel_pid.isIncrement = true;
    ctrl_bus.depth_controller.depth_vel_pid.vcmax = Speed_z_Max; // 待改动，目前按照手动模式最大值来设置
    ctrl_bus.depth_controller.depth_vel_pid.vcmin = -Speed_z_Max;
    ctrl_bus.depth_controller.depth_vel_pid.kp = 0;
    ctrl_bus.depth_controller.depth_vel_pid.ki = 0;
    ctrl_bus.depth_controller.depth_vel_pid.kd = 0;

    // 初始化输出
    ctrl_bus.depth_controller.forward_thrust = ctrl_bus.depth_controller.lateral_thrust = ctrl_bus.depth_controller.throttle_thrust = ctrl_bus.att_controller.roll_thrust = ctrl_bus.att_controller.pitch_thrust = ctrl_bus.att_controller.yaw_thrust = 0.0f;
}

void control_interface_step(uint32_t timestamp)
{
    ctrl_bus.ctrl_ins_bus = &ins_bus;
    ctrl_bus.ctrl_fms_bus = &fms_bus;

    control_step();
}

static void Manual_control()
{
    ctrl_bus.depth_controller.forward_thrust = ctrl_bus.ctrl_fms_bus->fms_out.u_cmd;
    ctrl_bus.depth_controller.lateral_thrust = ctrl_bus.ctrl_fms_bus->fms_out.v_cmd;
    ctrl_bus.depth_controller.throttle_thrust = ctrl_bus.ctrl_fms_bus->fms_out.w_cmd;
    ctrl_bus.att_controller.roll_thrust = ctrl_bus.ctrl_fms_bus->fms_out.p_cmd;
    ctrl_bus.att_controller.pitch_thrust = ctrl_bus.ctrl_fms_bus->fms_out.q_cmd;
    ctrl_bus.att_controller.yaw_thrust = ctrl_bus.ctrl_fms_bus->fms_out.r_cmd;
}

static void roll_pitch_control(uint32_t set_roll, uint32_t set_pitch)
{
    ctrl_bus.att_controller.roll_pid.ref = set_roll;
    ctrl_bus.att_controller.pitch_pid.ref = set_pitch;
    ctrl_bus.att_controller.roll_rate_pid.ref = ctrl_bus.att_controller.roll_pid.calc(ctrl_bus.att_controller.roll_pid.fb);
    ctrl_bus.att_controller.pitch_rate_pid.ref = ctrl_bus.att_controller.pitch_pid.calc(ctrl_bus.att_controller.pitch_pid.fb);

    ctrl_bus.att_controller.roll_thrust = ctrl_bus.att_controller.roll_rate_pid.calc(ctrl_bus.att_controller.roll_rate_pid.fb);
    ctrl_bus.att_controller.pitch_thrust = ctrl_bus.att_controller.pitch_rate_pid.calc(ctrl_bus.att_controller.pitch_rate_pid.fb);
}

static void yaw_rate_control(uint32_t set_yaw_rate)
{
    ctrl_bus.att_controller.yaw_rate_pid.ref = set_yaw_rate;
    ctrl_bus.att_controller.yaw_thrust = ctrl_bus.att_controller.yaw_rate_pid.calc(ctrl_bus.att_controller.yaw_rate_pid.fb);
}

static void yaw_heading_control(uint32_t set_yaw)
{
    ctrl_bus.att_controller.yaw_pid.ref = set_yaw;
    ctrl_bus.att_controller.yaw_rate_pid.ref = ctrl_bus.att_controller.yaw_pid.calc(ctrl_bus.att_controller.yaw_pid.fb);
    ctrl_bus.att_controller.yaw_thrust = ctrl_bus.att_controller.yaw_rate_pid.calc(ctrl_bus.att_controller.yaw_rate_pid.fb);
}

static void depth_control(uint32_t set_depth)
{
    ctrl_bus.depth_controller.depth_pid.ref = set_depth;

    ctrl_bus.depth_controller.depth_vel_pid.ref = ctrl_bus.depth_controller.depth_pid.calc(ctrl_bus.depth_controller.depth_pid.fb);
    ctrl_bus.depth_controller.throttle_thrust = ctrl_bus.depth_controller.depth_vel_pid.calc(ctrl_bus.depth_controller.depth_vel_pid.fb);
}

static void depth_rate_control(uint32_t set_depth_vel)
{
    ctrl_bus.depth_controller.depth_vel_pid.ref = set_depth_vel;
    ctrl_bus.depth_controller.throttle_thrust = ctrl_bus.depth_controller.depth_vel_pid.calc(ctrl_bus.depth_controller.depth_vel_pid.fb);
}

static void forward_lateral_control(uint32_t set_forward, uint32_t set_lateral)
{
    ctrl_bus.depth_controller.forward_thrust = set_forward;
    ctrl_bus.depth_controller.lateral_thrust = set_lateral;
}

static void thrust_max_init()
{
    // 可能还需要和控制器输出一起转换，目前将速度的最大值作为推力的最大值
    float thrust_max_vertical = abs(Roll_Rate_Max * motor1_factor[0]) +
                                abs(Pitch_Rate_Max * motor1_factor[1]) +
                                abs(Yaw_Rate_Max * motor1_factor[2]) +
                                abs(Speed_z_Max * motor1_factor[3]) +
                                abs(Speed_x_Max * motor1_factor[4]) +
                                abs(Speed_y_Max * motor1_factor[5]);

    float thrust_max_horizontal = abs(Roll_Rate_Max * motor4_factor[0]) +
                                  abs(Pitch_Rate_Max * motor4_factor[1]) +
                                  abs(Yaw_Rate_Max * motor4_factor[2]) +
                                  abs(Speed_z_Max * motor4_factor[3]) +
                                  abs(Speed_x_Max * motor4_factor[4]) +
                                  abs(Speed_y_Max * motor4_factor[5]);

    thrust_max = MAX(thrust_max_horizontal, thrust_max_vertical);
}

static void thrust_alloc()
{
    motor_thrust[MOTORS_MOT_1] = ctrl_bus.att_controller.roll_thrust * motor1_factor[0] +
                                 ctrl_bus.att_controller.pitch_thrust * motor1_factor[1] +
                                 ctrl_bus.att_controller.yaw_thrust * motor1_factor[2] +
                                 ctrl_bus.depth_controller.throttle_thrust * motor1_factor[3] +
                                 ctrl_bus.depth_controller.forward_thrust * motor1_factor[4] +
                                 ctrl_bus.depth_controller.lateral_thrust * motor1_factor[5];

    motor_thrust[MOTORS_MOT_2] = ctrl_bus.att_controller.roll_thrust * motor2_factor[0] +
                                 ctrl_bus.att_controller.pitch_thrust * motor2_factor[1] +
                                 ctrl_bus.att_controller.yaw_thrust * motor2_factor[2] +
                                 ctrl_bus.depth_controller.throttle_thrust * motor2_factor[3] +
                                 ctrl_bus.depth_controller.forward_thrust * motor2_factor[4] +
                                 ctrl_bus.depth_controller.lateral_thrust * motor2_factor[5];

    motor_thrust[MOTORS_MOT_3] = ctrl_bus.att_controller.roll_thrust * motor3_factor[0] +
                                 ctrl_bus.att_controller.pitch_thrust * motor3_factor[1] +
                                 ctrl_bus.att_controller.yaw_thrust * motor3_factor[2] +
                                 ctrl_bus.depth_controller.throttle_thrust * motor3_factor[3] +
                                 ctrl_bus.depth_controller.forward_thrust * motor3_factor[4] +
                                 ctrl_bus.depth_controller.lateral_thrust * motor3_factor[5];

    motor_thrust[MOTORS_MOT_4] = ctrl_bus.att_controller.roll_thrust * motor4_factor[0] +
                                 ctrl_bus.att_controller.pitch_thrust * motor4_factor[1] +
                                 ctrl_bus.att_controller.yaw_thrust * motor4_factor[2] +
                                 ctrl_bus.depth_controller.throttle_thrust * motor4_factor[3] +
                                 ctrl_bus.depth_controller.forward_thrust * motor4_factor[4] +
                                 ctrl_bus.depth_controller.lateral_thrust * motor4_factor[5];

    motor_thrust[MOTORS_MOT_5] = ctrl_bus.att_controller.roll_thrust * motor5_factor[0] +
                                 ctrl_bus.att_controller.pitch_thrust * motor5_factor[1] +
                                 ctrl_bus.att_controller.yaw_thrust * motor5_factor[2] +
                                 ctrl_bus.depth_controller.throttle_thrust * motor5_factor[3] +
                                 ctrl_bus.depth_controller.forward_thrust * motor5_factor[4] +
                                 ctrl_bus.depth_controller.lateral_thrust * motor5_factor[5];

    motor_thrust[MOTORS_MOT_6] = ctrl_bus.att_controller.roll_thrust * motor6_factor[0] +
                                 ctrl_bus.att_controller.pitch_thrust * motor6_factor[1] +
                                 ctrl_bus.att_controller.yaw_thrust * motor6_factor[2] +
                                 ctrl_bus.depth_controller.throttle_thrust * motor6_factor[3] +
                                 ctrl_bus.depth_controller.forward_thrust * motor6_factor[4] +
                                 ctrl_bus.depth_controller.lateral_thrust * motor6_factor[5];
    // PWM输出
    thrust_to_pwm();
}

static void thrust_to_pwm() // 转速和电压近似为线性关系，推力与转速近似为二次方关系，即推力与电压的二次方成正比关系
{
    for (int i = 0; i < MOTORS_MAX_NUM_MOTORS; i++)
    {
        if (motor_out[i] >= 0)
        {
            motor_out[i] = sqrt(motor_thrust[i] / thrust_max);
        }
        else
        {
            motor_out[i] = -sqrt(-motor_thrust[i] / thrust_max);
        }
    }
}

static void thrust_zero()
{
    for (int i = 0; i < MOTORS_MAX_NUM_MOTORS; i++)
    {
        motor_out[i] = 0.0f;
    }
}

static void control_step()
{
    if (ctrl_bus.ctrl_fms_bus->fms_out.status == Arm)
    {
        if (ctrl_bus.ctrl_fms_bus->fms_out.mode == Manual)
        {
            Manual_control();
            thrust_alloc();
        }
        else
        {
            ctrl_bus.att_controller.roll_pid.fb = ctrl_bus.ctrl_ins_bus->imu->ang_roll;
            ctrl_bus.att_controller.pitch_pid.fb = ctrl_bus.ctrl_ins_bus->imu->ang_pitch;
            ctrl_bus.att_controller.yaw_pid.fb = ctrl_bus.ctrl_ins_bus->imu->ang_yaw;

            ctrl_bus.att_controller.roll_rate_pid.fb = ctrl_bus.ctrl_ins_bus->imu->gyr_x;
            ctrl_bus.att_controller.pitch_rate_pid.fb = ctrl_bus.ctrl_ins_bus->imu->gyr_y;
            ctrl_bus.att_controller.yaw_rate_pid.fb = ctrl_bus.ctrl_ins_bus->imu->gyr_z;

            ctrl_bus.depth_controller.depth_pid.fb = ctrl_bus.ctrl_ins_bus->bar->depth;
            ctrl_bus.depth_controller.depth_vel_pid.fb = ctrl_bus.ctrl_ins_bus->bar->depvel;

            roll_pitch_control(ctrl_bus.ctrl_fms_bus->fms_out.phi_cmd, ctrl_bus.ctrl_fms_bus->fms_out.theta_cmd);
            yaw_rate_control(ctrl_bus.ctrl_fms_bus->fms_out.r_cmd);
            depth_control(ctrl_bus.ctrl_fms_bus->fms_out.z_cmd);
            forward_lateral_control(ctrl_bus.ctrl_fms_bus->fms_out.u_cmd, ctrl_bus.ctrl_fms_bus->fms_out.v_cmd);
            thrust_alloc();
        }
    }
    else
    {
        thrust_zero();
    }
}