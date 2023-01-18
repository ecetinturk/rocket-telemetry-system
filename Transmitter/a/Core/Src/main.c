/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "SX1278.h"
#include "bmp085.h"
#include <stdlib.h>
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define M_PI 3.14159265358979323846
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
float Temperature, Pressure, SeaLevelPressure, Altitude;

int ret;

char buffer[150];

int message;
int message_length;

int32_t acc_x, acc_y, acc_z, gyro_x, gyro_y, gyro_z;
float calacc_x, calacc_y, calacc_z, temperature, caltemperature, calgyro_x, calgyro_y, calgyro_z;
double pitch, roll, yaw;

uint8_t Rx_tempData[128];
uint8_t Rx_data[128];  //  creating a buffer of 512 bytes
uint8_t Rx_index = 0;

uint8_t receivedString[128];

uint8_t flag = 0;

//char* Message_ID;
//char* Time;
//char* Data_Valid;
//char* Raw_Latitude;
//char* N_S;
//char* Raw_Longitude;
//char* E_W;
//char* Speed;
//char* COG;
//char* Date;
//char* Magnetic_Variation;
//char* M_E_W;
char* Positioning_Mode;

char* Message_ID;
char* UTC_Of_Position;
char* Raw_Latitude;
char* N_S;
char* Raw_Longitude;
char* E_W;
char* Quality_Indicator;
char* Num_Of_Satellites;
char* HDOP;
char* Antenna_Altitude;
char* A_A_M;
char* Geoidal_Seperation;
char* G_S_M;
char* AoDC;
char* DRS_ID;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void setup_mpu_6050_registers(void);
void read_mpu_6050_data(void);

void Send_AccAll(void);
void Send_TempPresAlt(void);
void Send_MPUTemp(void);
void Send_Gyro(void);
void Send_PitchRollYaw(void);

void GPS_Parse(void);
char *strtok_f (char *s, char delim);
char *strtok_fr (char *s, char delim, char **save_ptr);

