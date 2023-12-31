/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "can.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t pulse;
uint8_t pulse2;
uint8_t butSost;

CAN_TxHeaderTypeDef TxHeader;
CAN_RxHeaderTypeDef RxHeader;
uint8_t TxData[8] = {0,};
uint8_t RxData[8] = {0,};
uint32_t TxMailbox = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void ledBlink(uint8_t timeBlink);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    if(HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData) == HAL_OK)
    {
       pulse2 = RxData[7]; 
       HAL_GPIO_TogglePin(led1_GPIO_Port, led1_Pin); 
    }   
}
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
  MX_CAN_Init();
  /* USER CODE BEGIN 2 */
  TxHeader.StdId = 0x0111;
  TxHeader.ExtId = 0;
  TxHeader.RTR = CAN_RTR_DATA; //CAN_RTR_REMOTE
  TxHeader.IDE = CAN_ID_STD;   // CAN_ID_EXT
  TxHeader.DLC = 8;
  TxHeader.TransmitGlobalTime = 0;

  HAL_CAN_Start(&hcan);
  HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING | CAN_IT_ERROR | CAN_IT_BUSOFF | CAN_IT_LAST_ERROR_CODE);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
    ledBlink(pulse2);
    
    butSost = HAL_GPIO_ReadPin(userButton_GPIO_Port, userButton_Pin);
    if(butSost == 0) {
      while(butSost == 0) {
        butSost = HAL_GPIO_ReadPin(userButton_GPIO_Port, userButton_Pin);
      }
      pulse += 1;
    }
    if(pulse >= 3) {
      pulse = 0;
    }
    
    TxData[7] = pulse;
      
    if(HAL_CAN_GetTxMailboxesFreeLevel(&hcan) != 0) {
      HAL_CAN_AddTxMessage(&hcan, &TxHeader, TxData, &TxMailbox);
    }  
      
    HAL_Delay(200); 

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

void ledBlink(uint8_t timeBlink) {
  
  switch (timeBlink) {
  	case 1:
      HAL_GPIO_WritePin(led1_GPIO_Port, led2_Pin, GPIO_PIN_SET);
  	  HAL_GPIO_WritePin(led1_GPIO_Port, led3_Pin|led4_Pin, GPIO_PIN_RESET);
  		break;
  
  	case 2:
  	  HAL_GPIO_WritePin(led1_GPIO_Port, led3_Pin, GPIO_PIN_SET);
  	  HAL_GPIO_WritePin(led1_GPIO_Port, led2_Pin|led4_Pin, GPIO_PIN_RESET);
  		break;
    
    case 3:
  	  HAL_GPIO_WritePin(led1_GPIO_Port, led4_Pin, GPIO_PIN_SET);
  	  HAL_GPIO_WritePin(led1_GPIO_Port, led3_Pin|led2_Pin, GPIO_PIN_RESET);
  		break;
  
  	default:
  	  HAL_GPIO_WritePin(led1_GPIO_Port, led2_Pin|led3_Pin|led4_Pin, GPIO_PIN_RESET);
  		break;
  }
  
//  if(timeBlink == 0) {
//    HAL_GPIO_WritePin(led1_GPIO_Port, led2_Pin|led3_Pin|led4_Pin, GPIO_PIN_RESET);
//  } else {
//      HAL_GPIO_WritePin(led1_GPIO_Port, led2_Pin|led3_Pin|led4_Pin, GPIO_PIN_SET);
//      HAL_Delay(timeBlink * 10);
//      HAL_GPIO_WritePin(led1_GPIO_Port, led2_Pin|led3_Pin|led4_Pin, GPIO_PIN_RESET);
//      HAL_Delay(timeBlink * 10);
//  }

}