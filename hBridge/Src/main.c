
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2019 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_hal.h"

/* USER CODE BEGIN Includes */
#define __HAL_TIM_GET_AUTORELOAD(__HANDLE__) ((__HANDLE__)->Instance->ARR)
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc3;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM3_Init(void);
static void MX_ADC3_Init(void);
                                    
void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);
                                

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
short set1 = 0; // dictates whether the pwm writes to h1 or l1
short set2 = 0; // dictates whether the pwm writes to h2 or l2
short set3 = 0; // dictates whether the pwm writes to h3 or l3
short increment = 0; // dictates how quickly the pwm moves through the lookup table

int percent = 0; //the percent (a counter)
int percentLimit = 5000; //the limit of the counter to speed/slow ramping (higher = slower ramp)

//float adc_value = 0;

//for underwater stuff
//int percentLimit = 25000; //the limit of the counter to speed/slow ramping (higher = slower ramp)
//int incrementLimit = 17; //underwater speed limit

int incrementLimit = 50; //the limit of the increment (higher = faster top speed)
int flag = 0; //keep this zero pls
int minLim = 10; //min speed for variable speed in the motor

short lookUp[] = {128,128,128,129,129,130,130,130,131,131,132,132,132,133,133,134,134,134,135,135,136,136,136,137,137,138,138,138,139,139,139,140,140,141,141,141,142,142,143,143,143,144,144,145,145,145,146,146,147,147,147,148,148,149,149,149,150,150,151,151,151,152,152,153,153,153,154,154,155,155,155,156,156,156,157,157,158,158,158,159,159,160,160,160,161,161,162,162,162,163,163,163,164,164,165,165,165,166,166,167,167,167,168,168,168,169,169,170,170,170,171,171,171,172,172,173,173,173,174,174,174,175,175,176,176,176,177,177,177,178,178,179,179,179,180,180,180,181,181,181,182,182,183,183,183,184,184,184,185,185,185,186,186,186,187,187,188,188,188,189,189,189,190,190,190,191,191,191,192,192,192,193,193,193,194,194,194,195,195,195,196,196,196,197,197,198,198,198,199,199,199,199,200,200,200,201,201,201,202,202,202,203,203,203,204,204,204,205,205,205,206,206,206,207,207,207,208,208,208,208,209,209,209,210,210,210,211,211,211,212,212,212,212,213,213,213,214,214,214,214,215,215,215,216,216,216,217,217,217,217,218,218,218,219,219,219,219,220,220,220,220,221,221,221,222,222,222,222,223,223,223,223,224,224,224,224,225,225,225,225,226,226,226,227,227,227,227,228,228,228,228,228,229,229,229,229,230,230,230,230,231,231,231,231,232,232,232,232,233,233,233,233,233,234,234,234,234,235,235,235,235,235,236,236,236,236,236,237,237,237,237,237,238,238,238,238,238,239,239,239,239,239,240,240,240,240,240,241,241,241,241,241,241,242,242,242,242,242,243,243,243,243,243,243,244,244,244,244,244,244,245,245,245,245,245,245,245,246,246,246,246,246,246,246,247,247,247,247,247,247,247,248,248,248,248,248,248,248,249,249,249,249,249,249,249,249,249,250,250,250,250,250,250,250,250,250,251,251,251,251,251,251,251,251,251,251,252,252,252,252,252,252,252,252,252,252,252,253,253,253,253,253,253,253,253,253,253,253,253,253,253,253,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,253,253,253,253,253,253,253,253,253,253,253,253,253,253,253,252,252,252,252,252,252,252,252,252,252,252,251,251,251,251,251,251,251,251,251,251,250,250,250,250,250,250,250,250,250,249,249,249,249,249,249,249,249,249,248,248,248,248,248,248,248,247,247,247,247,247,247,247,246,246,246,246,246,246,246,245,245,245,245,245,245,245,244,244,244,244,244,244,243,243,243,243,243,243,242,242,242,242,242,241,241,241,241,241,241,240,240,240,240,240,239,239,239,239,239,238,238,238,238,238,237,237,237,237,237,236,236,236,236,236,235,235,235,235,235,234,234,234,234,233,233,233,233,233,232,232,232,232,231,231,231,231,230,230,230,230,229,229,229,229,228,228,228,228,228,227,227,227,227,226,226,226,225,225,225,225,224,224,224,224,223,223,223,223,222,222,222,222,221,221,221,220,220,220,220,219,219,219,219,218,218,218,217,217,217,217,216,216,216,215,215,215,214,214,214,214,213,213,213,212,212,212,212,211,211,211,210,210,210,209,209,209,208,208,208,208,207,207,207,206,206,206,205,205,205,204,204,204,203,203,203,202,202,202,201,201,201,200,200,200,199,199,199,199,198,198,198,197,197,196,196,196,195,195,195,194,194,194,193,193,193,192,192,192,191,191,191,190,190,190,189,189,189,188,188,188,187,187,186,186,186,185,185,185,184,184,184,183,183,183,182,182,181,181,181,180,180,180,179,179,179,178,178,177,177,177,176,176,176,175,175,174,174,174,173,173,173,172,172,171,171,171,170,170,170,169,169,168,168,168,167,167,167,166,166,165,165,165,164,164,163,163,163,162,162,162,161,161,160,160,160,159,159,158,158,158,157,157,156,156,156,155,155,155,154,154,153,153,153,152,152,151,151,151,150,150,149,149,149,148,148,147,147,147,146,146,145,145,145,144,144,143,143,143,142,142,141,141,141,140,140,139,139,139,138,138,138,137,137,136,136,136,135,135,134,134,134,133,133,132,132,132,131,131,130,130,130,129,129,128,128};

