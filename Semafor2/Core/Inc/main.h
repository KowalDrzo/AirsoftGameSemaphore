/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "Tm1637.h"
#include "Points.h"
#include "Setup.h"

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/*!
 * \struct airsoftClock
 * \brief Struktura przechowująca dane potrzebne w urządzeniu.
 *
 * Zawiera pola z czasem obu drużyn, ostatnio wciśniętym przyciskiem oraz jasnością wyświetlaczy.
 *
 */

typedef struct {

	short int redTime;
	short int blueTime;
	int8_t bright;
	short int potenValue;

	short int ledState;
	_Bool ledGoinBack;
	short int gameModeUp;
	_Bool gameStarted;

} airsoftClock;

airsoftClock asgClock;



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
#define Przycisk_Pin GPIO_PIN_4
#define Przycisk_GPIO_Port GPIOA
#define Przycisk_EXTI_IRQn EXTI4_IRQn
#define BUZZER_Pin GPIO_PIN_8
#define BUZZER_GPIO_Port GPIOA
#define CZER_DIO_Pin GPIO_PIN_9
#define CZER_DIO_GPIO_Port GPIOA
#define CZER_CLK_Pin GPIO_PIN_10
#define CZER_CLK_GPIO_Port GPIOA
#define NIEB_DIO_Pin GPIO_PIN_11
#define NIEB_DIO_GPIO_Port GPIOA
#define NIEB_CLK_Pin GPIO_PIN_12
#define NIEB_CLK_GPIO_Port GPIOA
#define LED_CZER_Pin GPIO_PIN_6
#define LED_CZER_GPIO_Port GPIOB
#define LED_ZIEL_Pin GPIO_PIN_7
#define LED_ZIEL_GPIO_Port GPIOB
#define LED_NIEB_Pin GPIO_PIN_8
#define LED_NIEB_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
