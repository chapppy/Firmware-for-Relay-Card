/*
 * control.hpp
 *
 *  Created on: 5. 4. 2017
 *      Author: uzivatel
 */

#ifndef SUPPORTLIB_CONTROL_HPP_
#define SUPPORTLIB_CONTROL_HPP_

#include "stm32f0xx_hal.h"
#include "cmsis_os.h"


typedef enum {
  MODE_IN = 0,
  MODE_OUT_PP = 1,
  MODE_OUT_OD = 2,
  MODE_COUNT
}ePinMode;

typedef enum {
  PP_NO = 0,
  PP_UP = 1,
  PP_DOWN = 2,
  PUSH_COUNT
}ePinPush;

typedef enum {
  PIN_IO0 = 0,
  PIN_IO1 = 1,
  PIN_IO2 = 2,
  PIN_IO3 = 3,
  PIN_IO4 = 4,
  PIN_IO5 = 5,
  PIN_IO6 = 6,
  PIN_IO7 = 7,
  PIN_IO8 = 8,
  PIN_IO9 = 9,
  PIN_IO10 = 11,
  PIN_IO11 = 12,
  PIN_IO12 = 13,
  PIN_COUNT
}ePins;


typedef struct
{
  ePins pinID;
  GPIO_TypeDef* GPIOx;
  uint16_t GPIO_Pin;
  char name[10];
}sPinConfig;


uint8_t configurePin(uint8_t arg_id, ePinMode arg_mode, ePinPush arg_push);
uint8_t controllPin(uint8_t arg_id, uint8_t arg_state);
void reportPins(uint8_t arg_id);
void reportBin();
void controlCyclic(void);

extern osMessageQId cdcDataHandle;


#endif /* SUPPORTLIB_CONTROL_HPP_ */
