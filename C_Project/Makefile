#
TARGET = CSD_Demo

#
CROSS_COMPILE = arm-linux-gnueabihf-
CFLAGS = -static -g -Wall  -I${SOCEDS_DEST_ROOT}/ip/altera/hps/altera_hps/hwlib/include
LDFLAGS =  -g -Wall -lm 
CC = $(CROSS_COMPILE)gcc
ARCH= arm


build: $(TARGET)
$(TARGET): ServoControl_CSD.o UartLibrary.o DynamixelMX12.o BNO055.o I2C_Library.o
	$(CC) $(LDFLAGS)   $^ -o $@  
%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(TARGET) *.a *.o *~ 
