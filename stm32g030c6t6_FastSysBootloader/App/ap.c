/*
 * ap.c
 *
 *  Created on: Feb 16, 2023
 *      Author: baram
 */


#include "ap.h"

static void jumpToAddr(uint32_t addr);


void apInit(void)
{
  wdgInit();
  // buttonInit();
#if 0
  /* Read GPIO state */
  uint32_t gpio_state = LL_GPIO_ReadInputPort(boot_i2c1_GPIO_Port);

  /* Check GPIO state */
  if (gpio_state & boot_i2c1_Pin)
  {
    /* GPIO pin is high */
    jumpToAddr(0x8000800); // Jump To Firmware
  }
  else
  {
    /* GPIO pin is low */
    wdgBegin(2000);
    jumpToAddr(0x1FFF0000); // Jump To System Bootloader
  }
#endif

  {
    __HAL_RCC_GPIOA_CLK_ENABLE();
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_14;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  }

  //if (HAL_GPIO_ReadPin(boot_i2c1_GPIO_Port, boot_i2c1_Pin) == GPIO_PIN_RESET)
  if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_14) == GPIO_PIN_RESET)
  {
    /* low */
    HAL_GPIO_WritePin(STATUS_GPIO_Port, STATUS_Pin,GPIO_PIN_RESET);

    const uint32_t stmf030c_addr_sysmem = 0x1FFF0000;
    const uint32_t wait_download_timeout_ms = 1000*2;
    wdgBegin(wait_download_timeout_ms);
    jumpToAddr(stmf030c_addr_sysmem); // Jump To System Bootloaderdr_fw); // Jump To Firmware
  }
  else
  {
    /* GPIO pin is high */
    HAL_GPIO_WritePin(STATUS_GPIO_Port, STATUS_Pin,GPIO_PIN_SET);
    const uint32_t stmf030c_addr_fw = 0x08000800;
    jumpToAddr(stmf030c_addr_fw);

  }

}

void apMain(void)
{
  while(1)
  {

  }
}


void jumpToAddr(uint32_t addr)
{
  void (**jump_addr)(void) = (void (**)(void))(addr + 4);

  SysTick->CTRL = 0;
  SysTick->LOAD = 0;
  SysTick->VAL  = 0;

  NVIC->ICER[0] = 0xFFFFFFFF;
  __DSB();
  __ISB();

  __set_MSP(*(uint32_t *)addr);
  (*jump_addr)();
}
