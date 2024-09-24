#ifndef FMS_INTERFACE_H
#define FMS_INTERFACE_H

#include "rc_control.h"
#include "gcs_control.h"
#include "../../include/main.h"
#include "../ins/ins_interface.h"


#define JOYSTICK_READ_MIN       1000
#define JOYSTICK_READ_MAX       2000
#define JOYSTICK_READ_NEUTRAL   1500
#define JOYSTICK_OUTPUT_MIN     -1.0f
#define JOYSTICK_OUTPUT_MAX     1.0f

#define JOYSTICK_READ_MIN1       -1.0f
#define JOYSTICK_READ_MAX1       1.0f
#define JOYSTICK_READ_NEUTRAL1   0.0f


/* ----------------------- RC Switch Definition----------------------------- */
#define RC_SW_UP                ((uint16_t)1)
#define RC_SW_MID               ((uint16_t)0)
#define RC_SW_DOWN              ((uint16_t)-1)
#define switch_is_down(s)       (s == RC_SW_DOWN)
#define switch_is_mid(s)        (s == RC_SW_MID)
#define switch_is_up(s)         (s == RC_SW_UP)


#define Speed_x_Max 0.5
#define Speed_y_Max 0.5
#define Speed_z_Max 0.5

#define Acc_z_Max 0.1

#define Roll_Max 0.34
#define Pitch_Max 0.34
#define Yaw_Max 0.785
#define Yaw_Rate_Max 2.65
#define Roll_Rate_Max 2.65
#define Pitch_Rate_Max 2.65

typedef enum
{
    Disarm = 0,
    Arm,
} status_list;

typedef enum
{       
  Stabilize = 0,  
  Atthold,
  Depthhold,
  Manual,            
} mode_list;


typedef struct {
  /* fms output timestamp */
  uint32_t timestamp;

  /* roll rate command in body frame */
  uint32_t p_cmd;

  /* pitch rate command in body frame */
  uint32_t q_cmd;

  /* yaw rate command in body frame */
  uint32_t r_cmd;

  /* roll command in body frame */
  uint32_t phi_cmd;

  /* pitch command in body frame */
  uint32_t theta_cmd;

  /* yaw command in body frame */
  uint32_t psi_cmd;

  /* velocity x command in control frame */
  uint32_t u_cmd;

  /* velocity y command in control frame */
  uint32_t v_cmd;

  /* velocity z command in control frame */
  uint32_t w_cmd;

  /* position z command in control frame */
  uint32_t z_cmd;

  /* 摄像头舵机 */
  uint32_t servo_cmd;

  status_list status;

  mode_list mode;
  /* home position [x y h yaw], unit [m m m rad] */
  // uint32_t home[4];
} FMS_Out;

typedef struct
{
  RC_input_t *fms_rc_input;
  //const GCS_input_t *fms_gcs_input;
  const INS_Bus *fms_ins_bus;
  FMS_Out fms_out;
} FMS_Bus;

void fms_interface_init();
void fms_interface_step(uint32_t timestamp);

#endif