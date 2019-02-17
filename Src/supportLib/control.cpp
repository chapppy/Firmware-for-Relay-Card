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

#include "control.hpp"
#include "usbd_cdc_if.h"
#include "cString.hpp"
#include "parseString.hpp"
#include "usbd_cdc_if.h"


const char *infoArray[] = {
  "Relay Card Controller\r",
  "s.hamouz@gmail.com\r"
};


uint32_t control::pinModeConv[] = { GPIO_MODE_INPUT, GPIO_MODE_OUTPUT_PP, GPIO_MODE_OUTPUT_OD};
uint32_t control::pinPushConv[] = { GPIO_NOPULL, GPIO_PULLUP, GPIO_PULLDOWN};
control::sPinConfig control::pinsConfig[] =
{
  {PIN_IO0,  GPIOA, IO_RELAY_0_1_Pin,  "Relay0"},
  {PIN_IO1,  GPIOA, IO_RELAY_0_2_Pin,  "Relay1"},
  {PIN_IO2,  GPIOA, IO_RELAY_0_3_Pin,  "Relay2"},
  {PIN_IO3,  GPIOA, IO_RELAY_0_4_Pin,  "Relay3"},
  {PIN_IO4,  GPIOA, IO_RELAY_0_5_Pin,  "Relay4"},
  {PIN_IO5,  GPIOA, IO_RELAY_0_6_Pin,  "Relay5"},
  {PIN_IO6,  GPIOA, IO_RELAY_0_7_Pin,  "Relay6"},
  {PIN_IO7,  GPIOB, IO_RELAY_1_1_Pin,  "Relay7"},
  {PIN_IO8,  GPIOB, IO_RELAY_1_2_Pin,  "Relay8"},
  {PIN_IO9,  GPIOB, IO_RELAY_1_3_Pin,  "Relay9"},
  {PIN_IO10, GPIOB, IO_RELAY_1_4_Pin, "Relay10"},
  {PIN_IO11, GPIOB, IO_RELAY_1_5_Pin, "Relay11"},
  {PIN_IO12, GPIOB, IO_RELAY_1_6_Pin, "Relay12"},
  {PIN_IO13, GPIOA, IO_RELAY_1_7_Pin, "Relay13"},
};


control::control(void)
{

}

uint8_t control::enableRelay(uint8_t relay_id)
{
  uint8_t ret;

  if(relay_id < PIN_COUNT)
  {
    HAL_GPIO_WritePin(pinsConfig[relay_id].GPIOx , pinsConfig[relay_id].GPIO_Pin, GPIO_PIN_SET);
    ret = 1;
  }
  else
  {
	ret = 0;
  }

  return ret;
}

uint8_t control::disableRelay(uint8_t relay_id)
{
  uint8_t ret = 0;

  if(relay_id < PIN_COUNT)
  {
    HAL_GPIO_WritePin(pinsConfig[relay_id].GPIOx , pinsConfig[relay_id].GPIO_Pin, GPIO_PIN_RESET);
    ret = 1;
  }
  else
  {
	ret = 0;
  }

  return ret;
}

uint8_t control::isRelayEnabled(uint8_t relay_id)
{
  GPIO_PinState pinState;
  uint8_t ret;

  if(relay_id < PIN_COUNT)
  {
    pinState = HAL_GPIO_ReadPin(pinsConfig[relay_id].GPIOx, pinsConfig[relay_id].GPIO_Pin);
    if(pinState == GPIO_PIN_SET)
    {
      ret = 1;
    }
    else
    {
      ret = 0;
    }
  }
  else
  {
	ret = 0;
  }


  return ret;
}


uint8_t control::configurePin(uint8_t arg_id, ePinMode arg_mode, ePinPush arg_push)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  GPIO_InitStruct.Pin = pinsConfig[arg_id].GPIO_Pin;
  GPIO_InitStruct.Mode = pinModeConv[arg_mode];
  GPIO_InitStruct.Pull = pinPushConv[arg_push];
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

  if(arg_id < PIN_COUNT)
  {
    HAL_GPIO_DeInit(pinsConfig[arg_id].GPIOx, pinsConfig[arg_id].GPIO_Pin);
    HAL_GPIO_Init(pinsConfig[arg_id].GPIOx, &GPIO_InitStruct);
    CDC_Transmit_FS((uint8_t*)"As you wish!\r", strlen("As you wish!\r"));
  }
}

