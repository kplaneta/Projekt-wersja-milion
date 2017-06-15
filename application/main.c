#include "stm32f4xx_hal.h"
#include "GUI.h"
#include "cmsis_os2.h"


void SystemClock_Config(void);
void LED_Init(void);
void GUIThread(void* params);
void taskLED(void* params);
void myThread(void *params);
void DisplayBackground(void);

osMutexId_t myMutex;

int main(void)
{
	    // Initialize the RTOS Kernel.
    osKernelInitialize();
    // Initialize STM32Cube HAL library
    HAL_Init();

		LED_Init();
    // Initialize PLL
    SystemClock_Config();
	
		myMutex = osMutexNew(NULL);
    // Create and run the GUI thread
   // osThreadNew(GUIThread, NULL, NULL);
	  osThreadNew(taskLED, NULL, NULL);
		osThreadNew(myThread, NULL, NULL);
    // Start the OS
    osKernelStart();
	

	//GUIThread(NULL);
	return 0;
}

void GUIThread(void* params) {                    

		// Initialize the Graphics Component
		GUI_Init();
    // Put some string to display			
  //  GUI_DispString("Hello World!");
    while (1) {
			if(osOK == osMutexAcquire(myMutex, 100))
			{
       /* // Execute all GUI jobs ... Return 0 if nothing was done
        GUI_Exec();
        // Nothing left to do for the moment ... Idle processing
        GUI_X_ExecIdle();*/
				GUI_DispString("Hello World!");
				osDelay(200);
				osMutexRelease(myMutex);
			}
    }
}

void DisplayBackground(void)
{
	//GUI_Init();
	GUI_DrawRect(10, 10, 230,  310);
}

void myThread(void *params)
{
	GUI_Init();
	DisplayBackground();
	while(1)
	{
		if(osOK == osMutexAcquire(myMutex, 100))
		{
			 GUI_SetTextMode(GUI_TM_NORMAL);
			GUI_DispStringAt("Tekst2", 30, 60);
			GUI_DispStringHCenterAt("Oscyloskop", 120, 150);
			GUI_DispNextLine();
			GUI_DispFloat(12.5, 4);
			GUI_DispNextLine();
			osDelay(200);
			GUI_DrawHLine(120, 0, 160);
			osMutexRelease(myMutex);
		}
	}
}

void taskLED(void* params)
{
    // Toggle the LED on pin PG.13
    while (1) {
        // toggle the LED
        HAL_GPIO_TogglePin(GPIOG, GPIO_PIN_13);
        // introduce some delay
        osDelay(500);
    } // while (1)
} /* taskLED */

void SystemClock_Config(void) {
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
 
  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();
 
  /* The voltage scaling allows optimizing the power consumption when the
     device is clocked below the maximum system frequency (see datasheet). */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
 
  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);
 
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 |
                                RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
}

void LED_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
 
  // GPIO Ports Clock Enable
  __GPIOG_CLK_ENABLE();
 
  // Configure GPIO pin PG13
  GPIO_InitStruct.Pin   = GPIO_PIN_13;
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;        // push-pull output
  GPIO_InitStruct.Pull  = GPIO_PULLDOWN;              // pull-down enabled
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;             // analog pin bandwidth limited
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
        
} /* LED_Init */
