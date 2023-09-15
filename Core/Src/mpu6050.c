/*
 * mpu6050.c
 *
 *  Created on: Aug 25, 2023
 *      Author: satwikagarwal
 */

#include "mpu6050.h"
#include "stdio.h"

uint8_t mpu6050_init(I2C_HandleTypeDef *i2cHandle){
		uint8_t check;
	    uint8_t Data;
	    uint8_t i2c_timeout = 100;
	    HAL_StatusTypeDef res;

	    // check device ID WHO_AM_I

	    HAL_I2C_Mem_Read(i2cHandle, MPU6050_ADDRESS, MPU6050_WHO_AM_I, 1, &check, 1, HAL_MAX_DELAY);

	    if (check == 104) // 0x68 will be returned by the sensor if everything goes well
	    {
	    	Data = 0x80;
	    	res = HAL_I2C_Mem_Write(i2cHandle, MPU6050_ADDRESS, MPU6050_PWR_MGMT_1, 1, &Data, 1, i2c_timeout);

	        // Set DATA RATE of 1KHz by writing SMPLRT_DIV register
	        Data = 0x07;
	        res =  HAL_I2C_Mem_Write(i2cHandle, MPU6050_ADDRESS, MPU6050_SMPLRT_DIV, 1, &Data, 1, i2c_timeout);

	        // Set accelerometer configuration in ACCEL_CONFIG Register
	        // XA_ST=0,YA_ST=0,ZA_ST=0, FS_SEL=0 -> � 2g
	        Data = 0x00;
	        res = HAL_I2C_Mem_Write(i2cHandle, MPU6050_ADDRESS, MPU6050_ACCEL_CONFIG, 1, &Data, 1, i2c_timeout);

	        // Set Gyroscopic configuration in GYRO_CONFIG Register
	        // XG_ST=0,YG_ST=0,ZG_ST=0, FS_SEL=0 -> � 250 �/s
	        Data = 0x00;
	        res = HAL_I2C_Mem_Write(i2cHandle, MPU6050_ADDRESS, MPU6050_GYRO_CONFIG, 1, &Data, 1, i2c_timeout);

	        //Register 55 – INT Pin / Bypass Enable Configuration  INT_PIN_CFG
			//10100000 -> latch=1, rd_clear=0
			Data = 0x20;
			res = HAL_I2C_Mem_Write(i2cHandle, MPU6050_ADDRESS, MPU6050_INT_PIN_CFG, 1, &Data, 1, i2c_timeout);

			//Register 56 – Interrupt Enable  INT_ENABLE
			Data = 0x01;
			res = HAL_I2C_Mem_Write(i2cHandle, MPU6050_ADDRESS, MPU6050_INT_ENABLE, 1, &Data, 1, i2c_timeout);

	        // power management register 0X6B we should write all 0's to wake the sensor up
			Data = 0;
			res = HAL_I2C_Mem_Write(i2cHandle, MPU6050_ADDRESS, MPU6050_PWR_MGMT_1, 1, &Data, 1, i2c_timeout);


	        return 0;
	    }
	    return 1;
}

uint8_t mpu6050_read(I2C_HandleTypeDef *i2cHandle, MPU6050_DATA *data){
	//read values
	HAL_I2C_Mem_Read_IT(i2cHandle, MPU6050_ADDRESS, MPU6050_ACCEL_XOUT_H, 1, data->recData, 6);
	//read int status flag to reset interrupt
	uint8_t check = 0;
	HAL_I2C_Mem_Read(i2cHandle, MPU6050_ADDRESS, MPU6050_INT_STATUS, 1, &check, 1,100);

	return 0;
}


void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c){
	if(hi2c->Instance == I2C1){
		//make useful data from the raw read values
		printf("reading done\n");
	}
}
