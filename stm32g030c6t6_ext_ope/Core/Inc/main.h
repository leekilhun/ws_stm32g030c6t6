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
#define IO_OPE_KEY_STOP_Pin GPIO_PIN_13
#define IO_OPE_KEY_STOP_GPIO_Port GPIOC
#define IO_OPE_KEY_START_Pin GPIO_PIN_14
#define IO_OPE_KEY_START_GPIO_Port GPIOC
#define STATUS_Pin GPIO_PIN_15
#define STATUS_GPIO_Port GPIOC
#define HC165_SH_Pin GPIO_PIN_0
#define HC165_SH_GPIO_Port GPIOA
#define HC165_CLK_Pin GPIO_PIN_1
#define HC165_CLK_GPIO_Port GPIOA
#define HC165_GH_Pin GPIO_PIN_4
#define HC165_GH_GPIO_Port GPIOA
#define IO_OUT_7_Pin GPIO_PIN_2
#define IO_OUT_7_GPIO_Port GPIOB
#define IO_OUT_6_Pin GPIO_PIN_10
#define IO_OUT_6_GPIO_Port GPIOB
#define IO_OUT_5_Pin GPIO_PIN_11
#define IO_OUT_5_GPIO_Port GPIOB
#define IO_OUT_4_Pin GPIO_PIN_12
#define IO_OUT_4_GPIO_Port GPIOB
#define IO_OUT_3_Pin GPIO_PIN_13
#define IO_OUT_3_GPIO_Port GPIOB
#define IO_OUT_2_Pin GPIO_PIN_14
#define IO_OUT_2_GPIO_Port GPIOB
#define IO_OUT_1_Pin GPIO_PIN_15
#define IO_OUT_1_GPIO_Port GPIOB
#define IO_OUT_0_Pin GPIO_PIN_8
#define IO_OUT_0_GPIO_Port GPIOA
#define IO_TOWER_BUZZER_Pin GPIO_PIN_11
#define IO_TOWER_BUZZER_GPIO_Port GPIOA
#define IO_TOWER_GREEN_Pin GPIO_PIN_12
#define IO_TOWER_GREEN_GPIO_Port GPIOA
#define IO_TOWER_YELLOW_Pin GPIO_PIN_15
#define IO_TOWER_YELLOW_GPIO_Port GPIOA
#define IO_TOWER_RED_Pin GPIO_PIN_0
#define IO_TOWER_RED_GPIO_Port GPIOD
#define IO_OPE_LAMP_RESET_Pin GPIO_PIN_1
#define IO_OPE_LAMP_RESET_GPIO_Port GPIOD
#define IO_OPE_LAMP_STOP_Pin GPIO_PIN_2
#define IO_OPE_LAMP_STOP_GPIO_Port GPIOD
#define IO_OPE_LAMP_START_Pin GPIO_PIN_3
#define IO_OPE_LAMP_START_GPIO_Port GPIOD
#define IO_IN_SAFETY2_Pin GPIO_PIN_3
#define IO_IN_SAFETY2_GPIO_Port GPIOB
#define IO_IN_SAFETY1_Pin GPIO_PIN_4
#define IO_IN_SAFETY1_GPIO_Port GPIOB
#define IO_OPE_ESTOP_Pin GPIO_PIN_5
#define IO_OPE_ESTOP_GPIO_Port GPIOB
#define OPE_MODE_AUTO_Pin GPIO_PIN_8
#define OPE_MODE_AUTO_GPIO_Port GPIOB
#define IO_OPE_KEY_RESET_Pin GPIO_PIN_9
#define IO_OPE_KEY_RESET_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
