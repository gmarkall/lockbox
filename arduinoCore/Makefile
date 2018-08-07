CC = riscv32-unknown-elf-gcc
CXX = riscv32-unknown-elf-g++
AS = riscv32-unknown-elf-gcc
AR = riscv32-unknown-elf-ar

CPPFLAGS = -I../include -DF_CPU=16000000LL -DFREEDOM_E300_HIFIVE1 -DPRCI_BASE_ADD

COMMON_PATH = /home/sarah/.arduino15/packages/sifive/hardware/riscv/1.0.2

CCFLAGS = -c -O2 -march=rv32imac -fpeel-loops -ffreestanding -ffunction-sections -fdata-sections -Wall -I${COMMON_PATH}/system/include -I${COMMON_PATH}/freedom-e-sdk/bsp/include -I${COMMON_PATH}/freedom-e-sdk/bsp/env -I${COMMON_PATH}/freedom-e-sdk/bsp/drivers -I${COMMON_PATH}/freedom-e-sdk/bsp/env/freedom-e300-hifive1 -include sys/cdefs.h -g -DARDUINO=10805 -DF_CPU=16000000LL -DFREEDOM_E300_HIFIVE1 -I${COMMON_PATH}/cores/arduino -I${COMMON_PATH}/variants/standard 

CXXFLAGS = -c -O2 -march=rv32imac -fpeel-loops -ffreestanding -ffunction-sections -fdata-sections -fpermissive -Wall -fno-rtti -fno-exceptions -I${COMMON_PATH}/system/include -I${COMMON_PATH}/freedom-e-sdk/bsp/include -I${COMMON_PATH}/freedom-e-sdk/bsp/env -I${COMMON_PATH}/freedom-e-sdk/bsp/drivers -I${COMMON_PATH}/freedom-e-sdk/bsp/env/freedom-e300-hifive1 -include sys/cdefs.h -g -DARDUINO=10805 -DF_CPU=16000000LL -DFREEDOM_E300_HIFIVE1 -I${COMMON_PATH}/cores/arduino -I${COMMON_PATH}/variants/standard

ASFLAGS = -c -march=rv32imac -I${COMMON_PATH}/system/include -I${COMMON_PATH}/freedom-e-sdk/bsp/include -I${COMMON_PATH}/freedom-e-sdk/bsp/env -I${COMMON_PATH}/freedom-e-sdk/bsp/drivers -I${COMMON_PATH}/freedom-e-sdk/bsp/env/freedom-e300-hifive1 -g -DARDUINO=10805 -DF_CPU=16000000LL -DFREEDOM_E300_HIFIVE1 -I${COMMON_PATH}/cores/arduino -I${COMMON_PATH}/variants/standard 

DOT_O_FILENAMES = entry.o init.o start.o WInterrupts.o hooks.o itoa.o malloc.o sbrk.o wiring.o wiring_analog.o wiring_digital.o wiring_shift.o fe300prci_driver.o plic_driver.o Print.o UARTClass.o WMath.o WString.o abi.o main.o new.o wiring_pulse.o 
#HardwareSerial.o

all: core.a

clean:
	rm -f *.o ../core.a 

%.o: %.c
	${CC} ${CCFLAGS} $<

%.o: %.cpp
	${CXX} ${CXXFLAGS} $<

%.o: %.S
	${AS} ${ASFLAGS} $<
    
core.a: ${DOT_O_FILENAMES}
	${AR} rcs ../core.a $^





