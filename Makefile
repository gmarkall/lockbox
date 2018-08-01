CC = riscv32-unknown-elf-gcc
CXX = riscv32-unknown-elf-g++
LD = riscv32-unknown-elf-g++

CPPFLAGS = -Iinclude -DF_CPU=16000000LL -DFREEDOM_E300_HIFIVE1

CCFLAGS = -c -O2 -march=rv32imac -fpeel-loops -ffreestanding -g \
          -ffunction-sections -fdata-sections -Wall -Werror -Wextra

CXXFLAGS = ${CCFLAGS} -fno-rtti -fno-exceptions

LDFLAGS = -T hifive1.lds -nostartfiles -Wl,-N -Wl,--gc-sections -nostdlib

all: lockbox.exe

lockbox.exe: lockbox.o LiquidCrystal.o wiring_digital.o

clean:
	rm -f *.o lockbox.exe

%.o: %.c
	${CC} ${CPPFLAGS} ${CXXFLAGS} $<

%.o: %.cpp
	${CXX} ${CPPFLAGS} ${CXXFLAGS} $<

	${LD} ${LDFLAGS} -o lockbox.exe $^