void Send_All(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
SX1278_hw_t SX1278_hw;
SX1278_t SX1278;
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_I2C1_Init();
  MX_I2C2_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  setup_mpu_6050_registers();
  read_mpu_6050_data();
  bmpBegin(BMP085_STANDARD, &hi2c2);
  HAL_UART_Receive_IT(&huart2, Rx_tempData, 1);

  SX1278_hw.dio0.port = DIO0_GPIO_Port;
  SX1278_hw.dio0.pin = DIO0_Pin;
  SX1278_hw.nss.port = NSS_GPIO_Port;
  SX1278_hw.nss.pin = NSS_Pin;
  SX1278_hw.reset.port = RESET_GPIO_Port;
  SX1278_hw.reset.pin = RESET_Pin;
  SX1278_hw.spi = &hspi1;

  SX1278.hw = &SX1278_hw;

  SX1278_begin(&SX1278, SX1278_433MHZ, SX1278_POWER_17DBM, SX1278_LORA_SF_8,
  				SX1278_LORA_BW_20_8KHZ, 10);

  ret = SX1278_LoRaEntryTx(&SX1278, 150, 1500);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
//	  read_mpu_6050_data();
//	  if (flag == '$')
//	  	  {
//		  	  Send_All();
//	  		  HAL_Delay(15000);
//	  	  }
//	  HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);

	HAL_Delay(250);
	read_mpu_6050_data();
	HAL_Delay(250);
	Send_TempPresAlt();
	HAL_Delay(1050);
	Send_AccAll();
	HAL_Delay(1050);
	Send_Gyro();
	HAL_Delay(1050);
	Send_PitchRollYaw();
	HAL_Delay(1050);
	Send_MPUTemp();
	HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
	HAL_Delay(850);

	if (flag == '$')
	  {
		  GPS_Parse();
		  HAL_Delay(850);
	  }

//	read_mpu_6050_data();
//	HAL_Delay(250);
//	Send_TempPresAlt();
//	HAL_Delay(1050);
//	Send_AccAll();
//	HAL_Delay(1050);
//	Send_Gyro();
//	HAL_Delay(1050);
//	Send_MPUTemp();
//	HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
//	HAL_Delay(850);
//
//	if (flag == '$')
//	  {
//		  GPS_Parse();
//		  HAL_Delay(650);
//	  }




//	read_mpu_6050_data();
//	HAL_Delay(250);
//	Send_TempPresAlt();
//	HAL_Delay(1050);
//	Send_AccAll();
//	HAL_Delay(1050);
//	Send_Gyro();
//	HAL_Delay(950);
//	Send_MPUTemp();
//	HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
//	HAL_Delay(650);
//
//	if (flag == '$')
//	  {
//		  GPS_Parse();
//		  HAL_Delay(450);
//	  }

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void Send_AccAll(void) {

	message_length = sprintf(buffer, "Ax:%.3f\r\nAy:%.3f\r\nAz:%.3f\r\n", calacc_x, calacc_y, calacc_z);
	ret = SX1278_LoRaEntryTx(&SX1278, message_length, 500);
	ret = SX1278_LoRaTxPacket(&SX1278, (uint8_t *) buffer, message_length, 500);
	return;
}

void Send_MPUTemp(void) {

	message_length = sprintf(buffer, "MPUTmp: %.2f\r\n\r\n", caltemperature);
	ret = SX1278_LoRaEntryTx(&SX1278, message_length, 500);
	ret = SX1278_LoRaTxPacket(&SX1278, (uint8_t *) buffer, message_length, 500);
	return;
}

void Send_Gyro(void) {

	message_length = sprintf(buffer, "Gx:%.3f\r\nGy:%.3f\r\nGz:%.3f\r\n", calgyro_x, calgyro_y, calgyro_z);
	ret = SX1278_LoRaEntryTx(&SX1278, message_length, 450);
	ret = SX1278_LoRaTxPacket(&SX1278, (uint8_t *) buffer, message_length, 450);
	return;
}

void Send_PitchRollYaw(void) {
	message_length = sprintf(buffer, "Pi:%.3f\r\nRo:%.3f\r\nYa:%.3f\r\n", pitch, roll, yaw);
	ret = SX1278_LoRaEntryTx(&SX1278, message_length, 550);
	ret = SX1278_LoRaTxPacket(&SX1278, (uint8_t *) buffer, message_length, 550);
	return;
}

void Send_TempPresAlt(void) {

	Temperature = readBMPTemperature();
	Pressure = readBMPPressure();
	Altitude = readBMPAbsoluteAltitude();

	message_length = sprintf(buffer, "Temp: %.2f\r\nPres: %d\r\nAlt: %d\r\n", Temperature, (int)Pressure, (int)Altitude);
	ret = SX1278_LoRaEntryTx(&SX1278, message_length, 550);
	ret = SX1278_LoRaTxPacket(&SX1278, (uint8_t *) buffer, message_length, 550);
	return;
}

void read_mpu_6050_data(void){                                         //Subroutine for reading the raw gyro and accelerometer data
uint8_t datam[20];

datam[0]=0x3B; HAL_I2C_Master_Transmit(&hi2c1 ,0xd0,datam,1,5);
HAL_I2C_Master_Receive(&hi2c1 ,0xd1,datam,14,5);

acc_x = ((datam[0]<<8)|datam[1])&0xffff;   if (datam[0]&0x80) acc_x|=0xffff0000;
acc_y = ((datam[2]<<8)|datam[3])&0xffff;   if (datam[2]&0x80) acc_y|=0xffff0000;
acc_z = ((datam[4]<<8)|datam[5])&0xffff;   if (datam[4]&0x80) acc_z|=0xffff0000;

temperature = ((int16_t)datam[6]<<8)|datam[7];

gyro_x = datam[8]<<8|datam[9];   if (datam[8]&0x80) gyro_x|=0xffff0000;
gyro_y = datam[10]<<8|datam[11]; if (datam[10]&0x80) gyro_y|=0xffff0000;
gyro_z = datam[12]<<8|datam[13]; if (datam[12]&0x80) gyro_z|=0xffff0000;

calacc_x = ((float) -acc_x) * 8.0 / (float) 0x8000;
calacc_y = ((float) -acc_y) * 8.0 / (float) 0x8000;
calacc_z = ((float) -acc_z) * 8.0 / (float) 0x8000;

//temperature = datam[6]<<8|datam[7];
// caltemperature = temperature / 100;
caltemperature = ((float) temperature) / 333.87 + 21.0;

gyro_x = datam[8]<<8|datam[9];   if (datam[8]&0x80) gyro_x|=0xffff0000;
gyro_y = datam[10]<<8|datam[11]; if (datam[10]&0x80) gyro_y|=0xffff0000;
gyro_z = datam[12]<<8|datam[13]; if (datam[12]&0x80) gyro_z|=0xffff0000;

calgyro_x = ((float) -gyro_x) * 500.0 / (float) 0x8000;
calgyro_y = ((float) -gyro_y) * 500.0 / (float) 0x8000;
calgyro_z = ((float) -gyro_z) * 500.0 / (float) 0x8000;

pitch = 180 * atan(acc_x/sqrt(acc_y*acc_y + acc_z*acc_z))/M_PI;
roll = 180 * atan(acc_y/sqrt(acc_x*acc_x + acc_z*acc_z))/M_PI;
yaw = 2*(180 * atan (acc_z/sqrt(acc_x*acc_x + acc_z*acc_z))/M_PI);

}
void setup_mpu_6050_registers(void){
uint8_t data[20];

data[0]=0x6b; data[1]=0x00; HAL_I2C_Master_Transmit(&hi2c1 ,0xd0,data,2,50); //Activate the MPU-6050
data[0]=0x37; data[1]=0x02; HAL_I2C_Master_Transmit(&hi2c1 ,0xd0,data,2,50); //Registers for Bypass Mode:
data[0]=0x6a; data[1]=0x00; HAL_I2C_Master_Transmit(&hi2c1 ,0xd0,data,2,50);

data[0]=0x1c; data[1]=0x10; HAL_I2C_Master_Transmit(&hi2c1 ,0xd0,data,2,50); //Configure the accelerometer (+/-8g)
data[0]=0x1b; data[1]=0x08; HAL_I2C_Master_Transmit(&hi2c1 ,0xd0,data,2,50); //Configure the gyro (500dps full scale)
}

// GPGGA with Altitude Information

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart == &huart2) {
		switch(Rx_index) {
		case 0:
			if(Rx_tempData[0] == '$')
			{
				Rx_data[Rx_index] = Rx_tempData[0];
				Rx_index++;
			} else {
				Rx_index = 0;
			}
			break;
		case 1:
			if(Rx_tempData[0] == 'G')
			{
				Rx_data[Rx_index] = Rx_tempData[0];
				Rx_index++;
			} else {
				Rx_index = 0;
			}
			break;
		case 2:
			if(Rx_tempData[0] == 'P')
			{
				Rx_data[Rx_index] = Rx_tempData[0];
				Rx_index++;
			} else {
				Rx_index = 0;
			}
			break;
		case 3:
			if(Rx_tempData[0] == 'G')
			{
				Rx_data[Rx_index] = Rx_tempData[0];
				Rx_index++;
			} else {
				Rx_index = 0;
			}
			break;
		case 4:
			if(Rx_tempData[0] == 'G')
			{
				Rx_data[Rx_index] = Rx_tempData[0];
				Rx_index++;
			} else {
				Rx_index = 0;
			}
			break;
		case 5:
			if(Rx_tempData[0] == 'A')
			{
				Rx_data[Rx_index] = Rx_tempData[0];
				Rx_index++;
			} else {
				Rx_index = 0;
			}
			break;
		case 6:

		default:
			Rx_data[Rx_index] = Rx_tempData[0];
			if(Rx_tempData[0] == '\n')
			{
				// kopyalama yapılacak
				memcpy(receivedString, Rx_data, 128);
				flag = '$';
				Rx_index = 0;
			} else {
				Rx_index++;
			}
		}
		HAL_UART_Receive_IT(&huart2, Rx_tempData, 1);
	}
}

