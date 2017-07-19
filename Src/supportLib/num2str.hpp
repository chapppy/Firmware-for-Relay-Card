#ifndef NUM2STR_HPP
#define NUM2STR_HPP


#include "stdint.h"
#include "math.h"

template <class T>
class num2strBase
{
    public:
      static uint8_t itoa(T argValue, char *sp, uint8_t radix);
      static uint8_t ftoa(T argValue, char *res, uint8_t afterpoint);
    protected:


    private:
      

};


// Yet, another good itoa implementation
// returns: the length of the number string
template <class T>
uint8_t num2strBase<T>::itoa(T argValue, char *sp, uint8_t radix)
{
    char tmp[16];// be careful with the length of the buffer
    char *tp = tmp;
    int16_t i;
    T value;
    int8_t len;

    uint8_t sign = 0;    
    if((radix == 10) && (argValue < 0))
    {
      sign = 1;
      value = -argValue; 
    }
    else
    {
      value = (unsigned)argValue;
    }
      
    while (value || tp == tmp)
    {
      i = value % radix;
      value /= radix; // v/=radix uses less CPU clocks than v=v/radix does
      if (i < 10)
      {
        *tp++ = i+'0';
      }
      else
      {
        *tp++ = i + 'a' - 10;
      }      
    }

    len = tp - tmp;

    if (sign) 
    {
      *sp++ = '-';
      len++;
    }

    while (tp > tmp)
    {
      *sp++ = *--tp;
    }
    
    *sp++ = 0x00;
        
    return len;
}






template <class T>
class num2str : public num2strBase<T>
{
    public:
      static uint8_t getStr(T number, char *str, uint8_t radix);

    protected:


    private:

};




// Converts a floating point number to string.
template <class T>
uint8_t num2strBase<T>::ftoa(T argValue, char *res, uint8_t afterpoint)
{
    // Extract integer part
    int32_t ipart = (int)argValue;
 
    // Extract floating part
    float fpart = argValue - (float)ipart;
 
    // convert integer part to string
    //int i = intToStr(ipart, res, 0);
    uint8_t i = num2str<int32_t>::getStr(ipart, res, 10);
 
    // check for display option after point
    if (afterpoint != 0)
    {
        res[i] = '.';  // add dot
        
        if(fpart < 0)
        {
          fpart *= -1;
        }
 
        // Get the value of fraction part upto given no.
        // of points after dot. The third parameter is needed
        // to handle cases like 233.007
        fpart = fpart * pow(10, afterpoint);
 
        //intToStr((int)fpart, res + i + 1, afterpoint);
        num2str<uint32_t>::getStr((uint32_t)fpart, res + i + 1, 10);
    }
}


// Explicitly specialize for the member function
template <>
uint8_t num2str<int32_t>::getStr(int32_t number, char *str, uint8_t radix){
  return itoa(number, str, radix);
}

// Explicitly specialize for the member function
template <>
uint8_t num2str<int16_t>::getStr(int16_t number, char *str, uint8_t radix){
  return itoa(number, str, radix);
}

// Explicitly specialize for the member function
template <>
uint8_t num2str<int8_t>::getStr(int8_t number, char *str, uint8_t radix){
  return itoa((int32_t)number, str, radix);
}

// Explicitly specialize for the member function
template <>
uint8_t num2str<uint32_t>::getStr(uint32_t number, char *str, uint8_t radix){
  return itoa(number, str, radix);
}

// Explicitly specialize for the member function
template <>
uint8_t num2str<uint16_t>::getStr(uint16_t number, char *str, uint8_t radix){
  return itoa(number, str, radix);
}

// Explicitly specialize for the member function
template <>
uint8_t num2str<uint8_t>::getStr(uint8_t number, char *str, uint8_t radix){
  return itoa((int32_t)number, str, radix);
}


// Explicitly specialize for the member function
template <>
uint8_t num2str<float>::getStr(float number, char *str, uint8_t radix){
  return ftoa((float)number, str, radix);  //!H! ftoa does not return strlen!!!
}




/*
template<>
class num2str<int32_t> : public num2strBase<int32_t>
{
    public:
      static void getStr(int32_t number, char *str, uint8_t radix);

    protected:


    private:

};


void num2str<int32_t>::getStr(int32_t number, char *str, uint8_t radix)
{
   //itoa(number, str, radix);
}*/


#endif
