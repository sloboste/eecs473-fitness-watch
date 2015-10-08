class Imu {
public:
	Imu();
	~Imu();

    // Initializes the IMU
    int ImuInit();

    // I2C Reading and Writing
    int ImuI2CRead(unsigned char slave_addr, unsigned char reg_addr, 
                   unsigned char length, unsigned char * data);
    int ImuI2CWrite(unsigned char slave_addr, unsigned char reg_addr, 
                   unsigned char length, unsigned char * data);

    // Set the sample rates for the accelerameter and the gyroscope
    int ImuSetSampleRates(unsigned short acclRate, unsigned short gyroRate);

    // Configures the FIFO Buffer
    int ImuConfigureFIFO();

    // Reads from the FIFO Buffer
    int ImuReadFifo(char * buf);

    // Enables and Configures the Digital Motion Processor
    int ImuConfigureDMP(unsigned char mask);
    #define DMP_FEATURE_TAP (0x001)
    #define DMP_FEATURE_ANDROID_ORIENT (0x002)
    #define DMP_FEATURE_LP_QUAT (0x004)
    #define DMP_FEATURE_PEDOMETER (0x008)
    #define DMP_FEATURE_6X_LP_QUAT (0x010)
    #define DMP_FEATURE_GYRO_CAL (0x020)
    #define DMP_FEATURE_SEND_RAW_ACCEL (0x040)
    #define DMP_FEATURE_SEND_RAW_GYRO (0x080)
    #define DMP_FEATURE_SEND_CAL_GYRO (0x100)

    // Enables the Accelerameter
    int ImuEnableAccelerameter();
    
    // Disables the Accelerameter
    int ImuDisableAccelerameter();

    // Enables the Gyro
    int ImuEnableGyro();
    
    // Disables the Gyro
    int ImuDisableGyro();

    // Enables the Magnetometer
    int ImuEnableMagnetometer();
    
    // Disables the Magnetometer
    int ImuDisableMagnetometer();

};