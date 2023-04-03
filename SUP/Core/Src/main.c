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
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Display/e-ink/picture.h"
#include <stdlib.h>

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
I2C_HandleTypeDef hi2c1;

SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim2;
DMA_HandleTypeDef hdma_tim2_ch2_ch4;

/* Definitions for InitTask */
osThreadId_t InitTaskHandle;
const osThreadAttr_t InitTask_attributes = {
  .name = "InitTask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_I2C1_Init(void);
static void MX_SPI1_Init(void);
static void MX_TIM2_Init(void);
void StartInitTask(void *argument);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  //MCP4725 myMCP4725;
  //MCP4725_setValue(&myMCP4725, 2048, MCP4725_FAST_MODE, MCP4725_POWER_DOWN_OFF);
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_I2C1_Init();
  MX_SPI1_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  uint8_t num[10] ={0};
  uint8_t coutn = 0;
  HAL_StatusTypeDef type;
  // Подсчёт устройств в сети I2C 60-display, 72-ацп, 54-encoder, 96 -dac/
  for(uint8_t i = 1; i < 127 ; i++){
	  type = HAL_I2C_IsDeviceReady(&hi2c1, (i << 1), 2, 10);
	  if(type == HAL_OK){
		  num[coutn++] = i;
	  }
  }
  ARGB_Init();  // Initialization
  //ARGB_STATE ARGB_GetState;
  ARGB_Clear();
  while (ARGB_Show() != ARGB_OK);
  ARGB_SetBrightness(80);
 // ARGB_SetRGB(0, 255, 0, 128);

  // Update - Option 1
  //extern  unsigned char *gImage_BW;
  //extern  unsigned char *gImage_R;
  ssd1306_Init();
  ssd1306_Fill(Black);
  ssd1306_SetCursor(5, 10);
  ssd1306_WriteString("JetPro,Bro!", Font_11x18, White);
  ssd1306_UpdateScreen();
  HAL_Delay(1000);
  ssd1306_Fill(Black);
  ssd1306_UpdateScreen();

  ssd1306_SetCursor(0, 0);
    ssd1306_WriteString("TOK - ", Font_11x18, White);
    ssd1306_SetCursor(100, 0);
                   ssd1306_WriteString("A", Font_11x18, White);
    ssd1306_SetCursor(0, 19);
      ssd1306_WriteString("BAT - ", Font_11x18, White);
      ssd1306_SetCursor(100, 19);
            ssd1306_WriteString("%", Font_11x18, White);
  //EPD_HW_Init(); //Electronic paper initialization
  //EPD_WhiteScreen_ALL(gqImage_R,gqImage_R); //Refresh the picture in full screen
  //EPD_WhiteScreen_ALL(default_dis,gqImage_R);
  //EPD_DeepSleep(); //Enter deep sleep,Sleep instruction is necessary, please do not delete!!!
  //driver_delay_xms(5000);
  ADS1115_Config_t configReg;
  ADS1115_Handle_t *pADS;
	#define ADS1115_ADR 72
  configReg.channel = CHANNEL_AIN0_GND;
  configReg.pgaConfig = PGA_6_144;
  configReg.operatingMode = MODE_CONTINOUS;
  configReg.dataRate = DRATE_250;
  configReg.compareMode = COMP_HYSTERESIS;
  configReg.polarityMode = POLARITY_ACTIVE_LOW;
  configReg.latchingMode = LATCHING_NONE;
  configReg.queueComparator = QUEUE_ONE;
	uint8_t bytes[3] = {0};

	prepareConfigFrame(bytes, configReg);
	configReg.channel = CHANNEL_AIN1_GND;
	prepareConfigFrame(bytes, configReg);
	configReg.channel = CHANNEL_AIN2_GND;
	prepareConfigFrame(bytes, configReg);
	configReg.channel = CHANNEL_AIN3_GND;


  pADS = ADS1115_init(&hi2c1, ADS1115_ADR, configReg);
  ADS1115_updateConfig(pADS, configReg);
  ADS1115_startContinousMode(pADS);
  //ADS1115_setConversionReadyPin(pADS);
  float data_from_adc_0 = 0;
  MCP4725 myMCP4725 = MCP4725_init(&hi2c1, MCP4725A0_ADDR_A00, 3.30);
	// Check the connection:
	uint8_t state_dac = MCP4725_isConnected(&myMCP4725);
	MCP4725_setValue(&myMCP4725, 2048, MCP4725_FAST_MODE, MCP4725_POWER_DOWN_OFF);
  for(uint8_t temp = 0; temp < 80; temp++){
	  HAL_Delay(50);
	  data_from_adc_0 = (ADS1115_getData(pADS));
	  temp++;
  }
  configReg.channel = CHANNEL_AIN1_GND;
  ADS1115_updateConfig(pADS, configReg);
  for(uint8_t temp = 0; temp < 80; temp++){
	  HAL_Delay(50);
	  data_from_adc_0 = (ADS1115_getData(pADS));
	  temp++;
  }

  configReg.channel = CHANNEL_AIN2_GND;
  ADS1115_updateConfig(pADS, configReg);
  for(uint8_t temp = 0; temp < 80; temp++){
	  HAL_Delay(50);
	  data_from_adc_0 = (ADS1115_getData(pADS));
	  temp++;
  }


  uint16_t angle = 0;
  uint16_t raw_angle = 0;
  uint16_t staty_encoder = 0;
  for(uint8_t test = 0; test < 50; test++){
	  angle = AS5600_GetAngle();
	  raw_angle = AS5600_GetRawAngle();
	  staty_encoder = AS5600_GetStatus();
	  HAL_Delay(10);
  }
  uint8_t color = 0;
while(1){
	for(uint8_t i = 0 ; i < 11; i++){
		  char sym[3];
		  char car[3];
		  itoa(i,sym,10);
		  itoa(i*10,car,10);
		  	  ssd1306_SetCursor(67, 0);
		      ssd1306_WriteString(sym, Font_11x18, White);
		      ssd1306_SetCursor(67, 19);
		      ssd1306_WriteString(car, Font_11x18, White);
		      ssd1306_UpdateScreen();
		      HAL_Delay(200);
			  ARGB_SetHSV(i, color, 250	, 250);
			  color+=10;
			  while (ARGB_Show() != ARGB_OK);
	 }

	 for(uint8_t i = 10 ; i < 1; i--){
		  char sym[3];
		  char car[3];
		  itoa(i,sym,10);
		  itoa(i*10,car,10);
		  	  ssd1306_SetCursor(67, 0);
		      ssd1306_WriteString(sym, Font_11x18, White);
		      ssd1306_SetCursor(67, 19);
		      ssd1306_WriteString(car, Font_11x18, White);
		      ssd1306_UpdateScreen();
		      HAL_Delay(200);
	 }
}


  //Clean
  //EPD_HW_Init(); //Electronic paper initialization
  //EPD_WhiteScreen_ALL_Clean();
  //EPD_DeepSleep(); //Enter deep sleep,Sleep instruction is necessary, please do not delete!!!
  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of InitTask */
  InitTaskHandle = osThreadNew(StartInitTask, NULL, &InitTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

  /* Start scheduler */
  osKernelStart();
  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 144;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
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
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128;
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
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 89;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */
  HAL_TIM_MspPostInit(&htim2);

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream6_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream6_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream6_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, EPD_CS_Pin|EPD_Reset_Pin|EPD_Data_Control_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : EPD_CS_Pin EPD_Reset_Pin EPD_Data_Control_Pin */
  GPIO_InitStruct.Pin = EPD_CS_Pin|EPD_Reset_Pin|EPD_Data_Control_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : EPD_Busy_Pin */
  GPIO_InitStruct.Pin = EPD_Busy_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(EPD_Busy_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartInitTask */
/**
  * @brief  Function implementing the InitTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartInitTask */
void StartInitTask(void *argument)
{
  /* USER CODE BEGIN 5 */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END 5 */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM9 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM9) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

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
