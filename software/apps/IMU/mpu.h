#ifndef MPU_H
#define MPU_H

// struct s_mympu {
// 	//float ypr[3];
// 	//float gyro[3];
// 	unsigned long steps[1];
// };

//extern struct s_mympu mympu;

extern unsigned long steps[1];

int mympu_open(unsigned int rate);
int mympu_update();
void tap_detect(unsigned char direction, unsigned char count);
#endif

