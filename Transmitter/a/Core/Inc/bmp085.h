#ifndef _BMP085_H
#define _BMP085_H

#include "main.h"

// Mode
#define BMP085_STANDARD 1      //!< Standard mode

#define STD_ATM_PRESS 101325

// Sensor Init function
uint8_t bmpBegin(uint8_t mode, I2C_HandleTypeDef *i2cdev);

float readBMPTemperature(void);
float readBMPPressure(void);
double readBMPAbsoluteAltitude(void);

#endif /* _BMP085_H */
