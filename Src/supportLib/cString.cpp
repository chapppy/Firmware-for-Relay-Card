#include "cString.hpp"

#include "num2str.hpp"


cString::cString():string(0),maxSize(0),index(0)
{
  //ctor
}


cString::cString(char *arg_string, uint16_t arg_size):index(0)
{
  string = arg_string;
  maxSize = arg_size;
}

uint16_t cString::strcpy(const char *from)
{
	//for (; (string[index] = *from) != '\0'; ++from, ++index);
	while(*from != '\0')
  {
    string[index++] = *from;
    from++;
  }
	
	
	return index;
}



uint16_t cString::addInt32_t(int32_t number, uint8_t radix)
{
   uint16_t ret;
   ret = num2str<int32_t>::getStr(number, &(string[index]), radix); // buffer overflow
   index += ret;
   
   return index; 
}

uint16_t cString::addInt16_t(int16_t number, uint8_t radix)
{
   uint16_t ret;
   ret = num2str<int16_t>::getStr(number, &(string[index]), radix); // buffer overflow
   index += ret;
   
   return index; 
}


uint16_t cString::addInt8_t(int8_t number, uint8_t radix)
{
   uint16_t ret;
   ret = num2str<int8_t>::getStr(number, &(string[index]), radix); // buffer overflow
   index += ret;
   
   return index; 
}


uint16_t cString::addUint32_t(uint32_t number, uint8_t radix)
{
   uint16_t ret;
   ret = num2str<uint32_t>::getStr(number, &(string[index]), radix); // buffer overflow
   index += ret;
   
   return index; 
}

uint16_t cString::addUint16_t(uint16_t number, uint8_t radix)
{
   uint16_t ret;
   ret = num2str<uint16_t>::getStr(number, &(string[index]), radix); // buffer overflow
   index += ret;
   
   return index; 
}


uint16_t cString::addUint8_t(uint8_t number, uint8_t radix)
{
   uint16_t ret;
   ret = num2str<uint8_t>::getStr(number, &(string[index]), radix); // buffer overflow
   index += ret;
   
   return index; 
}


uint16_t cString::addChar(char arg_char)
{
  uint16_t ret = 0;
  
  if((index+1) < maxSize)
  {
    string[index++] = arg_char;  
    ret = index;
  }
  
  string[index] = 0x00;
  
  return ret;
}


uint16_t cString::addString(char *arg_string)
{
  uint16_t i = 0;
  uint16_t ret = 0;
  
  while(arg_string[i] != '\0')
  {
    ret = addChar(arg_string[i++]);
  }
  
  return ret;
}

cString &cString::operator+=(char *arg_string)
{
  addString(arg_string);
  
  return *this;
}