int size = sizeof(lookUp);

int arrayPos1 = 0; // keeps track of the first pwm's position in the look up table
int arrayPos2 = 2*sizeof(lookUp) / 3; // keeps track of the second pwm's position in the look up table
int arrayPos3 = 4*sizeof(lookUp) / 3; // keeps track of the third pwm's position in the look up table

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(percent >= percentLimit && increment < incrementLimit && !flag){
            increment++;
            percent = 0;

   //         uncomment to have motor change speed continuously
//            flag = (increment == incrementLimit) ? 1:0;
//    }else if(flag && percent >= percentLimit && increment > minLim){
//            increment--;
//            percent = 0;
//            flag = (increment == minLim) ? 0:1;

    }
    else{
            percent++;
    }
//    adc_value = HAL_ADC_GetValue(&hadc3);
//    adc_value *= 100;
//    int i;
//    for(i = 0; i < adc_value; i++){
//            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, 1);
//            HAL_Delay(10);
//            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, 0);
//            HAL_Delay(10);
//    }
//    if(adc_value > 0){
//            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, 1);
//    }
//    if(adc_value >0x000 )
//            {
//                HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, 1);
//            }
//            else
//                {
//                    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, 0);
//                }
    }

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

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
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_ADC3_Init();
  /* USER CODE BEGIN 2 */
    //initializes PWM and timers
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
    HAL_TIM_Base_Start_IT(&htim3);
    HAL_TIM_Base_Start(&htim2);
//    HAL_ADC_Start_DMA(&hadc3, 3);
    HAL_ADC_Start(&hadc3);

    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, 1); // turns on en-gate, allows for writing to motor pins
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, 1);  // enables the green light as a status light
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */


    while (1)
        {
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */

            arrayPos1 += increment;
            arrayPos2 += increment;
            arrayPos3 += increment;

            if(arrayPos1 >= size){
                    arrayPos1 %= size;
                    set1 = !set1;
            }
            else if(arrayPos2 >= size){
                    arrayPos2 %= size;
                    set2 = !set2;
            }
            else if(arrayPos3 >= size){
                    arrayPos3 %= size;
                    set3 = !set3;
            }
            TIM2->CCR1 = lookUp[arrayPos1] - 128;
            TIM2->CCR2 = lookUp[arrayPos2] - 128;
            TIM2->CCR3 = lookUp[arrayPos3] - 128;

            short c = TIM2->CNT;
            short one = c < TIM2->CCR1;
            short two = c < TIM2->CCR2;
            short three = c < TIM2->CCR3;

            if(set1){
                    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, (one) ? 1 : 0);
                    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, (!one) ? 1 : 0);
            }else{
                    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, (one) ? 1 : 0);
                    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, (!one) ? 1 : 0);
            }
            if(set2){
                    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, (two) ? 1 : 0);
                    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, (!two) ? 1 : 0);
            }else{
                    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, (two) ? 1 : 0);
                    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, (!two) ? 1 : 0);
            }
            if(set3){
                    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, (three) ? 1 : 0);
                    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, (!three) ? 1 : 0);
            }else{
                    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, (three) ? 1 : 0);
                    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, (!three) ? 1 : 0);
            }
        }
  /* USER CODE END 3 */

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* ADC3 init function */
static void MX_ADC3_Init(void)
{

  ADC_ChannelConfTypeDef sConfig;

    /**Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion) 
    */
  hadc3.Instance = ADC3;
  hadc3.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc3.Init.Resolution = ADC_RESOLUTION_12B;
  hadc3.Init.ScanConvMode = DISABLE;
  hadc3.Init.ContinuousConvMode = DISABLE;
  hadc3.Init.DiscontinuousConvMode = ENABLE;
  hadc3.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc3.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc3.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc3.Init.NbrOfConversion = 1;
  hadc3.Init.DMAContinuousRequests = ENABLE;
  hadc3.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc3) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
    */
  sConfig.Channel = ADC_CHANNEL_2;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* TIM2 init function */
static void MX_TIM2_Init(void)
{

  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_OC_InitTypeDef sConfigOC;

  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 32;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 8;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  HAL_TIM_MspPostInit(&htim2);

}

/* TIM3 init function */
static void MX_TIM3_Init(void)
{

  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;

  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 32;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 8;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, Green_LED_Pin|Red_LED_Pin|EN_GATE_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, L3_Pin|L2_Pin|L1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, H3_Pin|H2_Pin|H1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : Green_LED_Pin Red_LED_Pin EN_GATE_Pin */
  GPIO_InitStruct.Pin = Green_LED_Pin|Red_LED_Pin|EN_GATE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : L3_Pin L2_Pin L1_Pin */
  GPIO_InitStruct.Pin = L3_Pin|L2_Pin|L1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : H3_Pin H2_Pin H1_Pin */
  GPIO_InitStruct.Pin = H3_Pin|H2_Pin|H1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    while(1)
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
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
