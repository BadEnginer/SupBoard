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
#include "usb_device.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
//#include "Display/e-ink/picture.h"
#include "app_menu/app_menu.h"
#include <stdlib.h>

//EPD
#include "Display/e-ink/EPD/Display_EPD_W21_spi.h"
#include "Display/e-ink/EPD/Display_EPD_W21.h"
#include "Display/e-ink/pic/logo_v2.h"
#include "Display/e-ink/pic/butt_ok.h"
#include "Display/e-ink/pic/butt_stop.h"
#include "Display/e-ink/pic/logo_v1.h"
#include "Display/e-ink/pic/main_display.h"
//GUI
#include "Display/e-ink/GUI/GUI_Paint.h"
#include "Display/e-ink/Fonts/fonts.h"

unsigned char BlackImage[5000];//Define canvas space

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
//uint16_t global_DAC;
sSystemState SystemState = {};
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
uint8_t devises_list[10] = {};
HAL_StatusTypeDef stateDevise;
HAL_StatusTypeDef stateEXP_GPIO;
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;
DMA_HandleTypeDef hdma_i2c1_tx;
DMA_HandleTypeDef hdma_i2c1_rx;

SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim2;
DMA_HandleTypeDef hdma_tim2_ch2_ch4;

/* Definitions for InitTask */
osThreadId_t InitTaskHandle;
const osThreadAttr_t InitTask_attributes = {
  .name = "InitTask",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for TestTimer */
osTimerId_t TestTimerHandle;
const osTimerAttr_t TestTimer_attributes = {
  .name = "TestTimer"
};
/* Definitions for BlockI2C */
osMutexId_t BlockI2CHandle;
const osMutexAttr_t BlockI2C_attributes = {
  .name = "BlockI2C"
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
void CallbackTT(void *argument);

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

  //(Perform a full-screen refresh operation after 5 partial refreshes, otherwise the residual image may not be removed.)
  //////////////////////////partial display 0~9////////////////////////////////////////////


  /*After the partial refresh, you need to use OTP to clean the screen, otherwise the screen of the electronic paper may not be removed.*/
  	/******Clear screen after Partial refresh*****/
 	EPD_init(); //EPD init
  	EPD_full_display_Clear();//EPD Clear
  	EPD_sleep();//EPD_sleep,Sleep instruction is necessary, please do not delete!!!
  	HAL_Delay(100); //2s

  	/*After the display is completed, wait for about 10 seconds before turning off the power,
  	  otherwise the electronic paper may display abnormally */

    ///////////////////////////GUI///////////////////////////////////////////////////////////////////////////////////
     //Data initialization settings
     //Paint_NewImage(BlackImage, EPD_WIDTH, EPD_HEIGHT, 270, WHITE); //Set screen size and display orientation
     //Paint_SelectImage(BlackImage);//Set the virtual canvas data storage location

      /***********String***************************/
	EPD_init(); //EPD init
	EPD_full_display(gImage_logo_v1, gImage_logo_v1, 0);//Load logo
	EPD_sleep();//EPD_sleep,Sleep instruction is necessary, please do not delete!!!
	HAL_Delay(2000);

 	EPD_init(); //EPD init
  	EPD_full_display_Clear();//EPD Clear
  	EPD_sleep();//EPD_sleep,Sleep instruction is necessary, please do not delete!!!
  	HAL_Delay(100); //2s

	EPD_init(); //EPD init
	EPD_full_display(gImage_main_display, gImage_main_display, 0);//EPD_picture1
	EPD_sleep();//EPD_sleep,Sleep instruction is necessary, please do not delete!!!

	uint8_t x = 24, y = 16;

	uint8_t x_start, x_end;
	uint8_t y_start, y_end;

	uint8_t x0_start = 64, x0_end = x + x0_start;
	uint8_t y0_start = 72, y0_end = y + y0_start;

	uint8_t x1_start = 64,  x1_end = x + x1_start;
	uint8_t y1_start = 96, y1_end = y + y1_start;

	uint8_t x2_start = 64, x2_end = x + x2_start;
	uint8_t y2_start = 120, y2_end = y + y2_start;

	display_number(x0_start, x0_end, y0_start, y0_end, 0, 0, 0);
	display_number(x1_start, x1_end, y1_start, y1_end, 0, 0, 0);
	display_number(x2_start, x2_end, y2_start, y2_end, 0, 0, 0);

	x_start = x0_start; x_end = x0_end;
	y_start = y0_start; y_end = y0_end;

	for(uint8_t j = 1; j < 10; j++){
		x_start = x0_start; x_end = x0_end;
		y_start = y0_start; y_end = y0_end;
		for(uint8_t i = 1; i <= 10; i++){
			display_number(x_start, x_end, y_start, y_end, i, i-1, 1);
			HAL_Delay(100);
		}
		display_number(x_start, x_end, y_start, y_end, 0, 9, 1);
		x_start = x1_start; x_end = x1_end;
		y_start = y1_start; y_end = y1_end;
		display_number(x_start, x_end, y_start, y_end, j, j-1, 1);

	}
	/*
	for(uint8_t i = 1; i < 10; i++){
		display_number(x0_start, x0_end, y0_start, y0_end, i, i-1, 1);
		HAL_Delay(10-0);
	}
	*/
	HAL_Delay(2000);

  uint8_t j = 0;
  for(uint8_t i = 1; i < 127; i ++){
	  stateDevise = HAL_I2C_IsDeviceReady(&hi2c1, (i << 1), 2, 5);
	  if(stateDevise == HAL_OK){
		  devises_list[j] = i;
		  j++;
	  }
  }
  HAL_Delay(1000);
  ssd1306_Init();
  ssd1306_Fill(White);
  HAL_Delay(500);
  ssd1306_Fill(Black);
  SystemState.DisplayState.state = LOAD;
  ssd1306_SetCursor(5, 10);
  ssd1306_WriteString("JetPro,Bro!", Font_11x18, White);
  ssd1306_SetCursor(3, 40);
  ssd1306_WriteString(" Tap Start ", Font_11x18, White);
  ssd1306_UpdateScreen();
	SystemState.BattaryData.BatteryType = BATTARY_TYPE_LIPO;
	SystemState.BattaryData.numCell = NUM_CELL_4S;
	SystemState.BattaryData.MaxCellVoltage = 4200;
	SystemState.BattaryData.MinCellVoltage = 3000;
  HAL_Delay(1500);
  BlockI2CHandle = osMutexNew(&BlockI2C_attributes);
  //osStatus_t status = osMutexAcquire(BlockI2CHandle, 1000);
  //osMutexRelease (BlockI2CHandle);

  //HAL_Delay(1000);

  //EPD_HW_Init(); //Electronic paper initialization
  //EPD_WhiteScreen_ALL(gqImage_R,gqImage_R); //Refresh the picture in full screen
  //EPD_WhiteScreen_ALL(default_dis,gqImage_R);
  //EPD_DeepSleep(); //Enter deep sleep,Sleep instruction is necessary, please do not delete!!!

  //ADS1115_setConversionReadyPin(pADS);

  //Clean
  //EPD_HW_Init(); //Electronic paper initialization
  //EPD_WhiteScreen_ALL_Clean();
  //EPD_DeepSleep(); //Enter deep sleep,Sleep instruction is necessary, please do not delete!!!
  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();
  /* Create the mutex(es) */
  /* creation of BlockI2C */
  BlockI2CHandle = osMutexNew(&BlockI2C_attributes);

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* Create the timer(s) */
  /* creation of TestTimer */
  TestTimerHandle = osTimerNew(CallbackTT, osTimerPeriodic, NULL, &TestTimer_attributes);

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
  initUserTasks();
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
  RCC_OscInitStruct.PLL.PLLQ = 3;
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
  /* DMA1_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream0_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream0_IRQn);
  /* DMA1_Stream1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream1_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream1_IRQn);
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
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, EPD_CS_Pin|EPD_Reset_Pin|EPD_Data_Control_Pin|SSPI_SCK_Pin
                          |SSPI_MISO_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC14 */
  GPIO_InitStruct.Pin = GPIO_PIN_14;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : EPD_CS_Pin EPD_Reset_Pin EPD_Data_Control_Pin SSPI_SCK_Pin
                           SSPI_MISO_Pin */
  GPIO_InitStruct.Pin = EPD_CS_Pin|EPD_Reset_Pin|EPD_Data_Control_Pin|SSPI_SCK_Pin
                          |SSPI_MISO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : EPD_Busy_Pin PB3_LB_Pin PB4_EB_Pin PB5_EP_Pin */
  GPIO_InitStruct.Pin = EPD_Busy_Pin|PB3_LB_Pin|PB4_EB_Pin|PB5_EP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : EXT_button_on_Pin */
  GPIO_InitStruct.Pin = EXT_button_on_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(EXT_button_on_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PA15_EM_Pin */
  GPIO_InitStruct.Pin = PA15_EM_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(PA15_EM_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PB8_EM_Pin */
  GPIO_InitStruct.Pin = PB8_EM_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(PB8_EM_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	if(GPIO_Pin == EXT_button_on_Pin){
			buttonEnSet();
	}
}



void test_i2c_dev(){
	HAL_StatusTypeDef stateI2cADC;
	HAL_StatusTypeDef stateI2cDAC;
	HAL_StatusTypeDef stateI2cENC;
	HAL_StatusTypeDef stateI2cDIS;
	  // Подсчёт устройств в сети I2C 60-display, 72-ацп, 54-encoder, 96 -dac/
    if(osMutexAcquire(BlockI2CHandle, 1000) == osOK){
    	stateI2cENC = HAL_I2C_IsDeviceReady(&hi2c1, (54 << 1), 2, 5);
		stateI2cDIS = HAL_I2C_IsDeviceReady(&hi2c1, (60 << 1), 2, 5);
		stateI2cADC = HAL_I2C_IsDeviceReady(&hi2c1, (72 << 1), 2, 5);
		stateI2cDAC = HAL_I2C_IsDeviceReady(&hi2c1, (96 << 1), 2, 5);
		osMutexRelease(BlockI2CHandle);
		if(stateI2cENC == HAL_OK){
			SystemState.MagnitEncoderData.readyENCODER = DEVICE_READY;
			SystemState.ErrorState.error_encoder = DEVISE_WORK;
		}
		else {
			SystemState.MagnitEncoderData.readyENCODER = DEVICE_NO_ANSWER;
			SystemState.ErrorState.error_encoder = DEVISE_NO_ANSWER;
		}
		if(stateI2cADC == HAL_OK){
			SystemState.AdcData.readyADC = DEVICE_READY;
			SystemState.ErrorState.error_ADC = DEVISE_OK;
		}
		else {
			SystemState.AdcData.readyADC = DEVICE_NO_ANSWER;
			SystemState.ErrorState.error_ADC = DEVISE_ERROR;
		}

		if(stateI2cDAC == HAL_OK){
			SystemState.DacData.readyDAC = DEVICE_READY;
			SystemState.ErrorState.error_DAC = DEVISE_OK;
		}
		else {
			SystemState.DacData.readyDAC = DEVICE_NO_ANSWER;
			SystemState.ErrorState.error_DAC = DEVISE_ERROR;
		}

		if(stateI2cDIS == HAL_OK){
			SystemState.DisplayState.readyDISPLAY = DEVICE_READY;
			SystemState.ErrorState.error_DISPLAY = DEVISE_OK;
		}
		else {
			SystemState.DisplayState.readyDISPLAY = DEVICE_NO_ANSWER;
			SystemState.ErrorState.error_DISPLAY = DEVISE_ERROR;
		}
    }
}

void OutputErrorI2C(){
	  ssd1306_Fill(Black);
	  ssd1306_SetCursor(2, 22);
	  if(SystemState.DacData.readyDAC == DEVICE_READY)
		  ssd1306_WriteString("DAC:On", Font_11x18, White);
	  else
		  ssd1306_WriteString("DAC:Er", Font_11x18, White);

	  ssd1306_SetCursor(2+(11*7), 22);
	  if(SystemState.AdcData.readyADC == DEVICE_READY)
		  ssd1306_WriteString("ADC:On", Font_11x18, White);
	  else
		  ssd1306_WriteString("ADC:Er", Font_11x18, White);

	  ssd1306_SetCursor(2, 42);
	  if(SystemState.DisplayState.readyDISPLAY == DEVICE_READY)
		  ssd1306_WriteString("DIS:On", Font_11x18, White);
	  else
		  ssd1306_WriteString("DIS:Er", Font_11x18, White);

	  ssd1306_SetCursor(2+(11*7), 42);
	  if(SystemState.MagnitEncoderData.readyENCODER == DEVICE_READY)
		  ssd1306_WriteString("ENC:On", Font_11x18, White);
	  else
		  ssd1306_WriteString("ENC:Er", Font_11x18, White);
	  ssd1306_UpdateScreen();
  }
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
  /* init code for USB_DEVICE */
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN 5 */
  /* Infinite loop */
	  for(;;)
	  {
		test_i2c_dev();
	    osDelay(1000);
	  }
  /* USER CODE END 5 */
}

/* CallbackTT function */
void CallbackTT(void *argument)
{
  /* USER CODE BEGIN CallbackTT */

  /* USER CODE END CallbackTT */
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
