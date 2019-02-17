/*
IOController-FW - the firmware for STM32 microcontrollers which allows controls IO(GPIO) via USB 
Copyright (C) 2017 Stepan Hamouz, s.hamouz@gmail.com

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef SUPPORTLIB_CONTROL_HPP_
#define SUPPORTLIB_CONTROL_HPP_

#include "stm32f0xx_hal.h"
#include "cmsis_os.h"




class control{

private:
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
	  PIN_IO0  = 0,
	  PIN_IO1  = 1,
	  PIN_IO2  = 2,
	  PIN_IO3  = 3,
	  PIN_IO4  = 4,
	  PIN_IO5  = 5,
	  PIN_IO6  = 6,
	  PIN_IO7  = 7,
	  PIN_IO8  = 8,
	  PIN_IO9  = 9,
	  PIN_IO10 = 10,
	  PIN_IO11 = 11,
	  PIN_IO12 = 12,
	  PIN_IO13 = 13,
	  PIN_COUNT
	}ePins;






public:
	typedef struct
	{
	  ePins pinID;
	  GPIO_TypeDef* GPIOx;
	  uint16_t GPIO_Pin;
	  char name[10];
	}sPinConfig;

	static uint32_t pinModeConv[MODE_COUNT];
	static sPinConfig pinsConfig[PIN_COUNT];
	static uint32_t pinPushConv[PUSH_COUNT];

	control(void);
	uint8_t enableRelay(uint8_t relay_id);
	uint8_t disableRelay(uint8_t relay_id);
	uint8_t isRelayEnabled(uint8_t relay_id);
	uint8_t configurePin(uint8_t arg_id, ePinMode arg_mode, ePinPush arg_push);
	uint8_t controllPin(uint8_t arg_id, uint8_t arg_state);
	void reportPins(uint8_t arg_id);
	uint8_t controllPinSetBin(uint16_t tempNumber);
	uint8_t controllPinEnableBin(uint16_t tempNumber);
	uint8_t controllPinDisableBin(uint16_t tempNumber);
	void reportBin();
	void cyclic(void);
};



extern osMessageQId cdcDataHandle;


#endif /* SUPPORTLIB_CONTROL_HPP_ */
