/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "stm32g0xx_hal.h"
#include "stm32g0xx_ll_rcc.h"
#include "stm32g0xx_ll_bus.h"
#include "stm32g0xx_ll_system.h"
#include "stm32g0xx_ll_exti.h"
#include "stm32g0xx_ll_cortex.h"
#include "stm32g0xx_ll_utils.h"
#include "stm32g0xx_ll_pwr.h"
#include "stm32g0xx_ll_dma.h"
#include "stm32g0xx_ll_gpio.h"

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
#define IO_OUT_1_Pin GPIO_PIN_13
#define IO_OUT_1_GPIO_Port GPIOC
#define IO_OUT_0_Pin GPIO_PIN_14
#define IO_OUT_0_GPIO_Port GPIOC
#define STATUS_Pin GPIO_PIN_15
#define STATUS_GPIO_Port GPIOC
#define IO_IN_0_Pin GPIO_PIN_0
#define IO_IN_0_GPIO_Port GPIOA
#define IO_IN_1_Pin GPIO_PIN_1
#define IO_IN_1_GPIO_Port GPIOA
#define IO_IN_2_Pin GPIO_PIN_4
#define IO_IN_2_GPIO_Port GPIOA
#define IO_IN_3_Pin GPIO_PIN_5
#define IO_IN_3_GPIO_Port GPIOA
#define IO_IN_4_Pin GPIO_PIN_6
#define IO_IN_4_GPIO_Port GPIOA
#define IO_IN_5_Pin GPIO_PIN_7
#define IO_IN_5_GPIO_Port GPIOA
#define IO_IN_6_Pin GPIO_PIN_0
#define IO_IN_6_GPIO_Port GPIOB
#define IO_IN_7_Pin GPIO_PIN_1
#define IO_IN_7_GPIO_Port GPIOB
#define IO_IN_8_Pin GPIO_PIN_2
#define IO_IN_8_GPIO_Port GPIOB
#define IO_IN_9_Pin GPIO_PIN_10
#define IO_IN_9_GPIO_Port GPIOB
#define IO_IN_10_Pin GPIO_PIN_11
#define IO_IN_10_GPIO_Port GPIOB
#define IO_IN_11_Pin GPIO_PIN_12
#define IO_IN_11_GPIO_Port GPIOB
#define IO_IN_12_Pin GPIO_PIN_13
#define IO_IN_12_GPIO_Port GPIOB
#define IO_IN_13_Pin GPIO_PIN_14
#define IO_IN_13_GPIO_Port GPIOB
#define IO_IN_14_Pin GPIO_PIN_15
#define IO_IN_14_GPIO_Port GPIOB
#define IO_IN15_Pin GPIO_PIN_8
#define IO_IN15_GPIO_Port GPIOA
#define IO_OUT_15_Pin GPIO_PIN_6
#define IO_OUT_15_GPIO_Port GPIOC
#define IO_OUT_14_Pin GPIO_PIN_7
#define IO_OUT_14_GPIO_Port GPIOC
#define IO_OUT_13_Pin GPIO_PIN_11
#define IO_OUT_13_GPIO_Port GPIOA
#define IO_OUT_12_Pin GPIO_PIN_12
#define IO_OUT_12_GPIO_Port GPIOA
#define IO_OUT_11_Pin GPIO_PIN_15
#define IO_OUT_11_GPIO_Port GPIOA
#define IO_OUT_10_Pin GPIO_PIN_0
#define IO_OUT_10_GPIO_Port GPIOD
#define IO_OUT_9_Pin GPIO_PIN_1
#define IO_OUT_9_GPIO_Port GPIOD
#define IO_OUT_8_Pin GPIO_PIN_2
#define IO_OUT_8_GPIO_Port GPIOD
#define IO_OUT_7_Pin GPIO_PIN_3
#define IO_OUT_7_GPIO_Port GPIOD
#define IO_OUT_6_Pin GPIO_PIN_3
#define IO_OUT_6_GPIO_Port GPIOB
#define IO_OUT_5_Pin GPIO_PIN_4
#define IO_OUT_5_GPIO_Port GPIOB
#define IO_OUT_4_Pin GPIO_PIN_5
#define IO_OUT_4_GPIO_Port GPIOB
#define IO_OUT_3_Pin GPIO_PIN_8
#define IO_OUT_3_GPIO_Port GPIOB
#define IO_OUT_2_Pin GPIO_PIN_9
#define IO_OUT_2_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
