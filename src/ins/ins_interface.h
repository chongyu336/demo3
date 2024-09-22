#ifndef INS_INTERFACE_H
#define INS_INTERFACE_H


#include "sensor_driver.h"
#include "systim.h"
#include "uart_imu.h"

typedef struct {
  uint32_t timestamp;
  float gyr_x;
  float gyr_y;
  float gyr_z;
  float acc_x;
  float acc_y;
  float acc_z;
  float ang_roll;
  float ang_pitch;
  float ang_yaw;
  float quat[4];
} IMU_Bus;

typedef struct {
  uint32_t timestamp;
  float mag_x;
  float mag_y;
  float mag_z;
} MAG_Bus;

typedef struct {
  uint32_t timestamp;
  float depth;
  float pressure;
  float temperature;
  float depvel;
} Bar_Bus;

typedef struct {
  uint32_t timestamp;
  float distance;
} Rnf_Bus;

typedef struct {
    uint32_t timestamp;
    const IMU_Bus *imu;
    const MAG_Bus *mag;
    const Bar_Bus *bar;
    const Rnf_Bus *rnf;
} INS_Bus;

void ins_interface_init();
void ins_interface_step(uint32_t timestamp);

static void imu_update(uint32_t timestamp);

static void mag_update(uint32_t timestamp);

static void bar_update(uint32_t timestamp);

static void rnf_update(uint32_t timestamp);

static void ins_step();
#endif // INS_INTERFACE_H