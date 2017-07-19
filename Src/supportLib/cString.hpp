#ifndef CSTRING_HPP
#define CSTRING_HPP

#include "stdint.h"

class cString
{
  public:
    cString();
    cString(char *arg_string, uint16_t arg_size);
    
    char* getString(void) { return string; }
    uint16_t getSize(void) { return index; }
    void setString(char *arg_string, uint16_t arg_size) { string=arg_string; maxSize=arg_size;}
    void flush(void) {index=0; string[0]=0;}
    
    uint16_t addInt32_t(int32_t number, uint8_t radix);
    uint16_t addInt16_t(int16_t number, uint8_t radix);
    uint16_t addInt8_t (int8_t number,  uint8_t radix);
    
    uint16_t addUint32_t(uint32_t number, uint8_t radix);
    uint16_t addUint16_t(uint16_t number, uint8_t radix);
    uint16_t addUint8_t (uint8_t number,  uint8_t radix);  
    
    uint16_t addChar(char arg_char);
    uint16_t addString(char *arg_string);
    
    
    cString &operator+=(char *arg_string);  

  protected:

  private:
    char *string;
    uint16_t maxSize;
    uint16_t index;
    
    
    uint16_t strcpy(const char *from);
};


#endif // CSTRING_HPP
