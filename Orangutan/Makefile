TARGET = main
SRCS = main.c lcd.c serial.c schedule.c compass.c twimaster/twimaster.c sdmio.c motor.c
OBJS = $(SRCS:.c=.o)
DEPS = $(SRCS:.c=.d)

# device specifics
DEVICE = atmega1284p
MCU = atmega1284p
AVRDUDE_DEVICE = m1284p
PORT = /dev/ttyACM0

# define the C compiler to use
CC = avr-gcc
OBJ2HEX = avr-objcopy 
AVRDUDE = avrdude

INCLUDES = -I/usr/lib/avr/include/
LIBS = -lpololu_$(DEVICE)

# basic flags
MCU_FLAGS = -mmcu=$(MCU) $(DEVICE_SPECIFIC_CFLAGS) 
CFLAGS = -std=c99 -Wall -g -mcall-prologues $(MCU_FLAGS) -Os $(INCLUDES)
MIN_PRINTF= -Wl,-u,vfprintf -lprintf_min
LDFLAGS = -Wl,-gc-sections $(LIBS) -Wl,-relax

.PHONY: depend clean

all: $(TARGET).hex

install: $(TARGET).hex
	$(AVRDUDE) -p $(AVRDUDE_DEVICE) -c avrisp2 -P $(PORT) -U flash:w:$(TARGET).hex

$(TARGET).hex: $(TARGET).obj
	$(OBJ2HEX) -R .eeprom -O ihex $< $@

$(TARGET).obj: $(OBJS) $(DEPS)
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $@

%.d: %.c
	$(CC) -MM $(CFLAGS) $< -o $@

-include $(DEPS)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

check-syntax:
	$(CC) $(CFLAGS) -o /dev/null -S ${CHK_SOURCES}

depend: $(SRCS)

clean:
	$(RM) $(OBJS) $(TARGET).hex $(TARGET).obj *~

distclean:
	$(RM) $(OBJS) $(DEPS) $(TARGET).hex $(TARGET).obj *~

