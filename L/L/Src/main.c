
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
 * COPYRIGHT(c) 2018 STMicroelectronics
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

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
static void MX_USART2_UART_Init(void);                                    
void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);


/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
int arrayPos1 = 0; // keeps track of the first pwm's position in the look up table
int arrayPos2 = 162; // keeps track of the second pwm's position in the look up table
int arrayPos3 = 333; // keeps track of the third pwm's position in the look up table
short set1 = 0; // dictates whether the pwm writes to h1 or l1
short set2 = 0; // dictates whether the pwm writes to h2 or l2
short set3 = 0; // dictates whether the pwm writes to h3 or l3
short increment = 10; // dictates how quickly the pwm moves through the lookup table

int lookUp[] = {
        50,50,50,50,51,51,51,51,51,51,52,52,52,52,52,52,53,53,53,53,
        53,53,53,54,54,54,54,54,54,55,55,55,55,55,55,55,56,56,56,56,
        56,56,57,57,57,57,57,57,58,58,58,58,58,58,58,59,59,59,59,59,
        59,60,60,60,60,60,60,60,61,61,61,61,61,61,62,62,62,62,62,62,
        62,63,63,63,63,63,63,63,64,64,64,64,64,64,65,65,65,65,65,65,
        65,66,66,66,66,66,66,66,67,67,67,67,67,67,68,68,68,68,68,68,
        68,69,69,69,69,69,69,69,70,70,70,70,70,70,70,71,71,71,71,71,
        71,71,72,72,72,72,72,72,72,73,73,73,73,73,73,73,74,74,74,74,
        74,74,74,74,75,75,75,75,75,75,75,76,76,76,76,76,76,76,77,77,
        77,77,77,77,77,77,78,78,78,78,78,78,78,78,79,79,79,79,79,79,
        79,80,80,80,80,80,80,80,80,81,81,81,81,81,81,81,81,82,82,82,
        82,82,82,82,82,82,83,83,83,83,83,83,83,83,84,84,84,84,84,84,
        84,84,84,85,85,85,85,85,85,85,85,85,86,86,86,86,86,86,86,86,
        86,87,87,87,87,87,87,87,87,87,88,88,88,88,88,88,88,88,88,88,
        89,89,89,89,89,89,89,89,89,89,90,90,90,90,90,90,90,90,90,90,
        90,91,91,91,91,91,91,91,91,91,91,91,92,92,92,92,92,92,92,92,
        92,92,92,92,93,93,93,93,93,93,93,93,93,93,93,93,94,94,94,94,
        94,94,94,94,94,94,94,94,94,94,95,95,95,95,95,95,95,95,95,95,
        95,95,95,95,96,96,96,96,96,96,96,96,96,96,96,96,96,96,96,96,
        96,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,98,
        98,98,98,98,98,98,98,98,98,98,98,98,98,98,98,98,98,98,98,98,
        98,98,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,
        99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,100,100,100,100,100,
        100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,
        100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,
        100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,
        100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,
        100,100,100,100,100,100,99,99,99,99,99,99,99,99,99,99,99,99,99,99,
        99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,98,
        98,98,98,98,98,98,98,98,98,98,98,98,98,98,98,98,98,98,98,98,
        98,98,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,
        96,96,96,96,96,96,96,96,96,96,96,96,96,96,96,96,96,95,95,95,
        95,95,95,95,95,95,95,95,95,95,95,94,94,94,94,94,94,94,94,94,
        94,94,94,94,94,93,93,93,93,93,93,93,93,93,93,93,93,92,92,92,
        92,92,92,92,92,92,92,92,92,91,91,91,91,91,91,91,91,91,91,91,
        90,90,90,90,90,90,90,90,90,90,90,89,89,89,89,89,89,89,89,89,
        89,88,88,88,88,88,88,88,88,88,88,87,87,87,87,87,87,87,87,87,
        86,86,86,86,86,86,86,86,86,85,85,85,85,85,85,85,85,85,84,84,
        84,84,84,84,84,84,84,83,83,83,83,83,83,83,83,82,82,82,82,82,
        82,82,82,82,81,81,81,81,81,81,81,81,80,80,80,80,80,80,80,80,
        79,79,79,79,79,79,79,78,78,78,78,78,78,78,78,77,77,77,77,77,
        77,77,77,76,76,76,76,76,76,76,75,75,75,75,75,75,75,74,74,74,
        74,74,74,74,74,73,73,73,73,73,73,73,72,72,72,72,72,72,72,71,
        71,71,71,71,71,71,70,70,70,70,70,70,70,69,69,69,69,69,69,69,
        68,68,68,68,68,68,68,67,67,67,67,67,67,66,66,66,66,66,66,66,
        65,65,65,65,65,65,65,64,64,64,64,64,64,63,63,63,63,63,63,63,
        62,62,62,62,62,62,62,61,61,61,61,61,61,60,60,60,60,60,60,60,
        59,59,59,59,59,59,58,58,58,58,58,58,58,57,57,57,57,57,57,56,
        56,56,56,56,56,55,55,55,55,55,55,55,54,54,54,54,54,54,53,53,
        53,53,53,53,53,52,52,52,52,52,52,51,51,51,51,51,51,50,50,50,
        };
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
    MX_USART2_UART_Init();
    /* USER CODE BEGIN 2 */
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1); //initializes the pwm channels
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
    HAL_TIM_Base_Start_IT(&htim2);

    //    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, 1); // turns on en-gate, allows for writing to motor pins

    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 1);
    while (1)
        {
            /* USER CODE END WHILE */

            /* USER CODE BEGIN 3 */
            if(set1){
                    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, ((TIM2->CNT < TIM2->CCR1)? 1 : 0));
            }else{
                    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, ((TIM2->CNT < TIM2->CCR1)? 1 : 0));
            }
            if(set2){
                    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, ((TIM2->CNT < TIM2->CCR2)? 1 : 0));
            }else{
                    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, ((TIM2->CNT < TIM2->CCR2)? 1 : 0));
            }
            if(set3){
                    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, ((TIM2->CNT < TIM2->CCR3)? 1 : 0));
            }else{
                    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, ((TIM2->CNT < TIM2->CCR3)? 1 : 0));
            }
            /* USER CODE END 3 */

        }
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
    arrayPos1 += increment; // moves the value of the array up
    arrayPos2 += increment; // moves the value of the array up
    arrayPos3 += increment; // moves the value of the array up

    if(arrayPos1 >= 1000){
            arrayPos1 = 0;

            set1 = !set1;

    }
    else if(arrayPos2 >= 1000){
            arrayPos2 = 0;

            set2 = !set2;
    }
    else if(arrayPos3 >= 1000){
            arrayPos3 = 0;

            set3 = !set3;
    }
    TIM2->CCR1 = lookUp[arrayPos1];
    TIM2->CCR2 = lookUp[arrayPos2];
    TIM2->CCR3 = lookUp[arrayPos3];
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
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM = 16;
    RCC_OscInitStruct.PLL.PLLN = 336;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
    RCC_OscInitStruct.PLL.PLLQ = 4;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
        {
            _Error_Handler(__FILE__, __LINE__);
        }

    /**Initializes the CPU, AHB and APB busses clocks
     */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
            |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
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

