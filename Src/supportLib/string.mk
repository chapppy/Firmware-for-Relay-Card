# string.mk

# Include C files
#LIB_C_SRC += $(MY_LIBRARIES_PATH)/string/file.c
          
# Include C++ files
LIB_CPP_SRC += $(MY_LIBRARIES_PATH)/string/cString.cpp \
               $(MY_LIBRARIES_PATH)/string/parseString.cpp \
               $(MY_LIBRARIES_PATH)/string/str2num.cpp
               
# Required include directories
LIB_INC += $(MY_LIBRARIES_PATH)/string

# Compiler options          
#USE_OPT += -DSTM32F0xxLIB