uint8_t control::controllPin(uint8_t arg_id, uint8_t arg_state)
{
  uint8_t buffer[32] = {0};
  cString string((char*)buffer, 32);

  if(arg_id < PIN_COUNT)
  {
    if(arg_state == 1)
    {
      enableRelay(arg_id);
      string += "Channel ";
      string.addInt8_t((uint8_t)pinsConfig[arg_id].pinID, 10);
      //string += pinsConfig[arg_id].name;
      string += " is now enabled\r";
    }
    else
    {
      disableRelay(arg_id);
      string += "Channel ";
      string.addInt8_t((uint8_t)pinsConfig[arg_id].pinID, 10);
      //string += pinsConfig[arg_id].name;
      string += " is now disabled\r";
    }
  }
  else
  {
    string += "index out of range\r";
  }

  CDC_Transmit_FS(buffer, string.getSize());

}


uint8_t control::controllPinSetBin(uint16_t tempNumber)
{
  int8_t relayNumber;
  int8_t relayState;

  for(relayNumber = 0; relayNumber <= 14; relayNumber++)
  {
	relayState = tempNumber & 0x0001;
    if(relayState == 1)
    {
      enableRelay(relayNumber);
    }
    else
    {
      disableRelay(relayNumber);
    }

    tempNumber>>=1;
  }

  reportBin();
}

uint8_t control::controllPinEnableBin(uint16_t tempNumber)
{
  int8_t relayNumber;
  int8_t relayState;

  for(relayNumber = 0; relayNumber <= 14; relayNumber++)
  {
	relayState = tempNumber & 0x0001;
    if(relayState == 1)
    {
      enableRelay(relayNumber);
    }

    tempNumber>>=1;
  }

  reportBin();
}

uint8_t control::controllPinDisableBin(uint16_t tempNumber)
{
  int8_t relayNumber;
  int8_t relayState;


  for(relayNumber = 0; relayNumber <= 14; relayNumber++)
  {
	relayState = tempNumber & 0x0001;
    if(relayState == 0)
    {
      disableRelay(relayNumber);
    }

    tempNumber>>=1;
  }

  reportBin();
}


void control::reportPins(uint8_t arg_id)
{
  uint8_t buffer[64] = {0};
  cString string((char*)buffer, 64);
  uint8_t pinState;
  uint8_t isFirst = 1;

  if(arg_id == 0xff)
  {
     for(uint8_t i=0; i<PIN_COUNT; i++)
     {
    	 pinState = isRelayEnabled(i);
         if(pinState == 1)
         {
             if(isFirst == 0)
             {
               string += ", ";
             }
             else
             {
               string += "Enabled channels ";
               isFirst = 0;
             }
             string.addInt8_t((uint8_t)pinsConfig[i].pinID, 10);
         }
     }

     if(string.getSize() == 0)
     {
        string += "all channels disabled\n";
     }
     string += "\r";
  }
  else
  {
    if(arg_id < PIN_COUNT)
    {
      pinState = isRelayEnabled(arg_id);
      if(pinState == 1)
      {
        string += "Channel ";
        string.addInt8_t((uint8_t)arg_id, 10);
        string += " is enabled\r";
      }
      else
      {
        string += "Channel ";
        string.addInt8_t((uint8_t)arg_id, 10);
        string += " is disabled\r";
      }
    }
  }

  CDC_Transmit_FS(buffer, string.getSize());
}


void control::reportBin()
{
  uint8_t buffer[32] = {0};
  cString string((char*)buffer, 32);
  uint8_t pinState;
  uint8_t pinState2;
  uint8_t output = 0;

 for(uint8_t i=0; i<PIN_COUNT; i++)
 {
   //if(pinState == 1)
   //pinState = HAL_GPIO_ReadPin(pinsConfig[i].GPIOx , pinsConfig[i].GPIO_Pin);
   pinState = isRelayEnabled(i);
   if(pinState == GPIO_PIN_SET)
   {
     output |= 1 << i;
   }
 }

 string += "Channels state 0x";
 string.addUint8_t((uint8_t)output, 16);
 string += "\r";


  CDC_Transmit_FS(buffer, string.getSize());
}