// GPRMC

//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
//{
//	if (huart == &huart2) {
//		switch(Rx_index) {
//		case 0:
//			if(Rx_tempData[0] == '$')
//			{
//				Rx_data[Rx_index] = Rx_tempData[0];
//				Rx_index++;
//			} else {
//				Rx_index = 0;
//			}
//			break;
//		case 1:
//			if(Rx_tempData[0] == 'G')
//			{
//				Rx_data[Rx_index] = Rx_tempData[0];
//				Rx_index++;
//			} else {
//				Rx_index = 0;
//			}
//			break;
//		case 2:
//			if(Rx_tempData[0] == 'P')
//			{
//				Rx_data[Rx_index] = Rx_tempData[0];
//				Rx_index++;
//			} else {
//				Rx_index = 0;
//			}
//			break;
//		case 3:
//			if(Rx_tempData[0] == 'R')
//			{
//				Rx_data[Rx_index] = Rx_tempData[0];
//				Rx_index++;
//			} else {
//				Rx_index = 0;
//			}
//			break;
//		case 4:
//			if(Rx_tempData[0] == 'M')
//			{
//				Rx_data[Rx_index] = Rx_tempData[0];
//				Rx_index++;
//			} else {
//				Rx_index = 0;
//			}
//			break;
//		case 5:
//			if(Rx_tempData[0] == 'C')
//			{
//				Rx_data[Rx_index] = Rx_tempData[0];
//				Rx_index++;
//			} else {
//				Rx_index = 0;
//			}
//			break;
//		case 6:
//
//		default:
//			Rx_data[Rx_index] = Rx_tempData[0];
//			if(Rx_tempData[0] == '\n')
//			{
//				// kopyalama yapılacak
//				memcpy(receivedString, Rx_data, 128);
//				flag = '$';
//				Rx_index = 0;
//			} else {
//				Rx_index++;
//			}
//		}
//		HAL_UART_Receive_IT(&huart2, Rx_tempData, 1);
//	}
//}

