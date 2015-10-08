/* MPU6050.h
 * Interface for the MPU-6050 intertial measurement / digital motion processor
 * unit.
 */


#ifndef _MPU6050_H
#define _MPU6050_H

#include <stdint.h>


/* Initialize the IMU.
 */
void MPU6050_init(/* TODO pass in the MCU pins and I2C handle */);


/* Enable the accelerometer.
 */
void MPU6050_accelEnable();


/* Disables the accelerometer.
 */
void MPU6050_accelDisable();


/* Enable the gyroscope.
 */
void MPU6050_gyroEnable();


/* Disable the gyroscope.
 */
void MPU6050_gyroDisable();


/* Enable the Magnetometer.
 */
void MPU6050_magEnable();


/* Disables the Magnetometer
 */
void MPU6050_magDisable();


/* Set the sample rates for the accelerometer and the gyroscope.
 */
void MPU6050_setSampleRates(uint16_t acclRate, uint16_t gyroRate);


/* Configures the FIFO Buffer.
 */
uint16_t MPU6050_configFifo();


/* Reads from the FIFO Buffer.
 */
uint16_t MPU6050_readFifo(char * buf);


/* Bit masks for DMP configuration.
 */
enum MPU6050_dmpFeatureEnum = {
    TAP = 0x001,
    ANDROID_ORIENT = 0x002,
    LP_QUAT = 0x004,
    PEDOMETER = 0x008,
    X6_LP_QUAT = 0x010,
    GYRO_CAL = 0x020,
    SEND_RAW_ACCEL = 0x040,
    SEND_RAW_GYRO = 0x080,
    SEND_CAL_GYRO = 0x100,
};


/* Enables and Configures the Digital Motion Processor.
 */
void ImuConfigureDMP(enum MPU6050_dmpFeatureEnum mask);


/* Low level read interface to the MPU6050. This is exposed to support any
 * function without a sp6ecific function in this interface.
 */
void _MPU6050_writeReg(uint8_t reg, uint8_t length, uint8_t data[]);


/* Low level read interface to the MPU6050. This is exposed to support any
 * function without a specific function in this interface.
 */
uint8_t _MPU6050_readReg(uint8_t reg);

#endif
