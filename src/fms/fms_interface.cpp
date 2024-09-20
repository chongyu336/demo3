#include "fms_interface.h"

extern RC_input_t rc_input;
extern GCS_input_t gcs_input;
extern INS_Bus ins_bus;
extern bool rc_updated;
extern bool gcs_updated;

FMS_Bus fms_bus;


TimeTag fms_interval ={
    .tag = 0,
    .period = 100,
};


#define constrain_float(val, min_val, max_val) (val < min_val ? min_val : (val > max_val ? max_val : val))

static float norm_input(int16_t radio_in)
{
    float ret;
    if (radio_in < JOYSTICK_READ_NEUTRAL) {
        if (JOYSTICK_READ_MIN >= JOYSTICK_READ_NEUTRAL) {
            return 0.0f;
        }
        ret =(radio_in - JOYSTICK_READ_NEUTRAL) / (JOYSTICK_READ_NEUTRAL - JOYSTICK_READ_MIN);
    } else {
        if (JOYSTICK_READ_MAX <= JOYSTICK_READ_NEUTRAL) {
            return 0.0f;
        }
        ret = (radio_in - JOYSTICK_READ_NEUTRAL) / (JOYSTICK_READ_MAX  - JOYSTICK_READ_NEUTRAL);
    }
    return constrain_float(ret, -1.0f, 1.0f);
}

static float Limit(float pwm, float min, float max)
{
    return pwm < min ? min : (pwm > max ? max : pwm);
}

static float absself(float num)
{
    if(num > 0)
    {
        return num;
    }
    else{
        return -num;
    }
}
static float deadzone_range(float num1, float num2)
{
    if( absself(num1 - JOYSTICK_READ_NEUTRAL ) < num2)
    {
        return 0;
    }
    else{
        return num1;
    }
}

//在这一步将遥控器或者GCS转换，遥控器in , 期望out
static void FMS_step(void);


void fms_interface_init()
{
  //初始化遥控器和上位机控制
  rc_control_init();
  //待实现
  gcs_control_init();
}

uint32_t last_timestamp;
void fms_interface_step(uint32_t timestamp)
{
  //遥控器的优先级高
  if (rc_updated)
  {
    fms_bus.fms_rc_input = &rc_input;
    fms_bus.fms_rc_input->timestamp = timestamp;
  }
  else
  {
    fms_bus.fms_gcs_input = &gcs_input;
    fms_bus.fms_gcs_input->timestamp = timestamp;
  }
  fms_bus.fms_ins_bus = &ins_bus;


  FMS_step();
  

}

mode_list last_mode;
float depth_desired;

//在这一步将遥控器杆位转换，遥控器in , 期望out
static void FMS_step(void)
{
  if (rc_updated)
  {
    if(fms_bus.fms_rc_input->sw1  == RC_SW_UP)
    {
      fms_bus.fms_out.status = Arm;
    }
    else
    {
      fms_bus.fms_out.status = Disarm;
    }
    //模式判断
    //待实现
    if(fms_bus.fms_rc_input->sw2 == )
    {
      fms_bus.fms_out.mode = 
    }
    else if(fms_bus.fms_rc_input->sw3 == )
    {

    }

    float rollstick = deadzone_range(fms_bus.fms_rc_input->ch1, 50);
    float pitchstick = deadzone_range(fms_bus.fms_rc_input->ch2,50);
    float throttlestick = deadzone_range(fms_bus.fms_rc_input->ch3,50);
    float yawstick = deadzone_range(fms_bus.fms_rc_input->ch4,50);
    float forwardstick = deadzone_range(fms_bus.fms_rc_input->ch5,50);
    float lateralstick = deadzone_range(fms_bus.fms_rc_input->ch6,50);    

    //摄像头舵机控制
    fms_bus.fms_out.servo_cmd = ;

    
    //杆位输入，-1.0-1.0
    float roll_desired = norm_input(rollstick)*Roll_Max;
    float pitch_desired = norm_input(pitchstick)*Pitch_Max;
    float yaw_rate_desired = norm_input(yawstick)*Yaw_Rate_Max;
    float forwardout = norm_input(forwardstick) *Speed_x_Max;
    float lateralout = norm_input(lateralstick)*Speed_y_Max;

    uint32_t dt_ctrl = (fms_bus.fms_rc_input->timestamp >= last_timestamp) ? (fms_bus.fms_rc_input->timestamp - last_timestamp) : (0xFFFFFFFF - last_timestamp + fms_bus.fms_rc_input->timestamp);
    last_timestamp = fms_bus.fms_rc_input->timestamp;
    depth_desired += norm_input(throttlestick)*Speed_z_Max * dt_ctrl;

    //传感器角度数据
    float roll_feedback = fms_bus.fms_ins_bus->imu->ang_roll;
    float pitch_feedback = fms_bus.fms_ins_bus->imu->ang_pitch;
    float yaw_feedback = fms_bus.fms_ins_bus->imu->ang_yaw;

    //传感器角速度数据
    float roll_rate_feedback = fms_bus.fms_ins_bus->imu->gyr_x;
    float pitch_rate_feedback = fms_bus.fms_ins_bus->imu->gyr_y;
    float yaw_rate_feedback = fms_bus.fms_ins_bus->imu->gyr_z;

    // if(yaw_feedback > 180)
    // {
    //     yaw_feedback = yaw_feedback - 360;
    //     yaw_rate_feedback = - yaw_rate_feedback;
    // }

    //深度反馈
    float depth_feedback = fms_bus.fms_ins_bus->bar->depth;
    


    mode_list cur_mode = fms_bus.fms_out.mode;
    
    if ((last_mode == Stabilize || last_mode == Depthhold) && cur_mode == Atthold)
    {
        fms_bus.fms_out.phi_cmd = roll_feedback;
        fms_bus.fms_out.theta_cmd = pitch_feedback;
        fms_bus.fms_out.psi_cmd = yaw_feedback;
        fms_bus.fms_out.z_cmd = depth_desired;
    }
    else if ((last_mode == Stabilize || last_mode == Atthold) && cur_mode == Depthhold)
    {
        fms_bus.fms_out.phi_cmd = roll_desired;
        fms_bus.fms_out.theta_cmd = pitch_desired;
        fms_bus.fms_out.r_cmd = yaw_rate_desired;
        fms_bus.fms_out.z_cmd = depth_feedback;
    }
    else if(last_mode == Atthold && cur_mode == Atthold)
    {
        fms_bus.fms_out.z_cmd = depth_desired;
    }
    else if(last_mode == Depthhold && cur_mode == Depthhold)
    {
        fms_bus.fms_out.phi_cmd = roll_desired;
        fms_bus.fms_out.theta_cmd = pitch_desired;
        fms_bus.fms_out.r_cmd = yaw_rate_desired;
    }
    else{
        fms_bus.fms_out.phi_cmd = roll_desired;
        fms_bus.fms_out.theta_cmd = pitch_desired;
        fms_bus.fms_out.r_cmd = yaw_rate_desired;
        fms_bus.fms_out.z_cmd = depth_desired;
    }

    fms_bus.fms_out.u_cmd = forwardout;
    fms_bus.fms_out.v_cmd = lateralout;

  }
  elif (gcs_updated)
  {
    

  }

  
}