void control::cyclic(void)
{
  uint8_t dataBuff[64] = {0};
  uint8_t tempBuff[10] = {0};
  parseString<0> stringEx;
  uint8_t buffIndex = 0;
  uint8_t tempChar = 0;
  uint16_t tempNumber = 0;

  stringEx.Init((char*)dataBuff, 64);

  do
    {
    osEvent evt = osMessageGet(cdcDataHandle, osWaitForever);
    if (evt.status == osEventMessage)
    {
      dataBuff[buffIndex] = (uint8_t)evt.value.v;
      buffIndex++;
      dataBuff[buffIndex] = 0;
    }
  }while(dataBuff[buffIndex-1] != '\r');


  if(stringEx.strcmpAndShift("Enable ", strlen("Enable ")) == 1)
  {
    do
    {
      tempChar = stringEx.findNext((const char[2]){',', '\r'}, 2);
      tempNumber = stringEx.getUint(' ', tempChar);
      controllPin(tempNumber, 1);
    }while(tempChar != '\r');
  }

  if(stringEx.strcmpAndShift("Disable ", strlen("Disable ")) == 1)
  {
    do
    {
      tempChar = stringEx.findNext((const char[2]){',', '\r'}, 2);
      tempNumber = stringEx.getUint(' ', tempChar);
      controllPin(tempNumber, 0);
    }while(tempChar != '\r');
  }

  if(stringEx.strcmpAndShift("Set 0x", strlen("Set 0x")) == 1)
  {
    tempChar = stringEx.findNext((const char[1]){'\r'}, 1);
    tempNumber = stringEx.getUint('x', tempChar);
    controllPinSetBin(tempNumber);
  }

  if(stringEx.strcmpAndShift("Enable 0x", strlen("Enable 0x")) == 1)
  {
	 tempChar = stringEx.findNext((const char[1]){'\r'}, 1);
	 tempNumber = stringEx.getUint('x', tempChar);
	 controllPinEnableBin(tempNumber);
  }

  if(stringEx.strcmpAndShift("Disable 0x", strlen("Disable 0x")) == 1)
  {
	 tempChar = stringEx.findNext((const char[1]){'\r'}, 1);
	 tempNumber = stringEx.getUint('x', tempChar);
	 controllPinDisableBin(tempNumber);
  }

  if(stringEx.strcmpAndShift("Report\r", strlen("Report\r")) == 1)
  {
    reportPins(0xff);
  }

  if(stringEx.strcmpAndShift("Report binary\r", strlen("Report binary\r")) == 1)
  {
    reportBin();
  }

  if(stringEx.strcmpAndShift("Configure", strlen("Configure")) == 1)
  {
    ePinMode mode = MODE_COUNT;
    ePinPush push = PUSH_COUNT;
    uint8_t pinID = 0;
    uint8_t paramNumber = 0;

    do
    {
      tempChar = stringEx.findNext((const char[2]){',', '\r'}, 2);

      switch(paramNumber)
      {
        case 0:
          pinID = stringEx.getUint(' ', tempChar);
          break;

        case 1:
           stringEx.getStr((char*)tempBuff, 8, ' ', tempChar);

           if(strcmp((char*)tempBuff, "IN") == 0)
           {
              mode = (ePinMode)MODE_IN;
           }
           else if(strcmp((char*)tempBuff, "OUTPP") == 0)
           {
              mode = (ePinMode)MODE_OUT_PP;
             }
             else if(strcmp((char*)tempBuff, "OUTOD") == 0)
             {
                mode = (ePinMode)MODE_OUT_OD;
             }
             break;

           case 2:
             stringEx.getStr((char*)tempBuff, 8, ' ', tempChar);

             if(strcmp((char*)tempBuff, "PPNO") == 0)
             {
                push = (ePinPush)PP_NO;
             }
             else if(strcmp((char*)tempBuff, "PPUP") == 0)
             {
                push = (ePinPush)PP_UP;
             }
             else if(strcmp((char*)tempBuff, "PPDOWN") == 0)
             {
                push = (ePinPush)PP_DOWN;
             }

             break;

           default:
             break;

         }

         if((mode < MODE_COUNT) and (push < PUSH_COUNT))
         {
            configurePin(pinID, mode, push);
         }

         paramNumber++;
       }while(tempChar != '\r');



    }

  if(stringEx.strcmpAndShift("Info\r", strlen("Info\r")) == 1)
  {
    for(uint8_t i = 0; i<=1; i++)
    {
	  CDC_Transmit_FS((uint8_t*) infoArray[i], strlen(infoArray[i]));
    }

  }


  stringEx.reset();

}
