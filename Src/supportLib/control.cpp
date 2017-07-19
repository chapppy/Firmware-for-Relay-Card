
#include "control.hpp"
#include "usbd_cdc_if.h"
//#include "str2num.hpp"
#include "cString.hpp"
#include "parseString.hpp"
#include "usbd_cdc_if.h"


uint32_t sPinModeConv[MODE_COUNT] = { GPIO_MODE_INPUT, GPIO_MODE_OUTPUT_PP, GPIO_MODE_OUTPUT_OD};
uint32_t sPinPushConv[PUSH_COUNT] = { GPIO_NOPULL, GPIO_PULLUP, GPIO_PULLDOWN};


sPinConfig pinsConfig[PIN_COUNT] =
{
  {PIN_IO0, GPIOC, GPIO_PIN_6, "Relay0"},
  {PIN_IO1, GPIOC, GPIO_PIN_7, "Relay1"},
  {PIN_IO2, GPIOC, GPIO_PIN_8, "Relay2"},
  {PIN_IO3, GPIOA, GPIO_PIN_8, "Relay3"},
  {PIN_IO4, GPIOB, GPIO_PIN_3, "Relay4"},
  {PIN_IO5, GPIOB, GPIO_PIN_4, "Relay5"},
  {PIN_IO6, GPIOB, GPIO_PIN_5, "Relay6"},
  {PIN_IO7, GPIOB, GPIO_PIN_6, "Relay7"},
  {PIN_IO8, GPIOB, GPIO_PIN_7, "Relay8"},
  {PIN_IO9, GPIOB, GPIO_PIN_8, "Relay9"},
  {PIN_IO10, GPIOB, GPIO_PIN_9, "Relay10"},
  {PIN_IO11, GPIOA, GPIO_PIN_9, "Relay11"},
  {PIN_IO12, GPIOA, GPIO_PIN_10, "Relay12"}
};




uint8_t configurePin(uint8_t arg_id, ePinMode arg_mode, ePinPush arg_push)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  GPIO_InitStruct.Pin = pinsConfig[arg_id].GPIO_Pin;
  GPIO_InitStruct.Mode = sPinModeConv[arg_mode];
  GPIO_InitStruct.Pull = sPinPushConv[arg_push];
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

  if(arg_id < PIN_COUNT)
  {
    HAL_GPIO_DeInit(pinsConfig[arg_id].GPIOx, pinsConfig[arg_id].GPIO_Pin);
    HAL_GPIO_Init(pinsConfig[arg_id].GPIOx, &GPIO_InitStruct);
    CDC_Transmit_FS((uint8_t*)"As you wish!\r", strlen("As you wish!\r"));
  }
}

uint8_t controllPin(uint8_t arg_id, uint8_t arg_state)
{
  uint8_t buffer[32] = {0};
  cString string((char*)buffer, 32);

  if(arg_id < PIN_COUNT)
  {
    if(arg_state == 1)
    {
	  HAL_GPIO_WritePin(pinsConfig[arg_id].GPIOx , pinsConfig[arg_id].GPIO_Pin, GPIO_PIN_SET);
      string += "Channel ";
	  string.addInt8_t((uint8_t)pinsConfig[arg_id].pinID, 10);
	  //string += pinsConfig[arg_id].name;
      string += " is now enabled\r";
    }
    else
    {
	  HAL_GPIO_WritePin(pinsConfig[arg_id].GPIOx , pinsConfig[arg_id].GPIO_Pin, GPIO_PIN_RESET);
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

void reportPins(uint8_t arg_id)
{
  uint8_t buffer[64] = {0};
  cString string((char*)buffer, 64);
  GPIO_PinState pinState;
  uint8_t isFirst = 1;



  if(arg_id == 0xff)
  {

	 for(uint8_t i=0; i<PIN_COUNT; i++)
	 {
		 pinState = HAL_GPIO_ReadPin(pinsConfig[i].GPIOx , pinsConfig[i].GPIO_Pin);
		 if(pinState == GPIO_PIN_SET)
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
		string += "all channels disabled";
	 }
	 string += "\r";
  }
  else
  {
	if(arg_id < PIN_COUNT)
	{
	  pinState = HAL_GPIO_ReadPin(pinsConfig[arg_id < PIN_COUNT].GPIOx , pinsConfig[arg_id < PIN_COUNT].GPIO_Pin);
	  if(pinState == GPIO_PIN_SET)
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


void reportBin()
{
  uint8_t buffer[32] = {0};
  cString string((char*)buffer, 32);
  GPIO_PinState pinState;
  uint8_t output = 0;



 for(uint8_t i=0; i<PIN_COUNT; i++)
 {
   pinState = HAL_GPIO_ReadPin(pinsConfig[i].GPIOx , pinsConfig[i].GPIO_Pin);
   if(pinState == GPIO_PIN_SET)
   {
	 output |= 1 << i;
   }
 }

 string += "Channels state 0x";
 string.addInt8_t((uint8_t)output, 16);
 string += "\r";


  CDC_Transmit_FS(buffer, string.getSize());
}

void controlCyclic(void)
{
  uint8_t dataBuff[64] = {0};
  uint8_t tempBuff[10] = {0};
  parseString<0> stringEx;
  uint8_t buffIndex = 0;
  uint8_t tempChar = 0;
  uint8_t tempNumber = 0;

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

  if(stringEx.strcmpAndShift("Enable 0x", strlen("Enable 0x")) == 1)
  {

  }

  if(stringEx.strcmpAndShift("Disable 0x", strlen("Disable 0x")) == 1)
  {

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
	CDC_Transmit_FS((uint8_t*)"s.hamouz@gmail.com\r", strlen("s.hamouz@gmail.com\r"));
  }


  stringEx.reset();


}
