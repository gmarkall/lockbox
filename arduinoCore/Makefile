CC = riscv32-unknown-elf-gcc
CXX = riscv32-unknown-elf-g++
AS = riscv32-unknown-elf-gcc
AR = riscv32-unknown-elf-ar

INCLUDE = -I../include

CCFLAGS = -c -O2 -march=rv32imac -fpeel-loops -ffreestanding -ffunction-sections -fdata-sections -Wall -include sys/cdefs.h -g -DARDUINO=10805 -DF_CPU=16000000LL -DFREEDOM_E300_HIFIVE1 

CXXFLAGS = -c -O2 -march=rv32imac -fpeel-loops -ffreestanding -ffunction-sections -fdata-sections -fpermissive -Wall -fno-rtti -fno-exceptions -include sys/cdefs.h -g -DARDUINO=10805 -DF_CPU=16000000LL -DFREEDOM_E300_HIFIVE1

ASFLAGS = -c -march=rv32imac -g -DARDUINO=10805 -DF_CPU=16000000LL -DFREEDOM_E300_HIFIVE1  

DOT_O_FILENAMES = entry.o init.o start.o WInterrupts.o hooks.o itoa.o malloc.o sbrk.o wiring.o wiring_analog.o wiring_digital.o wiring_shift.o fe300prci_driver.o plic_driver.o Print.o UARTClass.o WMath.o WString.o abi.o main.o new.o wiring_pulse.o 
#HardwareSerial.o

all: core.a

clean:
	rm -f *.o ../core.a && cp HardwareSerialo HardwareSerial.o 

%.o: %.c
	${CC} ${INCLUDE} ${CCFLAGS} $<

%.o: %.cpp
	${CXX} ${INCLUDE} ${CXXFLAGS} $<

%.o: %.S
	${AS} ${INCLUDE} ${ASFLAGS} $<
    
core.a: ${DOT_O_FILENAMES}
	${AR} rcs ../core.a $^