void GPS_Parse(void)
{
//	Message_ID = strtok_f((char*)receivedString,',');
//	Time = strtok_f(NULL,',');
//	Data_Valid = strtok_f(NULL,',');
//	Raw_Latitude = strtok_f(NULL,',');
//	N_S = strtok_f(NULL,',');
//	Raw_Longitude = strtok_f(NULL,',');
//	E_W = strtok_f(NULL,',');
//	Speed = strtok_f(NULL,',');
//	COG = strtok_f(NULL,',');
//	Date = strtok_f(NULL,',');
//	Magnetic_Variation = strtok_f(NULL,',');
//	M_E_W = strtok_f(NULL,',');
//	Positioning_Mode = strtok_f(NULL,',');

	Message_ID = strtok_f((char*)receivedString,',');
	UTC_Of_Position = strtok_f(NULL, ',');
	Raw_Latitude = strtok_f(NULL,',');
	N_S = strtok_f(NULL,',');
	Raw_Longitude = strtok_f(NULL,',');
	E_W = strtok_f(NULL,',');
	Quality_Indicator = strtok_f(NULL,',');
	Num_Of_Satellites = strtok_f(NULL,',');
	HDOP = strtok_f(NULL,',');
	Antenna_Altitude = strtok_f(NULL,',');
	A_A_M = strtok_f(NULL,',');
	Geoidal_Seperation = strtok_f(NULL,',');
	G_S_M = strtok_f(NULL,',');
	AoDC = strtok_f(NULL,',');
	DRS_ID = strtok_f(NULL,',');

//	char *GPSParse = malloc(strlen(Raw_Latitude)+strlen(N_S)+strlen(Raw_Longitude)+strlen(E_W)+1);
//	sprintf(GPSParse, "Lat:%s %s\r\nLon:%s %s\r\n", Raw_Latitude, N_S, Raw_Longitude, E_W);

	char *GPSParse = malloc(strlen(Raw_Latitude)+strlen(N_S)+strlen(Raw_Longitude)+strlen(E_W)+strlen(Antenna_Altitude)+strlen(A_A_M)+1);
	sprintf(GPSParse, "Lat:%s %s\r\nLon:%s %s\r\nAAM:%s %s\r\n", Raw_Latitude, N_S, Raw_Longitude, E_W, Antenna_Altitude, A_A_M);

	message_length = sprintf(buffer, "%s", GPSParse);
	ret = SX1278_LoRaEntryTx(&SX1278, message_length, 1650);
	ret = SX1278_LoRaTxPacket(&SX1278, (uint8_t *) buffer, message_length, 1650);

	return;
}

char *strtok_f (char *s, char delim)
{
    static char *save_ptr;

    return strtok_fr (s, delim, &save_ptr);
}

char *strtok_fr (char *s, char delim, char **save_ptr)
{
    char *tail;
    char c;

    if (s == NULL) {
        s = *save_ptr;
    }
    tail = s;
    if ((c = *tail) == '\0') {
        s = NULL;
    }
    else {
        do {
            if (c == delim) {
                *tail++ = '\0';
                break;
           }
        }while ((c = *++tail) != '\0');
    }
    *save_ptr = tail;
    return s;
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
