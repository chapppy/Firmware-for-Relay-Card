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

#ifndef PARSESTRING_HPP
#define PARSESTRING_HPP

#include "stdint.h"
#include "str2num.hpp"


template<uint16_t SIZE>
class parseString
{
  public:
    parseString() {};

    void Init(char *arg_source, uint16_t arg_nSource) { source=arg_source; current=source; nSourceBackup=arg_nSource; nSource=arg_nSource;}
    uint32_t getUint(const char start, const char end);
    int32_t getInt(const char start, const char end);   
    uint8_t getStr(char *outString, uint8_t outSize, const char start, const char end);  
    float getFloat(const char start, const char end);  
    int32_t extractStr(char *dest, const char start, const char end);
    uint8_t getLastConversionState(void) { return stateLastConvers; }
    char getNextLetter(void) { return *current; }
    int32_t strcmpAndShift(const char *s1, uint16_t arg_size);
    
    size_t strlen(const char *str);
    
    char findNext(const char *end, uint8_t size);
    void reset() {current=source; nSource=nSourceBackup;}
    
  protected:

  private:
    char temp[16];
    char *source;
    uint16_t nSource;
    uint16_t nSourceBackup;
    char *current;
    uint8_t stateLastConvers;

    char isOneOf(const char currentChar, const char *possibleChar, uint8_t size);
};

template<uint16_t SIZE>
uint8_t parseString<SIZE>::getStr(char *outString, uint8_t outSize, const char start, const char end)  //TODO OUTSIZE se muzi pouzit, jinak muze pretect vystupni buffer
{
  uint32_t value;
  extractStr(outString, start, end); // TADY MUSI BYT TAKY VELIKOST VSTUPNIHO BUFFERU A OMEZEN
  return 1;  
}

template<uint16_t SIZE>
uint32_t parseString<SIZE>::getUint(const char start, const char end)
{
  uint32_t value;
  extractStr(temp, start, end);
  stateLastConvers = str2num<uint32_t>::getNumEx(&value, temp, strlen(temp), 10);
  return value;
}
 
template<uint16_t SIZE>
int32_t parseString<SIZE>::getInt(const char start, const char end)
{
  int32_t value;
  extractStr(temp, start, end);
  stateLastConvers = str2num<int32_t>::getNumEx(&value, temp, strlen(temp), 10);
  return value;
}

template<uint16_t SIZE>
float parseString<SIZE>::getFloat(const char start, const char end)
{
  //TODOczcc
}

template<uint16_t SIZE>
int32_t parseString<SIZE>::extractStr(char *dest, const char start, const char end)
{
    uint8_t loc_prog = 0;
    uint8_t loc_leng = 0;
    uint8_t loc_skipedLetters = 0;
    
    if(start == 0)
    {
      loc_prog = 1;
    }
    
      while ((nSource > 0) && (*current != 0))
    {
      nSource--;
      if(*current == start)
      {
        loc_prog = 1;
        loc_skipedLetters++;
        
      }
      else if(*current == end)
      {
        if(loc_prog == 1)
        {
          *dest = 0; 
          nSource++; 
          *current++; 
          return loc_leng;  
        }
        else
        {
          loc_skipedLetters++;
        }
      }
      else
      {
        if(loc_prog == 1)
        {
          *dest++ = *current;
          loc_leng++;
        }
        else
        {
          loc_skipedLetters++;
        }
      }
      
      *current++;
    }
    
  *dest = 0; 
    return 0;
}

template<uint16_t SIZE>
char parseString<SIZE>::isOneOf(const char currentChar, const char *possibleChar, uint8_t size)
{
  char returnVal = 0;
  
  while(size-- > 0)
  {
    if(currentChar == *(possibleChar+size))
    {
      returnVal = currentChar;
      break;
    }
  }
  
  return returnVal;
}


template<uint16_t SIZE>
char parseString<SIZE>::findNext(const char *end, uint8_t size)
{
  char *loc_s1 = current;
  char returnVal = 0;
  
  while(*loc_s1 != 0)                                                              
  {
    if(isOneOf(*loc_s1, end, size) != 0)
    {
      returnVal = *loc_s1;
      break;
    }
    loc_s1++;
  }
  
    return returnVal;
}



template<uint16_t SIZE>
int32_t parseString<SIZE>::strcmpAndShift(const char *s1, uint16_t arg_size)
{
  const char *loc_s1 = s1;
  char *loc_s2 = current;
     
    while (arg_size > 0)
  {
    arg_size--;
    if(*s1 == *loc_s2)
    {
      *s1++;
    }
    else
    {
      s1 = loc_s1;
    }
       
    if (*s1 == 0)
    {
      current = loc_s2;
      return 1;  
    }
      
    *loc_s2++;
  }
    return 0;
}

template<uint16_t SIZE>
size_t parseString<SIZE>::strlen(const char *str)
{
  const char *s;

  for (s = str; *s; ++s)
                ;
  return (s - str);
}

#endif // PARSESTRING_HPP