/* TIM2 init function */
static void MX_TIM2_Init(void)
{

    TIM_ClockConfigTypeDef sClockSourceConfig;
    TIM_MasterConfigTypeDef sMasterConfig;
    TIM_OC_InitTypeDef sConfigOC;

    htim2.Instance = TIM2;
    htim2.Init.Prescaler = 39999;
    htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim2.Init.Period = 100;
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

/* USART2 init function */
static void MX_USART2_UART_Init(void)
{

    huart2.Instance = USART2;
    huart2.Init.BaudRate = 115200;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart2) != HAL_OK)
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
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOA, LD2_Pin|L3_Pin|L2_Pin|H1_Pin
                      |H2_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(H3_GPIO_Port, H3_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(L1_GPIO_Port, L1_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin : B1_Pin */
    GPIO_InitStruct.Pin = B1_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pins : LD2_Pin L3_Pin L2_Pin H1_Pin
                           H2_Pin */
    GPIO_InitStruct.Pin = LD2_Pin|L3_Pin|L2_Pin|H1_Pin
            |H2_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /*Configure GPIO pin : H3_Pin */
    GPIO_InitStruct.Pin = H3_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(H3_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pin : L1_Pin */
    GPIO_InitStruct.Pin = L1_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(L1_GPIO_Port, &GPIO_InitStruct);

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
