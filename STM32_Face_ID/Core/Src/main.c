#include "main.h"
#include "st7789.h"


SPI_HandleTypeDef hspi1;
DMA_HandleTypeDef hdma_spi1_tx;

UART_HandleTypeDef huart1;


void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_SPI1_Init(void);
static void MX_USART1_UART_Init(void);

void LL_mDelay(uint32_t Delay);
void Delay_ms(uint32_t);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);

uint8_t Read_DT;
uint8_t array[20];
uint8_t index_ = 0;

uint8_t Flag_FaceID = 0;
uint8_t Flag_detection = 0;
uint8_t Flag_recognition = 0;
uint8_t Flag_eroll = 0;
uint8_t Flag_delete_ID = 0;

int main(void)
{

  HAL_Init();


  SystemClock_Config();


  MX_GPIO_Init();
  MX_DMA_Init();
  MX_SPI1_Init();
  MX_USART1_UART_Init();
	ST7789_Init();
	ST7789_FillScreen(BLACK);
	
	
	
//	ST7789_WriteString(20, 00, "Truong - DH - GTVT", Font_11x18, WHITE, BLACK);
//	LL_mDelay(1000);
//	ST7789_WriteString(20, 30, "DO AN TOT NGHIEP", Font_11x18, WHITE, BLACK);
//	LL_mDelay(1000);	
//	ST7789_WriteString(30, 60, "  LOOK_FACE_ID", Font_11x18, WHITE, BLACK);
//	LL_mDelay(1000);
//	ST7789_WriteString(50, 80, "   Le Van Dat  ", Font_7x10, WHITE, BLACK);
//	LL_mDelay(1000);
//	ST7789_WriteString(50, 100, "  Le Van Cuong ", Font_7x10, WHITE, BLACK);
//	LL_mDelay(1000);	
//	ST7789_WriteString(50, 120, "Nghiem Do Tuan Tu", Font_7x10, WHITE, BLACK);
//	LL_mDelay(1000);
	lcd_clear(BLACK);
//	ST7789_FillScreen(BLACK);
//	ST7789_WriteString(10, 00, "DATN - FACE_ID", Font_16x26, RED, BLACK);
//	LL_mDelay(1000);
//	ST7789_WriteString(70, 50, "STATUS", Font_16x26, BLUE, BLACK);
//	LL_mDelay(1000);
	ST7789_WriteString(30, 90, 	"Detection  : OFF", Font_11x18, WHITE, BLACK);	
	ST7789_WriteString(30, 120, "Recognition: OFF", Font_11x18, YELLOW, BLACK);	
	ST7789_WriteString(30, 150, "Eroll_ID   : OFF", Font_11x18, LBBLUE, BLACK);	
	ST7789_WriteString(30, 180, "Delete_ID  : ---", Font_11x18, GREEN, BLACK);	
	ST7789_WriteString(30, 210, "Flast      : OFF", Font_11x18, TRACE, BLACK);	
	
  while (1)
  {
//		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);
//		LL_mDelay(3000);
//		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);
//		LL_mDelay(3000);
		
		HAL_UART_Receive_IT(&huart1, &Read_DT, 1);
//		if(Flag_FaceID == 1 && Flag_delete_ID == 0)
//		{
//			lcd_clear(BLACK);
//			ST7789_FillScreen(WHITE);
//			ST7789_WriteString(10, 90, 	"FACE ID: PASS", Font_16x26, GREEN, WHITE);
//			for(int i = 0; i <= 3; i++)
//			{
//				for(int j = 0; j< 10; j++)
//				{
//					ST7789_WriteNumber(111, 150, i, RED, WHITE);
//					ST7789_WriteNumber(130, 150, j, RED, WHITE);
//					LL_mDelay(1000);
//				}
//			}	
//			Flag_FaceID = 0;
//		}
		
		if(Flag_detection == 1 && Flag_delete_ID == 0){
			ST7789_WriteString(173, 90, 	"ON ", Font_11x18, WHITE, BLACK);

		}
		if(Flag_recognition == 1){
			ST7789_WriteString(173, 120, 	"ON ", Font_11x18, YELLOW, BLACK);
		}
		if(Flag_eroll == 1 && Flag_delete_ID == 0){
			ST7789_WriteString(173, 150, 	"ON ", Font_11x18, LBBLUE, BLACK);
		}
		if(Flag_delete_ID == 1){
			ST7789_WriteString(173, 180, 	"OK_RST", Font_11x18, GREEN, BLACK);
		}		
		
		
		if(Flag_detection == 0) {
			ST7789_WriteString(173, 90, 	"OFF ", Font_11x18, WHITE, BLACK);
		}
		if(Flag_recognition == 0) {
			ST7789_WriteString(173, 120, 	"OFF ", Font_11x18, YELLOW, BLACK);
		}
		else if(Flag_eroll == 0) {
			ST7789_WriteString(173, 150, 	"OFF ", Font_11x18, LBBLUE, BLACK);
		}		
		
  }

}

void Delay_ms(uint32_t numb)
{
	uint32_t i,j;
	for(i = 0; i < numb; i ++)
	{
			for(j = 0; j < numb; j ++)
			{
				j++;
			}
		i++;
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

  UNUSED(huart);
	if(huart->Instance == USART1)
	{
		array[index_++] = Read_DT;

		if( Read_DT == '1')
		{
			Flag_detection = 1;
		
		}
		if( Read_DT == 'b')
		{
			Flag_recognition = 1;
			
		}
		if( Read_DT == 'c')
		{
			Flag_eroll = 1;
		}
		if( Read_DT == 'd')
		{
			Flag_delete_ID = 1;
		}
		
		
		if( Read_DT == 'x')
		{
			Flag_detection = 0;
		}
		if( Read_DT == 'y')
		{
			Flag_recognition = 0;
		}
		if( Read_DT == 'z')
		{
			Flag_eroll = 0;
		}
		HAL_UART_Receive_IT(&huart1, &Read_DT, 1);
	}
	
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}


static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_HIGH;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel3_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_12, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11|GPIO_PIN_0, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_SET);

  /*Configure GPIO pins : PA0 PA1 PA2 PA3 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PA4 PA5 PA6 */
  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PB10 */
  GPIO_InitStruct.Pin = GPIO_PIN_11|GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PA11 PA12 */
  GPIO_InitStruct.Pin = GPIO_PIN_11|GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

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
void LL_mDelay(uint32_t Delay)
{
  __IO uint32_t  tmp = SysTick->CTRL;  /* Clear the COUNTFLAG first */
  /* Add this code to indicate that local variable is not used */
  ((void)tmp);

  /* Add a period to guaranty minimum wait */
  if (Delay < LL_MAX_DELAY)
  {
    Delay++;
  }

  while (Delay)
  {
    if ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) != 0U)
    {
      Delay--;
    }
  }
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

