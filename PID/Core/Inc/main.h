/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define DM_PWM1_Pin GPIO_PIN_5
#define DM_PWM1_GPIO_Port GPIOE
#define DM_PWM2_Pin GPIO_PIN_6
#define DM_PWM2_GPIO_Port GPIOE
#define DM_INB3_Pin GPIO_PIN_1
#define DM_INB3_GPIO_Port GPIOF
#define DM_INA4_Pin GPIO_PIN_2
#define DM_INA4_GPIO_Port GPIOF
#define DM_INB1_Pin GPIO_PIN_3
#define DM_INB1_GPIO_Port GPIOF
#define DM_INA2_Pin GPIO_PIN_4
#define DM_INA2_GPIO_Port GPIOF
#define Enc_2A_Pin GPIO_PIN_0
#define Enc_2A_GPIO_Port GPIOA
#define Enc_2B_Pin GPIO_PIN_1
#define Enc_2B_GPIO_Port GPIOA
#define DM_PWM3_Pin GPIO_PIN_2
#define DM_PWM3_GPIO_Port GPIOA
#define DM_PWM4_Pin GPIO_PIN_3
#define DM_PWM4_GPIO_Port GPIOA
#define Enc_3A_Pin GPIO_PIN_6
#define Enc_3A_GPIO_Port GPIOA
#define Enc_3B_Pin GPIO_PIN_7
#define Enc_3B_GPIO_Port GPIOA
#define DM_INA3_Pin GPIO_PIN_4
#define DM_INA3_GPIO_Port GPIOC
#define DM_INB4_Pin GPIO_PIN_5
#define DM_INB4_GPIO_Port GPIOC
#define DM_INA1_Pin GPIO_PIN_11
#define DM_INA1_GPIO_Port GPIOF
#define DM_INB2_Pin GPIO_PIN_12
#define DM_INB2_GPIO_Port GPIOF
#define Enc_1A_Pin GPIO_PIN_9
#define Enc_1A_GPIO_Port GPIOE
#define Enc_1B_Pin GPIO_PIN_11
#define Enc_1B_GPIO_Port GPIOE
#define Enc_4A_Pin GPIO_PIN_12
#define Enc_4A_GPIO_Port GPIOD
#define Enc_4B_Pin GPIO_PIN_13
#define Enc_4B_GPIO_Port GPIOD

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
